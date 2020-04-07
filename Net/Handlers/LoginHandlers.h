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
	// Handler for a packet that contains the response to an attempt at logging in
	class LoginResultHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet that contains the status of the requested world
	class ServerStatusHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles the packet that contains information on worlds and channels
	class ServerlistHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet that contains information on all chars on this world
	class CharlistHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles the packet which contains the IP of a channel server to connect to
	class ServerIPHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet which responds to the request for a character name
	class CharnameResponseHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for the packet that notifies that a char was successfully created
	class AddNewCharEntryHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handler for a packet that responds to the request to the delete a character
	class DeleteCharResponseHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};

	// Handles the packet that contains information on recommended worlds
	class RecommendedWorldsHandler : public PacketHandler
	{
		void handle(InPacket& recv) const override;
	};
}