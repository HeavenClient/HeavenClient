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

#include "KeyAction.h"
#include "KeyType.h"

#include <map>

namespace ms
{
	class Keyboard
	{
	public:
		struct Mapping
		{
			KeyType::Id type;
			int32_t action;

			Mapping() : type(KeyType::Id::NONE), action(0) {}
			Mapping(KeyType::Id in_type, int32_t in_action) : type(in_type), action(in_action) {}

			bool operator==(const Mapping& other) const
			{
				return type == other.type && action == other.action;
			}

			bool operator!=(const Mapping& other) const
			{
				return type != other.type || action != other.action;
			}
		};

		Keyboard();

		void assign(uint8_t key, uint8_t type, int32_t action);
		void remove(uint8_t key);

		int32_t leftshiftcode() const;
		int32_t rightshiftcode() const;
		int32_t capslockcode() const;
		int32_t leftctrlcode() const;
		int32_t rightctrlcode() const;
		std::map<int32_t, Mapping> get_maplekeys() const;
		KeyAction::Id get_ctrl_action(int32_t keycode) const;
		Mapping get_mapping(int32_t keycode) const;
		Mapping get_maple_mapping(int32_t keycode) const;
		Mapping get_text_mapping(int32_t keycode, bool shift) const;

	private:
		std::map<int32_t, Mapping> keymap;
		std::map<int32_t, Mapping> maplekeys;
		std::map<int32_t, KeyAction::Id> textactions;
		std::map<int32_t, bool> keystate;
	};
}