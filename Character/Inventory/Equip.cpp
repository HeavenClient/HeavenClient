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
#include "Equip.h"

namespace ms
{
	Equip::Equip(int32_t item_id, int64_t expiration, const std::string& owner, int16_t flags, uint8_t slots,
				 uint8_t level, const EnumMap<EquipStat::Id, uint16_t>& stats, uint8_t itemlevel, int16_t itemexp,
				 int32_t vicious) : item_id(item_id), expiration(expiration), owner(owner), flags(flags), slots(slots),
		level(level), stats(stats), itemlevel(itemlevel), itemexp(itemexp),
		vicious(vicious)
	{
		potrank = Equip::Potential::POT_NONE;
		quality = EquipQuality::check_quality(item_id, level > 0, stats);
	}

	int32_t Equip::get_item_id() const
	{
		return item_id;
	}

	int64_t Equip::get_expiration() const
	{
		return expiration;
	}

	const std::string& Equip::get_owner() const
	{
		return owner;
	}

	int16_t Equip::get_flags() const
	{
		return flags;
	}

	uint8_t Equip::get_slots() const
	{
		return slots;
	}

	uint8_t Equip::get_level() const
	{
		return level;
	}

	uint8_t Equip::get_itemlevel() const
	{
		return itemlevel;
	}

	uint16_t Equip::get_stat(EquipStat::Id type) const
	{
		return stats[type];
	}

	int32_t Equip::get_vicious() const
	{
		return vicious;
	}

	Equip::Potential Equip::get_potrank() const
	{
		return potrank;
	}

	EquipQuality::Id Equip::get_quality() const
	{
		return quality;
	}
}