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

#include "MapleFrame.h"
#include "Tooltip.h"

#include "../../Graphics/Geometry.h"

namespace ms
{
	class MapTooltip : public Tooltip
	{
	public:
		MapTooltip();

		void draw(Point<int16_t> position) const override;

		void set_name(Tooltip::Parent parent, std::string name, bool bolded);

		void set_desc(std::string description);

		void set_mapid(int32_t mapid);

		void reset();

	private:
		static constexpr uint8_t MAX_LIFE = 10u;
		static constexpr uint8_t MIN_WIDTH = 166u;
		static constexpr uint8_t BOTTOM_PADDING = 8u;
		static constexpr Point<int16_t> SEPARATOR_ADJ = Point<int16_t>(5, 0);
		static constexpr Point<int16_t> LIFE_LABEL_ADJ = Point<int16_t>(20, 3);
		static constexpr Point<int16_t> LIFE_ICON_ADJ = Point<int16_t>(5, 9);

		MapleFrame frame;

		Texture cover;
		Texture Mob;
		Texture Npc;
		Texture Party;

		Tooltip::Parent parent;

		std::string name;
		std::string description;

		Text name_label;
		Text name_simple;
		Text desc_label;
		Text desc_simple;
		Text mob_labels[MAX_LIFE];
		Text npc_labels[MAX_LIFE];

		int16_t fillwidth;
		int16_t fillheight;

		ColorLine separator;
	};
}