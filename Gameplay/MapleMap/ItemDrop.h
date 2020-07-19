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

#include "Drop.h"

#include "../../Graphics/Texture.h"

namespace ms
{
	class ItemDrop : public Drop
	{
	public:
		ItemDrop(int32_t oid, int32_t owner, Point<int16_t> start, Point<int16_t> dest, int8_t type, int8_t mode,
				 int32_t iid, bool playerdrop, const Texture& icon);

		void draw(double viewx, double viewy, float alpha) const override;

	private:
		const Texture& icon;
		int32_t itemid;
	};
}