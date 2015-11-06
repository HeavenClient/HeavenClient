/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cstdint>
#include <string>

namespace Data
{
	using::std::uint8_t;
	using::std::uint16_t;
	using::std::string;
	// A frame of animation for a skill or similiar 'meta-stance'. 
	// This simply redirects to a different stance and frame to use.
	class BodyAction
	{
	public:
		BodyAction(string s, uint8_t f, uint16_t d)
		{
			stance = s;
			frame = f;
			delay = d;
		}

		BodyAction() {}

		string getstance() const
		{
			return stance;
		}

		uint8_t getframe() const
		{
			return frame;
		}

		uint16_t getdelay() const
		{
			return delay;
		}
	private:
		string stance;
		uint8_t frame;
		uint16_t delay;
	};
}