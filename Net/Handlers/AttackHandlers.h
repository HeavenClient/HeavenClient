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

#include "../PacketHandler.h"

#include "../../Gameplay/Combat/Attack.h"

namespace ms
{
	class AttackHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;

	protected:
		AttackHandler(Attack::Type type);

	private:
		Attack::Type type;
	};

	class CloseAttackHandler : public AttackHandler
	{
	public:
		CloseAttackHandler() : AttackHandler(Attack::Type::CLOSE)
		{}
	};

	class RangedAttackHandler : public AttackHandler
	{
	public:
		RangedAttackHandler() : AttackHandler(Attack::Type::RANGED)
		{}
	};

	class MagicAttackHandler : public AttackHandler
	{
	public:
		MagicAttackHandler() : AttackHandler(Attack::Type::MAGIC)
		{}
	};
}