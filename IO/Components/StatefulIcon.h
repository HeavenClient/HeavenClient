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

#include "Icon.h"

namespace ms
{
	class StatefulIcon : public Icon
	{
	public:
		enum State : uint8_t
		{
			NORMAL,
			DISABLED,
			MOUSEOVER,
			LENGTH
		};

		class Type : public Icon::Type
		{
		public:
			virtual ~Type()
			{}

			virtual void set_state(State state) = 0;
		};

		class NullType : public Type
		{
			void drop_on_stage() const override
			{}

			void drop_on_equips(EquipSlot::Id) const override
			{}

			bool drop_on_items(InventoryType::Id, EquipSlot::Id, int16_t, bool) const override
			{ return true; }

			void drop_on_bindings(Point<int16_t>, bool) const override
			{}

			void set_count(int16_t) override
			{}

			void set_state(State) override
			{}

			Icon::IconType get_type() override
			{ return IconType::NONE; }
		};

		StatefulIcon() : StatefulIcon(std::make_unique<NullType>(), Texture(), Texture(), Texture())
		{};

		StatefulIcon(std::unique_ptr<Type> type, Texture normal_tx, Texture disabled_tx, Texture mouseover_tx);

		Texture get_texture() const override;

		void set_state(State state);

	private:
		State state;
		EnumMap<State, Texture> textures;
	};
}