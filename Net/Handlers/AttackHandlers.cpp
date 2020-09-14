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
#include "AttackHandlers.h"

#include "../../Character/SkillId.h"
#include "../../Gameplay/Stage.h"

namespace ms
{
	AttackHandler::AttackHandler(Attack::Type t)
	{
		type = t;
	}

	void AttackHandler::handle(InPacket& recv) const
	{
		AttackResult attack;

		int32_t cid = recv.read_int();
		uint8_t numAttackedAndDamage = recv.read_byte();
		attack.hitcount = numAttackedAndDamage & 0xF;
		attack.mobcount = (numAttackedAndDamage >> 4) & 0xF;

		recv.skip(1);

		attack.type = type;
		attack.attacker = cid;
		attack.level = recv.read_byte();
		attack.skill = (attack.level > 0) ? recv.read_int() : 0;
		attack.display = recv.read_byte();
		attack.toleft = recv.read_bool();
		attack.stance = recv.read_byte();
		attack.speed = recv.read_byte();

		recv.skip(1);

		attack.bullet = recv.read_int();

		for (uint8_t i = 0; i < attack.mobcount; i++)
		{
			int32_t oid = recv.read_int();

			recv.skip(14);

			uint8_t length = (attack.skill == SkillId::Id::MESO_EXPLOSION) ? recv.read_byte() : attack.hitcount;

			for (uint8_t j = 0; j < length; j++)
			{
				int32_t damage = recv.read_int();
				bool critical = false; // TODO: ?
				auto singledamage = std::make_pair(damage, critical);
				attack.damagelines[oid].push_back(singledamage);
			}
		}
		if (attack.type == Attack::Type::RANGED)
			recv.skip(4);
		if (attack.type == Attack::Type::MAGIC) {
			if (recv.available() > 0) {
				attack.charge = recv.read_int();
			}
		}

		Stage::get().get_combat().push_attack(attack);
	}
}