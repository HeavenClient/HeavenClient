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

namespace ms
{
	namespace SkillId
	{
		enum Id : uint32_t
		{
			// Beginner
					FOLLOW_THE_LEAD = 8,
			ANGEL_BLESSING = 12,
			THREE_SNAILS = 1000,
			HEAL = 1001,
			FEATHER = 1002,
			LEGENDARY_SPIRIT = 1003,
			MONSTER_RIDER = 1004,
			ECHO_OF_THE_HERO = 1005,

			// Swordman
					IMPROVED_HP_RECOVERY = 1000000,
			IMPROVED_MAX_HP = 1000001,
			IMPROVED_LADDER_RECOVERY = 1000002,
			IRON_BODY = 1001003,
			POWER_STRIKE = 1001004,
			SLASH_BLAST = 1001005,

			// Fighter
					SWORD_MASTERY_FIGHTER = 1100000,
			AXE_MASTERY = 1100001,
			SWORD_FA_FIGHTER = 1100002,
			AXE_FA = 1100003,
			SWORD_BOOSTER_FIGHTER = 1101004,
			AXE_BOOSTER = 1101005,
			RAGE = 1101006,
			POWER_GUARD_FIGHTER = 1101007,

			// Crusader
					IMPROVED_MP_RECOVERY_CRUSADER = 1110000,
			SHIELD_MASTERY = 1110001,
			SWORD_PANIC = 1111003,
			AXE_PANIC = 1111004,
			SWORD_COMA = 1111005,
			AXE_COMA = 1111005,
			SHOUT = 1111008,

			// Hero
					ADVANCED_COMBO_ATTACK = 1120003,
			ACHILLES_HERO = 1120004,
			GUARDIAN_HERO = 1120005,
			RUSH_HERO = 1121006,
			BRANDISH = 1121008,

			// Page
					SWORD_MASTERY_PAGE = 1200000,
			BW_MASTERY = 1200001,
			SWORD_FA_PAGE = 1200002,
			BW_FA = 1200003,
			SWORD_BOOSTER_PAGE = 1201004,
			BW_BOOSTER = 1201005,
			THREATEN = 1201006,
			POWER_GUARD_PAGE = 1201007,

			// White Knight
					CHARGE = 1211002,

			// Paladin
					ACHILLES_PALADIN = 1220005,
			GUARDIAN_PALADIN = 1220006,
			ADVANCED_CHARGE = 1220010,
			RUSH_PALADIN = 1221007,
			BLAST = 1221009,
			HEAVENS_HAMMER = 1221011,

			// Spearman
					SPEAR_MASTERY = 1300000,
			PA_MASTERY = 1300001,
			SPEAR_FA = 1300002,
			PA_FA = 1300003,
			SPEAR_BOOSTER = 1301004,
			PA_BOOSTER = 1301005,
			IRON_WILL = 1301006,
			HYPER_BODY = 1301007,

			// Dragon Knight
					DRAGON_BUSTER = 1311001,
			DRAGON_FURY = 1311002,
			PA_BUSTER = 1311003,
			PA_FURY = 1311004,
			SACRIFICE = 1311005,
			DRAGONS_ROAR = 1311006,

			// Dark Knight
					ACHILLES_DK = 1320005,
			BERSERK = 1320006,
			BEHOLDER_HEAL = 1320008,
			BEHOLDER_BUFF = 1320009,
			RUSH_DK = 1321003,
			BEHOLDER = 1321007,

			// Magician
					IMPROVE_HP_RECOVERY = 2000000,
			IMPROVE_MAX_HP = 2000001,
			MAGIC_GUARD = 2001002,
			MAGIC_ARMOR = 2001003,
			ENERGY_BOLT = 2001004,
			MAGIC_CLAW = 2001005,

			// F/P Mage
					MP_EATER_FP = 2100000,
			MEDITATION_FP = 2101001,
			TELEPORT_FP = 2101002,
			SLOW_FP = 2101003,
			FIRE_ARROW = 2101004,
			POISON_BREATH = 2101005,

			// F/P Archmage
					PARTIAL_RESISTANCE_FP = 2110000,
			ELEMENT_AMPLIFICATION_FP = 2110001,
			EXPLOSION = 2111002,
			POISON_MIST = 2111003,
			SEAL_FP = 2111004,
			SPELL_BOOSTER_FP = 2111005,
			ELEMENT_COMPOSITION_FP = 2111006,

			// TODO: ?
			FIRE_DEMON = 2121003,
			ELQUINES = 2121005,
			PARALYZE = 2121006,
			METEOR_SHOWER = 2121007,

			// I/L Mage
					IL_TELEPORT = 2201002,

			// Priest
					PRIEST_TELEPORT = 2301001,

			// Hermit
					AVENGER = 4111005,
			FLASH_JUMP = 4111006,

			// Night Lord
					TRIPLE_THROW = 4121007,

			// Bandit
					MESO_EXPLOSION = 4211006
		};
	}
}