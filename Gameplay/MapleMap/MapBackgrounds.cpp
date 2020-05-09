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
#include "MapBackgrounds.h"

#include "../../Graphics/GraphicsGL.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	Background::Background(nl::node src)
	{
		VWIDTH = Constants::Constants::get().get_viewwidth();
		VHEIGHT = Constants::Constants::get().get_viewheight();
		WOFFSET = VWIDTH / 2;
		HOFFSET = VHEIGHT / 2;

		nl::node backsrc = nl::nx::map["Back"];

		animated = src["ani"].get_bool();
		animation = backsrc[src["bS"] + ".img"][animated ? "ani" : "back"][src["no"]];
		opacity = src["a"];
		flipped = src["f"].get_bool();
		cx = src["cx"];
		cy = src["cy"];
		rx = src["rx"];
		ry = src["ry"];

		moveobj.set_x(src["x"]);
		moveobj.set_y(src["y"]);

		Type type = typebyid(src["type"]);

		settype(type);
	}

	void Background::settype(Type type)
	{
		int16_t dim_x = animation.get_dimensions().x();
		int16_t dim_y = animation.get_dimensions().y();

		// TODO: Double check for zero. Is this a WZ reading issue?
		if (cx == 0)
			cx = (dim_x > 0) ? dim_x : 1;

		if (cy == 0)
			cy = (dim_y > 0) ? dim_y : 1;

		htile = 1;
		vtile = 1;

		switch (type)
		{
			case Type::HTILED:
			case Type::HMOVEA:
				htile = VWIDTH / cx + 3;
				break;
			case Type::VTILED:
			case Type::VMOVEA:
				vtile = VHEIGHT / cy + 3;
				break;
			case Type::TILED:
			case Type::HMOVEB:
			case Type::VMOVEB:
				htile = VWIDTH / cx + 3;
				vtile = VHEIGHT / cy + 3;
				break;
		}

		switch (type)
		{
			case Type::HMOVEA:
			case Type::HMOVEB:
				moveobj.hspeed = rx / 16;
				break;
			case Type::VMOVEA:
			case Type::VMOVEB:
				moveobj.vspeed = ry / 16;
				break;
		}
	}

	void Background::draw(double viewx, double viewy, float alpha) const
	{
		double x;

		if (moveobj.hmobile())
		{
			x = moveobj.get_absolute_x(viewx, alpha);
		} else
		{
			double shift_x = rx * (WOFFSET - viewx) / 100 + WOFFSET;
			x = moveobj.get_absolute_x(shift_x, alpha);
		}

		double y;

		if (moveobj.vmobile())
		{
			y = moveobj.get_absolute_y(viewy, alpha);
		} else
		{
			double shift_y = ry * (HOFFSET - viewy) / 100 + HOFFSET;
			y = moveobj.get_absolute_y(shift_y, alpha);
		}

		if (htile > 1)
		{
			while (x > 0)
				x -= cx;

			while (x < -cx)
				x += cx;
		}

		if (vtile > 1)
		{
			while (y > 0)
				y -= cy;

			while (y < -cy)
				y += cy;
		}

		int16_t ix = static_cast<int16_t>(std::round(x));
		int16_t iy = static_cast<int16_t>(std::round(y));

		int16_t tw = cx * htile;
		int16_t th = cy * vtile;

		for (int16_t tx = 0; tx < tw; tx += cx)
			for (int16_t ty = 0; ty < th; ty += cy)
				animation.draw(DrawArgument(Point<int16_t>(ix + tx, iy + ty), flipped, opacity / 255), alpha);
	}

	void Background::update()
	{
		moveobj.move();
		animation.update();
	}

	MapBackgrounds::MapBackgrounds(nl::node src)
	{
		int16_t no = 0;
		nl::node back = src[std::to_string(no)];

		while (back.size() > 0)
		{
			bool front = back["front"].get_bool();

			if (front)
				foregrounds.push_back(back);
			else
				backgrounds.push_back(back);

			no++;
			back = src[std::to_string(no)];
		}

		black = src["0"]["bS"].get_string() == "";
	}

	MapBackgrounds::MapBackgrounds()
	{}

	void MapBackgrounds::drawbackgrounds(double viewx, double viewy, float alpha) const
	{
		if (black)
			GraphicsGL::get().drawscreenfill(0.0f, 0.0f, 0.0f, 1.0f);

		for (auto &background : backgrounds)
			background.draw(viewx, viewy, alpha);
	}

	void MapBackgrounds::drawforegrounds(double viewx, double viewy, float alpha) const
	{
		for (auto &foreground : foregrounds)
			foreground.draw(viewx, viewy, alpha);
	}

	void MapBackgrounds::update()
	{
		for (auto &background : backgrounds)
			background.update();

		for (auto &foreground : foregrounds)
			foreground.update();
	}
}