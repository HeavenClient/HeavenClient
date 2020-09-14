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
#include "MonsterBook.h"

namespace ms
{
	MonsterBook::MonsterBook()
	{
		bookLevel = 1;
		normalCard = 0;
		specialCard = 0;
		cover = 0;
	}

	void MonsterBook::add_card(int16_t card, int8_t level)
	{
		cards[card] = level;
	}

	int32_t MonsterBook::get_level()
	{
		return bookLevel;
	}

	void MonsterBook::set_level(int32_t bookLevel)
	{
		bookLevel = bookLevel;
	}

	int32_t MonsterBook::get_special()
	{
		return specialCard;
	}

	void MonsterBook::set_special(int32_t specialCard)
	{
		specialCard = specialCard;
	}

	int32_t MonsterBook::get_normal()
	{
		return normalCard;
	}

	void MonsterBook::set_normal(int32_t normalCard)
	{
		normalCard = normalCard;
	}

	int32_t MonsterBook::get_cover()
	{
		return cover;
	}

	void MonsterBook::set_cover(int32_t cov)
	{
		cover = cov;
	}
}
