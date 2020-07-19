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
#include "EquipQuality.h"

#include "../../Data/EquipData.h"

namespace ms
{
	EquipQuality::Id
	EquipQuality::check_quality(int32_t item_id, bool scrolled, const EnumMap<EquipStat::Id, uint16_t>& stats)
	{
		const EquipData& data = EquipData::get(item_id);

		int16_t delta = 0;

		for (auto iter : stats)
		{
			EquipStat::Id es = iter.first;
			uint16_t stat = iter.second;
			uint16_t defstat = data.get_defstat(es);
			delta += stat - defstat;
		}

		if (delta < -5)
			return scrolled ? EquipQuality::Id::ORANGE : EquipQuality::Id::GREY;
		else if (delta < 7)
			return scrolled ? EquipQuality::Id::ORANGE : EquipQuality::Id::WHITE;
		else if (delta < 14)
			return EquipQuality::Id::BLUE;
		else if (delta < 21)
			return EquipQuality::Id::VIOLET;
		else
			return EquipQuality::Id::GOLD;
	}
}