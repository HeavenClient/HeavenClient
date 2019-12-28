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
#include "MapReactors.h"
#include "Reactor.h"

namespace ms
{
	void MapReactors::draw(Layer::Id layer, double viewx, double viewy, float alpha) const
	{
		reactors.draw(layer, viewx, viewy, alpha);
	}

	/*
	 * Spawns all reactors to map with proper footholds.
	 */
	void MapReactors::update(const Physics& physics)
	{
		for (; !spawns.empty(); spawns.pop())
		{
			const ReactorSpawn& spawn = spawns.front();

			int32_t oid = spawn.get_oid();

			if (auto reactor = reactors.get(oid))
				reactor->makeactive();
			else
				reactors.add(spawn.instantiate(physics));
		}

		reactors.update(physics);
	}

	void MapReactors::trigger(int32_t oid, int8_t state)
	{
		if (Optional<Reactor> reactor = reactors.get(oid)) {
			reactor->set_state(state);
		}
	}
	void MapReactors::spawn(ReactorSpawn&& spawn)
	{
		spawns.emplace(std::move(spawn));
	}

	void MapReactors::remove(int32_t oid, int8_t state, Point<int16_t> position)
	{
		if (Optional<Reactor> reactor = reactors.get(oid))
			reactor->destroy(state, position);
	}

	void MapReactors::clear()
	{
		reactors.clear();
	}

	MapReactors::reactor_obj MapReactors::check_reactor_near(Point<int16_t> playerpos, bool facing_right)
	{
		for (auto& mmo : reactors)
		{
			Optional<const Reactor> reactor = mmo.second.get();
			Point<int16_t> reactor_pos = reactor->get_position();
			bool acceptable_distance = acceptable_distance = std::abs(reactor->get_position().x() - playerpos.x()) < 60;
			bool facing_toward_reactor;
			if (facing_right) {
				facing_toward_reactor = reactor->get_position().x() > playerpos.x();
			}
			else {
				facing_toward_reactor = reactor->get_position().x() < playerpos.x();
			}
			if (reactor && reactor->get_position().y() == playerpos.y() && acceptable_distance)
			{
				int32_t oid = mmo.first;
				Point<int16_t> position = reactor->get_position();

				return { oid, position };
			}
		}

		return { 0, {} };
	}

	Reactor& MapReactors::get_reactor_by_oid(int32_t oid)
	{
		Optional<MapObject> reactor = reactors.get(oid);
		if (reactor) {
			return dynamic_cast<Reactor&>(*reactor);
		}
	}

	MapObjects* MapReactors::get_reactors()
	{
		return &reactors;
	}
}