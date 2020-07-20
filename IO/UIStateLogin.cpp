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
#include "UIStateLogin.h"

#include "UITypes/UICharSelect.h"
#include "UITypes/UILogin.h"
#include "UITypes/UILoginNotice.h"
#include "UITypes/UILogo.h"
#include "UITypes/UIRegion.h"

#include "../Configuration.h"

namespace ms
{
	UIStateLogin::UIStateLogin()
	{
		focused = UIElement::Type::NONE;

#ifndef DEBUG
		bool start_shown = Configuration::get().get_start_shown();

		if (!start_shown)
			emplace<UILogo>();
		else
#endif // !DEBUG
			emplace<UILogin>();
	}

	void UIStateLogin::draw(float inter, Point<int16_t> cursor) const
	{
		for (auto iter : elements)
		{
			UIElement* element = iter.second.get();

			if (element && element->is_active())
				element->draw(inter);
		}

		if (tooltip)
			tooltip->draw(cursor + Point<int16_t>(0, 22));
	}

	void UIStateLogin::update()
	{
		for (auto iter : elements)
		{
			UIElement* element = iter.second.get();

			if (element && element->is_active())
				element->update();
		}
	}

	void UIStateLogin::doubleclick(Point<int16_t> pos)
	{
		if (auto charselect = UI::get().get_element<UICharSelect>())
			charselect->doubleclick(pos);
	}

	void UIStateLogin::send_key(KeyType::Id type, int32_t action, bool pressed, bool escape)
	{
		if (UIElement* focusedelement = get(focused))
		{
			if (focusedelement->is_active())
			{
				return focusedelement->send_key(action, pressed, escape);
			}
			else
			{
				focused = UIElement::Type::NONE;

				return;
			}
		}
	}

	Cursor::State UIStateLogin::send_cursor(Cursor::State cursorstate, Point<int16_t> cursorpos)
	{
		bool clicked = cursorstate == Cursor::State::CLICKING || cursorstate == Cursor::State::VSCROLLIDLE;

		if (auto focusedelement = get(focused))
		{
			if (focusedelement->is_active())
			{
				remove_cursor(focusedelement->get_type());

				return focusedelement->send_cursor(clicked, cursorpos);
			}
			else
			{
				focused = UIElement::Type::NONE;

				return cursorstate;
			}
		}
		else
		{
			if (auto front = get_front())
			{
				remove_cursor(front->get_type());

				return front->send_cursor(clicked, cursorpos);
			}
			else
			{
				return Cursor::State::IDLE;
			}
		}
	}

	void UIStateLogin::send_close()
	{
		auto logo = UI::get().get_element<UILogo>();
		auto login = UI::get().get_element<UILogin>();
		auto region = UI::get().get_element<UIRegion>();

		if (logo && logo->is_active() || login && login->is_active() || region && region->is_active())
			UI::get().quit();
		else
			UI::get().emplace<UIQuitConfirm>();
	}

	void UIStateLogin::clear_tooltip(Tooltip::Parent parent)
	{
		if (parent == tooltipparent)
		{
			tetooltip.set_text("");
			tooltip = {};
			tooltipparent = Tooltip::Parent::NONE;
		}
	}

	void UIStateLogin::show_text(Tooltip::Parent parent, std::string text)
	{
		tetooltip.set_text(text);

		if (!text.empty())
		{
			tooltip = tetooltip;
			tooltipparent = parent;
		}
	}

	template <class T, typename...Args>
	void UIStateLogin::emplace(Args&& ...args)
	{
		if (auto iter = pre_add(T::TYPE, T::TOGGLED, T::FOCUSED))
		{
			(*iter).second = std::make_unique<T>(
				std::forward<Args>(args)...
				);
		}
	}

	UIState::Iterator UIStateLogin::pre_add(UIElement::Type type, bool, bool is_focused)
	{
		remove(type);

		if (is_focused)
			focused = type;

		return elements.find(type);
	}

	void UIStateLogin::remove(UIElement::Type type)
	{
		if (focused == type)
			focused = UIElement::Type::NONE;

		if (auto& element = elements[type])
		{
			element->deactivate();
			element.release();
		}
	}

	UIElement* UIStateLogin::get(UIElement::Type type)
	{
		return elements[type].get();
	}

	UIElement* UIStateLogin::get_front()
	{
		UIElement* front = nullptr;

		for (auto iter : elements)
		{
			auto& element = iter.second;

			if (element && element->is_active())
				front = element.get();
		}

		return front;
	}

	UIElement* UIStateLogin::get_front(std::list<UIElement::Type> types)
	{
		auto begin = types.rbegin();
		auto end = types.rend();

		for (auto iter = begin; iter != end; ++iter)
		{
			auto& element = elements[*iter];

			if (element && element->is_active())
				return element.get();
		}

		return nullptr;
	}

	UIElement* UIStateLogin::get_front(Point<int16_t> pos)
	{
		auto begin = elements.values().rbegin();
		auto end = elements.values().rend();

		for (auto iter = begin; iter != end; ++iter)
		{
			auto& element = *iter;

			if (element && element->is_active() && element->is_in_range(pos))
				return element.get();
		}

		return nullptr;
	}

	void UIStateLogin::remove_cursor(UIElement::Type type)
	{
		for (auto iter : elements)
		{
			auto& element = iter.second;

			if (element && element->is_active() && element->get_type() != type)
				element->remove_cursor();
		}
	}
}