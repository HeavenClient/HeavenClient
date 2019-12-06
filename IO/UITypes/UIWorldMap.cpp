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
#include "UIWorldMap.h"

#include "../IO/Components/MapleButton.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIWorldMap::UIWorldMap() : UIDragElement<PosMAP>(Point<int16_t>(0, 0))
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		nl::node WorldMap = nl::nx::ui["UIWindow2.img"]["WorldMap"];
		nl::node WorldMapSearch = WorldMap["WorldMapSearch"];
		nl::node Border = WorldMap["Border"]["0"];
		nl::node backgrnd = WorldMapSearch["backgrnd"];

		sprites.emplace_back(Border);

		search_background = backgrnd;
		search_notice = WorldMapSearch["notice"];

		bg_dimensions = Texture(Border).get_dimensions();
		bg_search_dimensions = search_background.get_dimensions();
		background_dimensions = Point<int16_t>(bg_dimensions.x(), 0);

		Point<int16_t> close_dimensions = Point<int16_t>(bg_dimensions.x() - 22, 4);

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, close_dimensions);
		buttons[Buttons::BT_SEARCH] = std::make_unique<MapleButton>(WorldMap["BtSearch"]);
		buttons[Buttons::BT_AUTOFLY] = std::make_unique<MapleButton>(WorldMap["BtAutoFly_1"]);
		buttons[Buttons::BT_NAVIREG] = std::make_unique<MapleButton>(WorldMap["BtNaviRegister"]);
		buttons[Buttons::BT_SEARCH_CLOSE] = std::make_unique<MapleButton>(close, close_dimensions + Point<int16_t>(bg_search_dimensions.x(), 0));
		buttons[Buttons::BT_ALLSEARCH] = std::make_unique<MapleButton>(WorldMapSearch["BtAllsearch"], background_dimensions);

		Point<int16_t> search_text_pos = Point<int16_t>(bg_dimensions.x() + 14, 25);
		Point<int16_t> search_box_dim = Point<int16_t>(83, 15);
		Rectangle<int16_t> search_text_dim = Rectangle<int16_t>(search_text_pos, search_text_pos + search_box_dim);

		search_text = Textfield(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, search_text_dim, 15);

		set_search(false);
	}

	void UIWorldMap::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		if (search)
		{
			search_background.draw(position + background_dimensions);
			search_notice.draw(position + background_dimensions);
			search_text.draw(position);
		}

		UIElement::draw_buttons(alpha);
	}

	void UIWorldMap::update()
	{
		UIElement::update();

		if (search)
			search_text.update(position);
	}

	void UIWorldMap::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
			deactivate();
	}

	Button::State UIWorldMap::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case Buttons::BT_CLOSE:
			deactivate();
			break;
		case Buttons::BT_SEARCH:
			set_search(!search);
			break;
		case Buttons::BT_SEARCH_CLOSE:
			set_search(false);
			break;
		}

		return Button::State::NORMAL;
	}

	Cursor::State UIWorldMap::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = search_text.send_cursor(cursorpos, clicked))
			return new_state;

		return UIDragElement::send_cursor(clicked, cursorpos);
	}

	void UIWorldMap::set_search(bool enable)
	{
		search = enable;

		buttons[Buttons::BT_SEARCH_CLOSE]->set_active(enable);
		buttons[Buttons::BT_ALLSEARCH]->set_active(enable);

		if (enable)
		{
			search_text.set_state(Textfield::State::NORMAL);
			dimension = bg_dimensions + bg_search_dimensions;
		}
		else
		{
			search_text.set_state(Textfield::State::DISABLED);
			dimension = bg_dimensions;
		}

		dragarea = Point<int16_t>(dimension.x(), 20);
	}
}