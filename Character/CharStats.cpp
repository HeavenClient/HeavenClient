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
#include "CharStats.h"

#include "StatCaps.h"

namespace ms
{
	CharStats::CharStats(const StatsEntry& s) : name(s.name), petids(s.petids), hp(s.hp), maxhp(s.maxhp), mp(s.mp), maxmp(s.maxmp), exp(s.exp), mapid(s.mapid), portal(s.portal), rank(s.rank), jobrank(s.jobrank), basestats(s.stats), female(s.female)
	{
		job = basestats[MapleStat::Id::JOB];

		init_totalstats();
	}

	CharStats::CharStats() {
	}

	void CharStats::init_totalstats()
	{
		basestats.clear(); // TODO: Makes all stats to uint32_t
		totalstats.clear();
		buffdeltas.clear();
		percentages.clear();

		basestats.emplace(MapleStat::Id::LEVEL, get_stat(MapleStat::Id::LEVEL));
		basestats.emplace(MapleStat::Id::FAME, get_stat(MapleStat::Id::FAME));
		basestats.emplace(MapleStat::Id::JOB, get_stat(MapleStat::Id::JOB));
		//basestats.emplace(MapleStat::Id::MAXHP, static_cast<uint16_t>(get_maxhp()));
		//basestats.emplace(MapleStat::Id::MAXMP, static_cast<uint16_t>(get_maxmp()));
		basestats.emplace(MapleStat::Id::STR, get_stat(MapleStat::Id::STR));
		basestats.emplace(MapleStat::Id::DEX, get_stat(MapleStat::Id::DEX));
		basestats.emplace(MapleStat::Id::LUK, get_stat(MapleStat::Id::LUK));
		basestats.emplace(MapleStat::Id::INT, get_stat(MapleStat::Id::INT));

		totalstats[EquipStat::Id::HP] = hp;
		totalstats[EquipStat::Id::MP] = mp;
		totalstats[EquipStat::Id::STR] = get_stat(MapleStat::Id::STR);
		totalstats[EquipStat::Id::DEX] = get_stat(MapleStat::Id::DEX);
		totalstats[EquipStat::Id::INT] = get_stat(MapleStat::Id::INT);
		totalstats[EquipStat::Id::LUK] = get_stat(MapleStat::Id::LUK);
		totalstats[EquipStat::Id::SPEED] = 100;
		totalstats[EquipStat::Id::JUMP] = 100;

		maxdamage = 0;
		mindamage = 0;
		honor = 0;
		attackspeed = 0;
		projectilerange = 400;
		mastery = 0.0f;
		critical = 0.05f;
		mincrit = 0.5f;
		maxcrit = 0.75f;
		damagepercent = 0.0f;
		bossdmg = 0.0f;
		ignoredef = 0.0f;
		stance = 0.0f;
		resiststatus = 0.0f;
		reducedamage = 0.0f;
	}

	void CharStats::close_totalstats()
	{
		totalstats[EquipStat::Id::ACC] += calculateaccuracy();

		for (auto iter : percentages)
		{
			EquipStat::Id stat = iter.first;
			int32_t total = totalstats[stat];
			total += static_cast<int32_t>(total * iter.second);
			set_total(stat, total);
		}

		int32_t primary = get_primary_stat();
		int32_t secondary = get_secondary_stat();
		int32_t attack = 0;
		if (weapontype == Weapon::Type::WAND || weapontype == Weapon::Type::STAFF) {
			attack = get_total(EquipStat::Id::MAGIC);
		}
		else {
			attack = get_total(EquipStat::Id::WATK);
		}
		float multiplier = damagepercent + static_cast<float>(attack) / 10;

		// Calculate Damage in range:
		maxdamage = static_cast<int32_t>((primary + secondary) * multiplier);
		mindamage = static_cast<int32_t>(((primary * 0.9f * mastery) + secondary) * multiplier);
	}

