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
#include "Afterimage.h"

#include "../../Util/Misc.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	Afterimage::Afterimage(int32_t skill_id, const std::string& name, const std::string& stance_name, int16_t level)
	{
		nl::node src;

		if (skill_id > 0)
		{
			std::string strid = string_format::extend_id(skill_id, 7);
			src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid]["afterimage"][name][stance_name];
		}

		if (!src)
			src = nl::nx::character["Afterimage"][name + ".img"][level / 10][stance_name];

		range = src;
		firstframe = 0;
		displayed = false;

		for (nl::node sub : src)
		{
			uint8_t frame = string_conversion::or_default<uint8_t>(sub.name(), 255);

			if (frame < 255)
			{
				animation = sub;
				firstframe = frame;
			}
		}
	}

	Afterimage::Afterimage()
	{
		firstframe = 0;
		displayed = true;
	}

	void Afterimage::draw(uint8_t stframe, const DrawArgument& args, float alpha) const
	{
		if (!displayed && stframe >= firstframe)
			animation.draw(args, alpha);
	}

	void Afterimage::update(uint8_t stframe, uint16_t timestep)
	{
		if (!displayed && stframe >= firstframe)
			displayed = animation.update(timestep);
	}

	uint8_t Afterimage::get_first_frame() const
	{
		return firstframe;
	}

	Rectangle<int16_t> Afterimage::get_range() const
	{
		return range;
	}
}