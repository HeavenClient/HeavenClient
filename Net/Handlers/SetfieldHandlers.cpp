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
#include "SetfieldHandlers.h"

#include "Helpers/ItemParser.h"
#include "Helpers/LoginParser.h"

#include "../Configuration.h"
#include "../Console.h"
#include "../Constants.h"
#include "../Timer.h"
#include "../Audio/Audio.h"
#include "../Gameplay/Stage.h"
#include "../Graphics/GraphicsGL.h"
#include "../IO/UI.h"
#include "../IO/UITypes/UICharSelect.h"
#include "../IO/Window.h"

namespace ms
{
	void SetfieldHandler::transition(int32_t mapid, uint8_t portalid) const
	{
		float fadestep = 0.025f;

		Window::get().fadeout(fadestep, [mapid, portalid]() {
			GraphicsGL::get().clear();
			Stage::get().load(mapid, portalid);
			UI::get().enable();
			Timer::get().start();
			GraphicsGL::get().unlock();
			});

		GraphicsGL::get().lock();
		Stage::get().clear();
		Timer::get().start();
	}

	void SetfieldHandler::handle(InPacket& recv) const
	{
		Constants::Constants::get().set_viewwidth(Setting<Width>::get().load());
		Constants::Constants::get().set_viewheight(Setting<Height>::get().load());

		int32_t channel = recv.read_int();
		int8_t mode1 = recv.read_byte();
		int8_t mode2 = recv.read_byte();

		if (mode1 == 0 && mode2 == 0)
			change_map(recv, channel);
		else
			set_field(recv);
	}

	void SetfieldHandler::change_map(InPacket& recv, int32_t) const
	{
		recv.skip(3);

		int32_t mapid = recv.read_int();
		int8_t portalid = recv.read_byte();

		transition(mapid, portalid);
	}

	void SetfieldHandler::set_field(InPacket& recv) const
	{
		recv.skip(23);

		int32_t cid = recv.read_int();
		auto charselect = UI::get().get_element<UICharSelect>();

		if (!charselect)
			return;

		const CharEntry& playerentry = charselect->get_character(cid);

		if (playerentry.id != cid)
			return;

		Stage::get().loadplayer(playerentry);

		LoginParser::parse_stats(recv);

		Player& player = Stage::get().get_player();

		recv.read_byte(); // 'buddycap'

		if (recv.read_bool())
			recv.read_string(); // 'linkedname'

		parse_inventory(recv, player.get_inventory());
		parse_skillbook(recv, player.get_skills());
		parse_cooldowns(recv, player);
		parse_questlog(recv, player.get_quests());
		parse_minigame(recv);
		parse_ring1(recv);
		parse_ring2(recv);
		parse_ring3(recv);
		parse_telerock(recv, player.get_telerock());
		parse_monsterbook(recv, player.get_monsterbook());
		parse_nyinfo(recv);
		parse_areainfo(recv);

		player.recalc_stats(true);

		uint8_t portalid = player.get_stats().get_portal();
		int32_t mapid = player.get_stats().get_mapid();

		transition(mapid, portalid);

		Sound(Sound::GAMESTART).play();

		UI::get().change_state(UI::GAME);
	}

	void SetfieldHandler::parse_inventory(InPacket& recv, Inventory& invent) const
	{
		invent.set_meso(recv.read_int());
		invent.set_slotmax(InventoryType::EQUIP, recv.read_byte());
		invent.set_slotmax(InventoryType::USE, recv.read_byte());
		invent.set_slotmax(InventoryType::SETUP, recv.read_byte());
		invent.set_slotmax(InventoryType::ETC, recv.read_byte());
		invent.set_slotmax(InventoryType::CASH, recv.read_byte());

		recv.skip(8);

		for (size_t i = 0; i < 3; i++)
		{
			InventoryType::Id inv = (i == 0) ? InventoryType::EQUIPPED : InventoryType::EQUIP;
			int16_t pos = recv.read_short();

			while (pos != 0)
			{
				int16_t slot = (i == 1) ? -pos : pos;
				ItemParser::parse_item(recv, inv, slot, invent);
				pos = recv.read_short();
			}
		}

		recv.skip(2);

		InventoryType::Id toparse[4] =
		{
			InventoryType::USE, InventoryType::SETUP, InventoryType::ETC, InventoryType::CASH
		};

		for (size_t i = 0; i < 4; i++)
		{
			InventoryType::Id inv = toparse[i];
			int8_t pos = recv.read_byte();

			while (pos != 0)
			{
				ItemParser::parse_item(recv, inv, pos, invent);
				pos = recv.read_byte();
			}
		}
	}

