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
#include "Debuff.h"

namespace ms
{
	namespace Debuffstat
	{
		const std::unordered_map<Id, uint64_t> first_codes =
		{ 
			{}
		};

		const std::unordered_map<Id, uint64_t> second_codes =
		{
			{ Id::NULL_, 0x0 },
			{ Id::SLOW, 0x1 },
			{ Id::SEDUCE, 0x80 },
			{ Id::FISHABLE, 0x100 },
			{ Id::ZOMBIFY, 0x4000 },
			{ Id::CONFUSE, 0x80000 },
			{ Id::STUN, 0x2000000000000L },
			{ Id::POISON, 0x4000000000000L },
			{ Id::SEAL, 0x8000000000000L },
			{ Id::DARKNESS, 0x10000000000000L },
			{ Id::WEAKEN, 0x4000000000000000L },
			{ Id::CURSE, 0x8000000000000000L }
		};
	}
}