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

#include "../OutPacket.h"

namespace ms
{
	// Packet which requests a dialog with a server-sided NPC
	// Opcode: TALK_TO_NPC(58)
	class TalkToNPCPacket : public OutPacket
	{
	public:
		TalkToNPCPacket(int32_t oid) : OutPacket(OutPacket::Opcode::TALK_TO_NPC)
		{
			write_int(oid);
		}
	};

	// Packet which sends a response to an NPC dialog to the server
	// Opcode: NPC_TALK_MORE(60)
	class NpcTalkMorePacket : public OutPacket
	{
	public:
		NpcTalkMorePacket(int8_t lastmsg, int8_t response) : OutPacket(OutPacket::Opcode::NPC_TALK_MORE)
		{
			write_byte(lastmsg);
			write_byte(response);
		}

		NpcTalkMorePacket(const std::string& response) : NpcTalkMorePacket(2, 1)
		{
			write_string(response);
		}

		NpcTalkMorePacket(int32_t selection) : NpcTalkMorePacket(4, 1)
		{
			write_int(selection);
		}
	};

	// Packet which tells the server of an interaction with an NPC shop
	// Opcode: NPC_SHOP_ACTION(61)
	class NpcShopActionPacket : public OutPacket
	{
	public:
		// Requests that an item should be bought from or sold to a NPC shop
		NpcShopActionPacket(int16_t slot, int32_t itemid, int16_t qty, bool buy) : NpcShopActionPacket(buy ? Mode::BUY : Mode::SELL)
		{
			write_short(slot);
			write_int(itemid);
			write_short(qty);
		}

		// Requests that an item should be recharged at a NPC shop
		NpcShopActionPacket(int16_t slot) : NpcShopActionPacket(Mode::RECHARGE)
		{
			write_short(slot);
		}

		// Requests exiting from a NPC shop
		NpcShopActionPacket() : NpcShopActionPacket(Mode::LEAVE) {}

	protected:
		enum Mode : int8_t
		{
			BUY, SELL, RECHARGE, LEAVE
		};

		NpcShopActionPacket(Mode mode) : OutPacket(OutPacket::Opcode::NPC_SHOP_ACTION)
		{
			write_byte(mode);
		}
	};
}