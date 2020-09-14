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
#include "PartyHandlers.h"

namespace ms
{
	// TODO
	void PartyResultHandler::handle(InPacket& recv) const
	{
		int8_t option = recv.read_byte();
		int32_t partyid;
		int32_t targetid;
		std::string targetname;
		int32_t townid;
		Point<int16_t> position;
		switch (option) {
			case 0xC: {
				partyid = recv.read_int();
				targetid = recv.read_int();
				int8_t op = recv.read_byte();
				if (op == 0) {
					recv.read_int();
				}
				else {
					recv.read_byte();
					targetname = recv.read_string();
					// addPartyStatus
				}
				break;
			}
			case 0xF: {
				partyid = recv.read_int();
				targetname = recv.read_string();
				// addPartyStatus
				break;
			}
			case 0x7: {
				partyid = recv.read_int();
				// addPartyStatus
				break;
			}
			case 0x1B: {
				targetid = recv.read_int();
				recv.skip(1);
				break;
			}
			case 0x15:
			case 0x17:
			case 0x18: {
				message(recv);
				break;
			}
			case 0x23: {
				position.set_x(recv.read_byte());
				position.set_y(recv.read_byte());
				break;
			}
			default:
				break;
		}
	}

	void PartyResultHandler::message(InPacket& recv) const {
		std::string charname = recv.read_string();
		// 21: Player is blocking any party invitations,
		// 22: Player is taking care of another invitation,
		// 23: Player have denied request to the party.
	}

	void PartyValueHandler::handle(InPacket& recv) const
	{
		std::string key = recv.read_string();
		recv.read_string(); // Might be read_time();
	}
}