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
	namespace KeyConfig
	{
		enum Key : uint8_t
		{
			NUM0 = 2,
			NUM1,
			NUM2,
			NUM3,
			NUM4,
			NUM5,
			NUM6,
			NUM7,
			NUM8,
			NUM9,
			MINUS,
			EQUAL,
			Q = 16,
			W,
			E,
			R,
			T,
			Y,
			U,
			I,
			O,
			P,
			LEFT_BRACKET,
			RIGHT_BRACKET,
			LEFT_CONTROL = 29,
			A,
			S,
			D,
			F,
			G,
			H,
			J,
			K,
			L,
			SEMICOLON,
			APOSTROPHE,
			GRAVE_ACCENT,
			LEFT_SHIFT,
			BACKSLASH,
			Z,
			X,
			C,
			V,
			B,
			N,
			M,
			COMMA,
			PERIOD,
			LEFT_ALT = 56,
			SPACE,
			F1 = 59,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			HOME,
			PAGE_UP = 73,
			END = 79,
			PAGE_DOWN = 81,
			INSERT,
			DELETE,
			ESCAPE,
			RIGHT_CONTROL,
			RIGHT_SHIFT,
			RIGHT_ALT,
			SCROLL_LOCK,
			LENGTH
		};

		inline Key actionbyid(int32_t id)
		{
			return static_cast<Key>(id);
		}
	}
}