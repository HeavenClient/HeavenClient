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
#include "MapTooltip.h"

#include "../../Util/Misc.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	MapTooltip::MapTooltip() : name(""), description(""), fillwidth(MIN_WIDTH), fillheight(0)
	{
		nl::node Frame = nl::nx::ui["UIToolTip.img"]["Item"]["Frame2"];
		nl::node WorldMap = nl::nx::ui["UIWindow2.img"]["ToolTip"]["WorldMap"];

		frame = Frame;
		cover = Frame["cover"];
		Mob = WorldMap["Mob"];
		Npc = WorldMap["Npc"];
		Party = WorldMap["Party"];
	}

	void MapTooltip::draw(Point<int16_t> pos) const
	{
		if (name_label.empty())
			return;

		int16_t max_width = Constants::Constants::get().get_viewwidth();
		int16_t max_height = Constants::Constants::get().get_viewheight();

		if (parent == Tooltip::Parent::MINIMAP && mob_labels->empty() && npc_labels->empty())
		{
			if (desc_label.empty())
			{
				int16_t new_width = name_simple.width();
				int16_t new_height = name_simple.height();

				int16_t cur_width = pos.x() + new_width + 21;
				int16_t cur_height = pos.y() + new_height + 40;

				int16_t adj_x = cur_width - max_width;
				int16_t adj_y = cur_height - max_height;

				if (adj_x > 0)
					pos.shift_x(adj_x * -1);

				if (adj_y > 0)
					pos.shift_y(adj_y * -1);

				frame.draw(pos + Point<int16_t>(new_width / 2 + 2, new_height - 7), new_width - 14, new_height - 18);

				if (new_height > 18)
					name_simple.draw(pos);
				else
					name_simple.draw(pos + Point<int16_t>(1, -3));
			}
			else
			{
				int16_t name_width = name_label.width();
				int16_t name_height = name_label.height();

				int16_t desc_width = desc_simple.width();

				int16_t new_width = (name_width > desc_width) ? name_width : desc_width;
				int16_t new_height = name_height + desc_simple.height() - 11;

				int16_t cur_width = pos.x() + new_width + 21;
				int16_t cur_height = pos.y() + new_height + 40;

				int16_t adj_x = cur_width - max_width;
				int16_t adj_y = cur_height - max_height;

				if (adj_x > 0)
					pos.shift_x(adj_x * -1);

				if (adj_y > 0)
					pos.shift_y(adj_y * -1);

				int16_t half_width = new_width / 2;

				frame.draw(pos + Point<int16_t>(half_width + 2, new_height - 7 + BOTTOM_PADDING), new_width - 14,
						   new_height - 18 + BOTTOM_PADDING);
				cover.draw(pos + Point<int16_t>(-5, -2));
				name_label.draw(pos + Point<int16_t>(half_width, -2));

				pos.shift_y(name_height);

				separator.draw(pos + SEPARATOR_ADJ);
				desc_simple.draw(pos + Point<int16_t>(half_width, -3));
			}
		}
		else
		{
			int16_t cur_width = pos.x() + fillwidth + 21;
			int16_t cur_height = pos.y() + fillheight + 40;

			int16_t adj_x = cur_width - max_width;
			int16_t adj_y = cur_height - max_height;

			if (adj_x > 0)
				pos.shift_x(adj_x * -1);

			if (adj_y > 0)
				pos.shift_y(adj_y * -1);

			int16_t half_width = fillwidth / 2;

			frame.draw(pos + Point<int16_t>(half_width + 2, fillheight - 7 + BOTTOM_PADDING), fillwidth - 14,
					   fillheight - 18 + BOTTOM_PADDING);
			cover.draw(pos + Point<int16_t>(-5, -2));
			name_label.draw(pos + Point<int16_t>(half_width, 0));

			int16_t name_height = name_label.height();

			if (!desc_label.empty())
			{
				pos.shift_y(name_height + 4);

				desc_label.draw(pos + Point<int16_t>(4, 0));

				pos.shift_y(desc_label.height() + BOTTOM_PADDING);
			}
			else
			{
				pos.shift_y(name_height + BOTTOM_PADDING);
			}

			if (mob_labels->length() > 0)
			{
				separator.draw(pos + SEPARATOR_ADJ);

				for (size_t i = 0; i < MAX_LIFE; i++)
				{
					if (!mob_labels[i].empty())
					{
						mob_labels[i].draw(pos + LIFE_LABEL_ADJ);

						if (i == 0)
							Mob.draw(pos + LIFE_ICON_ADJ);

						pos.shift_y(mob_labels[i].height());
					}
				}

				pos.shift_y(8);
			}

			if (npc_labels->length() > 0)
			{
				separator.draw(pos + SEPARATOR_ADJ);

				for (size_t i = 0; i < MAX_LIFE; i++)
				{
					if (!npc_labels[i].empty())
					{
						npc_labels[i].draw(pos + LIFE_LABEL_ADJ);

						if (i == 0)
							Npc.draw(pos + LIFE_ICON_ADJ);

						pos.shift_y(npc_labels[i].height());
					}
				}
			}
		}
	}

	void MapTooltip::set_name(Tooltip::Parent p, std::string n, bool bolded)
	{
		if (name == n || parent == p)
			return;

		name = n;
		parent = p;

		if (name.empty() || (parent != Tooltip::Parent::WORLDMAP && parent != Tooltip::Parent::MINIMAP))
			return;

		name_label = Text(bolded ? Text::Font::A12B : Text::Font::A12M, Text::Alignment::CENTER, Color::Name::WHITE,
						  name);
		name_simple = Text(bolded ? Text::Font::A12B : Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE,
						   name);

		int16_t width = name_label.width();
		int16_t height = name_label.height();

		if (width > fillwidth)
			fillwidth = width;

		separator = ColorLine(fillwidth - 6, Color::Name::WHITE, 0.40f, false);

		if (height > fillheight)
			fillheight = height;
	}

	void MapTooltip::set_desc(std::string d)
	{
		if (description == d)
			return;

		description = d;

		if (description.empty())
			return;

		desc_label = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE, description, fillwidth);
		desc_simple = Text(Text::Font::A12M, Text::Alignment::CENTER, Color::Name::WHITE, description, fillwidth);

		fillwidth += 17;

		if (parent == Tooltip::Parent::MINIMAP)
		{
			int16_t name_width = name_label.width();
			int16_t desc_width = desc_simple.width();
			int16_t new_width = (name_width > desc_width) ? name_width : desc_width;

			separator = ColorLine(new_width - 6, Color::Name::WHITE, 0.40f, false);
		}
		else
		{
			separator = ColorLine(fillwidth - 6, Color::Name::WHITE, 0.40f, false);
		}

		fillheight += desc_label.height() + 4;
	}

	void MapTooltip::set_mapid(int32_t mapid)
	{
		size_t m = 0;
		size_t n = 0;
		auto life = NxHelper::Map::get_life_on_map(mapid);

		for (auto l : life)
		{
			auto life_object = l.second;

			if (life_object.first == "m" && m < MAX_LIFE)
			{
				mob_labels[m] = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::CHARTREUSE,
									 life_object.second);
				fillheight += mob_labels->height() + 2;
				m++;
			}
			else if (life_object.first == "n" && n < MAX_LIFE)
			{
				npc_labels[n] = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::MALIBU, life_object.second);
				fillheight += npc_labels->height() + 2;
				n++;
			}
		}

		if (desc_label.empty())
			if (mob_labels->length() > 0 || npc_labels->length() > 0)
				fillheight += BOTTOM_PADDING;
	}

	void MapTooltip::reset()
	{
		set_name(Tooltip::Parent::NONE, "", false);
		set_desc("");

		desc_label.change_text("");
		name_label.change_text("");

		for (size_t i = 0; i < MAX_LIFE; i++)
		{
			mob_labels[i].change_text("");
			npc_labels[i].change_text("");
		}

		fillwidth = MIN_WIDTH;
		fillheight = 0;
	}
}