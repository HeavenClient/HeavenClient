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

#include "CharStats.h"

#include <memory>
#include <unordered_map>

namespace ms
{   //ConditionlessBuff = Multi Use;
	//PassiveBuff = One;
	// Interface for passive buffs
	class PassiveBuff
	{
	public:
		virtual ~PassiveBuff() {}

		virtual bool is_applicable(CharStats& stats, nl::node level) const = 0;
		virtual void apply_to(CharStats& stats, nl::node level) const = 0;
	};

	// Abstract base for passives without conditions
	class ConditionlessBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const final override;
	};

	template <Weapon::Type...W>
	// //Mastery
	class WeaponMasteryBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const override;
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	//Angel Blessing Job 0:
	class AngelBlessingBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	//Reduce Damage Class Warrior:
	class AchillesBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	//Berserk Job DK
	class BerserkBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const override;
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	
	//Shield Mastery Warrior + Shadower
	class ShieldMasteryBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	// Element resistance Class Mage:
	class ElementResistanceBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	// Element Amplification
	class ElementamplificationBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//Nimle Body | Bullet Time (Thief | Pirate)
	class NimbleBodyBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//The Blessing of Amazon 
	class TheBlessingofAmazonBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//Thrust Class Bow:
	class ThrustBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//Bow | CrossBow Expert
	class BowCrossBowExpertBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//Advance Charge
	class AdvanceChargeBuff : public PassiveBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};
	//Improve Range
	class ImproveRangeBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	// Collection of passive buffs
	class PassiveBuffs
	{
	public:
		// Register all effects
		PassiveBuffs();

		// Apply a passive skill effect to the character stats
		void apply_buff(CharStats& stats, int32_t skill_id, int32_t skill_level) const;

	private:
		std::unordered_map<int32_t, std::unique_ptr<PassiveBuff>> buffs;
	};
}
