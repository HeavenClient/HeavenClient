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

#include "../Constants.h"

#include <cstdint>

namespace ms
{
	class TimedBool
	{
	public:
		TimedBool()
		{
			value = false;
			delay = 0;
			last = 0;
		}

		explicit operator bool() const
		{
			return value;
		}

		void set_for(int64_t millis)
		{
			last = millis;
			delay = millis;
			value = true;
		}

		void update()
		{
			update(Constants::TIMESTEP);
		}

		void update(uint16_t timestep)
		{
			if (value)
			{
				if (timestep >= delay)
				{
					value = false;
					delay = 0;
				}
				else
				{
					delay -= timestep;
				}
			}
		}

		void operator=(bool b)
		{
			value = b;
			delay = 0;
			last = 0;
		}

		bool operator==(bool b) const
		{
			return value == b;
		}

		bool operator!=(bool b) const
		{
			return value != b;
		}

		float alpha() const
		{
			return 1.0f - static_cast<float>(static_cast<float>(delay) / last);
		}

	private:
		int64_t last;
		int64_t delay;
		bool value;
	};
}