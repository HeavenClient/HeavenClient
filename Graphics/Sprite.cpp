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
#include "Sprite.h"

namespace ms
{
	Sprite::Sprite(const Animation &a, const DrawArgument &args) : animation(a), stateargs(args)
	{}

	Sprite::Sprite(nl::node src, const DrawArgument &args) : animation(src), stateargs(args)
	{}

	Sprite::Sprite(nl::node src) : Sprite(src, {})
	{}

	Sprite::Sprite()
	{}

	void Sprite::draw(Point<int16_t> parentpos, float alpha) const
	{
		auto absargs = stateargs + parentpos;
		animation.draw(absargs, alpha);
	}

	bool Sprite::update(uint16_t timestep)
	{
		return animation.update(timestep);
	}

	bool Sprite::update()
	{
		return animation.update();
	}

	int16_t Sprite::width() const
	{
		return get_dimensions().x();
	}

	int16_t Sprite::height() const
	{
		return get_dimensions().y();
	}

	Point<int16_t> Sprite::get_origin() const
	{
		return animation.get_origin();
	}

	Point<int16_t> Sprite::get_dimensions() const
	{
		return animation.get_dimensions();
	}
}