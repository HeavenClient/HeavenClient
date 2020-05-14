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
#pragma once

#include "../MapleMap/Mob.h"
#include "../../Template/BoolPair.h"

namespace ms
{
	// Interface for hit effects, animations applied to a mob for each hit.
	class SkillHitEffect
	{
	public:
		virtual ~SkillHitEffect()
		{}

		virtual void apply(const AttackUser& user, Mob& target) const = 0;

	protected:
		class Effect
		{
		public:
			Effect(nl::node src)
			{
				animation = src;
				pos = src["pos"];
				z = src["z"];
			}

			void apply(Mob& target, bool flip) const
			{
				target.show_effect(animation, pos, z, flip);
			}

		private:
			Animation animation;
			int8_t pos;
			int8_t z;
		};
	};

	// No animation
	class NoHitEffect : public SkillHitEffect
	{
	public:
		void apply(const AttackUser&, Mob&) const override
		{}
	};

	// A single animation
	class SingleHitEffect : public SkillHitEffect
	{
	public:
		SingleHitEffect(nl::node src);

		void apply(const AttackUser& user, Mob& target) const override;

	private:
		Effect effect;
	};

	// The animation changes depending on the weapon used
	class TwoHandedHitEffect : public SkillHitEffect
	{
	public:
		TwoHandedHitEffect(nl::node src);

		void apply(const AttackUser& user, Mob& target) const override;

	private:
		BoolPair<Effect> effects;
	};

	// The animation changes with the character level
	class ByLevelHitEffect : public SkillHitEffect
	{
	public:
		ByLevelHitEffect(nl::node src);

		void apply(const AttackUser& user, Mob& target) const override;

	private:
		std::map<uint16_t, Effect> effects;
	};

	// The animation changes with the character level and weapon used
	class ByLevelTwoHandedHitEffect : public SkillHitEffect
	{
	public:
		ByLevelTwoHandedHitEffect(nl::node src);

		void apply(const AttackUser& user, Mob& target) const override;

	private:
		std::map<uint16_t, BoolPair<Effect>> effects;
	};

	// The animation changes with the skill level
	class BySkillLevelHitEffect : public SkillHitEffect
	{
	public:
		BySkillLevelHitEffect(nl::node src);

		void apply(const AttackUser& user, Mob& target) const override;

	private:
		std::map<int32_t, Effect> effects;
	};
}