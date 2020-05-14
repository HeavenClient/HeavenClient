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
	namespace Buffstat
	{
		enum Id
		{
			NONE,
			MORPH,
			RECOVERY,
			MAPLE_WARRIOR,
			STANCE,
			SHARP_EYES,
			MANA_REFLECTION,
			SHADOW_CLAW,
			INFINITY_,
			HOLY_SHIELD,
			HAMSTRING,
			BLIND,
			CONCENTRATE,
			ECHO_OF_HERO,
			GHOST_MORPH,
			AURA,
			CONFUSE,
			BERSERK_FURY,
			DIVINE_BODY,
			SPARK,
			FINALATTACK,
			BATTLESHIP,
			WATK,
			WDEF,
			MATK,
			MDEF,
			ACC,
			AVOID,
			HANDS,
			SHOWDASH,
			SPEED,
			JUMP,
			MAGIC_GUARD,
			DARKSIGHT,
			BOOSTER,
			POWERGUARD,
			HYPERBODYHP,
			HYPERBODYMP,
			INVINCIBLE,
			SOULARROW,
			STUN,
			POISON,
			SEAL,
			DARKNESS,
			COMBO,
			SUMMON,
			WK_CHARGE,
			DRAGONBLOOD,
			HOLY_SYMBOL,
			MESOUP,
			SHADOWPARTNER,
			PICKPOCKET,
			PUPPET,
			MESOGUARD,
			WEAKEN,
			DASH,
			DASH2,
			ELEMENTAL_RESET,
			ARAN_COMBO,
			COMBO_DRAIN,
			COMBO_BARRIER,
			BODY_PRESSURE,
			SMART_KNOCKBACK,
			PYRAMID_PQ,
			ENERGY_CHARGE,
			MONSTER_RIDING,
			HOMING_BEACON,
			SPEED_INFUSION,
			LENGTH
		};

		extern const std::unordered_map<Id, uint64_t> first_codes;
		extern const std::unordered_map<Id, uint64_t> second_codes;
	}

	struct Buff
	{
		Buffstat::Id stat;
		int16_t value;
		int32_t skillid;
		int32_t duration;

		constexpr Buff(Buffstat::Id stat, int16_t value, int32_t skillid, int32_t duration) : stat(stat), value(value),
			skillid(skillid),
			duration(duration)
		{}

		constexpr Buff() : Buff(Buffstat::Id::NONE, 0, 0, 0)
		{}
	};
}