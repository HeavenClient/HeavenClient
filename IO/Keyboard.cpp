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
#include "Keyboard.h"

#ifdef __linux__
#include <GLFW/glfw3.h>
#else
#include <glfw3.h>
#endif

namespace ms
{
	constexpr int32_t Keytable[90] =
			{
					0, 0, // 1
					GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7,
					GLFW_KEY_8, GLFW_KEY_9, GLFW_KEY_MINUS, GLFW_KEY_EQUAL,
					0, 0, // 15
					GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T, GLFW_KEY_Y, GLFW_KEY_U, GLFW_KEY_I,
					GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET,
					0, // 28
					GLFW_KEY_LEFT_CONTROL, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H,
					GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_GRAVE_ACCENT,
					GLFW_KEY_LEFT_SHIFT, GLFW_KEY_BACKSLASH, GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B,
					GLFW_KEY_N, GLFW_KEY_M, GLFW_KEY_COMMA, GLFW_KEY_PERIOD,
					0, 0, 0, // 55
					GLFW_KEY_LEFT_ALT, GLFW_KEY_SPACE,
					0, // 58
					GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7,
					GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_HOME,
					0, // 72
					GLFW_KEY_PAGE_UP,
					0, 0, 0, 0, 0, // 78
					GLFW_KEY_END,
					0, // 80
					GLFW_KEY_PAGE_DOWN, GLFW_KEY_INSERT, GLFW_KEY_DELETE, GLFW_KEY_ESCAPE, GLFW_KEY_RIGHT_CONTROL,
					GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_ALT, GLFW_KEY_SCROLL_LOCK
			};

	constexpr int32_t Shifttable[126] =
			{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  10
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  20
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //  30
					0, 0, 49, 39, 51, 52, 53, 55, 0, 57, //  40
					48, 56, 61, 0, 0, 0, 0, 0, 0, 0, //  50
					0, 0, 0, 0, 0, 0, 0, 59, 0, 44, //  60
					0, 46, 47, 50, 97, 98, 99, 100, 101, 102, //  70
					103, 104, 105, 106, 107, 108, 109, 110, 111, 112, //  80
					113, 114, 115, 116, 117, 118, 119, 120, 121, 122, //  90
					0, 0, 0, 54, 45, 0, 0, 0, 0, 0, // 100
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 110
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 120
					0, 0, 91, 92, 93, 96                     // 126
			};

	constexpr int32_t Specialtable[96] =
			{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 10
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 20
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 30
					0, 0, 0, 0, 0, 0, 0, 0, 34, 0, // 40
					0, 0, 0, 60, 95, 62, 63, 41, 33, 64, // 50
					35, 36, 37, 94, 38, 42, 40, 0, 58, 0, // 60
					43, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 70
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 90
					123, 124, 125, 0, 0, 126                     // 96
			};

	Keyboard::Keyboard()
	{
		keymap[GLFW_KEY_LEFT] = Mapping(KeyType::Id::ACTION, KeyAction::Id::LEFT);
		keymap[GLFW_KEY_RIGHT] = Mapping(KeyType::Id::ACTION, KeyAction::Id::RIGHT);
		keymap[GLFW_KEY_UP] = Mapping(KeyType::Id::ACTION, KeyAction::Id::UP);
		keymap[GLFW_KEY_DOWN] = Mapping(KeyType::Id::ACTION, KeyAction::Id::DOWN);
		keymap[GLFW_KEY_ENTER] = Mapping(KeyType::Id::ACTION, KeyAction::Id::RETURN);
		keymap[GLFW_KEY_KP_ENTER] = Mapping(KeyType::Id::ACTION, KeyAction::Id::RETURN);
		keymap[GLFW_KEY_TAB] = Mapping(KeyType::Id::ACTION, KeyAction::Id::TAB);

		textactions[GLFW_KEY_BACKSPACE] = KeyAction::Id::BACK;
		textactions[GLFW_KEY_ENTER] = KeyAction::Id::RETURN;
		textactions[GLFW_KEY_KP_ENTER] = KeyAction::Id::RETURN;
		textactions[GLFW_KEY_SPACE] = KeyAction::Id::SPACE;
		textactions[GLFW_KEY_TAB] = KeyAction::Id::TAB;
		textactions[GLFW_KEY_ESCAPE] = KeyAction::Id::ESCAPE;
		textactions[GLFW_KEY_HOME] = KeyAction::Id::HOME;
		textactions[GLFW_KEY_END] = KeyAction::Id::END;
		textactions[GLFW_KEY_DELETE] = KeyAction::Id::DELETE;
	}

