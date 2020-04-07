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

#include "TwoSpriteButton.h"

#include <functional>

namespace ms
{
	class Slider
	{
	public:
		Slider(int32_t type, Range<int16_t> vertical, int16_t x, int16_t unitrows, int16_t rowmax,
			   std::function<void(bool upwards)> onmoved);

		Slider();

		bool isenabled() const;

		void setenabled(bool enabled);

		void setrows(int16_t newrow, int16_t unitrows, int16_t rowmax);

		void setrows(int16_t unitrows, int16_t rowmax);

		void setvertical(Range<int16_t> vertical);
		Range<int16_t> getvertical() const;

		void draw(Point<int16_t> position) const;

		void remove_cursor();

		Cursor::State send_cursor(Point<int16_t> cursor, bool pressed);

		void send_scroll(double yoffset);

		enum Type
		{
			/// Default
			LINE_CYAN,
			LINE_CONTESSA = 2,
			SMALL_HAVELOCKBLUE,
			NORMAL_CALYPSO,
			NORMAL_ROCKBLUE,
			LINE_PUNGA,
			LINE_YELLOWMETAL,
			NORMAL_JUDGEGRAY,
			DEFAULT_SILVER,
			LINE_MINESHAFT,
			DEFAULT_ALTO,
			DEFAULT_SANDAL,
			DEFAULT_QUICKSAND,
			LINE_HOTCINNAMON,
			THIN_DUSTYGRAY_LIGHT,
			THIN_MINESHAFT,
			THIN_DUSTYGRAY,
			THIN_MINESHAFT_LIGHT,
			THIN_WOODYBROWN,
			BLIZZARDBLUE,
			DEFAULT_ARROWTOWN = 100,
			THIN_ZORBA,
			ARROWS_IRISHCOFFEE,
			THIN_MIKADO,
			ARROWS_TORYBLUE,
			THIN_SLATEGRAY,
			/// Custom
			CHATBAR
		};

	private:
		Point<int16_t> getthumbpos() const;

		std::function<void(bool upwards)> onmoved;

		Range<int16_t> vertical;
		Point<int16_t> start;
		Point<int16_t> end;
		int16_t type;
		int16_t buttonheight;
		int16_t rowheight;
		int16_t x;
		int16_t row;
		int16_t rowmax;
		bool scrolling;
		bool enabled;

		Texture dbase;
		Texture dnext;
		Texture dprev;
		Texture base;
		TwoSpriteButton next;
		TwoSpriteButton prev;
		TwoSpriteButton thumb;
	};
}