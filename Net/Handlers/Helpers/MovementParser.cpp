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
#include "MovementParser.h"

namespace ms
{
	std::vector<Movement> MovementParser::parse_movements(InPacket& recv)
	{
		std::vector<Movement> movements;
		uint8_t length = recv.read_byte();

		for (uint8_t i = 0; i < length; ++i)
		{
			Movement fragment;
			fragment.command = recv.read_byte();

			switch (fragment.command)
			{
				case 0:
				case 5:
				case 17:
					fragment.type = Movement::ABSOLUTE;
					fragment.xpos = recv.read_short();
					fragment.ypos = recv.read_short();
					fragment.lastx = recv.read_short();
					fragment.lasty = recv.read_short();
					fragment.fh = recv.read_short();
					fragment.newstate = recv.read_byte();
					fragment.duration = recv.read_short();
					break;
				case 1:
				case 2:
				case 6:
				case 12:
				case 13:
				case 16:
					fragment.type = Movement::RELATIVE;
					fragment.xpos = recv.read_short();
					fragment.ypos = recv.read_short();
					fragment.newstate = recv.read_byte();
					fragment.duration = recv.read_short();
					break;
				case 11:
					fragment.type = Movement::CHAIR;
					fragment.xpos = recv.read_short();
					fragment.ypos = recv.read_short();
					recv.skip(2);
					fragment.newstate = recv.read_byte();
					fragment.duration = recv.read_short();
					break;
				case 15:
					fragment.type = Movement::JUMPDOWN;
					fragment.xpos = recv.read_short();
					fragment.ypos = recv.read_short();
					fragment.lastx = recv.read_short();
					fragment.lasty = recv.read_short();
					recv.skip(2);
					fragment.fh = recv.read_short();
					fragment.newstate = recv.read_byte();
					fragment.duration = recv.read_short();
					break;
				case 3:
				case 4:
				case 7:
				case 8:
				case 9:
				case 14:
					fragment.type = Movement::NONE;
					break;
				case 10:
					fragment.type = Movement::NONE;
					// Change equip
					break;
			}

			movements.push_back(fragment);
		}

		return movements;
	}
}