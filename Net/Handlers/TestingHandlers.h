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

namespace ms
{
	// TODO: Comment
	class CheckSpwResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class LatestConnectedWorldHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class AdminResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class QuickslotKeyMappedManHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class sendAutoHpPotHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class sendAutoMpPotHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class updateInventorySlotLimitHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class MapTransferResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class SetTamingMobInfoHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// TODO: Comment
	class FieldEffectHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class SetShowQuestCompletionHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class skillBookResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class SueCharacterResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class TradeMoneyLimitHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class TownPortalHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class KeymapHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class NotifyLevelUpHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class FriendResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class ScriptProgressMessageHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	class ClockHandler : public PacketHandler
	{
	public:
		void handle(InPacket& recv) const override;
	private:
		void setClock(InPacket& recv) const;
		void setClockTime(InPacket& recv) const;
		void setEventTime(InPacket& recv) const;
	};
}