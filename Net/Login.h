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

#include "../Character/MapleStat.h"

#include <map>
#include <vector>

namespace ms
{
	struct Account
	{
		int32_t accid;
		int8_t female;
		bool admin;
		std::string name;
		bool muted;
		bool pin;
		int8_t pic;
	};

	struct World
	{
		std::string name;
		std::string message;
		std::vector<int32_t> chloads;
		uint8_t channelcount;
		uint8_t flag;
		int8_t wid;
	};

	struct RecommendedWorld
	{
		std::string message;
		int32_t wid;
	};

	struct StatsEntry
	{
		std::string name;
		bool female;
		std::vector<int64_t> petids;
		EnumMap<MapleStat::Id, uint16_t> stats;
		int64_t exp;
		int32_t mapid;
		uint8_t portal;
		std::pair<int32_t, int8_t> rank;
		std::pair<int32_t, int8_t> jobrank;
	};

	struct LookEntry
	{
		bool female;
		uint8_t skin;
		int32_t faceid;
		int32_t hairid;
		std::map<int8_t, int32_t> equips;
		std::map<int8_t, int32_t> maskedequips;
		std::vector<int32_t> petids;
	};

	struct CharEntry
	{
		StatsEntry stats;
		LookEntry look;
		int32_t id;
	};
}