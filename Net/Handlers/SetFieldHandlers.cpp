//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "SetFieldHandlers.h"

#include "Helpers/CharacterParser.h"
#include "Helpers/LoginParser.h"

#include "../Packets/GameplayPackets.h"

#include "../../Configuration.h"

#include "../../Gameplay/Stage.h"
#include "../../IO/UI.h"
#include "../../IO/Window.h"

#include "../../IO/UITypes/UICharSelect.h"

namespace ms
{
	void SetFieldHandler::transition(int32_t mapid, uint8_t portalid) const
	{
		float fadestep = 0.025f;

		Window::get().fadeout(
			fadestep,
			[mapid, portalid]()
			{
				GraphicsGL::get().clear();

				Stage::get().load(mapid, portalid);

				UI::get().enable();
				Timer::get().start();
				GraphicsGL::get().unlock();

				Stage::get().transfer_player();
			});

		GraphicsGL::get().lock();
		Stage::get().clear();
		Timer::get().start();
	}

	void SetFieldHandler::handle(InPacket& recv) const
	{
		Constants::Constants::get().set_viewwidth(Setting<Width>::get().load());
		Constants::Constants::get().set_viewheight(Setting<Height>::get().load());

		int32_t channel = recv.read_int();
		int8_t notifiermessage = recv.read_byte();
		bool characterdata = recv.read_bool();
		int32_t notifiercheck = recv.read_short();

		if (characterdata)
			set_field(recv);
		else
			change_map(recv, channel);
	}

	void SetFieldHandler::change_map(InPacket& recv, int32_t) const
	{
		recv.skip(1); // revive

		int32_t mapid = recv.read_int();
		int8_t portalid = recv.read_byte();

		transition(mapid, portalid);
	}

	void SetFieldHandler::set_field(InPacket& recv) const
	{
		uint32_t s1 = recv.read_int();
		uint32_t s2 = recv.read_int();
		uint32_t s3 = recv.read_int();
		
		recv.read_long(); // charflag
		recv.read_byte(); // combatorders

		int32_t cid = recv.read_int();
		auto charselect = UI::get().get_element<UICharSelect>();

		if (!charselect)
			return;

		const CharEntry& playerentry = charselect->get_character(cid);

		if (playerentry.id != cid)
			return;

		Stage::get().loadplayer(playerentry, Configuration::get().get_worldid(), Configuration::get().get_channelid());

		LoginParser::parse_stats(recv);

		Player& player = Stage::get().get_player();

		player.get_calcdamage().set_seed(s1, s2, s3);

		recv.read_byte(); // 'buddycap'

		if (recv.read_bool())
			recv.read_string(); // 'linkedname'

		CharacterParser::parse_inventory(recv, player.get_inventory());
		CharacterParser::parse_skillbook(recv, player.get_skills());
		CharacterParser::parse_cooldowns(recv, player);
		CharacterParser::parse_questlog(recv, player.get_quests());
		CharacterParser::parse_minigame(recv);
		CharacterParser::parse_ring1(recv);
		CharacterParser::parse_ring2(recv);
		CharacterParser::parse_ring3(recv);
		CharacterParser::parse_teleportrock(recv, player.get_teleportrock());
		CharacterParser::parse_monsterbook(recv, player.get_monsterbook());
		CharacterParser::parse_nyinfo(recv);
		CharacterParser::parse_areainfo(recv);

		recv.read_long(); // getTime(System.currentTimeMillis()

		player.recalc_stats(true);

		PlayerUpdatePacket().dispatch();

		uint8_t portalid = player.get_stats().get_portal();
		int32_t mapid = player.get_stats().get_mapid();

		transition(mapid, portalid);

		Sound(Sound::Name::GAMESTART).play();

		UI::get().change_state(UI::State::GAME);
	}
}