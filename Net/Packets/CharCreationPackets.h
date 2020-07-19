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

namespace ms
{
	// Reserve a name for the character to be created.
	// Opcode: NAME_CHAR(21)
	class NameCharPacket : public OutPacket
	{
	public:
		NameCharPacket(const std::string& name) : OutPacket(OutPacket::Opcode::NAME_CHAR)
		{
			write_string(name);
		}
	};

	// Requests creation of a character with the specified stats.
	// Opcode: CREATE_CHAR(22)
	class CreateCharPacket : public OutPacket
	{
	public:
		CreateCharPacket(const std::string& name, uint16_t job, int32_t face, int32_t hair, uint8_t hairc, uint8_t skin,
						 int32_t top, int32_t bot, int32_t shoes, int32_t weapon, bool female) : OutPacket(
			OutPacket::Opcode::CREATE_CHAR)
		{
			write_string(name);
			write_int(job);
			write_int(face);
			write_int(hair);
			write_int(hairc);
			write_int(skin);
			write_int(top);
			write_int(bot);
			write_int(shoes);
			write_int(weapon);
			write_byte(female);
		}
	};
}