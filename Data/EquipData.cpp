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
#include "EquipData.h"

#include <nlnx/nx.hpp>
#include <nlnx/node.hpp>

namespace ms
{
	EquipData::EquipData(int32_t id) : itemdata(ItemData::get(id))
	{
		std::string strid = "0" + std::to_string(id);
		std::string category = itemdata.get_category();
		nl::node src = nl::nx::character[category][strid + ".img"]["info"];

		cash = src["cash"].get_bool();
		tradeblock = src["tradeBlock"].get_bool();
		slots = src["tuc"];

		reqstats[Maplestat::Id::LEVEL] = src["reqLevel"];
		reqstats[Maplestat::Id::JOB] = src["reqJob"];
		reqstats[Maplestat::Id::STR] = src["reqSTR"];
		reqstats[Maplestat::Id::DEX] = src["reqDEX"];
		reqstats[Maplestat::Id::INT] = src["reqINT"];
		reqstats[Maplestat::Id::LUK] = src["reqLUK"];
		defstats[Equipstat::Id::STR] = src["incSTR"];
		defstats[Equipstat::Id::DEX] = src["incDEX"];
		defstats[Equipstat::Id::INT] = src["incINT"];
		defstats[Equipstat::Id::LUK] = src["incLUK"];
		defstats[Equipstat::Id::WATK] = src["incPAD"];
		defstats[Equipstat::Id::WDEF] = src["incPDD"];
		defstats[Equipstat::Id::MAGIC] = src["incMAD"];
		defstats[Equipstat::Id::MDEF] = src["incMDD"];
		defstats[Equipstat::Id::HP] = src["incMHP"];
		defstats[Equipstat::Id::MP] = src["incMMP"];
		defstats[Equipstat::Id::ACC] = src["incACC"];
		defstats[Equipstat::Id::AVOID] = src["incEVA"];
		defstats[Equipstat::Id::HANDS] = src["incHANDS"];
		defstats[Equipstat::Id::SPEED] = src["incSPEED"];
		defstats[Equipstat::Id::JUMP] = src["incJUMP"];

		constexpr size_t NON_WEAPON_TYPES = 15;
		constexpr size_t WEAPON_OFFSET = NON_WEAPON_TYPES + 15;
		constexpr size_t WEAPON_TYPES = 20;
		size_t index = (id / 10000) - 100;

		if (index < NON_WEAPON_TYPES)
		{
			constexpr char *types[NON_WEAPON_TYPES] =
					{
							"HAT",
							"FACE ACCESSORY",
							"EYE ACCESSORY",
							"EARRINGS",
							"TOP",
							"OVERALL",
							"BOTTOM",
							"SHOES",
							"GLOVES",
							"SHIELD",
							"CAPE",
							"RING",
							"PENDANT",
							"BELT",
							"MEDAL"
					};

			constexpr Equipslot::Id equipslots[NON_WEAPON_TYPES] =
					{
							Equipslot::Id::HAT,
							Equipslot::Id::FACE,
							Equipslot::Id::EYEACC,
							Equipslot::Id::EARACC,
							Equipslot::Id::TOP,
							Equipslot::Id::TOP,
							Equipslot::Id::BOTTOM,
							Equipslot::Id::SHOES,
							Equipslot::Id::GLOVES,
							Equipslot::Id::SHIELD,
							Equipslot::Id::CAPE,
							Equipslot::Id::RING1,
							Equipslot::Id::PENDANT1,
							Equipslot::Id::BELT,
							Equipslot::Id::MEDAL
					};

			type = types[index];
			eqslot = equipslots[index];
		} else if (index >= WEAPON_OFFSET && index < WEAPON_OFFSET + WEAPON_TYPES)
		{
			constexpr char *types[WEAPON_TYPES] =
					{
							"ONE-HANDED SWORD",
							"ONE-HANDED AXE",
							"ONE-HANDED MACE",
							"DAGGER",
							"", "", "",
							"WAND",
							"STAFF",
							"",
							"TWO-HANDED SWORD",
							"TWO-HANDED AXE",
							"TWO-HANDED MACE",
							"SPEAR",
							"POLEARM",
							"BOW",
							"CROSSBOW",
							"CLAW",
							"KNUCKLE",
							"GUN"
					};

			size_t weaponindex = index - WEAPON_OFFSET;
			type = types[weaponindex];
			eqslot = Equipslot::Id::WEAPON;
		} else
		{
			type = "CASH";
			eqslot = Equipslot::Id::NONE;
		}
	}

	bool EquipData::is_valid() const
	{
		return itemdata.is_valid();
	}

	EquipData::operator bool() const
	{
		return is_valid();
	}

	bool EquipData::is_weapon() const
	{
		return eqslot == Equipslot::Id::WEAPON;
	}

	int16_t EquipData::get_reqstat(Maplestat::Id stat) const
	{
		return reqstats[stat];
	}

	int16_t EquipData::get_defstat(Equipstat::Id stat) const
	{
		return defstats[stat];
	}

	Equipslot::Id EquipData::get_eqslot() const
	{
		return eqslot;
	}

	const std::string &EquipData::get_type() const
	{
		return type;
	}

	const ItemData &EquipData::get_itemdata() const
	{
		return itemdata;
	}
}