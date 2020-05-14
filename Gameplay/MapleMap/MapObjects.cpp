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
#include "MapObjects.h"

namespace ms
{
	void MapObjects::draw(Layer::Id layer, double viewx, double viewy, float alpha) const
	{
		for (auto& oid : layers[layer])
		{
			auto mmo = get(oid);

			if (mmo && mmo->is_active())
				mmo->draw(viewx, viewy, alpha);
		}
	}

	void MapObjects::update(const Physics& physics)
	{
		for (auto iter = objects.begin(); iter != objects.end();)
		{
			bool remove_mob = false;

			if (auto& mmo = iter->second)
			{
				int8_t oldlayer = mmo->get_layer();
				int8_t newlayer = mmo->update(physics);

				if (newlayer == -1)
				{
					remove_mob = true;
				}
				else if (newlayer != oldlayer)
				{
					int32_t oid = iter->first;
					layers[oldlayer].erase(oid);
					layers[newlayer].insert(oid);
				}
			}
			else
			{
				remove_mob = true;
			}

			if (remove_mob)
				iter = objects.erase(iter);
			else
				iter++;
		}
	}

	void MapObjects::clear()
	{
		objects.clear();

		for (auto& layer : layers)
			layer.clear();
	}

	bool MapObjects::contains(int32_t oid) const
	{
		return objects.count(oid) > 0;
	}

	void MapObjects::add(std::unique_ptr<MapObject> toadd)
	{
		int32_t oid = toadd->get_oid();
		int8_t layer = toadd->get_layer();
		objects[oid] = std::move(toadd);
		layers[layer].insert(oid);
	}

	void MapObjects::remove(int32_t oid)
	{
		auto iter = objects.find(oid);

		if (iter != objects.end() && iter->second)
		{
			int8_t layer = iter->second->get_layer();
			objects.erase(iter);

			layers[layer].erase(oid);
		}
	}

	Optional<MapObject> MapObjects::get(int32_t oid)
	{
		auto iter = objects.find(oid);

		return iter != objects.end() ? iter->second.get() : nullptr;
	}

	Optional<const MapObject> MapObjects::get(int32_t oid) const
	{
		auto iter = objects.find(oid);

		return iter != objects.end() ? iter->second.get() : nullptr;
	}

	MapObjects::underlying_t::iterator MapObjects::begin()
	{
		return objects.begin();
	}

	MapObjects::underlying_t::iterator MapObjects::end()
	{
		return objects.end();
	}

	MapObjects::underlying_t::const_iterator MapObjects::begin() const
	{
		return objects.begin();
	}

	MapObjects::underlying_t::const_iterator MapObjects::end() const
	{
		return objects.end();
	}

	MapObjects::underlying_t::size_type MapObjects::size() const
	{
		return objects.size();
	}
}