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

#include <cstdint>

namespace ms
{
	namespace Weapon
	{
		enum Type
		{
			NONE = 0,
			SWORD_1H = 130,
			AXE_1H = 131,
			MACE_1H = 132,
			DAGGER = 133,
			WAND = 137,
			STAFF = 138,
			SWORD_2H = 140,
			AXE_2H = 141,
			MACE_2H = 142,
			SPEAR = 143,
			POLEARM = 144,
			BOW = 145,
			CROSSBOW = 146,
			CLAW = 147,
			KNUCKLE = 148,
			GUN = 149,
			CASH = 170
		};

		Type by_value(int32_t value);
	};
}