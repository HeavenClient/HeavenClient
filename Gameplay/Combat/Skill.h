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

#include "SkillAction.h"
#include "SkillBullet.h"
#include "SkillHitEffect.h"
#include "SkillSound.h"
#include "SkillUseEffect.h"
#include "SpecialMove.h"

#include <memory>

namespace ms
{
	// The skill implementation of special move
	class Skill : public SpecialMove
	{
	public:
		Skill(int32_t skillid);

		void apply_useeffects(Char& user) const override;

		void apply_actions(Char& user, Attack::Type type) const override;

		void apply_stats(const Char& user, Attack& attack) const override;

		void apply_hiteffects(const AttackUser& user, Mob& target) const override;

		Animation get_bullet(const Char& user, int32_t bulletid) const override;

		bool is_attack() const override;

		bool is_skill() const override;

		int32_t get_id() const override;

		ForbidReason can_use(int32_t level, Weapon::Type weapon, const Job& job, uint16_t hp, uint16_t mp,
							 uint16_t bullets) const override;

	private:
		std::unique_ptr<SkillAction> action;
		std::unique_ptr<SkillBullet> bullet;
		std::unique_ptr<SkillSound> sound;
		std::unique_ptr<SkillUseEffect> useeffect;
		std::unique_ptr<SkillHitEffect> hiteffect;

		int32_t skillid;
		bool overregular;
		bool projectile;
	};
}