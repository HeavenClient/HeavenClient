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

#include "Components/ScrollingNotice.h"
#include "Components/Textfield.h"

namespace ms
{
	class UI : public Singleton<UI>
	{
	public:
		enum State
		{
			LOGIN,
			GAME,
			CASHSHOP
		};

		UI();

		void init();

		void draw(float alpha) const;

		void update();

		void enable();

		void disable();

		void change_state(State state);

		void quit();

		bool not_quitted() const;

		void send_cursor(Point<int16_t> pos);

		void send_cursor(bool pressed);

		void send_cursor(Point<int16_t> cursorpos, Cursor::State cursorstate);

		void send_focus(int focused);

		void send_scroll(double yoffset);

		void send_close();

		void rightclick();

		void doubleclick();

		void send_key(int32_t keycode, bool pressed);

		void set_scrollnotice(const std::string& notice);

		void focus_textfield(Textfield* textfield);

		void remove_textfield();

		void drag_icon(Icon* icon);

		void add_keymapping(uint8_t no, uint8_t type, int32_t action);

		void clear_tooltip(Tooltip::Parent parent);

		void show_equip(Tooltip::Parent parent, int16_t slot);

		void show_item(Tooltip::Parent parent, int32_t item_id);

		void
		show_skill(Tooltip::Parent parent, int32_t skill_id, int32_t level, int32_t masterlevel, int64_t expiration);

		void show_text(Tooltip::Parent parent, std::string text);

		void show_map(Tooltip::Parent parent, std::string name, std::string description, int32_t mapid, bool bolded);

		Keyboard& get_keyboard();

		int64_t get_uptime();

		uint16_t get_uplevel();

		int64_t get_upexp();

		template <class T, typename...Args>
		Optional<T> emplace(Args&& ...args);

		template <class T>
		Optional<T> get_element();

		void remove(UIElement::Type type);

	private:
		std::unique_ptr<UIState> state;
		Keyboard keyboard;
		Cursor cursor;
		ScrollingNotice scrollingnotice;

		Optional<Textfield> focusedtextfield;
		std::unordered_map<int32_t, bool> is_key_down;

		bool enabled;
		bool quitted;
		bool caps_lock_enabled = false;
	};

	template <class T, typename...Args>
	Optional<T> UI::emplace(Args&& ...args)
	{
		if (auto iter = state->pre_add(T::TYPE, T::TOGGLED, T::FOCUSED))
		{
			(*iter).second = std::make_unique<T>(
				std::forward<Args>(args)...
			);
		}

		return state->get(T::TYPE);
	}

	template <class T>
	Optional<T> UI::get_element()
	{
		UIElement::Type type = T::TYPE;
		UIElement* element = state->get(type);

		return static_cast<T*>(element);
	}
}