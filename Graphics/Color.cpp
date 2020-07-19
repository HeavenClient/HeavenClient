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
#include "Color.h"

#include "../Util/Lerp.h"

namespace ms
{
	const float* Color::data() const
	{
		return rgba.data();
	}

	Color::underlying_t::const_iterator Color::begin() const
	{
		return rgba.begin();
	}

	Color::underlying_t::const_iterator Color::end() const
	{
		return rgba.end();
	}

	Color Color::blend(const Color& other, float alpha) const
	{
		underlying_t blended;

		std::transform(
			begin(),
			end(),
			other.begin(),
			blended.begin(),
			[alpha](float f, float s)
			{
				return lerp(f, s, alpha);
			}
		);

		return blended;
	}
}