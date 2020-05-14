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
#include "MapObject.h"

namespace ms
{
	MapObject::MapObject(int32_t o, Point<int16_t> p) : oid(o)
	{
		set_position(p);
		active = true;
	}

	int8_t MapObject::update(const Physics& physics)
	{
		physics.move_object(phobj);

		return phobj.fhlayer;
	}

	void MapObject::set_position(int16_t x, int16_t y)
	{
		phobj.set_x(x);
		phobj.set_y(y);
	}

	void MapObject::set_position(Point<int16_t> position)
	{
		int16_t x = position.x();
		int16_t y = position.y();
		set_position(x, y);
	}

	void MapObject::makeactive()
	{
		active = true;
	}

	void MapObject::deactivate()
	{
		active = false;
	}

	bool MapObject::is_active() const
	{
		return active;
	}

	int8_t MapObject::get_layer() const
	{
		return phobj.fhlayer;
	}

	int32_t MapObject::get_oid() const
	{
		return oid;
	}

	Point<int16_t> MapObject::get_position() const
	{
		return phobj.get_position();
	}
}