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

#include "../../Graphics/Texture.h"

#include <unordered_map>

namespace ms
{
	class Charset
	{
	public:
		enum Alignment
		{
			LEFT,
			CENTER,
			RIGHT
		};

		Charset(nl::node source, Alignment alignment);

		Charset();

		void draw(int8_t character, const DrawArgument &args) const;

		int16_t draw(const std::string &text, const DrawArgument &args) const;

		int16_t draw(const std::string &text, int16_t hspace, const DrawArgument &args) const;

		int16_t getw(int8_t character) const;

	private:
		std::unordered_map<int8_t, Texture> chars;
		Alignment alignment;
	};
}