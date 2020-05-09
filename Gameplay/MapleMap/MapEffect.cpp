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
#include "MapEffect.h"

#include "../../Constants.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	MapEffect::MapEffect(std::string path) : active(false)
	{
		nl::node Effect = nl::nx::map["Effect.img"];

		effect = Effect.resolve(path);

		int16_t width = Constants::Constants::get().get_viewwidth();

		position = Point<int16_t>(width / 2, 250);
	}

	MapEffect::MapEffect()
	{}

	void MapEffect::draw() const
	{
		if (!active)
			effect.draw(position, 1.0f);
	}

	void MapEffect::update()
	{
		if (!active)
			active = effect.update(6);
	}
}