	int32_t Keyboard::leftshiftcode() const
	{
		return GLFW_KEY_LEFT_SHIFT;
	}

	int32_t Keyboard::rightshiftcode() const
	{
		return GLFW_KEY_RIGHT_SHIFT;
	}

	int32_t Keyboard::capslockcode() const
	{
		return GLFW_KEY_CAPS_LOCK;
	}

	int32_t Keyboard::leftctrlcode() const
	{
		return GLFW_KEY_LEFT_CONTROL;
	}

	int32_t Keyboard::rightctrlcode() const
	{
		return GLFW_KEY_LEFT_CONTROL;
	}

	std::map<int32_t, Keyboard::Mapping> Keyboard::get_maplekeys() const
	{
		return maplekeys;
	}

	KeyAction::Id Keyboard::get_ctrl_action(int32_t keycode) const
	{
		switch (keycode)
		{
			case GLFW_KEY_C:
				return KeyAction::Id::COPY;
			case GLFW_KEY_V:
				return KeyAction::Id::PASTE;
				/*case GLFW_KEY_A:
					return KeyAction::Id::SELECTALL;*/
			default:
				return KeyAction::Id::LENGTH;
		}
	}

	void Keyboard::assign(uint8_t key, uint8_t tid, int32_t action)
	{
		if (KeyType::Id type = KeyType::typebyid(tid))
		{
			Mapping mapping = Mapping(type, action);

			keymap[Keytable[key]] = mapping;
			maplekeys[key] = mapping;
		}
	}

	void Keyboard::remove(uint8_t key)
	{
		Mapping mapping = Mapping(KeyType::Id::NONE, 0);

		keymap[Keytable[key]] = mapping;
		maplekeys[key] = mapping;
	}

	Keyboard::Mapping Keyboard::get_text_mapping(int32_t keycode, bool shift) const
	{
		if (textactions.count(keycode))
		{
			return Mapping(KeyType::Id::ACTION, textactions.at(keycode));
		} else if (keycode == 39 || (keycode >= 44 && keycode <= 57) || keycode == 59 || keycode == 61 ||
				   (keycode >= 91 && keycode <= 93) || keycode == 96)
		{
			if (!shift)
				return Mapping(KeyType::Id::TEXT, keycode);
			else
				return Mapping(KeyType::Id::TEXT, Specialtable[keycode - 1]);
		} else if (keycode >= 33 && keycode <= 126)
		{
			if (shift)
				return Mapping(KeyType::Id::TEXT, keycode);
			else
				return Mapping(KeyType::Id::TEXT, Shifttable[keycode - 1]);
		} else
		{
			switch (keycode)
			{
				case GLFW_KEY_LEFT:
				case GLFW_KEY_RIGHT:
				case GLFW_KEY_UP:
				case GLFW_KEY_DOWN:
					return keymap.at(keycode);
				default:
					return Mapping(KeyType::Id::NONE, 0);
			}
		}
	}

	Keyboard::Mapping Keyboard::get_mapping(int32_t keycode) const
	{
		auto iter = keymap.find(keycode);

		if (iter == keymap.end())
			return Mapping(KeyType::Id::NONE, 0);

		return iter->second;
	}

	Keyboard::Mapping Keyboard::get_maple_mapping(int32_t keycode) const
	{
		auto iter = maplekeys.find(keycode);

		if (iter == maplekeys.end())
			return Mapping(KeyType::Id::NONE, 0);

		return iter->second;
	}
}