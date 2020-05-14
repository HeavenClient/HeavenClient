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

#include <unordered_map>

namespace ms
{
	// Externalising this for easier configurability.
	const std::unordered_map<EquipStat::Id, int32_t> EQSTAT_CAPS =
		{
			{EquipStat::Id::STR,   999},
			{EquipStat::Id::DEX,   999},
			{EquipStat::Id::INT,   999},
			{EquipStat::Id::LUK,   999},
			{EquipStat::Id::HP,    30000},
			{EquipStat::Id::MP,    30000},
			{EquipStat::Id::WATK,  999},
			{EquipStat::Id::MAGIC, 2000},
			{EquipStat::Id::WDEF,  999},
			{EquipStat::Id::MDEF,  999},
			{EquipStat::Id::ACC,   999},
			{EquipStat::Id::AVOID, 999},
			{EquipStat::Id::HANDS, 999},
			{EquipStat::Id::SPEED, 140},
			{EquipStat::Id::JUMP,  123},
		};
}