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

#include "Template/Singleton.h"

#include <cstdint>
#include <chrono>

namespace ms
{
	// Small class for measuring elapsed time between game loops.
	class Timer : public Singleton<Timer>
	{
	public:
		Timer()
		{
			start();
		}

		~Timer()
		{}

		// Start the timer by setting the last measurement to now.
		void start()
		{
			point = clock::now();
		}

		// Return time elapsed since the last measurement.
		int64_t stop()
		{
			clock::time_point last = point;
			point = clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point - last);
			return duration.count();
		}

	private:
		using clock = std::chrono::high_resolution_clock;

		clock::time_point point;
	};

	// Small class for measuring elapsed time given a specific start time.
	class ContinuousTimer : public Singleton<ContinuousTimer>
	{
	public:
		using point = std::chrono::time_point<std::chrono::steady_clock>;

		ContinuousTimer()
		{
			start();
		}

		~ContinuousTimer()
		{}

		// Return now from the clock to be used to calculate elapsed time later.
		point start()
		{
			return clock::now();
		}

		// Return time elapsed since the last measurement provided.
		int64_t stop(point last)
		{
			auto now = clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - last);
			return duration.count();
		}

	private:
#ifdef _WIN32
		using clock = std::chrono::high_resolution_clock;
#else
		using clock = std::chrono::steady_clock;
#endif
	};
}