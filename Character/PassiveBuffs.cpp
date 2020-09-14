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
#include "PassiveBuffs.h"

#include "SkillId.h"

#include "../Util/Misc.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	bool ConditionlessBuff::is_applicable(CharStats&, nl::node) const
	{
		return true;
	}



	template<Weapon::Type W1, Weapon::Type W2>
	bool f_is_applicable(CharStats& stats, nl::node level)
	{
		return f_is_applicable<W1>(stats, level) || f_is_applicable<W2>(stats, level);
	}

	template<Weapon::Type W1>
	bool f_is_applicable(CharStats& stats, nl::node)
	{
		return stats.get_weapontype() == W1;
	}
	
	template <Weapon::Type...W>
	bool WeaponMasteryBuff<W...>::is_applicable(CharStats& stats, nl::node level) const
	{
		return f_is_applicable<W...>(stats, level);
	}
	//Mastery
	template <Weapon::Type...W>
	void WeaponMasteryBuff<W...>::apply_to(CharStats& stats, nl::node level) const
	{
		float mastery = static_cast<float>(level["mastery"]) / 100;
		stats.set_mastery(mastery);
		stats.add_value(EquipStat::Id::ACC, level["x"]);
	}
	//Angel Blessing Job 0:
	void AngelBlessingBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_value(EquipStat::Id::WATK, level["x"]);
		stats.add_value(EquipStat::Id::MAGIC, level["y"]);
		stats.add_value(EquipStat::Id::ACC, level["z"]);
		stats.add_value(EquipStat::Id::AVOID, level["z"]);
	}

	//Reduce Damage Warrior:
	void AchillesBuff::apply_to(CharStats& stats, nl::node level) const
	{
		float reducedamage = static_cast<float>(level["x"]) / 1000;
		stats.set_reducedamage(reducedamage);
	}

	//Berserk Job DK
	bool BerserkBuff::is_applicable(CharStats& stats, nl::node level) const
	{
		float hp_percent = static_cast<float>(level["x"]) / 100;
		int32_t hp_threshold = static_cast<int32_t>(stats.get_total(EquipStat::Id::HP) * hp_percent);
		int32_t hp_current = stats.get_hp();

		return hp_current <= hp_threshold;
	}
	void BerserkBuff::apply_to(CharStats& stats, nl::node level) const
	{
		float damagepercent = static_cast<float>(level["damage"]) / 100;
		stats.set_damagepercent(damagepercent);
	}

	//Shield Mastery Warrior + Shadower
	void ShieldMasteryBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_percent(EquipStat::Id::WDEF, static_cast<float>(level["x"]) /100);
	}

	//Element resistance Class Mage:
	void ElementResistanceBuff::apply_to(CharStats& stats, nl::node level) const
	{
		float resistance= static_cast<float>(level["x"]) / 100;
		stats.set_resistance(resistance);
	}

	// Element resistance Class Mage:
	void ElementamplificationBuff::apply_to(CharStats& stats, nl::node level) const
	{	
		//SU DUNG MANA THEM X% THEO SKILL! UNHANDLED
		stats.add_value(EquipStat::Id::MAGIC, static_cast<float>(level["y"]) / 100);
	}

	//Nimle Body | Bullet Time (Thief | Pirate)
	void NimbleBodyBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_value(EquipStat::Id::ACC, level["x"]);
		stats.add_value(EquipStat::Id::AVOID, level["y"]);
	}

	//The Blessing of Amazon 
	void TheBlessingofAmazonBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_value(EquipStat::Id::ACC, level["x"]);
	}

	//Thrust Class Bow: 
	void ThrustBuff::apply_to(CharStats& stats, nl::node level) const
	{
		stats.add_value(EquipStat::Id::SPEED, level["speed"]);
	}

	//Bow | CrossBow Expert
	void BowCrossBowExpertBuff::apply_to(CharStats& stats, nl::node level) const {
		float mastery = static_cast<float>(level["mastery"]) / 100;
		stats.set_mastery(mastery);
	}

	//Advance Charge
	void AdvanceChargeBuff::apply_to(CharStats& stats, nl::node level) const {
		//ADD damage vao 1 skil job 3 khi su dung skill do
	}
	//Improve Range
	void ImproveRangeBuff::apply_to(CharStats& stats, nl::node level) const {
		int32_t range = static_cast<int32_t>(level["range"]);
		stats.add_range(range);
	}
	//


	//TODO

	//ALCHEMIST
	//SHADOWN SHIFTER | Guardian -> khi mob attack chinh minh thi co chance co hoi block
	//STUN MASTERY -> khi attack mob co x% ti le tang damage
	//CRITICAL DAMAGE CUA ARCHER -> add th
	//ADVANCE_COMBO ATTACK? Unhandel Combo Attack

	//MP EATER THEO CHANCE


	PassiveBuffs::PassiveBuffs()
	{
		// Beginner
			buffs[SkillId::Id::ANGEL_BLESSING] = std::make_unique<AngelBlessingBuff>();
		
		// Warrior
		// Fighter
			buffs[SkillId::Id::SWORD_MASTERY_FIGHTER] = std::make_unique<WeaponMasteryBuff<Weapon::Type::SWORD_1H, Weapon::Type::SWORD_2H>>();
			buffs[SkillId::Id::AXE_MASTERY_FIGHTER] = std::make_unique<WeaponMasteryBuff<Weapon::Type::AXE_1H, Weapon::Type::AXE_2H>>();
		// Crusader
			buffs[SkillId::Id::SHIELD_MASTERY_CRUSADER] = std::make_unique<ShieldMasteryBuff>();
		// Hero
			buffs[SkillId::Id::ACHILLES_HERO] = std::make_unique<AchillesBuff>();
		// Page
			buffs[SkillId::Id::FA_SWORD_PAGE] = std::make_unique<WeaponMasteryBuff<Weapon::Type::SWORD_1H, Weapon::Type::SWORD_2H>>();
			buffs[SkillId::Id::BW_MASTERY_PAGE] = std::make_unique<WeaponMasteryBuff<Weapon::Type::MACE_1H, Weapon::Type::MACE_2H>>();
		// White Knight
			//CHARGEDBLOW
			buffs[SkillId::Id::SHIELD_MASTERY_WHITE_KNIGHT] = std::make_unique<ShieldMasteryBuff>();
		// Paladin
			buffs[SkillId::Id::ACHILLES_PALADIN] = std::make_unique<AchillesBuff>();
		// Spearman
			buffs[SkillId::Id::SPEAR_MASTERY_SPEARMAN] = std::make_unique<WeaponMasteryBuff<Weapon::Type::SPEAR>>();
			buffs[SkillId::Id::PA_MASTERY_SPEARMAN] = std::make_unique<WeaponMasteryBuff<Weapon::Type::POLEARM>>();
		// Dragon Knight
			buffs[SkillId::Id::ELEMENTAL_RESISTANCE_DK] = std::make_unique<ElementResistanceBuff>();
		// Dark Knight
			buffs[SkillId::Id::ACHILLES_DK] = std::make_unique<AchillesBuff>();
			buffs[SkillId::Id::BERSERK] = std::make_unique<BerserkBuff>();
		
		// Magician
		// Fire/Poison Wizard
		// Fire/Poison Mage 
			buffs[SkillId::Id::PARTIAL_RESISTANCE_FP] = std::make_unique<ElementResistanceBuff>();
			buffs[SkillId::Id::ELEMENT_AMPLIFICATION_FP] = std::make_unique<ElementamplificationBuff>();
		// Fire/Poison Archmage
		// Ice/Lightning Wizard
		// Ice/Lightning Mage
			buffs[SkillId::Id::PARTIAL_RESISTANCE_FP] = std::make_unique<ElementResistanceBuff>();
			buffs[SkillId::Id::ELEMENT_AMPLIFICATION_IL] = std::make_unique<ElementamplificationBuff>();
		// Ice/Lightning Archmage
		// Cleric 
		// Priest
			buffs[SkillId::Id::ELEMENTAL_RESISTANCE_PRIEST] = std::make_unique<ElementResistanceBuff>();
		// Bishop
		
		// Archer
			buffs[SkillId::Id::THE_BLESSING_OF_AMAZON] = std::make_unique<TheBlessingofAmazonBuff>();
			buffs[SkillId::Id::THE_EYE_OF_AMAZON] = std::make_unique<ImproveRangeBuff>();
		// Hunter
			buffs[SkillId::Id::BOW_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::BOW>>();
		// Ranger
			buffs[SkillId::Id::THRUST_BOWMASTER] = std::make_unique<ThrustBuff>();
		// BowMaster
			buffs[SkillId::Id::BOW_EXPERT] = std::make_unique<BowCrossBowExpertBuff>();
		// CrossBowman
			buffs[SkillId::Id::CROSSBOW_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::CROSSBOW>>();
		// Sniper 
			buffs[SkillId::Id::THRUST_SNIPER] = std::make_unique<ThrustBuff>();
		// Marksman
			buffs[SkillId::Id::MARKSMAN_BOOST] = std::make_unique<BowCrossBowExpertBuff>();
		
		// Rogue
			buffs[SkillId::Id::NIMBLE_BODY] = std::make_unique<NimbleBodyBuff>();
			buffs[SkillId::Id::KEEN_EYES] = std::make_unique<ImproveRangeBuff>();
		// Assasin 
			buffs[SkillId::Id::CLAW_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::CLAW>>();
		// Hermit
		// Night Lord
		// Bandit
			buffs[SkillId::Id::DAGGER_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::DAGGER>>();
		// Chief Bandit
			buffs[SkillId::Id::SHIELD_MASTERY] = std::make_unique<ShieldMasteryBuff>();
		// Shadower

		// Pirate
			buffs[SkillId::Id::BULLET_TIME] = std::make_unique<NimbleBodyBuff>();
		// Brawler
			buffs[SkillId::Id::KNUCKLER_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::KNUCKLE>>();
		// Marauder
		// Buccaneer
		// Gunslinger  
			buffs[SkillId::Id::GUN_MASTERY] = std::make_unique<WeaponMasteryBuff<Weapon::Type::GUN>>();
		// Outlaw 
		// Consair 
		
	}

	void PassiveBuffs::apply_buff(CharStats& stats, int32_t skill_id, int32_t skill_level) const
	{
		auto iter = buffs.find(skill_id);

		if (iter == buffs.end())
			return;

		bool wrong_job = !stats.get_job().can_use(skill_id);

		if (wrong_job)
			return;

		std::string strid;

		if (skill_id < 10000000)
			strid = string_format::extend_id(skill_id, 7);
		else
			strid = std::to_string(skill_id);

		nl::node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid]["level"][skill_level];

		const PassiveBuff* buff = iter->second.get();

		if (buff && buff->is_applicable(stats, src))
			buff->apply_to(stats, src);
	}
}
