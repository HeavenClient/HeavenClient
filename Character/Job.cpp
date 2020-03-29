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
#include "Job.h"

namespace ms
{
	Job::Job(uint16_t i)
	{
		change_job(i);
	}

	Job::Job()
	{
		change_job(0);
	}

	void Job::change_job(uint16_t i)
	{
		id = i;
		name = get_name(id);

		if (id == 0)
			level = Level::BEGINNER;
		else if (id % 100 == 0)
			level = Level::FIRST;
		else if (id % 10 == 0)
			level = Level::SECOND;
		else if (id % 10 == 1)
			level = Level::THIRD;
		else
			level = Level::FOURTH;
	}

	bool Job::is_sub_job(uint16_t subid) const
	{
		for (int32_t lvit = BEGINNER; lvit <= FOURTH; lvit++)
		{
			Level lv = static_cast<Level>(lvit);

			if (subid == get_subjob(lv))
				return true;
		}

		return false;
	}

	bool Job::can_use(int32_t skill_id) const
	{
		uint16_t required = static_cast<uint16_t>(skill_id / 10000);
		return is_sub_job(required);
	}

	uint16_t Job::get_id() const
	{
		return id;
	}

	uint16_t Job::get_subjob(Level lv) const
	{
		if (lv <= level)
		{
			switch (lv)
			{
				case Level::BEGINNER:
					return 0;
				case Level::FIRST:
					return (id / 100) * 100;
				case Level::SECOND:
					return (id / 10) * 10;
				case Level::THIRD:
					return (level == Level::FOURTH) ? id - 1 : id;
				case Level::FOURTH:
					return id;
			}
		}

		return 0;
	}

	const std::string &Job::get_name() const
	{
		return name;
	}

	Job::Level Job::get_level() const
	{
		return level;
	}

	std::string Job::get_name(uint16_t jid) const
	{
		switch (jid)
		{
			case 0:
				return "Beginner";
			case 100:
				return "Swordsman";
			case 110:
				return "Fighter";
			case 111:
				return "Crusader";
			case 112:
				return "Hero";
			case 120:
				return "Page";
			case 121:
				return "White Knight";
			case 122:
				return "Paladin";
			case 130:
				return "Spearman";
			case 131:
				return "Dragon Knight";
			case 132:
				return "Dark Knight";
			case 200:
				return "Magician";
			case 210:
				return "Wizard (F/P)";
			case 211:
				return "Mage (F/P)";
			case 212:
				return "Archmage (F/P)";
			case 220:
				return "Wizard (I/L)";
			case 221:
				return "Mage (I/L)";
			case 222:
				return "Archmage (I/L)";
			case 230:
				return "Cleric";
			case 231:
				return "Priest";
			case 232:
				return "Bishop";
			case 300:
				return "Archer";
			case 310:
				return "Hunter";
			case 311:
				return "Ranger";
			case 312:
				return "Bowmaster";
			case 320:
				return "Crossbowman";
			case 321:
				return "Sniper";
			case 322:
				return "Marksman";
			case 400:
				return "Rogue";
			case 410:
				return "Assassin";
			case 411:
				return "Hermit";
			case 412:
				return "Nightlord";
			case 420:
				return "Bandit";
			case 421:
				return "Chief Bandit";
			case 422:
				return "Shadower";
			case 500:
				return "Pirate";
			case 510:
				return "Brawler";
			case 511:
				return "Marauder";
			case 512:
				return "Buccaneer";
			case 520:
				return "Gunslinger";
			case 521:
				return "Outlaw";
			case 522:
				return "Corsair";
			case 1000:
				return "Noblesse";
			case 2000:
			case 2100:
			case 2110:
			case 2111:
			case 2112:
				return "Aran";
			case 900:
				return "GM";
			case 910:
				return "SuperGM";
			default:
				return "";
		}
	}

	Equipstat::Id Job::get_primary(Weapon::Type weapontype) const
	{
		switch (id / 100)
		{
			case 2:
				return Equipstat::Id::INT;
			case 3:
				return Equipstat::Id::DEX;
			case 4:
				return Equipstat::Id::LUK;
			case 5:
				return (weapontype == Weapon::Type::GUN) ? Equipstat::Id::DEX : Equipstat::Id::STR;
			default:
				return Equipstat::Id::STR;
		}
	}

	Equipstat::Id Job::get_secondary(Weapon::Type weapontype) const
	{
		switch (id / 100)
		{
			case 2:
				return Equipstat::Id::LUK;
			case 3:
				return Equipstat::Id::STR;
			case 4:
				return Equipstat::Id::DEX;
			case 5:
				return (weapontype == Weapon::Type::GUN) ? Equipstat::Id::STR : Equipstat::Id::DEX;
			default:
				return Equipstat::Id::DEX;
		}
	}
}