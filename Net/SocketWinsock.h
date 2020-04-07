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

#include "../MapleStory.h"

#ifndef USE_ASIO
#include "NetConstants.h"

namespace ms
{
#ifdef USE_CRYPTO
	const size_t HANDSHAKE_LEN = 16;
#else
	const size_t HANDSHAKE_LEN = 2;
#endif

	class SocketWinsock
	{
	public:
		bool open(const char* address, const char* port);
		bool close();

		bool dispatch(const int8_t* bytes, size_t length) const;
		size_t receive(bool* connected);
		const int8_t* get_buffer() const;

	private:
		uint64_t sock;
		int8_t buffer[MAX_PACKET_LENGTH];
	};
}
#endif