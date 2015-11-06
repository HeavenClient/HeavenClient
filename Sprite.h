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
#include "Graphic.h"
#include "node.hpp"

namespace Graphics
{
	using::nl::node;

	class Sprite : public Graphic
	{
	public:
		Sprite(node, vector2d<int32_t>);
		~Sprite();
		void draw(const DrawArgument&) const;
		void update(uint16_t);
		vector2d<int32_t> getorigin() const;
		vector2d<int32_t> getdimensions() const;
	private:
		Graphic* graphic;
		vector2d<int32_t> position;
	};
}

