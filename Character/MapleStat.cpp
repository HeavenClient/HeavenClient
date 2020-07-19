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
#include "MapleStat.h"

#include <iostream>

namespace ms
{
	namespace MapleStat
	{
		Id by_id(size_t id)
		{
			if (id >= LENGTH)
				std::cout << "Unknown MapleStat::Id id: [" << id << "]" << std::endl;

			return static_cast<Id>(id);
		}

		const EnumMap<Id, int32_t> codes =
			{
				0x1,
				0x2,
				0x4,
				0x10,
				0x20,
				0x40,
				0x80,
				0x100,
				0x200,
				0x400,
				0x800,
				0x1000,
				0x2000,
				0x4000,
				0x8000,
				0x10000,
				0x20000,
				0x40000,
				0x180008,
				0x200000
			};
	}
}