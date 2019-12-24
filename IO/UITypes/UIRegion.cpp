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
#include "UIRegion.h"
#include "UIWorldSelect.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIRegion::UIRegion() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600))
	{
		nl::node Common = nl::nx::ui["Login.img"]["Common"];
		nl::node Gateway = nl::nx::ui["Gateway.img"]["WorldSelect"];
		nl::node na = Gateway["BtButton0"];
		nl::node eu = Gateway["BtButton1"];

		sprites.emplace_back(Gateway["backgrnd2"]);
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));

		int16_t pos_y = 84;
		Point<int16_t> na_pos = Point<int16_t>(155, pos_y);
		Point<int16_t> eu_pos = Point<int16_t>(424, pos_y);

		buttons[Buttons::NA] = std::make_unique<MapleButton>(na, na_pos);
		buttons[Buttons::EU] = std::make_unique<MapleButton>(eu, eu_pos);
		buttons[Buttons::EXIT] = std::make_unique<MapleButton>(Common["BtExit"], Point<int16_t>(0, 540));

		Point<int16_t> na_dim = Texture(na["normal"]["0"]).get_dimensions();
		Point<int16_t> eu_dim = Texture(eu["normal"]["0"]).get_dimensions();

		na_rect = Rectangle<int16_t>(na_pos, na_pos + na_dim);
		eu_rect = Rectangle<int16_t>(eu_pos, eu_pos + eu_dim);
	}

	Cursor::State UIRegion::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		clear_tooltip();

		if (na_rect.contains(cursorpos))
			UI::get().show_text(Tooltip::Parent::TEXT, "Warning: You may experience latency and connection issues when connecting to the NA server from Europe.");

		if (eu_rect.contains(cursorpos))
			UI::get().show_text(Tooltip::Parent::TEXT, "Warning: You may experience latency and connection issues when connecting to the EU server from North America.");

		return UIElement::send_cursor(clicked, cursorpos);
	}

	Button::State UIRegion::button_pressed(uint16_t buttonid)
	{
		clear_tooltip();

		switch (buttonid)
		{
		case Buttons::NA:
		case Buttons::EU:
		{
			// TODO: Update UIWorldSelect after selecting new region
			//uint8_t region = (buttonid == Buttons::NA) ? 5 : 6;

			if (auto worldselect = UI::get().get_element<UIWorldSelect>())
			{
				UI::get().remove(UIElement::Type::REGION);

				//worldselect->set_region(region);
				worldselect->makeactive();
			}

			break;
		}
		case Buttons::EXIT:
			UI::get().quit();
			break;
		default:
			break;
		}

		return Button::State::NORMAL;
	}

	void UIRegion::clear_tooltip()
	{
		UI::get().clear_tooltip(Tooltip::Parent::TEXT);
	}
}