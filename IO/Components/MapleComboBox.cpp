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
#include "MapleComboBox.h"

#include "AreaButton.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	MapleComboBox::MapleComboBox(Type type, std::vector<std::string> o, uint16_t default_option, Point<int16_t> ppos,
								 Point<int16_t> pos, int64_t w) : options(o), selected_index(default_option),
																  parentpos(ppos), rwidth(w)
	{
		std::string combobox = "ComboBox";

		if (type != Type::DEFAULT)
			combobox += std::to_string(type);

		nl::node src = nl::nx::ui["Basic.img"][combobox];

		textures[Button::State::PRESSED][0] = src["pressed"][0];
		textures[Button::State::PRESSED][1] = src["pressed"][1];
		textures[Button::State::PRESSED][2] = src["pressed"][2];

		textures[Button::State::MOUSEOVER][0] = src["mouseOver"][0];
		textures[Button::State::MOUSEOVER][1] = src["mouseOver"][1];
		textures[Button::State::MOUSEOVER][2] = src["mouseOver"][2];

		textures[Button::State::NORMAL][0] = src["normal"][0];
		textures[Button::State::NORMAL][1] = src["normal"][1];
		textures[Button::State::NORMAL][2] = src["normal"][2];

		textures[Button::State::DISABLED][0] = src["disabled"][0];
		textures[Button::State::DISABLED][1] = src["disabled"][1];
		textures[Button::State::DISABLED][2] = src["disabled"][2];

		for (auto &option : options)
			option_text.push_back(Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::BLACK, option));

		Text::Font selected_font = Text::Font::A12M;
		Color::Name selected_color = Color::Name::BLACK;
		selected_adj = Point<int16_t>(2, -3);

		if (type == Type::BLACKL)
		{
			selected_font = Text::Font::A11M;
			selected_color = Color::Name::WHITE;
			selected_adj = Point<int16_t>(11, 2);
		}

		selected = Text(selected_font, Text::Alignment::LEFT, selected_color, options[selected_index]);

		state = Button::State::NORMAL;
		background = ColorBox(width(), options.size() * HEIGHT, Color::Name::DUSTYGRAY, 1.0f);
		rect = ColorBox(width() - 2, options.size() * HEIGHT - 2, Color::Name::GALLERY, 1.0f);
		current_rect = ColorBox(width() - 2, HEIGHT - 2, Color::Name::GRAYOLIVE, 1.0f);

		Point<int16_t> option_pos =
				Point<int16_t>(position.x(), position.y() + textures[state][0].get_dimensions().y()) + parentpos;

		for (size_t i = 0; i < option_text.size(); i++)
			buttons[i] = std::make_unique<AreaButton>(
					Point<int16_t>(option_pos.x() + 1, option_pos.y() + (i * HEIGHT) + 1),
					Point<int16_t>(width() - 2, HEIGHT - 2));

		current_pos = 0;
		current_shown = false;
		last_shown = 0;

		position = pos;
		active = true;
		pressed = false;
	}

	void MapleComboBox::draw(Point<int16_t>) const
	{
		if (active)
		{
			Point<int16_t> lpos = Point<int16_t>(position + parentpos);

			textures[state][0].draw(lpos);
			lpos.shift_x(textures[state][0].width());

			int16_t middle_width = textures[state][1].width();
			int16_t current_width = middle_width;

			while (current_width < rwidth)
			{
				textures[state][1].draw(lpos);
				lpos.shift_x(middle_width);
				current_width += middle_width;
			}

			textures[state][2].draw(lpos);

			selected.draw(Point<int16_t>(position + parentpos) + selected_adj);

			if (pressed)
			{
				Point<int16_t> pos =
						Point<int16_t>(position.x(), position.y() + textures[state][0].get_dimensions().y()) +
						parentpos;

				background.draw(pos + Point<int16_t>(0, 2));
				rect.draw(pos + Point<int16_t>(1, 3));

				if (current_shown)
					current_rect.draw(DrawArgument(pos.x() + 1, pos.y() + current_pos + 3));

				for (size_t i = 0; i < option_text.size(); i++)
					option_text[i].draw(DrawArgument(pos.x() + 6, pos.y() + (i * HEIGHT) - 4));
			}
		}
	}

	Rectangle<int16_t> MapleComboBox::bounds(Point<int16_t>) const
	{
		auto lt = parentpos + position - origin();
		auto rb = lt + textures[state][0].get_dimensions();

		auto end = textures[state][2].get_dimensions();

		rb = Point<int16_t>(rb.x() + end.x() + rwidth, rb.y());

		return Rectangle<int16_t>(lt, rb);
	}

	int16_t MapleComboBox::width() const
	{
		return textures[state][0].width() + textures[state][2].width() + rwidth;
	}

	Point<int16_t> MapleComboBox::origin() const
	{
		return textures[state][0].get_origin();
	}

	Cursor::State MapleComboBox::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Cursor::State ret = clicked ? Cursor::State::CLICKING : Cursor::State::IDLE;

		current_shown = false;
		option_text[last_shown].change_color(Color::Name::BLACK);

		for (auto &btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					btit.second->set_state(Button::State::MOUSEOVER);
					ret = Cursor::State::CANCLICK;
				} else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));

						ret = Cursor::State::IDLE;
					} else
					{
						ret = Cursor::State::CANCLICK;
						current_pos = btit.first * HEIGHT;
						current_shown = true;
						last_shown = btit.first;
						option_text[btit.first].change_color(Color::Name::WHITE);
					}
				}
			} else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				btit.second->set_state(Button::State::NORMAL);
			}
		}

		return ret;
	}

	bool MapleComboBox::in_combobox(Point<int16_t> cursorpos)
	{
		Point<int16_t> lt =
				Point<int16_t>(position.x() + 1, position.y() + textures[state][0].get_dimensions().y() + 1) +
				parentpos;
		Point<int16_t> rb = lt + Point<int16_t>(width() - 2, options.size() * HEIGHT - 2);

		return Rectangle<int16_t>(lt, rb).contains(cursorpos);
	}

	uint16_t MapleComboBox::get_selected() const
	{
		return selected_index;
	}

	Button::State MapleComboBox::button_pressed(uint16_t buttonid)
	{
		selected_index = buttonid;

		selected.change_text(options[selected_index]);

		toggle_pressed();

		return Button::State::NORMAL;
	}
}