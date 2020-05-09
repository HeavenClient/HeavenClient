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
#include "Camera.h"

#include "../Constants.h"

namespace ms
{
	Camera::Camera()
	{
		x.set(0.0);
		y.set(0.0);

		VWIDTH = Constants::Constants::get().get_viewwidth();
		VHEIGHT = Constants::Constants::get().get_viewheight();
	}

	void Camera::update(Point<int16_t> position)
	{
		int32_t new_width = Constants::Constants::get().get_viewwidth();
		int32_t new_height = Constants::Constants::get().get_viewheight();

		if (VWIDTH != new_width || VHEIGHT != new_height)
		{
			VWIDTH = new_width;
			VHEIGHT = new_height;
		}

		double next_x = x.get();
		double hdelta = VWIDTH / 2 - position.x() - next_x;

		if (std::abs(hdelta) >= 5.0)
			next_x += hdelta * (12.0 / VWIDTH);

		double next_y = y.get();
		double vdelta = VHEIGHT / 2 - position.y() - next_y;

		if (std::abs(vdelta) >= 5.0)
			next_y += vdelta * (12.0 / VHEIGHT);

		if (next_x > hbounds.first() || hbounds.length() < VWIDTH)
			next_x = hbounds.first();
		else if (next_x < hbounds.second() + VWIDTH)
			next_x = hbounds.second() + VWIDTH;

		if (next_y > vbounds.first() || vbounds.length() < VHEIGHT)
			next_y = vbounds.first();
		else if (next_y < vbounds.second() + VHEIGHT)
			next_y = vbounds.second() + VHEIGHT;

		x = next_x;
		y = next_y;
	}

	void Camera::set_position(Point<int16_t> position)
	{
		int32_t new_width = Constants::Constants::get().get_viewwidth();
		int32_t new_height = Constants::Constants::get().get_viewheight();

		if (VWIDTH != new_width || VHEIGHT != new_height)
		{
			VWIDTH = new_width;
			VHEIGHT = new_height;
		}

		x.set(VWIDTH / 2 - position.x());
		y.set(VHEIGHT / 2 - position.y());
	}

	void Camera::set_view(Range<int16_t> mapwalls, Range<int16_t> mapborders)
	{
		hbounds = -mapwalls;
		vbounds = -mapborders;
	}

	Point<int16_t> Camera::position() const
	{
		auto shortx = static_cast<int16_t>(std::round(x.get()));
		auto shorty = static_cast<int16_t>(std::round(y.get()));

		return {shortx, shorty};
	}

	Point<int16_t> Camera::position(float alpha) const
	{
		auto interx = static_cast<int16_t>(std::round(x.get(alpha)));
		auto intery = static_cast<int16_t>(std::round(y.get(alpha)));

		return {interx, intery};
	}

	Point<double> Camera::realposition(float alpha) const
	{
		return {x.get(alpha), y.get(alpha)};
	}
}