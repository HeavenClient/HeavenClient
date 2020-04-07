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

#include "Buff.h"
#include "CharStats.h"

#include <memory>

namespace ms
{
	// Interface for active buffs which are applied to character stats
	class ActiveBuff
	{
	public:
		virtual ~ActiveBuff()
		{}

		virtual void apply_to(CharStats &stats, int16_t value) const = 0;
	};

	template<EquipStat::Id STAT>
	// Template for buffs which just add their value to a stat
	class SimpleStatBuff : public ActiveBuff
	{
		void apply_to(CharStats &stats, int16_t value) const override;
	};

	template<EquipStat::Id STAT>
	// Template for buffs which apply an increase by percentage
	class PercentageStatBuff : public ActiveBuff
	{
		void apply_to(CharStats &stats, int16_t value) const override;
	};

	// Buff for MAPLEWARRIOR
	class MapleWarriorBuff : public ActiveBuff
	{
		void apply_to(CharStats &stats, int16_t value) const override;
	};

	// Buff for STANCE
	class StanceBuff : public ActiveBuff
	{
		void apply_to(CharStats &stats, int16_t value) const override;
	};

	// Buff for BOOSTER
	class BoosterBuff : public ActiveBuff
	{
		void apply_to(CharStats &stats, int16_t value) const override;
	};

	class ActiveBuffs
	{
	public:
		// Register all buffs effects
		ActiveBuffs();

		// Return the buff effect associated with the buff stat.
		void apply_buff(CharStats &stats, Buffstat::Id stat, int16_t value) const;

	private:
		EnumMap<Buffstat::Id, std::unique_ptr<ActiveBuff>> buffs;
	};
}