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
#include "Hair.h"

#include <iostream>

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	Hair::Hair(int32_t hairid, const BodyDrawInfo &drawinfo)
	{
		nl::node hairnode = nl::nx::character["Hair"]["000" + std::to_string(hairid) + ".img"];

		for (auto stance_iter : Stance::names)
		{
			Stance::Id stance = stance_iter.first;
			const std::string &stancename = stance_iter.second;

			nl::node stancenode = hairnode[stancename];

			if (!stancenode)
				continue;

			for (uint8_t frame = 0; nl::node framenode = stancenode[frame]; ++frame)
			{
				for (nl::node layernode : framenode)
				{
					std::string layername = layernode.name();
					auto layer_iter = layers_by_name.find(layername);

					if (layer_iter == layers_by_name.end())
					{
						std::cout << "Unknown Hair::Layer name: [" << layername << "]\tLocation: [" << hairnode.name() << "][" << stancename << "][" << frame << "]" << std::endl;
						continue;
					}

					Layer layer = layer_iter->second;

					Point<int16_t> brow = layernode["map"]["brow"];
					Point<int16_t> shift = drawinfo.gethairpos(stance, frame) - brow;

					stances[stance][layer]
							.emplace(frame, layernode)
							.first->second.shift(shift);
				}
			}
		}

		name = std::string(nl::nx::string["Eqp.img"]["Eqp"]["Hair"][std::to_string(hairid)]["name"]);

		constexpr size_t NUM_COLORS = 8;

		constexpr char *haircolors[NUM_COLORS] =
				{
						"Black", "Red", "Orange", "Blonde", "Green", "Blue", "Violet", "Brown"
				};

		size_t index = hairid % 10;
		color = (index < NUM_COLORS) ? haircolors[index] : "";
	}

	void Hair::draw(Stance::Id stance, Layer layer, uint8_t frame, const DrawArgument &args) const
	{
		auto frameit = stances[stance][layer].find(frame);

		if (frameit == stances[stance][layer].end())
		return;

		frameit->second.draw(args);
	}

	const std::string &Hair::get_name() const
	{
		return name;
	}

	const std::string &Hair::getcolor() const
	{
		return color;
	}

	const std::unordered_map<std::string, Hair::Layer> Hair::layers_by_name =
	{
		{ "hair",					Hair::Layer::DEFAULT		},
		{ "hairBelowBody",			Hair::Layer::BELOWBODY		},
		{ "hairOverHead",			Hair::Layer::OVERHEAD		},
		{ "hairShade",				Hair::Layer::SHADE			},
		{ "backHair",				Hair::Layer::BACK			},
		{ "backHairBelowCap",		Hair::Layer::BELOWCAP		},
		{ "backHairBelowCapNarrow",	Hair::Layer::BELOWCAPNARROW },
		{ "backHairBelowCapWide",	Hair::Layer::BELOWCAPWIDE	}
	};
}