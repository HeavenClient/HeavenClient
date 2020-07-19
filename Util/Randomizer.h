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

#include <cstdint>
#include <random>

namespace ms
{
	// Can be used to generate random numbers.
	class Randomizer
	{
	public:
		bool next_bool() const
		{
			return next_int(2) == 1;
		}

		bool below(float percent) const
		{
			return next_real(1.0f) < percent;
		}

		bool above(float percent) const
		{
			return next_real(1.0f) > percent;
		}

		template <class T>
		T next_real(T to) const
		{
			return next_real<T>(0, to);
		}

		template <class T>
		T next_real(T from, T to) const
		{
			if (from >= to)
				return from;

			std::uniform_real_distribution<T> range(from, to);
			std::random_device rd;
			std::default_random_engine engine{rd()};

			return range(engine);
		}

		template <class T>
		T next_int(T to) const
		{
			return next_int<T>(0, to);
		}

		template <class T>
		T next_int(T from, T to) const
		{
			if (from >= to)
				return from;

			std::uniform_int_distribution<T> range(from, to - 1);
			std::random_device rd;
			std::default_random_engine engine{rd()};

			return range(engine);
		}

		template <class E>
		E next_enum(E to = E::LENGTH) const
		{
			return next_enum(E(), to);
		}

		template <class E>
		E next_enum(E from, E to) const
		{
			auto next_underlying = next_int<typename std::underlying_type<E>::type>(from, to);

			return static_cast<E>(next_underlying);
		}
	};
}