	int32_t CharStats::calculateaccuracy() const
	{
		int32_t totaldex = get_total(EquipStat::Id::DEX);
		int32_t totalluk = get_total(EquipStat::Id::LUK);

		return static_cast<int32_t>(totaldex * 0.8f + totalluk * 0.5f);
	}

	int32_t CharStats::get_primary_stat() const
	{
		EquipStat::Id primary = job.get_primary(weapontype);

		return static_cast<int32_t>(get_multiplier() * get_total(primary));
	}

	int32_t CharStats::get_secondary_stat() const
	{
		EquipStat::Id secondary = job.get_secondary(weapontype);

		return get_total(secondary);
	}

	float CharStats::get_multiplier() const
	{
		switch (weapontype)
		{
		case Weapon::Type::SWORD_1H:
			return 4.0f;
		case Weapon::Type::AXE_1H:
		case Weapon::Type::MACE_1H:
		case Weapon::Type::WAND:
		case Weapon::Type::STAFF:
			return 4.4f;
		case Weapon::Type::DAGGER:
		case Weapon::Type::CROSSBOW:
		case Weapon::Type::CLAW:
		case Weapon::Type::GUN:
			return 3.6f;
		case Weapon::Type::SWORD_2H:
			return 4.6f;
		case Weapon::Type::AXE_2H:
		case Weapon::Type::MACE_2H:
		case Weapon::Type::KNUCKLE:
			return 4.8f;
		case Weapon::Type::SPEAR:
		case Weapon::Type::POLEARM:
			return 5.0f;
		case Weapon::Type::BOW:
			return 3.4f;
		default:
			return 0.0f;
		}
	}

	void CharStats::set_stat(MapleStat::Id stat, uint16_t value)
	{
		basestats[stat] = value;
	}

	void CharStats::set_total(EquipStat::Id stat, int32_t value)
	{
		auto iter = EQSTAT_CAPS.find(stat);

		if (iter != EQSTAT_CAPS.end())
		{
			int32_t cap_value = iter->second;

			if (value > cap_value)
				value = cap_value;
		}

		totalstats[stat] = value;
	}

	void CharStats::add_buff(EquipStat::Id stat, int32_t value)
	{
		int32_t current = get_total(stat);
		set_total(stat, current + value);
		buffdeltas[stat] += value;
	}

	void CharStats::add_value(EquipStat::Id stat, int32_t value)
	{
		int32_t current = get_total(stat);
		set_total(stat, current + value);
	}

	void CharStats::add_percent(EquipStat::Id stat, float percent)
	{
		percentages[stat] += percent;
	}
	void CharStats::add_range(int32_t range)
	{
		projectilerange += range;
	}

	void CharStats::set_weapontype(Weapon::Type w)
	{
		weapontype = w;
	}

	void CharStats::set_exp(int64_t e)
	{
		exp = e;
	}

	void CharStats::set_portal(uint8_t p)
	{
		portal = p;
	}

	void CharStats::set_mastery(float m)
	{
		mastery = 0.5f + m;
	}

	void CharStats::add_damagepercent(float d)
	{
		damagepercent += d;
	}

	void CharStats::set_damagepercent(float d)
	{
		damagepercent = d;
	}

	void CharStats::set_reducedamage(float r)
	{
		reducedamage = r;
	}
	void CharStats::set_resistance(float re) ///////////
	{
		resiststatus = re;
	}

	void CharStats::change_job(uint16_t id)
	{
		basestats[MapleStat::Id::JOB] = id;
		job.change_job(id);
	}

	int32_t CharStats::calculate_damage(int32_t mobatk) const
	{
		// TODO: Random stuff, need to find the actual formula somewhere.
		auto weapon_def = get_total(EquipStat::Id::WDEF);

		if (weapon_def == 0)
			return mobatk;

		int32_t reduceatk = mobatk / 2 + mobatk / weapon_def;

		return reduceatk - static_cast<int32_t>(reduceatk * reducedamage);
	}

