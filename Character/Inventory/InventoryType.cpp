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
#include "InventoryType.h"

#include <iostream>

namespace ms
{
	InventoryType::Id InventoryType::by_item_id(int32_t item_id)
	{
		constexpr Id values_by_id[6] =
			{
				NONE,
				EQUIP,
				USE,
				SETUP,
				ETC,
				CASH
			};

		int32_t prefix = item_id / 1000000;

		return (prefix > Id::NONE && prefix <= Id::CASH) ? values_by_id[prefix] : Id::NONE;
	}

	InventoryType::Id InventoryType::by_value(int8_t value)
	{
		switch (value)
		{
			case -1:
				return Id::EQUIPPED;
			case 1:
				return Id::EQUIP;
			case 2:
				return Id::USE;
			case 3:
				return Id::SETUP;
			case 4:
				return Id::ETC;
			case 5:
				return Id::CASH;
		}

		std::cout << "Unknown InventoryType::Id value: [" << value << "]" << std::endl;

		return Id::NONE;
	}
}