	void SetfieldHandler::parse_skillbook(InPacket& recv, Skillbook& skills) const
	{
		int16_t size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int32_t skill_id = recv.read_int();
			int32_t level = recv.read_int();
			int64_t expiration = recv.read_long();
			bool fourthtjob = ((skill_id % 100000) / 10000 == 2);
			int32_t masterlevel = fourthtjob ? recv.read_int() : 0;
			skills.set_skill(skill_id, level, masterlevel, expiration);
		}
	}

	void SetfieldHandler::parse_cooldowns(InPacket& recv, Player& player) const
	{
		int16_t size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int32_t skill_id = recv.read_int();
			int32_t cooltime = recv.read_short();
			player.add_cooldown(skill_id, cooltime);
		}
	}

	void SetfieldHandler::parse_questlog(InPacket& recv, Questlog& quests) const
	{
		int16_t size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int16_t qid = recv.read_short();
			std::string qdata = recv.read_string();

			if (quests.is_started(qid))
			{
				int16_t qidl = quests.get_last_started();
				quests.add_in_progress(qidl, qid, qdata);
				i--;
			}
			else
			{
				quests.add_started(qid, qdata);
			}
		}

		std::map<int16_t, int64_t> completed;
		size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int16_t qid = recv.read_short();
			int64_t time = recv.read_long();
			quests.add_completed(qid, time);
		}
	}

	void SetfieldHandler::parse_ring1(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();

		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
		}
	}

	void SetfieldHandler::parse_ring2(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();

		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_int();
		}
	}

	void SetfieldHandler::parse_ring3(InPacket& recv) const
	{
		int16_t rsize = recv.read_short();

		for (int16_t i = 0; i < rsize; i++)
		{
			recv.read_int();
			recv.read_int();
			recv.read_int();
			recv.read_short();
			recv.read_int();
			recv.read_int();
			recv.read_padded_string(13);
			recv.read_padded_string(13);
		}
	}

	void SetfieldHandler::parse_minigame(InPacket& recv) const
	{
		recv.skip(2);
		//int16_t mgsize = recv.read_short();

		//for (int16_t i = 0; i < mgsize; i++) {}
	}

	void SetfieldHandler::parse_monsterbook(InPacket& recv, Monsterbook& monsterbook) const
	{
		monsterbook.set_cover(recv.read_int());

		recv.skip(1);

		int16_t size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int16_t cid = recv.read_short();
			int8_t mblv = recv.read_byte();

			monsterbook.add_card(cid, mblv);
		}
	}

	void SetfieldHandler::parse_telerock(InPacket& recv, Telerock& trock) const
	{
		for (size_t i = 0; i < 5; i++)
			trock.addlocation(recv.read_int());

		for (size_t i = 0; i < 10; i++)
			trock.addviplocation(recv.read_int());
	}

	void SetfieldHandler::parse_nyinfo(InPacket& recv) const
	{
		//int16_t nysize = recv.read_short();

		//for (int16_t i = 0; i < nysize; i++) {}
	}

	void SetfieldHandler::parse_areainfo(InPacket& recv) const
	{
		std::map<int16_t, std::string> areainfo;
		int16_t arsize = recv.read_short();

		for (int16_t i = 0; i < arsize; i++)
		{
			int16_t area = recv.read_short();
			areainfo[area] = recv.read_string();
		}
	}
}