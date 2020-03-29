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

#include "Physics/PhysicsObject.h"

namespace ms
{
	struct Movement
	{
		enum Type
		{
			NONE,
			ABSOLUTE,
			RELATIVE,
			CHAIR,
			JUMPDOWN
		};

		Movement(Type t, uint8_t c, int16_t x, int16_t y, int16_t lx, int16_t ly, uint16_t f, uint8_t s, int16_t d)
				: type(t), command(c), xpos(x), ypos(y), lastx(lx), lasty(ly), fh(f), newstate(s), duration(d)
		{}

		Movement(int16_t x, int16_t y, int16_t lx, int16_t ly, uint8_t s, int16_t d) : Movement(Type::ABSOLUTE, 0, x, y,
																								lx, ly, 0, s, d)
		{}

		Movement(const PhysicsObject &phobj, uint8_t s) : Movement(Type::ABSOLUTE, 0, phobj.get_x(), phobj.get_y(),
																   phobj.get_last_x(), phobj.get_last_y(), phobj.fhid,
																   s, 1)
		{}

		Movement() : Movement(Type::NONE, 0, 0, 0, 0, 0)
		{}

		bool hasmoved(const Movement &newmove) const
		{
			return newmove.newstate != newstate || newmove.xpos != xpos || newmove.ypos != ypos ||
				   newmove.lastx != lastx || newmove.lasty != lasty;
		}

		Type type;
		uint8_t command;
		int16_t xpos;
		int16_t ypos;
		int16_t lastx;
		int16_t lasty;
		uint16_t fh;
		uint8_t newstate;
		int16_t duration;
	};
}