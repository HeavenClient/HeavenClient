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

#include "Color.h"

#include "../Template/Rectangle.h"

namespace ms
{
	class DrawArgument
	{
	public:
		constexpr DrawArgument() : DrawArgument(0, 0)
		{}

		constexpr DrawArgument(int16_t x, int16_t y) : DrawArgument(Point<int16_t>(x, y))
		{}

		constexpr DrawArgument(Point<int16_t> position) : DrawArgument(position, 1.0f)
		{}

		constexpr DrawArgument(Point<int16_t> position, float xscale, float yscale) : DrawArgument(position, position,
																								   xscale, yscale, 1.0f)
		{}

		constexpr DrawArgument(Point<int16_t> position, Point<int16_t> stretch) : DrawArgument(position, position,
																							   stretch, 1.0f, 1.0f,
																							   1.0f, 0.0f)
		{}

		constexpr DrawArgument(Point<int16_t> position, bool flip) : DrawArgument(position, flip, 1.0f)
		{}

		constexpr DrawArgument(float angle, Point<int16_t> position, float opacity) : DrawArgument(angle, position,
																								   false, opacity)
		{}

		constexpr DrawArgument(Point<int16_t> position, float opacity) : DrawArgument(position, false, opacity)
		{}

		constexpr DrawArgument(Point<int16_t> position, Color color) : DrawArgument(position, position,
																					Point<int16_t>(0, 0), 1.0f, 1.0f,
																					color, 0.0f)
		{}

		constexpr DrawArgument(Point<int16_t> position, bool flip, Point<int16_t> center) : DrawArgument(position,
																										 center,
																										 flip ? -1.0f
																											  : 1.0f,
																										 1.0f, 1.0f)
		{}

		constexpr DrawArgument(Point<int16_t> position, Point<int16_t> center, float xscale, float yscale,
							   float opacity) : DrawArgument(position, center, Point<int16_t>(0, 0), xscale, yscale,
															 opacity, 0.0f)
		{}

		constexpr DrawArgument(bool flip) : DrawArgument(flip ? -1.0f : 1.0f, 1.0f, 1.0f)
		{}

		constexpr DrawArgument(float xscale, float yscale, float opacity) : DrawArgument(Point<int16_t>(0, 0), xscale,
																						 yscale, opacity)
		{}

		constexpr DrawArgument(Point<int16_t> position, float xscale, float yscale, float opacity) : DrawArgument(
				position, position, xscale, yscale, opacity)
		{}

		constexpr DrawArgument(Point<int16_t> position, bool flip, float opacity) : DrawArgument(position, position,
																								 flip ? -1.0f : 1.0f,
																								 1.0f, opacity)
		{}

		constexpr DrawArgument(float angle, Point<int16_t> position, bool flip, float opacity) : DrawArgument(position,
																											  position,
																											  Point<int16_t>(
																													  0,
																													  0),
																											  flip
																											  ? -1.0f
																											  : 1.0f,
																											  1.0f,
																											  opacity,
																											  angle)
		{}

		constexpr DrawArgument(Point<int16_t> position, Point<int16_t> center, Point<int16_t> stretch, float xscale,
							   float yscale, float opacity, float angle) : pos(position), center(center),
																		   stretch(stretch), xscale(xscale),
																		   yscale(yscale),
																		   color(1.0f, 1.0f, 1.0f, opacity),
																		   angle(angle)
		{}

		constexpr DrawArgument(Point<int16_t> position, Point<int16_t> center, Point<int16_t> stretch, float xscale,
							   float yscale, Color color, float angle) : pos(position), center(center),
																		 stretch(stretch), xscale(xscale),
																		 yscale(yscale), color(color), angle(angle)
		{}

		constexpr Point<int16_t> getpos() const
		{
			return pos;
		}

		constexpr Point<int16_t> getstretch() const
		{
			return stretch;
		}

		constexpr float get_xscale() const
		{
			return xscale;
		}

		constexpr float get_yscale() const
		{
			return yscale;
		}

		constexpr const Color &get_color() const
		{
			return color;
		}

		constexpr float get_angle() const
		{
			return angle;
		}

		constexpr DrawArgument operator+(Point<int16_t> argpos) const
		{
			return {
					pos + argpos,
					center + argpos,
					stretch, xscale, yscale, color, angle
			};
		}

		constexpr DrawArgument operator+(float argopc) const
		{
			return {
					pos, center, stretch, xscale, yscale,
					color.a() * argopc,
					angle
			};
		}

		constexpr DrawArgument operator+(const DrawArgument &o) const
		{
			return {
					pos + o.pos,
					center + o.center,
					stretch + o.stretch,
					xscale * o.xscale,
					yscale * o.yscale,
					color * o.color,
					angle + o.angle
			};
		}

		constexpr DrawArgument operator-(const DrawArgument &o) const
		{
			return {
					pos - o.pos,
					center - o.center,
					stretch - o.stretch,
					xscale / o.xscale,
					yscale / o.yscale,
					color / o.color,
					angle - o.angle
			};
		}

		Rectangle<int16_t> get_rectangle(Point<int16_t> origin, Point<int16_t> dimensions) const
		{
			int16_t w = stretch.x();

			if (w == 0)
				w = dimensions.x();

			int16_t h = stretch.y();

			if (h == 0)
				h = dimensions.y();

			Point<int16_t> rlt = pos - center - origin;
			int16_t rl = rlt.x();
			int16_t rr = rlt.x() + w;
			int16_t rt = rlt.y();
			int16_t rb = rlt.y() + h;
			int16_t cx = center.x();
			int16_t cy = center.y();

			return {
					cx + static_cast<int16_t>(xscale * rl),
					cx + static_cast<int16_t>(xscale * rr),
					cy + static_cast<int16_t>(yscale * rt),
					cy + static_cast<int16_t>(yscale * rb)
			};
		}

	private:
		Point<int16_t> pos;
		Point<int16_t> center;
		Point<int16_t> stretch;
		float xscale;
		float yscale;
		float angle;
		Color color;
	};
}