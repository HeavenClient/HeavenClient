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

#include "MovementPacket.h"

namespace ms
{
	// Requests the server to warp the player to a different map.
	// Opcode: CHANGE_MAP(38)
	class ChangeMapPacket : public OutPacket
	{
	public:
		ChangeMapPacket(bool died, int32_t targetid, const std::string& targetp, bool usewheel) : OutPacket(OutPacket::Opcode::CHANGEMAP)
		{
			write_byte(died);
			write_int(targetid);
			write_string(targetp);
			skip(1);
			write_short(usewheel ? 1 : 0);
		}
	};

	// Requests the server to set map transition complete
	// Opcode: PLAYER_MAP_TRANSFER(207)
	class PlayerMapTransferPacket : public OutPacket
	{
	public:
		PlayerMapTransferPacket() : OutPacket(OutPacket::Opcode::PLAYER_MAP_TRANSFER) {}
	};

	// Updates the player's position with the server.
	// Opcode: MOVE_PLAYER(41)
	class MovePlayerPacket : public MovementPacket
	{
	public:
		MovePlayerPacket(const Movement& movement) : MovementPacket(OutPacket::Opcode::MOVE_PLAYER)
		{
			skip(9);
			write_byte(1);
			writemovement(movement);
		}
	};

	// Requests various party-related things.
	// Opcode: PARTY_OPERATION(124)
	class PartyOperationPacket : public OutPacket
	{
	public:
		enum Operation : int8_t
		{
			CREATE = 1,
			LEAVE = 2,
			JOIN = 3,
			INVITE = 4,
			EXPEL = 5,
			PASS_LEADER = 6
		};

	protected:
		PartyOperationPacket(Operation op) : OutPacket(OutPacket::Opcode::PARTY_OPERATION)
		{
			write_byte(op);
		}
	};

	// Creates a new party.
	// Operation: CREATE(1)
	class CreatePartyPacket : public PartyOperationPacket
	{
	public:
		CreatePartyPacket() : PartyOperationPacket(PartyOperationPacket::Operation::CREATE) {}
	};

	// Leaves a party
	// Operation: LEAVE(2)
	class LeavePartyPacket : public PartyOperationPacket
	{
	public:
		LeavePartyPacket() : PartyOperationPacket(PartyOperationPacket::Operation::LEAVE) {}
	};

	// Joins a party.
	// Operation: JOIN(3)
	class JoinPartyPacket : public PartyOperationPacket
	{
	public:
		JoinPartyPacket(int32_t party_id) : PartyOperationPacket(PartyOperationPacket::Operation::JOIN)
		{
			write_int(party_id);
		}
	};

	// Invites a player to a party.
	// Operation: INVITE(4)
	class InviteToPartyPacket : public PartyOperationPacket
	{
	public:
		InviteToPartyPacket(const std::string& name) : PartyOperationPacket(PartyOperationPacket::Operation::INVITE)
		{
			write_string(name);
		}
	};

	// Expels someone from a party.
	// Operation: EXPEL(5)
	class ExpelFromPartyPacket : public PartyOperationPacket
	{
	public:
		ExpelFromPartyPacket(int32_t cid) : PartyOperationPacket(PartyOperationPacket::Operation::EXPEL)
		{
			write_int(cid);
		}
	};

	// Passes party leadership to another character.
	// Operation: PASS_LEADER(6)
	class ChangePartyLeaderPacket : public PartyOperationPacket
	{
	public:
		ChangePartyLeaderPacket(int32_t cid) : PartyOperationPacket(PartyOperationPacket::Operation::PASS_LEADER)
		{
			write_int(cid);
		}
	};

	// Updates a mob's position with the server.
	// Opcode: MOVE_MONSTER(188)
	class MoveMobPacket : public MovementPacket
	{
	public:
		MoveMobPacket(int32_t oid, int16_t type, int8_t skillb, int8_t skill0, int8_t skill1, int8_t skill2, int8_t skill3, int8_t skill4, Point<int16_t> startpos, const Movement& movement) : MovementPacket(OutPacket::Opcode::MOVE_MONSTER)
		{
			write_int(oid);
			write_short(type);
			write_byte(skillb);
			write_byte(skill0);
			write_byte(skill1);
			write_byte(skill2);
			write_byte(skill3);
			write_byte(skill4);

			skip(13);

			write_point(startpos);

			write_byte(1);
			writemovement(movement);
		}
	};

	// Requests picking up an item.
	// Opcode: PICKUP_ITEM(202)
	class PickupItemPacket : public OutPacket
	{
	public:
		PickupItemPacket(int32_t oid, Point<int16_t> position) : OutPacket(OutPacket::Opcode::PICKUP_ITEM)
		{
			write_int(0);
			write_byte(0);
			write_point(position);
			write_int(oid);
		}
	};

	// Requests damaging a reactor.
	// Opcode: DAMAGE_REACTOR(205)
	class DamageReactorPacket : public OutPacket
	{
	public:
		DamageReactorPacket(int32_t oid, Point<int16_t> position, int16_t stance, int skillid) : OutPacket(OutPacket::Opcode::DAMAGE_REACTOR)
		{
			write_int(oid);
			write_point(position);
			write_short(stance);
			skip(4);
			write_int(skillid);
		};
	};
}