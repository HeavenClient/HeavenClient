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

#include <stdexcept>

namespace ms
{
	template <typename T>
	class BoolPair
	{
	public:
		template <typename...Args>
		BoolPair(Args&& ...argsf, Args&& ...argss) : first(std::forward<Args>(argsf)...), second(
			std::forward<Args>(argss)...)
		{}

		BoolPair(T f, T s) : first(f), second(s)
		{}

		BoolPair()
		{}

		template <typename...Args>
		void set(bool b, Args&& ...args)
		{
			if (b)
				first = T(std::forward<Args>(args)...);
			else
				second = T(std::forward<Args>(args)...);
		}

		T& operator[](bool b)
		{
			return b ? first : second;
		}

		const T& operator[](bool b) const
		{
			return b ? first : second;
		}

	private:
		T first;
		T second;
	};
}