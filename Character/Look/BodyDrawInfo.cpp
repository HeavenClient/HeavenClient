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
#include "BodyDrawInfo.h"

#include "Body.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	void BodyDrawInfo::init()
	{
		nl::node bodynode = nl::nx::character["00002000.img"];
		nl::node headnode = nl::nx::character["00012000.img"];

		for (nl::node stancenode : bodynode)
		{
			std::string ststr = stancenode.name();

			uint16_t attackdelay = 0;

			for (uint8_t frame = 0; nl::node framenode = stancenode[frame]; ++frame)
			{
				bool isaction = framenode["action"].data_type() == nl::node::type::string;

				if (isaction)
				{
					BodyAction action = framenode;
					body_actions[ststr][frame] = action;

					if (action.isattackframe())
						attack_delays[ststr].push_back(attackdelay);

					attackdelay += action.get_delay();
				}
				else
				{
					Stance::Id stance = Stance::by_string(ststr);
					int16_t delay = framenode["delay"];

					if (delay <= 0)
						delay = 100;

					stance_delays[stance][frame] = delay;

					std::unordered_map<Body::Layer, std::unordered_map<std::string, Point<int16_t>>> bodyshiftmap;

					for (auto partnode : framenode)
					{
						std::string part = partnode.name();

						if (part != "delay" && part != "face")
						{
							std::string zstr = partnode["z"];
							Body::Layer z = Body::layer_by_name(zstr);

							for (auto mapnode : partnode["map"])
								bodyshiftmap[z].emplace(mapnode.name(), mapnode);
						}
					}

					nl::node headmap = headnode[ststr][frame]["head"]["map"];

					for (auto mapnode : headmap)
						bodyshiftmap[Body::Layer::HEAD].emplace(mapnode.name(), mapnode);

					body_positions[stance][frame] = bodyshiftmap[Body::Layer::BODY]["navel"];

					arm_positions[stance][frame] = bodyshiftmap.count(Body::Layer::ARM) ?
												   (bodyshiftmap[Body::Layer::ARM]["hand"] -
													bodyshiftmap[Body::Layer::ARM]["navel"] +
													bodyshiftmap[Body::Layer::BODY]["navel"]) :
												   (bodyshiftmap[Body::Layer::ARM_OVER_HAIR]["hand"] -
													bodyshiftmap[Body::Layer::ARM_OVER_HAIR]["navel"] +
													bodyshiftmap[Body::Layer::BODY]["navel"]);

					hand_positions[stance][frame] = bodyshiftmap[Body::Layer::HAND_BELOW_WEAPON]["handMove"];
					head_positions[stance][frame] =
						bodyshiftmap[Body::Layer::BODY]["neck"] - bodyshiftmap[Body::Layer::HEAD]["neck"];
					face_positions[stance][frame] =
						bodyshiftmap[Body::Layer::BODY]["neck"] - bodyshiftmap[Body::Layer::HEAD]["neck"] +
						bodyshiftmap[Body::Layer::HEAD]["brow"];
					hair_positions[stance][frame] =
						bodyshiftmap[Body::Layer::HEAD]["brow"] - bodyshiftmap[Body::Layer::HEAD]["neck"] +
						bodyshiftmap[Body::Layer::BODY]["neck"];
				}
			}
		}
	}

	Point<int16_t> BodyDrawInfo::get_body_position(Stance::Id stance, uint8_t frame) const
	{
		auto iter = body_positions[stance].find(frame);

		if (iter == body_positions[stance].end())
			return {};

		return iter->second;
	}

	Point<int16_t> BodyDrawInfo::get_arm_position(Stance::Id stance, uint8_t frame) const
	{
		auto iter = arm_positions[stance].find(frame);

		if (iter == arm_positions[stance].end())
			return {};

		return iter->second;
	}

	Point<int16_t> BodyDrawInfo::get_hand_position(Stance::Id stance, uint8_t frame) const
	{
		auto iter = hand_positions[stance].find(frame);

		if (iter == hand_positions[stance].end())
			return {};

		return iter->second;
	}

	Point<int16_t> BodyDrawInfo::get_head_position(Stance::Id stance, uint8_t frame) const
	{
		auto iter = head_positions[stance].find(frame);

		if (iter == head_positions[stance].end())
			return {};

		return iter->second;
	}

	Point<int16_t> BodyDrawInfo::gethairpos(Stance::Id stance, uint8_t frame) const
	{
		auto iter = hair_positions[stance].find(frame);

		if (iter == hair_positions[stance].end())
			return {};

		return iter->second;
	}

	Point<int16_t> BodyDrawInfo::getfacepos(Stance::Id stance, uint8_t frame) const
	{
		auto iter = face_positions[stance].find(frame);

		if (iter == face_positions[stance].end())
			return {};

		return iter->second;
	}

	uint8_t BodyDrawInfo::nextframe(Stance::Id stance, uint8_t frame) const
	{
		if (stance_delays[stance].count(frame + 1))
			return frame + 1;
		else
			return 0;
	}

	uint16_t BodyDrawInfo::get_delay(Stance::Id stance, uint8_t frame) const
	{
		auto iter = stance_delays[stance].find(frame);

		if (iter == stance_delays[stance].end())
			return 100;

		return iter->second;
	}

	uint16_t BodyDrawInfo::get_attackdelay(std::string action, size_t no) const
	{
		auto action_iter = attack_delays.find(action);

		if (action_iter != attack_delays.end())
			if (no < action_iter->second.size())
				return action_iter->second[no];

		return 0;
	}

	uint8_t BodyDrawInfo::next_actionframe(std::string action, uint8_t frame) const
	{
		auto action_iter = body_actions.find(action);

		if (action_iter != body_actions.end())
			if (action_iter->second.count(frame + 1))
				return frame + 1;

		return 0;
	}

	const BodyAction* BodyDrawInfo::get_action(std::string action, uint8_t frame) const
	{
		auto action_iter = body_actions.find(action);

		if (action_iter != body_actions.end())
		{
			auto frame_iter = action_iter->second.find(frame);

			if (frame_iter != action_iter->second.end())
				return &(frame_iter->second);
		}

		return nullptr;
	}
}
