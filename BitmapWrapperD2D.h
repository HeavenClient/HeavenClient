/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Journey.h"
#ifndef JOURNEY_USE_OPENGL
#include "BitmapWrapper.h"
#include "LocatorD2D.h"
#include "bitmap.hpp"

namespace Graphics
{
	using::nl::bitmap;
	// Implementation of BitmapWrapper in D2D.
	class BitmapWrapperD2D : public BitmapWrapper
	{
	public:
		BitmapWrapperD2D(bitmap);
		~BitmapWrapperD2D();
		void draw(rectangle2d<int32_t>, float, float, vector2d<int32_t>, float) const;
	private:
		ID2D1Bitmap* source;
	};
}
#endif
