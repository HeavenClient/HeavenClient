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

#include "Foothold.h"
#include "PhysicsObject.h"

#include <unordered_map>

namespace ms
{
	// The collection of platforms in a maple map
	// Used for collision-detection
	class FootholdTree
	{
	public:
		FootholdTree(nl::node source);

		FootholdTree();

		// Takes an accelerated PhysicsObject and limits its movement based on the platforms in this tree.
		void limit_movement(PhysicsObject &touse) const;

		// Updates a PhysicsObject's fhid based on it's position.
		void update_fh(PhysicsObject &touse) const;

		// Determine the point on the ground below the specified position.
		int16_t get_y_below(Point<int16_t> position) const;

		// Returns the leftmost and rightmost platform positions of the map.
		Range<int16_t> get_walls() const;

		// Returns the topmost and bottommost platform positions of the map.
		Range<int16_t> get_borders() const;

	private:
		uint16_t get_fhid_below(double fx, double fy) const;

		double get_wall(uint16_t fhid, bool left, double fy) const;

		double get_edge(uint16_t fhid, bool left) const;

		const Foothold &get_fh(uint16_t fhid) const;

		std::unordered_map<uint16_t, Foothold> footholds;
		std::unordered_multimap<int16_t, uint16_t> footholdsbyx;

		Foothold nullfh;
		Range<int16_t> walls;
		Range<int16_t> borders;
	};
}