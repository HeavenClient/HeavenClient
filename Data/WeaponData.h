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

#include "EquipData.h"

#include "../Audio/Audio.h"

#include "../Character/Inventory/Weapon.h"

namespace ms
{
	// Contains information about a weapon
	class WeaponData : public Cache<WeaponData>
	{
	public:
		// Returns whether the weapon was loaded correctly
		bool is_valid() const;

		// Returns whether the weapon was loaded correctly
		explicit operator bool() const;

		// Return whether this weapon uses two-handed stances
		bool is_twohanded() const;

		// Return the attack speed.
		uint8_t get_speed() const;

		// Return the attack type.
		uint8_t get_attack() const;

		// Return the speed as displayed in a tooltip
		std::string getspeedstring() const;

		// Return the attack delay
		uint8_t get_attackdelay() const;

		// Return the weapon type
		Weapon::Type get_type() const;

		// Return the sound to play when attacking
		Sound get_usesound(bool degenerate) const;

		// Return the name of the afterimage
		const std::string &get_afterimage() const;

		// Return the general equip data
		const EquipData &get_equipdata() const;

	private:
		// Allow the cache to use the constructor
		friend Cache<WeaponData>;

		// Load a weapon from the game files
		WeaponData(int32_t id);

		const EquipData &equipdata;

		Weapon::Type type;
		bool twohanded;
		uint8_t attackspeed;
		uint8_t attack;
		BoolPair<Sound> usesounds;
		std::string afterimage;
	};
}