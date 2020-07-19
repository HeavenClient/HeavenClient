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
#include "MapPortals.h"

#include "../../Constants.h"

#include "../../Util/Misc.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	MapPortals::MapPortals(nl::node src, int32_t mapid)
	{
		for (auto sub : src)
		{
			int8_t portal_id = string_conversion::or_default<int8_t>(sub.name(), -1);

			if (portal_id < 0)
				continue;

			Portal::Type type = Portal::typebyid(sub["pt"]);
			std::string name = sub["pn"];
			std::string target_name = sub["tn"];
			int32_t target_id = sub["tm"];
			Point<int16_t> position = {sub["x"], sub["y"]};

			const Animation* animation = &animations[type];
			bool intramap = target_id == mapid;

			portals_by_id.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(portal_id),
				std::forward_as_tuple(animation, type, name, intramap, position, target_id, target_name)
			);

			portal_ids_by_name.emplace(name, portal_id);
		}

		cooldown = WARPCD;
	}

	MapPortals::MapPortals()
	{
		cooldown = WARPCD;
	}

	void MapPortals::update(Point<int16_t> playerpos)
	{
		animations[Portal::REGULAR].update(Constants::TIMESTEP);
		animations[Portal::HIDDEN].update(Constants::TIMESTEP);

		for (auto& iter : portals_by_id)
		{
			Portal& portal = iter.second;
			switch (portal.get_type())
			{
				case Portal::HIDDEN:
				case Portal::TOUCH:
					portal.update(playerpos);
					break;
			}
		}

		if (cooldown > 0)
			cooldown--;
	}

	void MapPortals::draw(Point<int16_t> viewpos, float inter) const
	{
		for (auto& ptit : portals_by_id)
			ptit.second.draw(viewpos, inter);
	}

	Point<int16_t> MapPortals::get_portal_by_id(uint8_t portal_id) const
	{
		auto iter = portals_by_id.find(portal_id);

		if (iter != portals_by_id.end())
		{
			constexpr Point<int16_t> ABOVE(0, 30);

			return iter->second.get_position() - ABOVE;
		}
		else
		{
			return {};
		}
	}

	Point<int16_t> MapPortals::get_portal_by_name(const std::string& portal_name) const
	{
		auto iter = portal_ids_by_name.find(portal_name);

		if (iter != portal_ids_by_name.end())
			return get_portal_by_id(iter->second);
		else
			return {};
	}

	Portal::WarpInfo MapPortals::find_warp_at(Point<int16_t> playerpos)
	{
		if (cooldown == 0)
		{
			cooldown = WARPCD;

			for (auto& iter : portals_by_id)
			{
				const Portal& portal = iter.second;

				if (portal.bounds().contains(playerpos))
					return portal.getwarpinfo();
			}
		}

		return {};
	}

	void MapPortals::init()
	{
		nl::node src = nl::nx::map["MapHelper.img"]["portal"]["game"];

		animations[Portal::HIDDEN] = src["ph"]["default"]["portalContinue"];
		animations[Portal::REGULAR] = src["pv"];
	}

	std::unordered_map<Portal::Type, Animation> MapPortals::animations;
}