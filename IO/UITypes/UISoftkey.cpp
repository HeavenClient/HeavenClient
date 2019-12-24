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
#include "UISoftKey.h"

#include "../Constants.h"

#include "../Components/MapleButton.h"
#include "../Components/TwoSpriteButton.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UISoftkey::UISoftkey(OkCallback ok, CancelCallback cancel, std::string tooltip_text, Point<int16_t> tooltip_pos) : ok_callback(ok), cancel_callback(cancel), tooltipposition(tooltip_pos)
	{
		Point<int16_t> screen_adj = Point<int16_t>(-1, 0);

		nl::node SoftKey = nl::nx::ui["Login.img"]["Common"]["SoftKey"];
		nl::node backgrnd = SoftKey["backgrnd"];

		nl::node Tab = SoftKey["Tab"];
		nl::node TabNormal = Tab["normal"];
		nl::node TabSelected = Tab["selected"];

		sprites.emplace_back(backgrnd, screen_adj);
		sprites.emplace_back(SoftKey["backgrnd2"]);
		sprites.emplace_back(SoftKey["backgrnd3"]);

		buttons[Buttons::NEXT] = std::make_unique<MapleButton>(SoftKey["BtNext"]);
		buttons[Buttons::DEL] = std::make_unique<MapleButton>(SoftKey["BtDel"]);
		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(SoftKey["BtCancel"], Point<int16_t>(72, 235));
		buttons[Buttons::OK] = std::make_unique<MapleButton>(SoftKey["BtOK"], Point<int16_t>(14, 235));

		for (size_t i = 0; i < 3; i++)
		{
			buttons[Buttons::TAB0 + i] = std::make_unique<TwoSpriteButton>(TabNormal[i], TabSelected[i], screen_adj);

			if (i == 0)
				buttons[Buttons::TAB0 + i]->set_state(Button::State::PRESSED);
			else
				buttons[Buttons::TAB0 + i]->set_state(Button::State::DISABLED);
		}

		for (size_t i = 0; i < NUM_KEYS; i++)
			buttons[Buttons::NUM0 + i] = std::make_unique<MapleButton>(SoftKey["BtNum"][i]);

		entry = Textfield(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::LIGHTGREY, Rectangle<int16_t>(Point<int16_t>(-3, -4), Point<int16_t>(150, 24)), MAX_SIZE);
		entry.set_cryptchar('*');

		shufflekeys();
		show_text(tooltip_text);

		position = Point<int16_t>(330, 160);
		dimension = Texture(backgrnd).get_dimensions();
	}

	UISoftkey::UISoftkey(OkCallback ok_callback, CancelCallback cancel_callback, std::string tooltip_text) : UISoftkey(ok_callback, cancel_callback, tooltip_text, Point<int16_t>(0, 0)) {}
	UISoftkey::UISoftkey(OkCallback ok_callback, CancelCallback cancel_callback) : UISoftkey(ok_callback, cancel_callback, "") {}
	UISoftkey::UISoftkey(OkCallback ok_callback) : UISoftkey(ok_callback, []() {}) {}

	void UISoftkey::draw(float inter) const
	{
		UIElement::draw(inter);

		entry.draw(position + Point<int16_t>(15, 43));

		if (tooltip)
			tooltip->draw(position + Point<int16_t>(71, 46) + tooltipposition);
	}

	void UISoftkey::update()
	{
		UIElement::update();

		if (tooltip)
		{
			if (timestamp > 0)
				timestamp -= Constants::TIMESTEP;
			else
				clear_tooltip();
		}
	}

	void UISoftkey::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
				button_pressed(Buttons::CANCEL);
			else if (keycode == KeyAction::Id::RETURN)
				button_pressed(Buttons::OK);
		}
	}

	Button::State UISoftkey::button_pressed(uint16_t buttonid)
	{
		std::string entered = entry.get_text();
		size_t size = entered.size();

		if (buttonid == Buttons::DEL)
		{
			if (size > 0)
			{
				entered.pop_back();
				entry.change_text(entered);
			}
		}
		else if (buttonid == Buttons::CANCEL)
		{
			if (cancel_callback)
			{
				cancel_callback();
				deactivate();
			}
		}
		else if (buttonid == Buttons::OK)
		{
			if (size >= MIN_SIZE)
			{
				if (ok_callback)
				{
					ok_callback(entered);
					deactivate();
				}
			}
			else
			{
				clear_tooltip();
				show_text("The PIC needs to be at least 6 characters long.");
			}
		}
		else if (buttonid >= Buttons::NUM0)
		{
			if (size < MAX_SIZE)
			{
				entered.append(std::to_string(buttonid - Buttons::NUM0));
				entry.change_text(entered);
			}
		}

		return Button::State::NORMAL;
	}

	void UISoftkey::shufflekeys()
	{
		std::vector<uint8_t> reserve;

		for (size_t i = 0; i < NUM_KEYS; i++)
			reserve.push_back(i);

		for (size_t i = 0; i < NUM_KEYS; i++)
		{
			size_t rand = random.next_int(reserve.size());
			Point<int16_t> pos = keypos(reserve[rand]);

			buttons[Buttons::NUM0 + i]->set_position(pos);

			reserve.erase(reserve.begin() + rand);
		}
	}

	void UISoftkey::show_text(std::string text)
	{
		tetooltip.set_text(text);

		if (!text.empty())
		{
			tooltip = tetooltip;
			timestamp = 7 * 1000;
		}
	}

	void UISoftkey::clear_tooltip()
	{
		tooltipposition = Point<int16_t>(0, 0);
		tetooltip.set_text("");
		tooltip = Optional<Tooltip>();
	}

	Point<int16_t> UISoftkey::keypos(uint8_t num) const
	{
		auto x = (num % 3) * 39;
		auto y = (num / 3) * 35;

		return Point<int16_t>(12 + x, 94 + y);
	}
}