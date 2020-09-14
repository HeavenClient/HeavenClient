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
#include "Rand32.h"

namespace ms
{
	Rand32::Rand32() {}

	int64_t Rand32::random() {
		int32_t v3;
		int32_t v4;
		int32_t v5;

		v3 = ((client_seed1 & 0xFFFFFFFE) << 12) ^ ((client_seed1 & 0x7FFC0 ^ (client_seed1 >> 13)) >> 6);
		v4 = 16 * (client_seed2 & 0xFFFFFFF8) ^ (((client_seed2 >> 2) ^ client_seed2 & 0x3F800000) >> 23);
		v5 = ((client_seed3 & 0xFFFFFFF0) << 17) ^ (((client_seed3 >> 3) ^ client_seed3 & 0x1FFFFF00) >> 8);

		client_seed3 = v5;
		client_seed1 = v3;
		client_seed2 = v4;

		return (client_seed1 ^ client_seed2 ^ client_seed3) & 0xFFFFFFFFL;
	}

	void Rand32::set_seed(int32_t server_seed1, int32_t server_seed2, int32_t server_seed3)
	{
		client_seed1 = server_seed1;
		client_seed2 = server_seed2;
		client_seed3 = server_seed3;
	}
}