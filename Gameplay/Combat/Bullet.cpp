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
#include "Bullet.h"

namespace ms
{
	Bullet::Bullet(Animation a, Point<int16_t> origin, bool toleft)
	{
		animation = a;

		moveobj.set_x(origin.x() + (toleft ? -30.0 : 30.0));
		moveobj.set_y(origin.y() - 26.0);
	}

	void Bullet::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> bulletpos = moveobj.get_absolute(viewx, viewy, alpha);
		DrawArgument args(bulletpos, flip);
		animation.draw(args, alpha);
	}

	bool Bullet::settarget(Point<int16_t> target)
	{
		double xdelta = target.x() - moveobj.crnt_x();
		double ydelta = target.y() - moveobj.crnt_y();

		if (std::abs(xdelta) < 10.0)
			return true;

		flip = xdelta > 0.0;

		moveobj.hspeed = xdelta / 32;

		if (xdelta > 0.0)
		{
			if (moveobj.hspeed < 3.0)
				moveobj.hspeed = 3.0;
			else if (moveobj.hspeed > 6.0)
				moveobj.hspeed = 6.0;
		}
		else if (xdelta < 0.0)
		{
			if (moveobj.hspeed > -3.0)
				moveobj.hspeed = -3.0;
			else if (moveobj.hspeed < -6.0)
				moveobj.hspeed = -6.0;
		}

		moveobj.vspeed = moveobj.hspeed * ydelta / xdelta;

		return false;
	}

	bool Bullet::update(Point<int16_t> target)
	{
		animation.update();
		moveobj.move();

		int16_t xdelta = target.x() - moveobj.get_x();
		return moveobj.hspeed > 0.0 ? xdelta < 10 : xdelta > 10;
	}
}