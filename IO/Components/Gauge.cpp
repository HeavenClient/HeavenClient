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
#include "Gauge.h"

namespace ms
{
	Gauge::Gauge(Type type, Texture front, int16_t max, float percent) : type(type), barfront(front), maximum(max), percentage(percent)
	{
		target = percentage;
	}

	Gauge::Gauge(Type type, Texture front, Texture mid, int16_t max, float percent) : type(type), barfront(front), barmid(mid), maximum(max), percentage(percent)
	{
		target = percentage;
	}

	Gauge::Gauge(Type type, Texture front, Texture mid, Texture end, int16_t max, float percent) : type(type), barfront(front), barmid(mid), barend(end), maximum(max), percentage(percent)
	{
		target = percentage;
	}

	Gauge::Gauge() {}

	void Gauge::draw(const DrawArgument& args) const
	{
		int16_t length = static_cast<int16_t>(percentage * maximum);

		if (length > 0)
		{
			if (type == Type::GAME)
			{
				barfront.draw(args + DrawArgument(Point<int16_t>(0, 0), Point<int16_t>(length, 0)));
				barmid.draw(args);
				barend.draw(args + Point<int16_t>(length + 8, 20));
			}
			else if (type == Type::CASHSHOP)
			{
				Point<int16_t> pos_adj = Point<int16_t>(45, 1);

				barfront.draw(args - pos_adj);
				barmid.draw(args + DrawArgument(Point<int16_t>(0, 0), Point<int16_t>(length, 0)));
				barend.draw(args - pos_adj + Point<int16_t>(length + barfront.width(), 0));
			}
		}
	}

	void Gauge::update(float t)
	{
		if (target != t)
		{
			target = t;
			step = (target - percentage) / 24;
		}

		if (percentage != target)
		{
			percentage += step;

			if (step < 0.0f)
			{
				if (target - percentage >= step)
					percentage = target;
			}
			else if (step > 0.0f)
			{
				if (target - percentage <= step)
					percentage = target;
			}
		}
	}
}