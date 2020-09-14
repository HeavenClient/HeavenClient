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
	// Tell the server which character was picked.
	// Opcode: SELECT_CHAR(19)
	class SelectCharPacket : public OutPacket
	{
	public:
		SelectCharPacket(int32_t cid) : OutPacket(OutPacket::Opcode::CHAR_SELECT)
		{
			write_int(cid);
			write_hardware_info();
		}
	};

	// Registers a pic and tells the server which character was picked.
	// Opcode: REGISTER_PIC(29)
	class RegisterPicPacket : public OutPacket
	{
	public:
		RegisterPicPacket(int32_t cid, const std::string& pic) : OutPacket(OutPacket::Opcode::REGISTER_PIC)
		{
			skip(1);

			write_int(cid);
			write_hardware_info();
			write_string(pic);
		}
	};

	// Requests using the specified character with the specified pic.
	// Opcode: SELECT_CHAR_PIC(30)
	class SelectCharPicPacket : public OutPacket
	{
	public:
		SelectCharPicPacket(const std::string& pic, int32_t cid) : OutPacket(OutPacket::Opcode::CHAR_SELECT_WITH_PIC)
		{
			write_string(pic);
			write_int(cid);
			write_hardware_info();
		}
	};

	// Requests deleting the specified character without a pic.
	// Opcode: DELETE_CHAR(23)
	class DeleteCharPacket : public OutPacket
	{
	public:
		DeleteCharPacket(int32_t cid) : OutPacket(OutPacket::Opcode::DELETE_CHAR)
		{
			write_string("");
			write_int(cid);
		}
	};

	// Requests deleting the specified character with the specified pic.
	// Opcode: DELETE_CHAR(23)
	class DeleteCharPicPacket : public OutPacket
	{
	public:
		DeleteCharPicPacket(const std::string& pic, int32_t cid) : OutPacket(OutPacket::Opcode::DELETE_CHAR)
		{
			write_string(pic);
			write_int(cid);
		}
	};
}