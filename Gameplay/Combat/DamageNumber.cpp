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
#include "DamageNumber.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	DamageNumber::DamageNumber(Type t, int32_t damage, int16_t starty, int16_t x)
	{
		type = t;

		if (damage > 0)
		{
			miss = false;

			std::string number = std::to_string(damage);
			firstnum = number[0];

			if (number.size() > 1)
			{
				restnum = number.substr(1);
				multiple = true;
			} else
			{
				restnum = "";
				multiple = false;
			}

			int16_t total = getadvance(firstnum, true);

			for (size_t i = 0; i < restnum.length(); i++)
			{
				char c = restnum[i];
				int16_t advance;

				if (i < restnum.length() - 1)
				{
					char n = restnum[i + 1];
					advance = (getadvance(c, false) + getadvance(n, false)) / 2;
				} else
				{
					advance = getadvance(c, false);
				}

				total += advance;
			}

			shift = total / 2;
		} else
		{
			shift = charsets[type][true].getw('M') / 2;
			miss = true;
		}

		moveobj.set_x(x);
		moveobj.set_y(starty);
		moveobj.vspeed = -0.25;
		opacity.set(1.5f);
	}

	DamageNumber::DamageNumber()
	{}

	void DamageNumber::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absolute = moveobj.get_absolute(viewx, viewy, alpha);
		Point<int16_t> position = absolute - Point<int16_t>(0, shift);
		float interopc = opacity.get(alpha);

		if (miss)
		{
			charsets[type][true].draw('M', {position, interopc});
		} else
		{
			charsets[type][false].draw(firstnum, {position, interopc});

			if (multiple)
			{
				int16_t first_advance = getadvance(firstnum, true);
				position.shift_x(first_advance);

				for (size_t i = 0; i < restnum.length(); i++)
				{
					char c = restnum[i];
					Point<int16_t> yshift = {0, (i % 2) ? -2 : 2};
					charsets[type][true].draw(c, {position + yshift, interopc});

					int16_t advance;

					if (i < restnum.length() - 1)
					{
						char n = restnum[i + 1];
						int16_t c_advance = getadvance(c, false);
						int16_t n_advance = getadvance(n, false);
						advance = (c_advance + n_advance) / 2;
					} else
					{
						advance = getadvance(c, false);
					}

					position.shift_x(advance);
				}
			}
		}
	}

	int16_t DamageNumber::getadvance(char c, bool first) const
	{
		constexpr size_t LENGTH = 10;

		constexpr int16_t advances[LENGTH] =
				{
						24, 20, 22, 22, 24, 23, 24, 22, 24, 24
				};

		size_t index = c - 48;

		if (index < LENGTH)
		{
			int16_t advance = advances[index];

			switch (type)
			{
				case DamageNumber::Type::CRITICAL:
					if (first)
						advance += 8;
					else
						advance += 4;

					break;
				default:
					if (first)
						advance += 2;

					break;
			}

			return advance;
		} else
		{
			return 0;
		}
	}

	void DamageNumber::set_x(int16_t headx)
	{
		moveobj.set_x(headx);
	}

	bool DamageNumber::update()
	{
		moveobj.move();

		constexpr float FADE_STEP = Constants::TIMESTEP * 1.0f / FADE_TIME;
		opacity -= FADE_STEP;

		return opacity.last() <= 0.0f;
	}

	int16_t DamageNumber::rowheight(bool critical)
	{
		return critical ? 36 : 30;
	}

	void DamageNumber::init()
	{
		charsets[DamageNumber::Type::NORMAL].set(false, nl::nx::effect["BasicEff.img"]["NoRed1"],
												 Charset::Alignment::LEFT);
		charsets[DamageNumber::Type::NORMAL].set(true, nl::nx::effect["BasicEff.img"]["NoRed0"],
												 Charset::Alignment::LEFT);
		charsets[DamageNumber::Type::CRITICAL].set(false, nl::nx::effect["BasicEff.img"]["NoCri1"],
												   Charset::Alignment::LEFT);
		charsets[DamageNumber::Type::CRITICAL].set(true, nl::nx::effect["BasicEff.img"]["NoCri0"],
												   Charset::Alignment::LEFT);
		charsets[DamageNumber::Type::TOPLAYER].set(false, nl::nx::effect["BasicEff.img"]["NoViolet1"],
												   Charset::Alignment::LEFT);
		charsets[DamageNumber::Type::TOPLAYER].set(true, nl::nx::effect["BasicEff.img"]["NoViolet0"],
												   Charset::Alignment::LEFT);
	}

	BoolPair<Charset> DamageNumber::charsets[NUM_TYPES];
}