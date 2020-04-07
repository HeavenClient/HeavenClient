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
#include "UIStateCashShop.h"

#include "UITypes/UICashShop.h"

namespace ms
{
	UIStateCashShop::UIStateCashShop()
	{
		focused = UIElement::NONE;

		emplace<UICashShop>();
	}

	void UIStateCashShop::draw(float inter, Point<int16_t> cursor) const
	{
		for (auto iter : elements)
		{
			UIElement* element = iter.second.get();

			if (element && element->is_active())
				element->draw(inter);
		}
	}

	void UIStateCashShop::update()
	{
		for (auto iter : elements)
		{
			UIElement* element = iter.second.get();

			if (element && element->is_active())
				element->update();
		}
	}

	Cursor::State UIStateCashShop::send_cursor(Cursor::State cursorstate, Point<int16_t> cursorpos)
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
				focused = UIElement::NONE;

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

	UIState::Iterator UIStateCashShop::pre_add(UIElement::Type type, bool toggled, bool is_focused)
	{
		remove(type);

		if (is_focused)
			focused = type;

		return elements.find(type);
	}

	void UIStateCashShop::remove(UIElement::Type type)
	{
		if (focused == type)
			focused = UIElement::Type::NONE;

		if (auto& element = elements[type])
		{
			element->deactivate();
			element.release();
		}
	}

	UIElement* UIStateCashShop::get(UIElement::Type type)
	{
		return elements[type].get();
	}

	UIElement* UIStateCashShop::get_front()
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

	void UIStateCashShop::remove_cursor(UIElement::Type type)
	{
		for (auto iter : elements)
		{
			auto& element = iter.second;

			if (element && element->is_active() && element->get_type() != type)
				element->remove_cursor();
		}
	}

	template <class T, typename...Args>
	void UIStateCashShop::emplace(Args&& ...args)
	{
		if (auto iter = pre_add(T::TYPE, T::TOGGLED, T::FOCUSED))
		{
			(*iter).second = std::make_unique<T>(
				std::forward<Args>(args)...
				);
		}
	}
}