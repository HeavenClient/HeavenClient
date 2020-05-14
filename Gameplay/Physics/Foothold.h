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

#include "../../MapleStory.h"

#include "../../Template/Range.h"

#ifdef USE_NX

#include <nlnx/node.hpp>

#else
#include "../../Util/WzFiles.h"
#endif

namespace ms
{
	// Represents a platform part on a maple map
	class Foothold
	{
	public:
		Foothold();

		Foothold(nl::node src, uint16_t id, uint8_t layer);

		// Returns the foothold id aka the identifier in game data of this platform
		uint16_t id() const;

		// Returns the platform left to this.
		uint16_t prev() const;

		// Returns the platform right to this.
		uint16_t next() const;

		// Returns the platform's layer.
		uint8_t layer() const;

		// Returns the horizontal component.
		const Range<int16_t>& horizontal() const;

		// Returns the vertical component.
		const Range<int16_t>& vertical() const;

		// Return the left edge
		int16_t l() const;

		// Return the right edge.
		int16_t r() const;

		// Return the top edge.
		int16_t t() const;

		// Return the bottom edge.
		int16_t b() const;

		// Return the first horizontal component.
		int16_t x1() const;

		// Return the second horizontal component.
		int16_t x2() const;

		// Return the first vertical component.
		int16_t y1() const;

		// Return the second vertical component.
		int16_t y2() const;

		// Return if the platform is a wall (x1 == x2).
		bool is_wall() const;

		// Return if the platform is a floor (y1 == y2).
		bool is_floor() const;

		// Return if this platform is a left edge.
		bool is_left_edge() const;

		// Return if this platform is a right edge.
		bool is_right_edge() const;

		// Returns if a x-coordinate is above or below this platform.
		bool hcontains(int16_t x) const;

		// Returns if a y-coordinate is right or left of this platform.
		bool vcontains(int16_t y) const;

		// Check whether this foothold blocks an object.
		bool is_blocking(const Range<int16_t>& vertical) const;

		// Returns the width.
		int16_t hdelta() const;

		// Returns the height.
		int16_t vdelta() const;

		// Returns the slope as a ratio of vertical/horizontal.
		double slope() const;

		// Returns a y-coordinate right below the given x-coordinate.
		double ground_below(double x) const;

	private:
		uint16_t m_id;
		uint16_t m_prev;
		uint16_t m_next;
		uint8_t m_layer;
		Range<int16_t> m_horizontal;
		Range<int16_t> m_vertical;
	};
}