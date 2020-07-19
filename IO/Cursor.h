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

#include "../Graphics/Animation.h"
#include "../Template/EnumMap.h"

namespace ms
{
	// Class that represents the mouse cursor
	class Cursor
	{
	public:
		// Maple cursor states that are linked to the cursor's animation
		enum State
		{
			IDLE,
			CANCLICK,
			GAME,
			HOUSE,
			CANCLICK2,
			CANGRAB,
			GIFT,
			VSCROLL,
			HSCROLL,
			VSCROLLIDLE,
			HSCROLLIDLE,
			GRABBING,
			CLICKING,
			RCLICK,
			LEAF = 18,
			CHATBARVDRAG = 67,
			CHATBARHDRAG,
			CHATBARBLTRDRAG,
			CHATBARMOVE = 72,
			CHATBARBRTLDRAG,
			LENGTH
		};

		Cursor();

		void init();

		void draw(float alpha) const;

		void update();

		void set_state(State state);

		void set_position(Point<int16_t> position);

		State get_state() const;

		Point<int16_t> get_position() const;

	private:
		EnumMap<State, Animation> animations;

		State state;
		Point<int16_t> position;
		int32_t hide_counter;

		static constexpr int64_t HIDE_TIME = 15'000;
	};
}