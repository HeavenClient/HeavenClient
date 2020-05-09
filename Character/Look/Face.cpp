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
#include "Face.h"

#include <iostream>

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	Expression::Id Expression::byaction(size_t action)
	{
		action -= 98;

		if (action < Expression::Id::LENGTH)
			return static_cast<Id>(action);

		std::cout << "Unknown Expression::Id action: [" << action << "]" << std::endl;

		return Expression::Id::DEFAULT;
	}

	const EnumMap<Expression::Id, std::string> Expression::names =
			{
					"default",
					"blink",
					"hit",
					"smile",
					"troubled",
					"cry",
					"angry",
					"bewildered",
					"stunned",
					"blaze",
					"bowing",
					"cheers",
					"chu",
					"dam",
					"despair",
					"glitter",
					"hot",
					"hum",
					"love",
					"oops",
					"pain",
					"shine",
					"vomit",
					"wink"
			};

	Face::Face(int32_t faceid)
	{
		std::string strid = "000" + std::to_string(faceid);
		nl::node facenode = nl::nx::character["Face"][strid + ".img"];

		for (auto iter : Expression::names)
		{
			Expression::Id exp = iter.first;

			if (exp == Expression::Id::DEFAULT)
			{
				expressions[Expression::Id::DEFAULT].emplace(0, facenode["default"]);
			} else
			{
				const std::string &expname = iter.second;
				nl::node expnode = facenode[expname];

				for (uint8_t frame = 0; nl::node framenode = expnode[frame]; ++frame)
					expressions[exp].emplace(frame, framenode);
			}
		}

		name = std::string(nl::nx::string["Eqp.img"]["Eqp"]["Face"][std::to_string(faceid)]["name"]);
	}

	void Face::draw(Expression::Id expression, uint8_t frame, const DrawArgument &args) const
	{
		auto frameit = expressions[expression].find(frame);

		if (frameit != expressions[expression].end())
			frameit->second.texture.draw(args);
	}

	uint8_t Face::nextframe(Expression::Id exp, uint8_t frame) const
	{
		return expressions[exp].count(frame + 1) ? frame + 1 : 0;
	}

	int16_t Face::get_delay(Expression::Id exp, uint8_t frame) const
	{
		auto delayit = expressions[exp].find(frame);
		return delayit != expressions[exp].end() ? delayit->second.delay : 100;
	}

	const std::string &Face::get_name() const
	{
		return name;
	}
}