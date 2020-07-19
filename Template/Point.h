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

#include "../MapleStory.h"

#ifdef USE_NX

#include <nlnx/node.hpp>

#else
#include "../Util/WzFiles.h"
#endif

#if defined(__linux__) || defined(__APPLE__)

#include <cmath>

#endif

namespace ms
{
	template <class T>
	class Point
	{
	public:
		// Construct a point from a vector property
		Point(nl::node src)
		{
			a = static_cast<T>(src.x());
			b = static_cast<T>(src.y());
		}

		// Construct a point from the specified coordinates
		constexpr Point(T first, T second) : a(first), b(second)
		{}

		// Construct a point with coordinates (0, 0)
		constexpr Point() : Point(0, 0)
		{}

		// Return the x-coordinate
		constexpr T x() const
		{
			return a;
		}

		// Return the y-coordinate
		constexpr T y() const
		{
			return b;
		}

		// Return the inner product
		constexpr T length() const
		{
			return static_cast<T>(
				std::sqrt(a * a + b * b)
			);
		}

		// Check whether the coordinates are equal
		constexpr bool straight() const
		{
			return a == b;
		}

		// Return a string representation of the point
		std::string to_string() const
		{
			return "(" + std::to_string(a) + "," + std::to_string(b) + ")";
		}

		// Return the distance to another point
		constexpr T distance(Point<T> v) const
		{
			return Point<T>(a - v.a, b - v.b).length();
		}

		// Set the x-coordinate
		void set_x(T v)
		{
			a = v;
		}

		// Set the y-coordinate
		void set_y(T v)
		{
			b = v;
		}

		// Shift the x-coordinate by the specified amount
		void shift_x(T v)
		{
			a += v;
		}

		// Shift the y-coordinate by the specified amount
		void shift_y(T v)
		{
			b += v;
		}

		// Shift the coordinates by the specified amounts
		void shift(T x, T y)
		{
			a += x;
			b += y;
		}

		// Shift the this point by the amounts defined by another point
		// Equivalent to += operator
		void shift(Point<T> v)
		{
			a += v.a;
			b += v.b;
		}

		// Take the absolute value of the point
		Point<T> abs()
		{
			return {
				static_cast<T>(std::abs(a)),
				static_cast<T>(std::abs(b))
			};
		}

		// Check whether point is equivalent to the specified point
		constexpr bool operator==(const Point<T>& v) const
		{
			return a == v.a && b == v.b;
		}

		// Check whether point is not equivalent to the specified point
		constexpr bool operator!=(const Point<T>& v) const
		{
			return !(*this == v);
		}

		// Shift the this point by the amounts defined by another point
		void operator+=(Point<T> v)
		{
			a += v.a;
			b += v.b;
		}

		// Shift the this point in reverse direction by the amounts defined by another point
		void operator-=(Point<T> v)
		{
			a -= v.a;
			b -= v.b;
		}

		// Return a point whose coordinates are the negation of this point's coordinates
		constexpr Point<T> operator-() const
		{
			return {-a, -b};
		}

		// Return a point whose coordinates have been added the specified amount
		constexpr Point<T> operator+(T v) const
		{
			return {a + v, b + v};
		}

		// Return a point whose coordinates have been subtracted the specified amount
		constexpr Point<T> operator-(T v) const
		{
			return {a - v, b - v};
		}

		// Return a point whose coordinates have been multiplied by the specified amount
		constexpr Point<T> operator*(T v) const
		{
			return {a * v, b * v};
		}

		// Return a point whose coordinates have been divided by the specified amount
		constexpr Point<T> operator/(T v) const
		{
			return {a / v, b / v};
		}

		// Return a point whose coordinates are the sum of this and another points coordinates
		constexpr Point<T> operator+(Point<T> v) const
		{
			return {a + v.a, b + v.b};
		}

		// Return a point whose coordinates are the difference of this and another points coordinates
		constexpr Point<T> operator-(Point<T> v) const
		{
			return {a - v.a, b - v.b};
		}

		// Return a point whose coordinates are the product of this and another points coordinates
		constexpr Point<T> operator*(Point<T> v) const
		{
			return {a / v.a, b / v.b};
		}

		// Return a point whose coordinates are the division of this and another points coordinates
		constexpr Point<T> operator/(Point<T> v) const
		{
			return {
				a / (v.a == 0 ? 1 : v.a),
				b / (v.b == 0 ? 1 : v.b)
			};
		}

	private:
		T a;
		T b;
	};
}