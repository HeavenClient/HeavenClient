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

#include "ItemData.h"

#include "../Graphics/Animation.h"

namespace ms
{
	// Information about a bullet type item.
	class BulletData : public Cache<BulletData>
	{
	public:
		// Returns whether the bullet was loaded correctly.
		bool is_valid() const;

		// Returns whether the bullet was loaded correctly.
		explicit operator bool() const;

		// Returns the weapon attack increase when using this bullet.
		int16_t get_watk() const;

		// Returns the bullet animation.
		const Animation &get_animation() const;

		// Returns the general item data.
		const ItemData &get_itemdata() const;

	private:
		// Allow the cache to use the constructor.
		friend Cache<BulletData>;

		// Load a bullet from the game files.
		BulletData(int32_t id);

		const ItemData &itemdata;

		Animation bullet;
		int16_t watk;
	};
}