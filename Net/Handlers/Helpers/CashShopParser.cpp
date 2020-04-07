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
#include "CashShopParser.h"

#include "CharacterParser.h"

#include "../../../Gameplay/Stage.h"

namespace ms
{
	namespace CashShopParser
	{
		StatsEntry parseCharacterInfo(InPacket& recv)
		{
			recv.read_long();
			recv.read_byte();

			StatsEntry statsentry = parseCharStats(recv);

			Player& player = Stage::get().get_player();

			player.get_stats().set_mapid(statsentry.mapid);
			player.get_stats().set_portal(statsentry.portal);

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
			CharacterParser::parse_teleportrock(recv, player.get_teleport_rock());
			CharacterParser::parse_monsterbook(recv, player.get_monsterbook());
			CharacterParser::parse_nyinfo(recv);
			CharacterParser::parse_areainfo(recv);

			player.recalc_stats(true);

			recv.read_short();

			return statsentry;
		}

		StatsEntry parseCharStats(InPacket& recv)
		{
			recv.read_int(); // character id

			// TODO: This is similar to LoginParser.cpp, try and merge these.
			StatsEntry statsentry;

			statsentry.name = recv.read_padded_string(13);
			statsentry.female = recv.read_bool();

			recv.read_byte();	// skin
			recv.read_int();	// face
			recv.read_int();	// hair

			for (size_t i = 0; i < 3; i++)
				statsentry.petids.push_back(recv.read_long());

			statsentry.stats[MapleStat::Id::LEVEL] = recv.read_byte(); // TODO: Change to recv.read_short(); to increase level cap

			auto job = recv.read_short();

			statsentry.stats[MapleStat::Id::JOB] = job;
			statsentry.stats[MapleStat::Id::STR] = recv.read_short();
			statsentry.stats[MapleStat::Id::DEX] = recv.read_short();
			statsentry.stats[MapleStat::Id::INT] = recv.read_short();
			statsentry.stats[MapleStat::Id::LUK] = recv.read_short();
			statsentry.stats[MapleStat::Id::HP] = recv.read_short();
			statsentry.stats[MapleStat::Id::MAXHP] = recv.read_short();
			statsentry.stats[MapleStat::Id::MP] = recv.read_short();
			statsentry.stats[MapleStat::Id::MAXMP] = recv.read_short();
			statsentry.stats[MapleStat::Id::AP] = recv.read_short();

			if (hasSPTable(job))
				parseRemainingSkillInfo(recv);
			else
				recv.read_short(); // remaining sp

			statsentry.exp = recv.read_int();
			statsentry.stats[MapleStat::Id::FAME] = recv.read_short();

			recv.skip(4); // gachaexp

			statsentry.mapid = recv.read_int();
			statsentry.portal = recv.read_byte();

			recv.skip(4); // timestamp

			return statsentry;
		}

		bool hasSPTable(int16_t job)
		{
			switch (job)
			{
			case Jobs::EVAN:
			case Jobs::EVAN1:
			case Jobs::EVAN2:
			case Jobs::EVAN3:
			case Jobs::EVAN4:
			case Jobs::EVAN5:
			case Jobs::EVAN6:
			case Jobs::EVAN7:
			case Jobs::EVAN8:
			case Jobs::EVAN9:
			case Jobs::EVAN10:
				return true;
			default:
				return false;
			}
		}

		void parseRemainingSkillInfo(InPacket& recv)
		{
			int count = recv.read_byte();

			for (int i = 0; i < count; i++)
			{
				recv.read_byte(); // Remaining SP index for job 
				recv.read_byte(); // The actual SP for that class
			}
		}
	}
}