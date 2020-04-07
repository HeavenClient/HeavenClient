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
#include "CharacterParser.h"

#include "ItemParser.h"

namespace ms
{
	void CharacterParser::parse_inventory(InPacket& recv, Inventory& invent)
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

	void CharacterParser::parse_skillbook(InPacket& recv, SkillBook& skills)
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

	void CharacterParser::parse_cooldowns(InPacket& recv, Player& player)
	{
		int16_t size = recv.read_short();

		for (int16_t i = 0; i < size; i++)
		{
			int32_t skill_id = recv.read_int();
			int32_t cooltime = recv.read_short();
			player.add_cooldown(skill_id, cooltime);
		}
	}

	void CharacterParser::parse_questlog(InPacket& recv, QuestLog& quests)
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
				//i--; // This was causing issues
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

	void CharacterParser::parse_ring1(InPacket& recv)
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

	void CharacterParser::parse_ring2(InPacket& recv)
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

	void CharacterParser::parse_ring3(InPacket& recv)
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

	void CharacterParser::parse_minigame(InPacket& recv)
	{
		recv.skip(2);
	}

	void CharacterParser::parse_monsterbook(InPacket& recv, MonsterBook& monsterbook)
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

	void CharacterParser::parse_teleportrock(InPacket& recv, TeleportRock& teleportrock)
	{
		for (size_t i = 0; i < 5; i++)
			teleportrock.addlocation(recv.read_int());

		for (size_t i = 0; i < 10; i++)
			teleportrock.addviplocation(recv.read_int());
	}

	void CharacterParser::parse_nyinfo(InPacket& recv)
	{
		int16_t nysize = recv.read_short();

		for (int16_t i = 0; i < nysize; i++)
		{
			recv.read_int();	// NewYear Id
			recv.read_int();	// NewYear SenderId
			recv.read_string();	// NewYear SenderName
			recv.read_bool();	// NewYear enderCardDiscarded
			recv.read_long();	// NewYear DateSent
			recv.read_int();	// NewYear ReceiverId
			recv.read_string();	// NewYear ReceiverName
			recv.read_bool();	// NewYear eceiverCardDiscarded
			recv.read_bool();	// NewYear eceiverCardReceived
			recv.read_long();	// NewYear DateReceived
			recv.read_string();	// NewYear Message
		}
	}

	void CharacterParser::parse_areainfo(InPacket& recv)
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