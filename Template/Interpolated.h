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

#include "../Util/Lerp.h"

namespace ms
{
	template <typename T>
	class Nominal
	{
	public:
		constexpr Nominal() : now(T()), before(T()), threshold(0.0f)
		{}

		T get() const
		{
			return now;
		}

		T get(float alpha) const
		{
			return alpha >= threshold ? now : before;
		}

		T last() const
		{
			return before;
		}

		void set(T value)
		{
			now = value;
			before = value;
		}

		void normalize()
		{
			before = now;
		}

		bool normalized() const
		{
			return before == now;
		}

		void next(T value, float thrs)
		{
			before = now;
			now = value;
			threshold = thrs;
		}

		bool operator==(T value) const
		{
			return now == value;
		}

		bool operator!=(T value) const
		{
			return now != value;
		}

		T operator+(T value) const
		{
			return now + value;
		}

		T operator-(T value) const
		{
			return now - value;
		}

		T operator*(T value) const
		{
			return now * value;
		}

		T operator/(T value) const
		{
			return now / value;
		}

	private:
		T now;
		T before;
		float threshold;
	};

	template <typename T>
	class Linear
	{
	public:
		T get() const
		{
			return now;
		}

		T get(float alpha) const
		{
			return lerp<T>(before, now, alpha);
		}

		T last() const
		{
			return before;
		}

		void set(T value)
		{
			now = value;
			before = value;
		}

		void normalize()
		{
			before = now;
		}

		bool normalized() const
		{
			return before == now;
		}

		void operator=(T value)
		{
			before = now;
			now = value;
		}

		void operator+=(T value)
		{
			before = now;
			now += value;
		}

		void operator-=(T value)
		{
			before = now;
			now -= value;
		}

		bool operator==(T value) const
		{
			return now == value;
		}

		bool operator!=(T value) const
		{
			return now != value;
		}

		bool operator<(T value) const
		{
			return now < value;
		}

		bool operator<=(T value) const
		{
			return now <= value;
		}

		bool operator>(T value) const
		{
			return now > value;
		}

		bool operator>=(T value) const
		{
			return now >= value;
		}

		T operator+(T value) const
		{
			return now + value;
		}

		T operator-(T value) const
		{
			return now - value;
		}

		T operator*(T value) const
		{
			return now * value;
		}

		T operator/(T value) const
		{
			return now / value;
		}

		T operator+(Linear<T> value) const
		{
			return now + value.get();
		}

		T operator-(Linear<T> value) const
		{
			return now - value.get();
		}

		T operator*(Linear<T> value) const
		{
			return now * value.get();
		}

		T operator/(Linear<T> value) const
		{
			return now / value.get();
		}

	private:
		T now;
		T before;
	};
}