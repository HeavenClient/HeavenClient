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
#include "CalcDamage.h"
#include "Rand32.h"

namespace ms
{
	CalcDamage::CalcDamage() {}

	void CalcDamage::set_seed(int32_t server_seed1, int32_t server_seed2, int32_t server_seed3)
	{
		Rand32 RndGenForCharacter = Rand32();
		RndGenForCharacter.set_seed(server_seed1, server_seed1, server_seed3);

		Rand32 RndForCheckDamageMiss = Rand32();
		RndForCheckDamageMiss.set_seed(server_seed1, server_seed1, server_seed3);

		Rand32 RndForMortalBlow = Rand32();
		RndForMortalBlow.set_seed(server_seed1, server_seed1, server_seed3);

		Rand32 RndForSummoned = Rand32();
		RndForSummoned.set_seed(server_seed1, server_seed1, server_seed3);

		Rand32 RndForMob = Rand32();
		RndForSummoned.set_seed(server_seed1, server_seed1, server_seed3);

		Rand32 RndGenForMob = Rand32();
		RndGenForMob.set_seed(server_seed1, server_seed1, server_seed3);
	}

	Rand32& CalcDamage::get_RndGenForCharacter() {
		return RndGenForCharacter;
	}

	Rand32& CalcDamage::get_RndForCheckDamageMiss() {
		return RndForCheckDamageMiss;
	}

	Rand32& CalcDamage::get_RndForMortalBlow() {
		return RndForMortalBlow;
	}

	Rand32& CalcDamage::get_RndForSummoned() {
		return RndForSummoned;
	}

	Rand32& CalcDamage::get_RndGenForMob() {
		return RndGenForMob;
	}
}
