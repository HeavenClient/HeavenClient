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

#include "MapObjects.h"

#include "../Spawn.h"

#include <queue>

namespace ms
{
	class MapDrops
	{
	public:
		MapDrops();

		// Initialize the meso icons
		void init();

		// Draw all drops on a layer
		void draw(Layer::Id layer, double viewx, double viewy, float alpha) const;

		// Update all drops.
		void update(const Physics& physics);

		// Spawn a new drop.
		void spawn(DropSpawn&& spawn);

		// Remove a drop.
		void remove(int32_t oid, int8_t mode, const PhysicsObject* looter);

		// Remove all drops.
		void clear();

		// Find a drop which can be picked up at the specified position
		using Loot = std::pair<int32_t, Point<int16_t>>;

		Loot find_loot_at(Point<int16_t> playerpos);

	private:
		MapObjects drops;

		enum MesoIcon
		{
			BRONZE,
			GOLD,
			BUNDLE,
			BAG,
			NUM_ICONS
		};

		std::array<Animation, MesoIcon::NUM_ICONS> mesoicons;
		bool lootenabled;

		std::queue<DropSpawn> spawns;
	};
}