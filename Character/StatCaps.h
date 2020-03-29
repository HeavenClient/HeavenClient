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
#include <cstdint>

namespace ms
{
	// Externalising this for easier configurability.
	const std::unordered_map<Equipstat::Id, int32_t> EQSTAT_CAPS =
			{
					{Equipstat::Id::STR,   999},
					{Equipstat::Id::DEX,   999},
					{Equipstat::Id::INT,   999},
					{Equipstat::Id::LUK,   999},
					{Equipstat::Id::HP,    30000},
					{Equipstat::Id::MP,    30000},
					{Equipstat::Id::WATK,  999},
					{Equipstat::Id::MAGIC, 2000},
					{Equipstat::Id::WDEF,  999},
					{Equipstat::Id::MDEF,  999},
					{Equipstat::Id::ACC,   999},
					{Equipstat::Id::AVOID, 999},
					{Equipstat::Id::HANDS, 999},
					{Equipstat::Id::SPEED, 140},
					{Equipstat::Id::JUMP,  123},
			};
}