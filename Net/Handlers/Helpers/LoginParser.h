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
	namespace LoginParser
	{
		Account parse_account(InPacket& recv);
		World parse_world(InPacket& recv);
		RecommendedWorld parse_recommended_world(InPacket& recv);
		CharEntry parse_charentry(InPacket& recv);
		StatsEntry parse_stats(InPacket& recv);
		LookEntry parse_look(InPacket& recv);
		void parse_login(InPacket& recv);
	}
}