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

#include "Nametag.h"

namespace ms
{
	Nametag::Nametag(nl::node src, Text::Font f, std::string n)
	{
		name = OutlinedText(f, Text::Alignment::CENTER, Color::Name::EAGLE, Color::Name::JAMBALAYA);
		name.change_text(n);

		textures[false].push_back(src["0"]["0"]);
		textures[false].push_back(src["0"]["1"]);
		textures[false].push_back(src["0"]["2"]);
		textures[true].push_back(src["1"]["0"]);
		textures[true].push_back(src["1"]["1"]);
		textures[true].push_back(src["1"]["2"]);

		selected = false;
	}

	void Nametag::draw(Point<int16_t> position) const
	{
		position = position + Point<int16_t>(1, 2);

		auto &tag = textures[selected];

		int16_t width = name.width();
		auto startpos = position - Point<int16_t>(6 + width / 2, -1);

		tag[0].draw(startpos);
		tag[1].draw(DrawArgument(startpos + Point<int16_t>(6, 0), Point<int16_t>(width, 0)));
		tag[2].draw(DrawArgument(startpos + Point<int16_t>(width + 6, 0)));

		name.draw(position);
	}

	void Nametag::set_selected(bool s)
	{
		selected = s;

		if (s)
			name.change_color(Color::Name::WHITE);
		else
			name.change_color(Color::Name::EAGLE);
	}
}