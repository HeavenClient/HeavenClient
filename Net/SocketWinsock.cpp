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
#include "SocketWinsock.h"

#ifndef USE_ASIO
#include <ws2tcpip.h>

// TODO: Can this be moved?
#pragma comment (lib, "Ws2_32.lib")

namespace ms
{
	bool SocketWinsock::open(const char* iaddr, const char* port)
	{
		WSADATA wsa_info;
		sock = INVALID_SOCKET;

		struct addrinfo* addr_info = NULL;
		struct addrinfo* ptr = NULL;
		struct addrinfo hints;

		int result = WSAStartup(MAKEWORD(2, 2), &wsa_info);

		if (result != 0)
			return false;

		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		result = getaddrinfo(iaddr, port, &hints, &addr_info);

		if (result != 0)
		{
			WSACleanup();

			return false;
		}

		for (ptr = addr_info; ptr != NULL; ptr = ptr->ai_next)
		{
			sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (sock == INVALID_SOCKET)
			{
				WSACleanup();

				return false;
			}

			result = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);

			if (result == SOCKET_ERROR)
			{
				closesocket(sock);

				sock = INVALID_SOCKET;

				continue;
			}

			break;
		}

		freeaddrinfo(addr_info);

		if (sock == INVALID_SOCKET)
		{
			WSACleanup();

			return false;
		}

		result = recv(sock, (char*)buffer, 32, 0);

		if (result == HANDSHAKE_LEN)
		{
			return true;
		}
		else
		{
			WSACleanup();

			return false;
		}
	}

	bool SocketWinsock::close()
	{
		int error = closesocket(sock);

		WSACleanup();

		return error != SOCKET_ERROR;
	}

	bool SocketWinsock::dispatch(const int8_t* bytes, size_t length) const
	{
		return send(sock, (char*)bytes, static_cast<int>(length), 0) != SOCKET_ERROR;
	}

	size_t SocketWinsock::receive(bool* success)
	{
		timeval timeout = { 0, 0 };
		fd_set sockset = { 0 };

		FD_SET(sock, &sockset);

		int result = select(0, &sockset, 0, 0, &timeout);

		if (result > 0)
			result = recv(sock, (char*)buffer, MAX_PACKET_LENGTH, 0);

		if (result == SOCKET_ERROR)
		{
			*success = false;

			return 0;
		}
		else
		{
			return result;
		}
	}

	const int8_t* SocketWinsock::get_buffer() const
	{
		return buffer;
	}
}
#endif