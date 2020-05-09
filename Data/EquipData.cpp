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

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

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

		reqstats[MapleStat::Id::LEVEL] = src["reqLevel"];
		reqstats[MapleStat::Id::JOB] = src["reqJob"];
		reqstats[MapleStat::Id::STR] = src["reqSTR"];
		reqstats[MapleStat::Id::DEX] = src["reqDEX"];
		reqstats[MapleStat::Id::INT] = src["reqINT"];
		reqstats[MapleStat::Id::LUK] = src["reqLUK"];
		defstats[EquipStat::Id::STR] = src["incSTR"];
		defstats[EquipStat::Id::DEX] = src["incDEX"];
		defstats[EquipStat::Id::INT] = src["incINT"];
		defstats[EquipStat::Id::LUK] = src["incLUK"];
		defstats[EquipStat::Id::WATK] = src["incPAD"];
		defstats[EquipStat::Id::WDEF] = src["incPDD"];
		defstats[EquipStat::Id::MAGIC] = src["incMAD"];
		defstats[EquipStat::Id::MDEF] = src["incMDD"];
		defstats[EquipStat::Id::HP] = src["incMHP"];
		defstats[EquipStat::Id::MP] = src["incMMP"];
		defstats[EquipStat::Id::ACC] = src["incACC"];
		defstats[EquipStat::Id::AVOID] = src["incEVA"];
		defstats[EquipStat::Id::HANDS] = src["incHANDS"];
		defstats[EquipStat::Id::SPEED] = src["incSPEED"];
		defstats[EquipStat::Id::JUMP] = src["incJUMP"];

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

			constexpr EquipSlot::Id equipslots[NON_WEAPON_TYPES] =
					{
							EquipSlot::Id::HAT,
							EquipSlot::Id::FACE,
							EquipSlot::Id::EYEACC,
							EquipSlot::Id::EARACC,
							EquipSlot::Id::TOP,
							EquipSlot::Id::TOP,
							EquipSlot::Id::BOTTOM,
							EquipSlot::Id::SHOES,
							EquipSlot::Id::GLOVES,
							EquipSlot::Id::SHIELD,
							EquipSlot::Id::CAPE,
							EquipSlot::Id::RING1,
							EquipSlot::Id::PENDANT1,
							EquipSlot::Id::BELT,
							EquipSlot::Id::MEDAL
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
			eqslot = EquipSlot::Id::WEAPON;
		} else
		{
			type = "CASH";
			eqslot = EquipSlot::Id::NONE;
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
		return eqslot == EquipSlot::Id::WEAPON;
	}

	int16_t EquipData::get_reqstat(MapleStat::Id stat) const
	{
		return reqstats[stat];
	}

	int16_t EquipData::get_defstat(EquipStat::Id stat) const
	{
		return defstats[stat];
	}

	EquipSlot::Id EquipData::get_eqslot() const
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