	bool CharStats::is_damage_buffed() const
	{
		return get_buffdelta(EquipStat::Id::WATK) > 0 || get_buffdelta(EquipStat::Id::MAGIC) > 0;
	}

	uint16_t CharStats::get_stat(MapleStat::Id stat) const
	{
		return basestats[stat];
	}

	EnumMap<MapleStat::Id, uint16_t> CharStats::get_basestats() const
	{
		return basestats;
	}

	int32_t CharStats::get_total(EquipStat::Id stat) const
	{
		return totalstats[stat];
	}

	int32_t CharStats::get_buffdelta(EquipStat::Id stat) const
	{
		return buffdeltas[stat];
	}

	Rectangle<int16_t> CharStats::get_range() const
	{
		return Rectangle<int16_t>(-projectilerange, -5, -50, 50);
	}


	void CharStats::set_mapid(int32_t id)
	{
		mapid = id;
	}

	int32_t CharStats::get_mapid() const
	{
		return mapid;
	}

	uint8_t CharStats::get_portal() const
	{
		return portal;
	}

	void CharStats::set_hp(uint32_t newhp)
	{
		hp = newhp;
	}

	void CharStats::add_hp(int32_t delta)
	{
		hp = hp + delta;
	}

	uint32_t CharStats::get_hp() const
	{
		return hp;
	}

	void CharStats::set_maxhp(uint32_t newmaxhp)
	{
		maxhp = newmaxhp;
	}

	uint32_t CharStats::get_maxhp() const
	{
		return maxhp;
	}

	void CharStats::set_mp(uint32_t newmp)
	{
		mp = newmp;
	}

	void CharStats::add_mp(int32_t delta)
	{
		mp = mp + delta;
	}

	uint32_t CharStats::get_mp() const
	{
		return mp;
	}

	void CharStats::set_maxmp(uint32_t newmaxmp)
	{
		maxmp = newmaxmp;
	}

	uint32_t CharStats::get_maxmp() const
	{
		return maxmp;
	}

	int64_t CharStats::get_exp() const
	{
		return exp;
	}

	const std::string& CharStats::get_name() const
	{
		return name;
	}

	const std::string& CharStats::get_jobname() const
	{
		return job.get_name();
	}

	Weapon::Type CharStats::get_weapontype() const
	{
		return weapontype;
	}

	float CharStats::get_mastery() const
	{
		return mastery;
	}

	void CharStats::add_critical(float c)
	{
		critical += c;
	}

	void CharStats::set_critical(float c)
	{
		critical = c;
	}

	float CharStats::get_critical() const
	{
		return critical;
	}

	float CharStats::get_mincrit() const
	{
		return mincrit;
	}

	float CharStats::get_maxcrit() const
	{
		return maxcrit;
	}

	float CharStats::get_reducedamage() const
	{
		return reducedamage;
	}

	float CharStats::get_bossdmg() const
	{
		return bossdmg;
	}

	float CharStats::get_ignoredef() const
	{
		return ignoredef;
	}

	void CharStats::set_stance(float s)
	{
		stance = s;
	}

	float CharStats::get_stance() const
	{
		return stance;
	}

	float CharStats::get_resistance() const
	{
		return resiststatus;
	}

	int32_t CharStats::get_maxdamage() const
	{
		return maxdamage;
	}

	int32_t CharStats::get_mindamage() const
	{
		return mindamage;
	}

	uint16_t CharStats::get_honor() const
	{
		return honor;
	}

	void CharStats::set_attackspeed(int8_t as)
	{
		attackspeed = as;
	}

	int8_t CharStats::get_attackspeed() const
	{
		return attackspeed;
	}

	const Job& CharStats::get_job() const
	{
		return job;
	}

	bool CharStats::get_female() const
	{
		return female;
	}

	void CharStats::set_female(uint8_t type)
	{
		female = type;
	}
}
