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
#include "Portal.h"

namespace ms
{
	Portal::Portal(const Animation *a, Type t, std::string nm, bool intramap, Point<int16_t> p, int32_t tid,
				   std::string tnm) : animation(a), type(t), name(nm), position(p), warpinfo(tid, intramap, tnm, nm)
	{
		touched = false;
	}

	Portal::Portal() : Portal(nullptr, SPAWN, "", false, Point<int16_t>(), 0, "")
	{}

	void Portal::update(Point<int16_t> playerpos)
	{
		touched = bounds().contains(playerpos);
	}

	void Portal::draw(Point<int16_t> viewpos, float inter) const
	{
		if (!animation || (type == HIDDEN && !touched))
			return;

		animation->draw(position + viewpos, inter);
	}

	std::string Portal::get_name() const
	{
		return name;
	}

	Portal::Type Portal::get_type() const
	{
		return type;
	}

	Point<int16_t> Portal::get_position() const
	{
		return position;
	}

	Rectangle<int16_t> Portal::bounds() const
	{
		auto lt = position + Point<int16_t>(-25, -100);
		auto rb = position + Point<int16_t>(25, 25);

		return Rectangle<int16_t>(lt, rb);
	}

	Portal::WarpInfo Portal::getwarpinfo() const
	{
		return warpinfo;
	}
}