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

#ifdef USE_ASIO

#include "NetConstants.h"

#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB

#include "asio.hpp"

namespace ms
{
#ifndef USE_CRYPTO
	const size_t HANDSHAKE_LEN = 2;
#else
	const size_t HANDSHAKE_LEN = 16;
#endif

	using asio::io_service;
	using asio::ip::tcp;
	using asio::error_code;

	// Class that wraps an asio socket.
	class SocketAsio
	{
	public:
		SocketAsio();

		~SocketAsio();

		bool open(const char* address, const char* port);

		bool close();

		size_t receive(bool* connected);

		const int8_t* get_buffer() const;

		bool dispatch(const int8_t* bytes, size_t length);

	private:
		io_service ioservice;
		tcp::resolver resolver;
		tcp::socket socket;
		int8_t buffer[MAX_PACKET_LENGTH];
	};
}
#endif