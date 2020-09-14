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
#include "ActiveBuffs.h"
#include "../../Gameplay/Stage.h"
#include "math.h"
namespace ms
{
	template <EquipStat::Id STAT>
	void SimpleStatBuff<STAT>::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_buff(STAT, value);
	}

	template <EquipStat::Id STAT>
	void PercentageStatBuff<STAT>::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_percent(STAT, static_cast<float>(value) / 100);
	}

	void MapleWarriorBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.add_percent(EquipStat::Id::STR, static_cast<float>(value) / 100);
		stats.add_percent(EquipStat::Id::DEX, static_cast<float>(value) / 100);
		stats.add_percent(EquipStat::Id::INT, static_cast<float>(value) / 100);
		stats.add_percent(EquipStat::Id::LUK, static_cast<float>(value) / 100);
	}

	void StanceBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.set_stance(static_cast<float>(value) / 100);
	}

	void BoosterBuff::apply_to(CharStats& stats, int16_t value) const
	{
		stats.set_attackspeed(static_cast<int8_t>(value));
	}

	void SharpEyesBuff::apply_to(CharStats& stats, int16_t value) const
	{
		Player& player = Stage::get().get_player();
		int32_t skillid = 0;
		if (player.get_skilllevel(3121002) > 0) {
			skillid = 3121002;
		}
		else if (player.get_skilllevel(3221002) > 0) {
			skillid = 3221002;
		}
		if (skillid != 0) {
			int32_t x = ceilf(player.get_skilllevel(skillid) / 2);
			int32_t y = player.get_skilllevel(skillid) + 10;
			stats.add_critical(static_cast<float>(x) / 100);
			// TODO + damage % based on y
		}
	}

	void ConcentrateBuff::apply_to(CharStats& stats, int16_t value) const
	{
		Player& player = Stage::get().get_player();
		int32_t skillid = 3121008;
		uint32_t additionalPAD = ceilf(player.get_skilllevel(skillid) / 2) + 10;
		stats.set_total(EquipStat::Id::WATK, stats.get_total(EquipStat::Id::WATK) + additionalPAD);
		/*	if (player.get_skilllevel(skillid) <= 20) {
			stats.set_mpcost(static_cast<float>(player.get_skilllevel(skillid) * 2) / 100);
		}
		else {
			stats.set_mpcost(static_cast<float>(player.get_skilllevel(skillid) + 20) / 100);
		}*/
	}

	void EchoOfHeroBuff::apply_to(CharStats& stats, int16_t value) const
	{
		Player& player = Stage::get().get_player();
		if (player.get_skilllevel(10001005) == 0 && (player.get_skilllevel(1005) == 1 || player.get_skilllevel(20001005) == 1)) {
			stats.set_total(EquipStat::Id::WATK, static_cast<int32_t>(stats.get_total(EquipStat::Id::WATK) * 1.04));
			stats.set_total(EquipStat::Id::MAGIC, static_cast<int32_t>(stats.get_total(EquipStat::Id::MAGIC) * 1.04));
		}
		else {
			stats.set_total(EquipStat::Id::WATK, static_cast<int32_t>(stats.get_total(EquipStat::Id::WATK) * 1.02));
			stats.set_total(EquipStat::Id::MAGIC, static_cast<int32_t>(stats.get_total(EquipStat::Id::MAGIC) * 1.02));
		}
	}

	void SpecialBuff::apply_to(CharStats& stats, int16_t value) const	{}

	ActiveBuffs::ActiveBuffs()
	{
		buffs[Buffstat::Id::MORPH] = std::make_unique<SpecialBuff>(); // Client handles effects.

		buffs[Buffstat::Id::RECOVERY] = std::make_unique<SpecialBuff>(); // Server handled

		buffs[Buffstat::Id::MAPLE_WARRIOR] = std::make_unique<MapleWarriorBuff>();
		buffs[Buffstat::Id::STANCE] = std::make_unique<StanceBuff>();
		buffs[Buffstat::Id::SHARP_EYES] = std::make_unique<SharpEyesBuff>();

		buffs[Buffstat::Id::MANA_REFLECTION] = std::make_unique<SpecialBuff>(); // Server handled -HP, Client handled damageShow

		buffs[Buffstat::Id::SHADOW_CLAW] = std::make_unique<SpecialBuff>(); // Use 200 stars to throw infinite throwing-stars
		buffs[Buffstat::Id::INFINITY_] = std::make_unique<SpecialBuff>(); // Enables one to temporarily draw magic powers from sources surrounding the mage and use it in place of one's own MP.
		buffs[Buffstat::Id::HOLY_SHIELD] = std::make_unique<SpecialBuff>(); // Temporarily protects all party members from abnormal conditions.
		buffs[Buffstat::Id::HAMSTRING] = std::make_unique<SpecialBuff>(); // prop -> chance ; -x * skillLevel * 2 -> monster speed
		
		buffs[Buffstat::Id::CONCENTRATE] = std::make_unique<ConcentrateBuff>();
		buffs[Buffstat::Id::ECHO_OF_HERO] = std::make_unique<EchoOfHeroBuff>();

		// Invisible or Does'nt exist:
		buffs[Buffstat::Id::GHOST_MORPH] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::AURA] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::CONFUSE] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::BERSERK_FURY] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::DIVINE_BODY] = std::make_unique<SpecialBuff>();

		buffs[Buffstat::Id::SPARK] = std::make_unique<SpecialBuff>(); // + mobCount + damage
		buffs[Buffstat::Id::FINALATTACK] = std::make_unique<SpecialBuff>();

		buffs[Buffstat::Id::WATK] = std::make_unique<SimpleStatBuff<EquipStat::Id::WATK>>();
		buffs[Buffstat::Id::WDEF] = std::make_unique<SimpleStatBuff<EquipStat::Id::WDEF>>();
		buffs[Buffstat::Id::MATK] = std::make_unique<SimpleStatBuff<EquipStat::Id::MAGIC>>();
		buffs[Buffstat::Id::MDEF] = std::make_unique<SimpleStatBuff<EquipStat::Id::MDEF>>();
		buffs[Buffstat::Id::ACC] = std::make_unique<SimpleStatBuff<EquipStat::Id::ACC>>();
		buffs[Buffstat::Id::AVOID] = std::make_unique<SimpleStatBuff<EquipStat::Id::AVOID>>();
		buffs[Buffstat::Id::SPEED] = std::make_unique<SimpleStatBuff<EquipStat::Id::SPEED>>();
		buffs[Buffstat::Id::JUMP] = std::make_unique<SimpleStatBuff<EquipStat::Id::JUMP>>();
		buffs[Buffstat::Id::MAGIC_GUARD] = std::make_unique<SpecialBuff>(); // Server handled

		buffs[Buffstat::Id::DARKSIGHT] = std::make_unique<SpecialBuff>(); // Client handle on Effect; Server handles TakeDamage

		buffs[Buffstat::Id::BOOSTER] = std::make_unique<BoosterBuff>();

		buffs[Buffstat::Id::POWERGUARD] = std::make_unique<SpecialBuff>(); // Server handled - damage, Client handled damageShow

		buffs[Buffstat::Id::HYPERBODYHP] = std::make_unique<PercentageStatBuff<EquipStat::Id::HP>>();
		buffs[Buffstat::Id::HYPERBODYMP] = std::make_unique<PercentageStatBuff<EquipStat::Id::MP>>();

		buffs[Buffstat::Id::INVINCIBLE] = std::make_unique<SpecialBuff>(); // -(10 + skilllevel)% Weapon Damage received
		buffs[Buffstat::Id::SOULARROW] = std::make_unique<SpecialBuff>(); // Temporarily allows the character to fire bow arrows without using up the arrows
		
		// Debuff to Monsters: (Server handled)
		buffs[Buffstat::Id::BLIND] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::STUN] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::POISON] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::SEAL] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::DARKNESS] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::WEAKEN] = std::make_unique<SpecialBuff>();
		buffs[Buffstat::Id::CONFUSE] = std::make_unique<SpecialBuff>();
		//

		buffs[Buffstat::Id::COMBO] = std::make_unique<SpecialBuff>(); // Handle on Attacks
		buffs[Buffstat::Id::SUMMON] = std::make_unique<SpecialBuff>(); // Handle on Summon.cpp
		buffs[Buffstat::Id::WK_CHARGE] = std::make_unique<SpecialBuff>(); // Add Effects on Attacks: slow, frozen, fire, lighting,...  + damage to monster
		buffs[Buffstat::Id::DRAGONBLOOD] = std::make_unique<SpecialBuff>(); // Only add effect on client, -hp by time is handled on Server
		buffs[Buffstat::Id::HOLY_SYMBOL] = std::make_unique<SpecialBuff>(); // Add if skilllevel <= 20, +(skillLevel*2)% expGain, else +(skillLevel +20)% expGain
		buffs[Buffstat::Id::MESOUP] = std::make_unique<SpecialBuff>(); // Server handled
		buffs[Buffstat::Id::SHADOWPARTNER] = std::make_unique<SpecialBuff>(); // Create a shadow 
		buffs[Buffstat::Id::PICKPOCKET] = std::make_unique<SpecialBuff>(); // Server handled
		buffs[Buffstat::Id::PUPPET] = std::make_unique<SpecialBuff>(); // Client handles effect summon, Server handles HP 
		buffs[Buffstat::Id::MESOGUARD] = std::make_unique<SpecialBuff>(); // Server handled

			
	}

	void ActiveBuffs::apply_buff(CharStats& stats, Buffstat::Id stat, int16_t value) const
	{
		if (auto& buff = buffs[stat])
			buff->apply_to(stats, value);
	}
}