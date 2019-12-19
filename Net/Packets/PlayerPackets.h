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
#pragma once

#include "../OutPacket.h"

#include "../Character/Maplestat.h"

#include "../IO/UITypes/UIKeyConfig.h"

namespace ms
{
	// Requests a stat increase by spending ap.
	// Opcode: SPEND_AP(87)
	class SpendApPacket : public OutPacket
	{
	public:
		SpendApPacket(Maplestat::Id stat) : OutPacket(OutPacket::Opcode::SPEND_AP)
		{
			write_time();
			write_int(Maplestat::codes[stat]);
		}
	};

	// Requests a skill level increase by spending sp.
	// Opcode: SPEND_SP(90)
	class SpendSpPacket : public OutPacket
	{
	public:
		SpendSpPacket(int32_t skill_id) : OutPacket(OutPacket::Opcode::SPEND_SP)
		{
			write_time();
			write_int(skill_id);
		}
	};

	// Requests the server to change kep mappings
	// Opcode: CHANGE_KEYMAP(135)
	class ChangeKeyMapPacket : public OutPacket
	{
	public:
		ChangeKeyMapPacket(std::vector<std::tuple<KeyConfig::Key, KeyType::Id, int32_t>> updated_actions) : OutPacket(OutPacket::Opcode::CHANGE_KEYMAP)
		{
			write_int(0); // mode
			write_int(updated_actions.size()); // Number of key changes

			for (size_t i = 0; i < updated_actions.size(); i++)
			{
				auto keymap = updated_actions[i];

				write_int(std::get<0>(keymap)); // key
				write_byte(std::get<1>(keymap));// type
				write_int(std::get<2>(keymap)); // action
			}
		}
	};
}