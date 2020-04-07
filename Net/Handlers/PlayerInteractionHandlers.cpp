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
#include "PlayerInteractionHandlers.h"

#include "../../IO/UI.h"

#include "../../IO/UITypes/UICharInfo.h"

namespace ms
{
	void CharInfoHandler::handle(InPacket& recv) const
	{
		int32_t character_id = recv.read_int();
		int8_t character_level = recv.read_byte();
		int16_t character_job_id = recv.read_short();
		int16_t character_fame = recv.read_short();
		recv.skip_byte(); // character_marriage_ring

		std::string guild_name = recv.read_string();
		std::string alliance_name = recv.read_string();

		recv.skip_byte();

		int8_t pet_unique_id = recv.read_byte();

		while (pet_unique_id != 0)
		{
			recv.skip_int();	// pet_id
			recv.skip_string();	// pet_name
			recv.skip_byte();	// pet_level
			recv.skip_short();	// pet_closeness
			recv.skip_byte();	// pet_fullness

			recv.skip_short();

			recv.skip_int(); // pet_inventory_id

			pet_unique_id = recv.read_byte();
		}

		int8_t mount = recv.read_byte();

		if (mount != 0)
		{
			recv.skip_int(); // mount_level
			recv.skip_int(); // mount_exp
			recv.skip_int(); // mount_tiredness
		}

		int8_t wishlist_size = recv.read_byte();

		for (int8_t sn = 0; sn < wishlist_size; sn++)
			recv.skip_int(); // wishlist_item

		recv.skip_int(); // monster_book_level
		recv.skip_int(); // monster_book_card_normal
		recv.skip_int(); // monster_book_card_special
		recv.skip_int(); // monster_book_cards_total
		recv.skip_int(); // monster_book_cover

		recv.skip_int(); // medal

		int16_t medal_quests_size = recv.read_short();

		for (int16_t s = 0; s < medal_quests_size; s++)
			recv.skip_short(); // medal_quest

		// Update the character information window
		if (auto charinfo = UI::get().get_element<UICharInfo>())
			charinfo->update_stats(character_id, character_job_id, character_level, character_fame, guild_name, alliance_name);
	}
}