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
#include "SkillTooltip.h"

#include "../../Data/SkillData.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	SkillTooltip::SkillTooltip()
	{
		nl::node Frame = nl::nx::ui["UIToolTip.img"]["Item"]["Frame2"];

		frame = Frame;
		cover = Frame["cover"];

		skill_id = 0;
	}

	void SkillTooltip::set_skill(int32_t id, int32_t level, int32_t mlevel, int64_t expiration)
	{
		if (skill_id == id)
			return;

		skill_id = id;

		if (skill_id == 0)
			return;

		const SkillData &data = SkillData::get(id);

		int32_t masterlevel;

		if (mlevel > 0)
			masterlevel = mlevel;
		else
			masterlevel = data.get_masterlevel();

		std::string descstr = data.get_desc();

		if (masterlevel > 0)
		{
			const std::string mltag = "Master Level";
			const std::string mlstr = std::to_string(masterlevel);
			size_t mlstart = descstr.find(mltag);
			size_t mlpos = descstr.find(':', mlstart) + 2;
			size_t mlend = descstr.find("]", mlstart);

			if (mlpos < mlend && mlend != std::string::npos)
			{
				size_t mlsize = mlend - mlpos;
				descstr.erase(mlpos, mlsize);
				descstr.insert(mlpos, mlstr);

				// Fixing errors in the files...
				if (mlstart == 0)
				{
					descstr.insert(0, "[");
					mlend++;
				}

				size_t linebreak = descstr.find("]\\n", mlstart);

				if (linebreak != mlend)
					descstr.insert(mlend + 1, "\\n");
			} else
			{
				descstr.insert(0, "[" + mltag + ": " + mlstr + "]\\n");
			}
		}

		const std::string exptag = "#cAvailable until";

		if (expiration > 0)
		{
			// TODO: Blank
		} else
		{
			size_t expstart = descstr.find(exptag);
			size_t expend = descstr.find('#', expstart + 1);

			if (expstart < expend && expend != std::string::npos)
			{
				size_t expsize = expend - expstart + 1;
				descstr.erase(expstart, expsize);
			}
		}

		std::string levelstr;
		bool current = level > 0;
		bool next = level < masterlevel;

		if (current)
			levelstr += "[Current Level: " + std::to_string(level) + "]\\n" + data.get_level_desc(level);

		if (current && next)
			levelstr += "\\n";

		if (next)
			levelstr += "[Next Level: " + std::to_string(level + 1) + "]\\n" + data.get_level_desc(level + 1);

		icon = data.get_icon(SkillData::Icon::NORMAL);
		name = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::WHITE, data.get_name(), 320);
		desc = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE, descstr, 210);
		leveldesc = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE, levelstr, 290);

		int16_t desc_height = desc.height() + 11;

		icon_offset = name.height();
		level_offset = std::max<int16_t>(desc_height, 85);
		height = icon_offset + level_offset + leveldesc.height();

		int16_t icon_width = (icon.get_dimensions().x() * 2) + 4;
		width = 292;

		line = ColorLine(width + 16, Color::Name::WHITE, 1.0f, false);
		box = ColorBox(icon_width, icon_width, Color::Name::WHITE, 0.65f);
	}

	void SkillTooltip::draw(Point<int16_t> pos) const
	{
		if (skill_id == 0)
			return;

		int16_t max_width = Constants::Constants::get().get_viewwidth();
		int16_t max_height = Constants::Constants::get().get_viewheight();
		int16_t cur_width = pos.x() + width + 45;
		int16_t cur_height = pos.y() + height + 35;

		int16_t adj_x = cur_width - max_width;
		int16_t adj_y = cur_height - max_height;

		if (adj_x > 0)
			pos.shift_x(adj_x * -1);

		if (adj_y > 0)
			pos.shift_y(adj_y * -1);

		frame.draw(pos + Point<int16_t>(176, height + 11), width, height - 1);
		name.draw(pos + Point<int16_t>(33, 3));
		cover.draw(pos + Point<int16_t>(16, -1));

		pos.shift_y(icon_offset);

		box.draw(DrawArgument(pos + Point<int16_t>(26, 21)));
		icon.draw(DrawArgument(pos + Point<int16_t>(28, 87), 2.0f, 2.0f));
		desc.draw(pos + Point<int16_t>(102, 15));

		pos.shift_y(level_offset);

		line.draw(pos + Point<int16_t>(22, 10));
		leveldesc.draw(pos + Point<int16_t>(25, 11));
	}
}