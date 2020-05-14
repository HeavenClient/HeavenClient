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
#include "Slider.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	Slider::Slider(int32_t t, Range<int16_t> ver, int16_t xp, int16_t ur, int16_t rm, std::function<void(bool)> om)
		: type(t), vertical(ver), x(xp), onmoved(om)
	{
		start = Point<int16_t>(x, vertical.first());
		end = Point<int16_t>(x, vertical.second());

		nl::node src;
		std::string base_str = "base";

		if (type == Type::CHATBAR)
		{
			src = nl::nx::ui["StatusBar3.img"]["chat"]["common"]["scroll"];
			base_str += "_c";
		}
		else
		{
			std::string VScr = "VScr";

			if (type != Type::LINE_CYAN)
				VScr += std::to_string(type);

			src = nl::nx::ui["Basic.img"][VScr];
		}

		nl::node dsrc = src["disabled"];

		dbase = dsrc[base_str];

		dprev = dsrc["prev"];
		dnext = dsrc["next"];

		nl::node esrc = src["enabled"];

		base = esrc[base_str];

		prev = TwoSpriteButton(esrc["prev0"], esrc["prev1"], start);
		next = TwoSpriteButton(esrc["next0"], esrc["next1"], end);

		thumb = TwoSpriteButton(esrc["thumb0"], esrc["thumb1"]);

		buttonheight = dnext.get_dimensions().y();

		setrows(ur, rm);

		enabled = true;
		scrolling = false;
	}

	Slider::Slider() : Slider(0, {}, 0, 0, 0, {})
	{}

	bool Slider::isenabled() const
	{
		return enabled;
	}

	void Slider::setenabled(bool en)
	{
		enabled = en;
	}

	void Slider::setrows(int16_t nr, int16_t ur, int16_t rm)
	{
		rowmax = rm - ur;

		if (rowmax > 0)
			rowheight = (vertical.length() - buttonheight * 2) / rowmax;
		else
			rowheight = 0;

		row = nr;
	}

	void Slider::setrows(int16_t ur, int16_t rm)
	{
		setrows(0, ur, rm);
	}

	void Slider::setvertical(Range<int16_t> ver)
	{
		vertical = ver;
		start = Point<int16_t>(x, vertical.first());
		end = Point<int16_t>(x, vertical.second());
		prev.set_position(start);
		next.set_position(end);

		if (rowmax > 0)
			rowheight = (vertical.length() - buttonheight * 2) / rowmax;
		else
			rowheight = 0;
	}

	Range<int16_t> Slider::getvertical() const
	{
		return vertical;
	}

	void Slider::draw(Point<int16_t> position) const
	{
		Point<int16_t> base_pos = position + start;
		Point<int16_t> fill = Point<int16_t>(0, vertical.length() + buttonheight - 2);
		DrawArgument base_arg = DrawArgument(Point<int16_t>(base_pos.x(), base_pos.y() + 1), fill);

		int16_t height = dbase.height();
		int16_t maxheight = vertical.first() + height;

		while (maxheight < vertical.second())
		{
			dbase.draw(position + Point<int16_t>(start.x(), maxheight));

			maxheight += height;
		}

		if (enabled)
		{
			if (rowheight > 0)
			{
				prev.draw(position);
				next.draw(position);
				thumb.draw(position + getthumbpos());
			}
			else
			{
				dprev.draw(position + start);
				dnext.draw(position + end);
			}
		}
		else
		{
			dprev.draw(position + start);
			dnext.draw(position + end);
		}
	}

	void Slider::remove_cursor()
	{
		scrolling = false;

		thumb.set_state(Button::State::NORMAL);
		next.set_state(Button::State::NORMAL);
		prev.set_state(Button::State::NORMAL);
	}

	Point<int16_t> Slider::getthumbpos() const
	{
		int16_t y =
			row < rowmax ?
			vertical.first() + row * rowheight + buttonheight :
			vertical.second() - buttonheight * 2 - 2;

		return Point<int16_t>(x, y);
	}

	Cursor::State Slider::send_cursor(Point<int16_t> cursor, bool pressed)
	{
		Point<int16_t> relative = cursor - start;

		if (scrolling)
		{
			if (pressed)
			{
				int16_t thumby = row * rowheight + buttonheight * 2;
				int16_t delta = relative.y() - thumby;

				if (delta > rowheight / 2 && row < rowmax)
				{
					row++;
					onmoved(false);
				}
				else if (delta < -rowheight / 2 && row > 0)
				{
					row--;
					onmoved(true);
				}

				return Cursor::State::VSCROLLIDLE;
			}
			else
			{
				scrolling = false;
			}
		}
		else if (relative.x() < 0 || relative.y() < 0 || relative.x() > 8 || relative.y() > vertical.second())
		{
			thumb.set_state(Button::State::NORMAL);
			next.set_state(Button::State::NORMAL);
			prev.set_state(Button::State::NORMAL);

			return Cursor::State::IDLE;
		}

		Point<int16_t> thumbpos = getthumbpos();

		if (thumb.bounds(thumbpos).contains(cursor))
		{
			if (pressed)
			{
				scrolling = true;
				thumb.set_state(Button::State::PRESSED);

				return Cursor::State::VSCROLLIDLE;
			}
			else
			{
				thumb.set_state(Button::State::NORMAL);

				return Cursor::State::VSCROLL;
			}
		}
		else
		{
			thumb.set_state(Button::State::NORMAL);
		}

		if (prev.bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row > 0)
				{
					row--;
					onmoved(true);
				}

				prev.set_state(Button::State::PRESSED);

				return Cursor::State::VSCROLLIDLE;
			}
			else
			{
				prev.set_state(Button::State::MOUSEOVER);

				return Cursor::State::VSCROLL;
			}
		}
		else
		{
			prev.set_state(Button::State::NORMAL);
		}

		if (next.bounds(Point<int16_t>()).contains(cursor))
		{
			if (pressed)
			{
				if (row < rowmax)
				{
					row++;
					onmoved(false);
				}

				next.set_state(Button::State::PRESSED);

				return Cursor::State::VSCROLLIDLE;
			}
			else
			{
				next.set_state(Button::State::MOUSEOVER);

				return Cursor::State::VSCROLL;
			}
		}
		else
		{
			next.set_state(Button::State::NORMAL);
		}

		if (cursor.y() < vertical.second())
		{
			if (pressed)
			{
				auto yoffset = static_cast<double>(relative.y() - buttonheight * 2);
				auto cursorrow = static_cast<int16_t>(std::round(yoffset / rowheight));

				if (cursorrow < 0)
					cursorrow = 0;
				else if (cursorrow > rowmax)
					cursorrow = rowmax;

				int16_t delta = row - cursorrow;

				for (size_t i = 0; i < 2; i++)
				{
					if (delta > 0)
					{
						row--;
						delta--;
						onmoved(true);
					}

					if (delta < 0)
					{
						row++;
						delta++;
						onmoved(false);
					}
				}

				return Cursor::State::VSCROLLIDLE;
			}
		}

		return Cursor::State::VSCROLL;
	}

	void Slider::send_scroll(double yoffset)
	{
		if (yoffset < 0 && row < rowmax)
		{
			row++;
			onmoved(false);
		}

		if (yoffset > 0 && row > 0)
		{
			row--;
			onmoved(true);
		}
	}
}