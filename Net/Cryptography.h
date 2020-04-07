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

#include "NetConstants.h"

#include "../MapleStory.h"

namespace ms
{
	// Used to encrypt and decrypt packets for communication with the server
	class Cryptography
	{
	public:
		// Obtain the initialization vector from the handshake
		Cryptography(const int8_t* handshake);
		Cryptography();
		~Cryptography();

		// Encrypt a byte array with the given length and iv
		void encrypt(int8_t* bytes, size_t length);
		// Decrypt a byte array with the given length and iv
		void decrypt(int8_t* bytes, size_t length);
		// Generate a header for the specified length and key
		void create_header(int8_t* buffer, size_t length) const;
		// Use the 4-byte header of a received packet to determine its length
		size_t check_length(const int8_t* header) const;

	private:
		// Add the maple custom encryption
		void mapleencrypt(int8_t* bytes, size_t length) const;
		// Remove the maple custom encryption
		void mapledecrypt(int8_t* bytes, size_t length) const;
		// Update a key
		void updateiv(uint8_t* iv) const;
		// Perform a roll-left operation
		int8_t rollleft(int8_t byte, size_t count) const;
		// Perform a roll-right operation
		int8_t rollright(int8_t byte, size_t count) const;

		// Apply AES OFB to a byte array
		void aesofb(int8_t* bytes, size_t length, uint8_t* iv) const;
		// Encrypt a byte array with AES
		void aesencrypt(uint8_t* bytes) const;
		// AES add round key step
		void addroundkey(uint8_t* bytes, uint8_t round) const;
		// AES sub bytes step
		void subbytes(uint8_t* bytes) const;
		// AES shift rows step
		void shiftrows(uint8_t* bytes) const;
		// AES mix columns step
		void mixcolumns(uint8_t* bytes) const;
		// Perform a Galois multiplication
		uint8_t gmul(uint8_t byte) const;

#ifdef USE_CRYPTO
		uint8_t sendiv[HEADER_LENGTH];
		uint8_t recviv[HEADER_LENGTH];
#endif
	};
}