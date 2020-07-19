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

#include "UIState.h"

#include "Components/TextTooltip.h"

#include "../Template/EnumMap.h"

#include <memory>
#include <list>

namespace ms
{
	class UIStateLogin : public UIState
	{
	public:
		UIStateLogin();

		void draw(float inter, Point<int16_t> cursor) const override;

		void update() override;

		void doubleclick(Point<int16_t> pos) override;

		void rightclick(Point<int16_t>) override
		{}

		void send_key(KeyType::Id type, int32_t action, bool pressed, bool escape) override;

		Cursor::State send_cursor(Cursor::State mst, Point<int16_t> pos) override;

		void send_scroll(double) override
		{}

		void send_close() override;

		void drag_icon(Icon*) override
		{}

		void clear_tooltip(Tooltip::Parent parent) override;

		void show_equip(Tooltip::Parent, int16_t) override
		{}

		void show_item(Tooltip::Parent, int32_t) override
		{}

		void show_skill(Tooltip::Parent, int32_t, int32_t, int32_t, int64_t) override
		{}

		void show_text(Tooltip::Parent parent, std::string text) override;

		void show_map(Tooltip::Parent, std::string, std::string, int32_t, bool)
		{}

		Iterator pre_add(UIElement::Type type, bool toggled, bool focused) override;

		void remove(UIElement::Type type) override;

		UIElement* get(UIElement::Type type) override;

		UIElement* get_front();

		UIElement* get_front(std::list<UIElement::Type> types) override;

		UIElement* get_front(Point<int16_t> pos) override;

	private:
		void remove_cursor(UIElement::Type type);

		template <class T, typename...Args>
		void emplace(Args&& ...args);

		EnumMap<UIElement::Type, UIElement::UPtr, UIElement::Type::NUM_TYPES> elements;
		UIElement::Type focused;

		TextTooltip tetooltip;
		Optional<Tooltip> tooltip;
		Tooltip::Parent tooltipparent;
	};
}