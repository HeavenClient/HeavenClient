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

#include "../../Graphics/Texture.h"

#include "../../Character/Inventory/Inventory.h"

#include <memory>

namespace ms
{
	class Icon
	{
	public:
		enum IconType : uint8_t
		{
			NONE,
			SKILL,
			EQUIP,
			ITEM,
			KEY,
			NUM_TYPES
		};

		class Type
		{
		public:
			virtual ~Type()
			{}

			virtual void drop_on_stage() const = 0;

			virtual void drop_on_equips(EquipSlot::Id eqslot) const = 0;

			virtual bool drop_on_items(InventoryType::Id tab, EquipSlot::Id eqslot, int16_t slot, bool equip) const = 0;

			virtual void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const = 0;

			virtual void set_count(int16_t) = 0;

			virtual IconType get_type() = 0;
		};

		class NullType : public Type
		{
			void drop_on_stage() const override {}
			void drop_on_equips(EquipSlot::Id) const override {}
			bool drop_on_items(InventoryType::Id, EquipSlot::Id, int16_t, bool) const override { return true; }
			void drop_on_bindings(Point<int16_t>, bool) const override {}
			void set_count(int16_t) override {}
			IconType get_type() override { return IconType::NONE; }
		};

		Icon(std::unique_ptr<Type> type, Texture texture, int16_t count);

		Icon();

		void drop_on_stage() const;
		void drop_on_equips(EquipSlot::Id eqslot) const;
		bool drop_on_items(InventoryType::Id tab, EquipSlot::Id eqslot, int16_t slot, bool equip) const;
		void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const;

		void set_count(int16_t count);

		IconType get_type();

		void draw(Point<int16_t> position) const;

		void dragdraw(Point<int16_t> cursorpos) const;

		void start_drag(Point<int16_t> offset);

		void reset();

		virtual Texture get_texture() const;

		int16_t get_count() const;

		bool get_drag();

	private:
		std::unique_ptr<Type> type;
		bool showcount;
		int16_t count;

		Texture texture;
		bool dragged;
		Point<int16_t> cursoroffset;
	};
}