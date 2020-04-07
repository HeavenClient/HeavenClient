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

#include "../../InPacket.h"
#include "../../Login.h"

namespace ms
{
	namespace CashShopParser
	{
		enum Jobs : uint16_t
		{
			EVAN = 2001,
			EVAN1 = 2200,
			EVAN2 = 2210,
			EVAN3 = 2211,
			EVAN4 = 2212,
			EVAN5 = 2213,
			EVAN6 = 2214,
			EVAN7 = 2215,
			EVAN8 = 2216,
			EVAN9 = 2217,
			EVAN10 = 2218
		};

		StatsEntry parseCharacterInfo(InPacket& recv);
		StatsEntry parseCharStats(InPacket& recv);
		bool hasSPTable(int16_t job);
		void parseRemainingSkillInfo(InPacket& recv);
	}
}