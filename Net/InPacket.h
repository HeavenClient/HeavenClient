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

#include "PacketError.h"

#include "../Template/Point.h"

#include <cstdint>

namespace ms
{
	// A packet received from the server
	// Contains reading functions
	class InPacket
	{
	public:
		// Construct a packet from an array of bytes
		InPacket(const int8_t* bytes, size_t length);

		// Check if there are more bytes available
		bool available() const;
		// Return the remaining length in bytes
		size_t length() const;
		// Skip a number of bytes (by increasing the offset)
		void skip(size_t count);

		// Read a byte and check if it is equal to one
		bool read_bool();
		// Read a byte
		int8_t read_byte();
		// Read a short
		int16_t read_short();
		// Read a int
		int32_t read_int();
		// Read a long
		int64_t read_long();

		// Read a point
		Point<int16_t> read_point();

		// Read a string
		std::string read_string();
		// Read a fixed-length string
		std::string read_padded_string(uint16_t length);

		// Skip a byte
		void skip_bool();
		// Skip a byte
		void skip_byte();
		// Skip a short
		void skip_short();
		// Skip a int
		void skip_int();
		// Skip a long
		void skip_long();

		// Skip a point
		void skip_point();

		// Skip a string
		void skip_string();
		// Skip a fixed-length string
		void skip_padded_string(uint16_t length);

		// Inspect a byte and check if it is 1. Does not advance the buffer position.
		bool inspect_bool();
		// Inspect a byte. Does not advance the buffer position.
		int8_t inspect_byte();
		// Inspect a short. Does not advance the buffer position.
		int16_t inspect_short();
		// Inspect an int. Does not advance the buffer position.
		int32_t inspect_int();
		// Inspect a long. Does not advance the buffer position.
		int64_t inspect_long();

	private:
		template <typename T>
		// Read a number and advance the buffer position
		T read()
		{
			size_t count = sizeof(T) / sizeof(int8_t);
			T all = 0;

			for (size_t i = 0; i < count; i++)
			{
				T val = static_cast<uint8_t>(bytes[pos]);
				all += val << (8 * i);

				skip(1);
			}

			return static_cast<T>(all);
		}

		template <typename T>
		// Read without advancing the buffer position
		T inspect()
		{
			size_t before = pos;
			T value = read<T>();
			pos = before;

			return value;
		}

		const int8_t* bytes;
		size_t top;
		size_t pos;
	};
}