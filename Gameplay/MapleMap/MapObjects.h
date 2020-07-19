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

#include "Layer.h"
#include "MapObject.h"

#include "../../Template/Optional.h"

#include <memory>
#include <unordered_set>

namespace ms
{
	// A collection of generic MapObjects
	class MapObjects
	{
	public:
		// Draw all MapObjects that are on the specified layer
		void draw(Layer::Id layer, double viewx, double viewy, float alpha) const;

		// Update all mapobjects of this type. Also updates layers eg. drawing order.
		void update(const Physics& physics);

		// Adds a MapObjects of this type
		void add(std::unique_ptr<MapObject> mapobject);

		// Removes the mapobject with the given oid.
		void remove(int32_t oid);

		// Removes all mapobjects of this type.
		void clear();

		// Check if a map object with the specified id exists on the map
		bool contains(int32_t oid) const;

		// Obtains a pointer to the mapobject with the given oid.
		Optional<MapObject> get(int32_t oid);

		// Obtains a const pointer to the mapobject with the given oid.
		Optional<const MapObject> get(int32_t oid) const;

		using underlying_t = typename std::unordered_map<int32_t, std::unique_ptr<MapObject>>;

		// Return a begin iterator.
		underlying_t::iterator begin();

		// Return an end iterator.
		underlying_t::iterator end();

		// Return a begin iterator.
		underlying_t::const_iterator begin() const;

		// Return an end iterator.
		underlying_t::const_iterator end() const;

		// Return the size of the iterator.
		underlying_t::size_type size() const;

	private:
		std::unordered_map<int32_t, std::unique_ptr<MapObject>> objects;
		std::array<std::unordered_set<int32_t>, Layer::Id::LENGTH> layers;
	};
}