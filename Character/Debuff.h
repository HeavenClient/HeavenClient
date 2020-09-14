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

#include <cstdint>
#include <unordered_map>

namespace ms
{
	namespace Debuffstat
	{
		enum Id
		{
			NULL_,
			SLOW,
			SEDUCE,
			FISHABLE,
			ZOMBIFY,
			CONFUSE,
			STUN,
			POISON,
			SEAL,
			DARKNESS,
			WEAKEN,
			CURSE
		};

		extern const std::unordered_map<Id, uint64_t> first_codes;
		extern const std::unordered_map<Id, uint64_t> second_codes;
	}

	struct Debuff
	{
		Debuffstat::Id stat;
		int16_t value;
		int16_t skillid;
		int16_t skilllevel;
		int32_t duration;

		constexpr Debuff(Debuffstat::Id stat, int16_t value, int16_t skillid, int16_t skilllevel, int32_t duration) : stat(stat), value(value), skillid(skillid), skilllevel(skilllevel), duration(duration) {}
		constexpr Debuff() : Debuff(Debuffstat::Id::NULL_, 0, 0, 0, 0) {}
	};
}