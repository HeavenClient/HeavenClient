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

#include "Text.h"

namespace ms
{
	struct OutlinedText
	{
		Text inner;
		Text l;
		Text r;
		Text t;
		Text b;

		OutlinedText(Text::Font font, Text::Alignment alignment, Color::Name innerColor, Color::Name outerColor)
		{
			inner = Text(font, alignment, innerColor);
			l = Text(font, alignment, outerColor);
			r = Text(font, alignment, outerColor);
			t = Text(font, alignment, outerColor);
			b = Text(font, alignment, outerColor);
		}

		OutlinedText()
		{}

		void draw(Point <int16_t> parentpos) const
		{
			l.draw(parentpos + Point<int16_t>(-1, 0));
			r.draw(parentpos + Point<int16_t>(1, 0));
			t.draw(parentpos + Point<int16_t>(0, -1));
			b.draw(parentpos + Point<int16_t>(0, 1));
			inner.draw(parentpos);
		}

		void change_text(const std::string &text)
		{
			inner.change_text(text);
			l.change_text(text);
			r.change_text(text);
			t.change_text(text);
			b.change_text(text);
		}

		void change_color(Color::Name color)
		{
			inner.change_color(color);
		}

		int16_t width() const
		{
			return inner.width();
		}
	};

	struct ShadowText
	{
		Text top;
		Text shadow;

		ShadowText(Text::Font font, Text::Alignment alignment, Color::Name topColor, Color::Name shadowColor)
		{
			top = Text(font, alignment, topColor);
			shadow = Text(font, alignment, shadowColor);
		}

		ShadowText()
		{}

		void draw(Point <int16_t> parentpos) const
		{
			shadow.draw(parentpos + Point<int16_t>(1, 1));
			top.draw(parentpos);
		}

		void change_text(const std::string &text)
		{
			top.change_text(text);
			shadow.change_text(text);
		}

		void change_color(Color::Name color)
		{
			top.change_color(color);
		}

		int16_t width() const
		{
			return top.width();
		}
	};
}