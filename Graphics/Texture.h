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

#include "DrawArgument.h"

#ifdef USE_NX
#include <nlnx/bitmap.hpp>
#endif

namespace ms
{
	// Represents a single image loaded from a of game data
	class Texture
	{
	public:
		Texture(nl::node source);

		Texture();

		~Texture();

		void draw(const DrawArgument &args) const;

		void shift(Point<int16_t> amount);

		bool is_valid() const;

		int16_t width() const;

		int16_t height() const;

		Point<int16_t> get_origin() const;

		Point<int16_t> get_dimensions() const;

	private:
		nl::node find_child(nl::node source, std::string link);

		nl::bitmap bitmap;
		Point<int16_t> origin;
		Point<int16_t> dimensions;
	};
}