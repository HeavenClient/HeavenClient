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

#include "../../Graphics/Animation.h"

namespace ms
{
	class Afterimage
	{
	public:
		Afterimage(int32_t skill_id, const std::string& name, const std::string& stance, int16_t level);

		Afterimage();

		void draw(uint8_t stframe, const DrawArgument& args, float alpha) const;

		void update(uint8_t stframe, uint16_t timestep);

		uint8_t get_first_frame() const;

		Rectangle<int16_t> get_range() const;

	private:
		Animation animation;
		Rectangle<int16_t> range;
		uint8_t firstframe;
		bool displayed;
	};
}