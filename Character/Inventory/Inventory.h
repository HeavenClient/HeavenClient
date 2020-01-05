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

#include "InventoryType.h"
#include "Item.h"
#include "Equip.h"
#include "Pet.h"
#include "Weapon.h"

#include "../Look/Equipslot.h"
#include "../Template/EnumMap.h"
#include "../Template/Optional.h"

#include <map>
#include <unordered_map>

namespace ms
{
	// The player's inventory.
	class Inventory
	{
	public:
		enum Movement : int8_t
		{
			MOVE_NONE = -1,
			MOVE_INTERNAL = 0,
			MOVE_UNEQUIP = 1,
			MOVE_EQUIP = 2
		};

		enum Modification : int8_t
		{
			ADD,
			CHANGECOUNT,
			SWAP,
			REMOVE,
			ADDCOUNT
		};

		// Return the move type by value.
		static Movement movementbyvalue(int8_t value);

		Inventory();

		// Recalculate sums of equip stats.
		void recalc_stats(Weapon::Type weapontype);
		// Set the meso amount.
		void set_meso(int64_t meso);
		// Set the number of slots for a given inventory.
		void set_slotmax(InventoryType::Id type, uint8_t value);

		// Modify the inventory with info from a packet.
		void modify(InventoryType::Id type, int16_t pos, int8_t mode, int16_t arg, Movement movement);
		// Add a general item.
		void add_item(InventoryType::Id type, int16_t slot, int32_t itemid, bool cash, int64_t expire, uint16_t count, const std::string& owner, int16_t flag);
		// Add a pet item.
		void add_pet(InventoryType::Id type, int16_t slot, int32_t itemid, bool cash, int64_t expire, const std::string& name, int8_t level, int16_t closeness, int8_t fullness);
		// Add an equip item.
		void add_equip(InventoryType::Id type, int16_t slot, int32_t itemid, bool cash, int64_t expire, uint8_t slots, uint8_t level, const EnumMap<Equipstat::Id, uint16_t>& stats, const std::string& owner, int16_t flag, uint8_t itemlevel, uint16_t itemexp, int32_t vicious);

		// Check if the use inventory contains at least one projectile.
		bool has_projectile() const;
		// Return if an equip is equipped in the specfied slot.
		bool has_equipped(Equipslot::Id slot) const;
		// Return the currently active projectile slot.
		int16_t get_bulletslot() const;
		// Return the count of the currently active projectile.
		uint16_t get_bulletcount() const;
		// Return the itemid of the currently active projectile.
		int32_t get_bulletid() const;
		// Return the number of slots for the specified inventory.
		uint8_t get_slotmax(InventoryType::Id type) const;
		// Return a total stat.
		uint16_t get_stat(Equipstat::Id type) const;
		// Return the amount of meso.
		int64_t get_meso() const;
		// Find a free slot for the specified equip.
		Equipslot::Id find_equipslot(int32_t itemid) const;
		// Find a free slot in the specified inventory.
		int16_t find_free_slot(InventoryType::Id type) const;
		// Return the first slot which contains the specified item.
		int16_t find_item(InventoryType::Id type, int32_t itemid) const;
		// Return the count of an item. Returns 0 if the slot is empty.
		int16_t get_item_count(InventoryType::Id type, int16_t slot) const;
		// Return the total count of an item. Returns 0 if no instances of item found.
		int16_t get_total_item_count(int32_t itemid) const;
		// Return the id of an item. Returns 0 if the slot is empty.
		int32_t get_item_id(InventoryType::Id type, int16_t slot) const;

		// Return a pointer to an equip.
		Optional<const Equip> get_equip(InventoryType::Id type, int16_t slot) const;

	private:
		// Add an inventory slot and return the unique_id.
		int32_t add_slot(InventoryType::Id type, int16_t slot, int32_t item_id, int16_t count, bool cash);
		// Change the quantity of an item.
		void change_count(InventoryType::Id type, int16_t slot, int16_t count);
		// Swap two items.
		void swap(InventoryType::Id firsttype, int16_t firstslot, InventoryType::Id secondtype, int16_t secondslot);
		// Remove an item.
		void remove(InventoryType::Id type, int16_t slot);

		struct Slot
		{
			int32_t unique_id;
			int32_t item_id;
			int16_t count;
			bool cash;
		};

		EnumMap<InventoryType::Id, std::map<int16_t, Slot>> inventories;
		std::unordered_map<int32_t, Item> items;
		std::unordered_map<int32_t, Equip> equips;
		std::unordered_map<int32_t, Pet> pets;
		int32_t running_uid;

		EnumMap<Equipstat::Id, uint16_t> totalstats;
		EnumMap<InventoryType::Id, uint8_t> slotmaxima;
		int64_t meso;
		int16_t bulletslot;
	};
}