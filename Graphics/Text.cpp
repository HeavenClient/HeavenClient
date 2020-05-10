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
#include "Text.h"

#include "GraphicsGL.h"

namespace ms
{
	Text::Text(Font f, Alignment a, Color::Name c, Background b, const std::string& t, uint16_t mw, bool fm, int16_t la) : font(f), alignment(a), color(c), background(b), maxwidth(mw), formatted(fm), line_adj(la)
	{
		change_text(t);
	}

	Text::Text(Font f, Alignment a, Color::Name c, const std::string& t, uint16_t mw, bool fm, int16_t la) : Text(f, a, c, Background::NONE, t, mw, fm, la) {}
	Text::Text() : Text(Font::A11M, Alignment::LEFT, Color::BLACK) {}

	void Text::reset_layout()
	{
		if (text.empty())
			return;

		layout = GraphicsGL::get().createlayout(text, font, alignment, maxwidth, formatted, line_adj);
	}

	void Text::change_text(const std::string& t)
	{
		if (text == t)
			return;

		text = t;

		reset_layout();
	}

	void Text::change_color(Color::Name c)
	{
		if (color == c)
			return;

		color = c;

		reset_layout();
	}

	void Text::set_background(Background b)
	{
		background = b;
	}

	void Text::draw(const DrawArgument& args) const
	{
		draw(args, Range<int16_t>(0, 0));
	}

	void Text::draw(const DrawArgument& args, const Range<int16_t>& vertical) const
	{
		GraphicsGL::get().drawtext(args, vertical, text, layout, font, color, background);
	}

	uint16_t Text::advance(size_t pos) const
	{
		return layout.advance(pos);
	}

	bool Text::empty() const
	{
		return text.empty();
	}

	size_t Text::length() const
	{
		return text.size();
	}

	int16_t Text::width() const
	{
		return layout.width();
	}

	int16_t Text::height() const
	{
		return layout.height();
	}

	Point<int16_t> Text::dimensions() const
	{
		return layout.get_dimensions();
	}

	Point<int16_t> Text::endoffset() const
	{
		return layout.get_endoffset();
	}

	const std::string& Text::get_text() const
	{
		return text;
	}

	Text::Layout::Layout(const std::vector<Layout::Line>& l, const std::vector<int16_t>& a, int16_t w, int16_t h, int16_t ex, int16_t ey) : lines(l), advances(a), dimensions(w, h), endoffset(ex, ey) {}
	Text::Layout::Layout() : Layout(std::vector<Layout::Line>(), std::vector<int16_t>(), 0, 0, 0, 0) {}

	int16_t Text::Layout::width() const
	{
		return dimensions.x();
	}

	int16_t Text::Layout::height() const
	{
		return dimensions.y();
	}

	int16_t Text::Layout::advance(size_t index) const
	{
		return index < advances.size() ? advances[index] : 0;
	}

	Point<int16_t> Text::Layout::get_dimensions() const
	{
		return dimensions;
	}

	Point<int16_t> Text::Layout::get_endoffset() const
	{
		return endoffset;
	}

	Text::Layout::iterator Text::Layout::begin() const
	{
		return lines.begin();
	}

	Text::Layout::iterator Text::Layout::end() const
	{
		return lines.end();
	}
}