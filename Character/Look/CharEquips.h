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

#include "Clothing.h"

namespace ms
{
	// A characters equipment (The visual part)
	class CharEquips
	{
	public:
		// Cap types (vslot)
		enum CapType
		{
			NONE,
			HEADBAND,
			HAIRPIN,
			HALFCOVER,
			FULLCOVER
		};

		// Initialize pointers with zero
		CharEquips();

		// Draw an equip.
		void draw(EquipSlot::Id slot, Stance::Id stance, Clothing::Layer layer, uint8_t frame,
				  const DrawArgument &args) const;

		// Add an equip, if not in cache, the equip is created from the files.
		void add_equip(int32_t itemid, const BodyDrawInfo &drawinfo);

		// Remove an equip.
		void remove_equip(EquipSlot::Id slot);

		// Check if an equip is visible.
		bool is_visible(EquipSlot::Id slot) const;

		// Check if the equip at the specified slot in the specified stance contains a part on the specified layer.
		bool comparelayer(EquipSlot::Id slot, Stance::Id stance, Clothing::Layer layer) const;

		// Return if there is an overall equipped.
		bool has_overall() const;

		// Return if there is a weapon equipped.
		bool has_weapon() const;

		// Return whether the equipped weapon is twohanded.
		bool is_twohanded() const;

		// Return the cap type (vslot).
		CapType getcaptype() const;

		// Return a stance which has been adjusted to the equipped weapon type.
		Stance::Id adjust_stance(Stance::Id stance) const;

		// Return the item id of the equip at the specified slot.
		int32_t get_equip(EquipSlot::Id slot) const;

		// Return the item id of the equipped weapon.
		int32_t get_weapon() const;

	private:
		EnumMap<EquipSlot::Id, const Clothing *> clothes;

		static std::unordered_map<int32_t, Clothing> cloth_cache;
	};
}