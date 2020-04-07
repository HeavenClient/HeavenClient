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

#include "../../Template/Enumeration.h"
#include "../../Template/EnumMap.h"

namespace ms
{
	namespace EquipSlot
	{
		enum Id : int16_t
		{
			NONE = 0,
			HAT = 1,
			FACE = 2,
			EYEACC = 3,
			EARACC = 4,
			TOP = 5,
			BOTTOM = 6,
			SHOES = 7,
			GLOVES = 8,
			CAPE = 9,
			SHIELD = 10, // TODO: Where is this now?
			WEAPON = 11,
			RING1 = 12,
			RING2 = 13,
			RING3 = 15,
			RING4 = 16,
			PENDANT1 = 17,
			TAMEDMOB = 18,	// TODO: Where is this now?
			SADDLE = 19,	// TODO: Where is this now?
			MEDAL = 49,
			BELT = 50,
			POCKET,		// TODO: What is the proper value for this?
			BOOK,		// TODO: What is the proper value for this?
			PENDANT2,	// TODO: What is the proper value for this?
			SHOULDER,	// TODO: What is the proper value for this?
			ANDROID,	// TODO: What is the proper value for this?
			EMBLEM,		// TODO: What is the proper value for this?
			BADGE,		// TODO: What is the proper value for this?
			SUBWEAPON,	// TODO: What is the proper value for this?
			HEART,		// TODO: What is the proper value for this?
			LENGTH
		};

		Id by_id(size_t id);

		constexpr Enumeration<Id> values;
	};
}
