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
#include "UIBuffList.h"

#include "../Data/ItemData.h"
#include "../Util/Misc.h"

#include <nlnx/nx.hpp>
#include <nlnx/node.hpp>

namespace ms
{
	BuffIcon::BuffIcon(int32_t buff, int32_t dur) : cover(IconCover::BUFF, dur - FLASH_TIME)
	{
		buffid = buff;
		duration = dur;
		opacity.set(1.0f);
		opcstep = -0.05f;

		if (buffid >= 0)
		{
			std::string strid = string_format::extend_id(buffid, 7);
			nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];
			icon = src["icon"];
		}
		else
		{
			icon = ItemData::get(-buffid).get_icon(true);
		}
	}

	void BuffIcon::draw(Point<int16_t> position, float alpha) const
	{
		icon.draw(DrawArgument(position, opacity.get(alpha)));
		cover.draw(position + Point<int16_t>(1, -31), alpha);
	}

	bool BuffIcon::update()
	{
		if (duration <= FLASH_TIME)
		{
			opacity += opcstep;

			bool fadedout = opcstep < 0.0f && opacity.last() <= 0.0f;
			bool fadedin = opcstep > 0.0f && opacity.last() >= 1.0f;

			if (fadedout || fadedin)
				opcstep = -opcstep;
		}

		cover.update();

		duration -= Constants::TIMESTEP;

		return duration < Constants::TIMESTEP;
	}

	// TODO: This needs a dimension set on it in order to cancel active buffs and hover over icons
	UIBuffList::UIBuffList() : UIElement(Point<int16_t>(750, 40), Point<int16_t>(0, 0)) {}

	void UIBuffList::draw(float alpha) const
	{
		Point<int16_t> icpos = position;

		for (auto& icon : icons)
		{
			icon.second.draw(icpos, alpha);
			icpos.shift_x(-32);
		}
	}

	void UIBuffList::update()
	{
		for (auto iter = icons.begin(); iter != icons.end();)
		{
			bool expired = iter->second.update();

			if (expired)
				iter = icons.erase(iter);
			else
				iter++;
		}
	}

	Cursor::State UIBuffList::send_cursor(bool pressed, Point<int16_t> cursorposition)
	{
		return UIElement::send_cursor(pressed, cursorposition);
	}

	void UIBuffList::add_buff(int32_t buffid, int32_t duration)
	{
		icons.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(buffid),
			std::forward_as_tuple(buffid, duration)
		);
	}
}