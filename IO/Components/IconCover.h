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

#include "../../Graphics/Geometry.h"
#include "../../Template/Interpolated.h"

namespace ms
{
	// A transparent rectangle with icon size (30x30)
	class IconCover
	{
	public:
		enum Type
		{
			BUFF,
			COOLDOWN
		};

		IconCover(Type type, int32_t duration);

		void draw(Point<int16_t> position, float alpha) const;

		void update();

	private:
		ColorBox cover;
		Linear<float> yscale;
		float scalestep;
		Type type;
	};
}