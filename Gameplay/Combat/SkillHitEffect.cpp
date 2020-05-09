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
#include "SkillHitEffect.h"

#include "../../Util/Misc.h"

namespace ms
{
	SingleHitEffect::SingleHitEffect(nl::node src) : effect(src["hit"]["0"])
	{}

	void SingleHitEffect::apply(const AttackUser &user, Mob &target) const
	{
		effect.apply(target, user.flip);
	}

	TwoHandedHitEffect::TwoHandedHitEffect(nl::node src) : effects(src["hit"]["0"], src["hit"]["1"]) {}

	void TwoHandedHitEffect::apply(const AttackUser& user, Mob& target) const
	{
		effects[user.secondweapon].apply(target, user.flip);
	}

	ByLevelHitEffect::ByLevelHitEffect(nl::node src)
	{
		for (auto sub : src["CharLevel"])
		{
			uint16_t level = string_conversion::or_zero<uint16_t>(sub.name());
			effects.emplace(level, sub["hit"]["0"]);
		}
	}

	void ByLevelHitEffect::apply(const AttackUser &user, Mob &target) const
	{
		if (effects.empty())
			return;

		auto iter = effects.begin();
		for (; iter != effects.end() && user.level > iter->first; ++iter)
		{}

		if (iter != effects.begin())
			iter--;

		iter->second.apply(target, user.flip);
	}

	ByLevelTwoHandedHitEffect::ByLevelTwoHandedHitEffect(nl::node src)
	{
		for (auto sub : src["CharLevel"])
		{
			auto level = string_conversion::or_zero<uint16_t>(sub.name());

			effects.emplace(std::piecewise_construct,
							std::forward_as_tuple(level),
							std::forward_as_tuple(sub["hit"]["0"], sub["hit"]["1"])
			);
		}
	}

	void ByLevelTwoHandedHitEffect::apply(const AttackUser &user, Mob &target) const
	{
		if (effects.empty())
			return;

		auto iter = effects.begin();
		for (; iter != effects.end() && user.level > iter->first; ++iter)
		{}

		if (iter != effects.begin())
			iter--;

		iter->second[user.secondweapon].apply(target, user.flip);
	}

	BySkillLevelHitEffect::BySkillLevelHitEffect(nl::node src)
	{
		for (auto sub : src["level"])
		{
			auto level = string_conversion::or_zero<int32_t>(sub.name());
			effects.emplace(level, sub["hit"]["0"]);
		}
	}

	void BySkillLevelHitEffect::apply(const AttackUser &user, Mob &target) const
	{
		auto iter = effects.find(user.skilllevel);

		if (iter != effects.end())
			iter->second.apply(target, user.flip);
	}
}