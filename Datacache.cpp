/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Datacache.h"

namespace Data
{
	Datacache::Datacache() {}

	Datacache::~Datacache() {}

	void Datacache::init()
	{
		equips.init();
		/*items.init();
		effects.init();
		sounds.init();*/
	}

	const ItemData& Datacache::getitem(int32_t iid)
	{
		int32_t prefix = iid / 1000000;
		if (prefix == 1)
		{
			return equips.getequip(iid);
		}
		else
		{
			return items.getitem(iid);
		}
	}
}