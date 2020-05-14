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
{
	// Interface for passive buffs
	class PassiveBuff
	{
	public:
		virtual ~PassiveBuff()
		{}

		virtual bool is_applicable(CharStats& stats, nl::node level) const = 0;

		virtual void apply_to(CharStats& stats, nl::node level) const = 0;
	};

	// Abstract base for passives without conditions
	class ConditionlessBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const final override;
	};

	// Buff for angel blessing/blessing of the spirit
	class AngelBlessingBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	template <Weapon::Type...W>
	// Buff for Mastery skills.
	class WeaponMasteryBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const override;

		void apply_to(CharStats& stats, nl::node level) const override;
	};

	// Buff for Achilles
	class AchillesBuff : public ConditionlessBuff
	{
	public:
		void apply_to(CharStats& stats, nl::node level) const override;
	};

	// Buff for Berserk
	class BerserkBuff : public PassiveBuff
	{
	public:
		bool is_applicable(CharStats& stats, nl::node level) const override;

		void apply_to(CharStats& stats, nl::node level) const override;
	};

	// Collection of passive buffs
	class PassiveBuffs
	{
	public:
		// Register all effects
		PassiveBuffs();

		// Apply a passive skill effect to the character stats.
		void apply_buff(CharStats& stats, int32_t skill_id, int32_t skill_level) const;

	private:
		std::unordered_map<int32_t, std::unique_ptr<PassiveBuff>> buffs;
	};
}