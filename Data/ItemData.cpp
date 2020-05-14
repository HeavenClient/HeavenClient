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
#include "ItemData.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	ItemData::ItemData(int32_t id) : itemid(id)
	{
		untradable = false;
		unique = false;
		unsellable = false;
		cashitem = false;
		gender = 0;

		nl::node src;
		nl::node strsrc;

		std::string strprefix = "0" + std::to_string(get_item_prefix(itemid));
		std::string strid = "0" + std::to_string(itemid);
		int32_t prefix = get_prefix(itemid);

		switch (prefix)
		{
			case 1:
				category = get_eqcategory(itemid);
				src = nl::nx::character[category][strid + ".img"]["info"];
				strsrc = nl::nx::string["Eqp.img"]["Eqp"][category][std::to_string(itemid)];
				break;
			case 2:
				category = "Consume";
				src = nl::nx::item["Consume"][strprefix + ".img"][strid]["info"];
				strsrc = nl::nx::string["Consume.img"][std::to_string(itemid)];
				break;
			case 3:
				category = "Install";
				src = nl::nx::item["Install"][strprefix + ".img"][strid]["info"];
				strsrc = nl::nx::string["Ins.img"][std::to_string(itemid)];
				break;
			case 4:
				category = "Etc";
				src = nl::nx::item["Etc"][strprefix + ".img"][strid]["info"];
				strsrc = nl::nx::string["Etc.img"]["Etc"][std::to_string(itemid)];
				break;
			case 5:
				category = "Cash";
				src = nl::nx::item["Cash"][strprefix + ".img"][strid]["info"];
				strsrc = nl::nx::string["Cash.img"][std::to_string(itemid)];
				break;
		}

		if (src)
		{
			icons[false] = src["icon"];
			icons[true] = src["iconRaw"];
			price = src["price"];
			untradable = src["tradeBlock"].get_bool();
			unique = src["only"].get_bool();
			unsellable = src["notSale"].get_bool();
			cashitem = src["cash"].get_bool();
			gender = get_item_gender(itemid);

			name = std::string(strsrc["name"]);
			desc = std::string(strsrc["desc"]);

			valid = true;
		}
		else
		{
			valid = false;
		}
	}

	std::string ItemData::get_eqcategory(int32_t id) const
	{
		constexpr char* categorynames[15] =
			{
				"Cap",
				"Accessory",
				"Accessory",
				"Accessory",
				"Coat",
				"Longcoat",
				"Pants",
				"Shoes",
				"Glove",
				"Shield",
				"Cape",
				"Ring",
				"Accessory",
				"Accessory",
				"Accessory"
			};

		int32_t index = get_item_prefix(id) - 100;

		if (index < 15)
			return categorynames[index];
		else if (index >= 30 && index <= 70)
			return "Weapon";
		else
			return "";
	}

	int32_t ItemData::get_prefix(int32_t id) const
	{
		return id / 1000000;
	}

	int32_t ItemData::get_item_prefix(int32_t id) const
	{
		return id / 10000;
	}

	int8_t ItemData::get_item_gender(int32_t id) const
	{
		const int32_t item_prefix = get_item_prefix(id);

		if ((get_prefix(id) != 1 && item_prefix != 254) || item_prefix == 119 || item_prefix == 168)
			return 2;

		const int32_t gender_digit = id / 1000 % 10;

		return (gender_digit > 1) ? 2 : gender_digit;
	}

	bool ItemData::is_valid() const
	{
		return valid;
	}

	bool ItemData::is_untradable() const
	{
		return untradable;
	}

	bool ItemData::is_unique() const
	{
		return unique;
	}

	bool ItemData::is_unsellable() const
	{
		return unsellable;
	}

	bool ItemData::is_cashitem() const
	{
		return cashitem;
	}

	ItemData::operator bool() const
	{
		return is_valid();
	}

	int32_t ItemData::get_id() const
	{
		return itemid;
	}

	int32_t ItemData::get_price() const
	{
		return price;
	}

	int8_t ItemData::get_gender() const
	{
		return gender;
	}

	const std::string& ItemData::get_name() const
	{
		return name;
	}

	const std::string& ItemData::get_desc() const
	{
		return desc;
	}

	const std::string& ItemData::get_category() const
	{
		return category;
	}

	const Texture& ItemData::get_icon(bool raw) const
	{
		return icons[raw];
	}
}