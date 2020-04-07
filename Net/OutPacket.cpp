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
#include "OutPacket.h"

#include "Session.h"

#include "../Configuration.h"

#include "../Util/Randomizer.h"

#include <chrono>

namespace ms
{
	OutPacket::OutPacket(int16_t opc) : opcode(opc)
	{
		write_short(opcode);
	}

	void OutPacket::dispatch()
	{
		Session::get().write(bytes.data(), bytes.size());

		if (Configuration::get().get_show_packets())
		{
			if (opcode == Opcode::PONG)
				std::cout << "Sent Packet: PONG" << std::endl;
			else
				std::cout << "Sent Packet: " << std::to_string(opcode) << std::endl;
		}
	}

	void OutPacket::skip(size_t count)
	{
		for (size_t i = 0; i < count; i++)
			bytes.push_back(0);
	}

	void OutPacket::write_byte(int8_t ch)
	{
		bytes.push_back(ch);
	}

	void OutPacket::write_short(int16_t sh)
	{
		for (size_t i = 0; i < sizeof(short); i++)
		{
			write_byte(static_cast<int8_t>(sh));
			sh >>= 8;
		}
	}

	void OutPacket::write_int(int32_t in)
	{
		for (size_t i = 0; i < sizeof(int); i++)
		{
			write_byte(static_cast<int8_t>(in));
			in >>= 8;
		}
	}

	void OutPacket::write_long(int64_t lg)
	{
		for (size_t i = 0; i < sizeof(long); i++)
		{
			write_byte(static_cast<int8_t>(lg));
			lg >>= 8;
		}
	}

	void OutPacket::write_time()
	{
		auto duration = std::chrono::steady_clock::now().time_since_epoch();
		auto since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		int32_t timestamp = static_cast<int32_t>(since_epoch.count());

		write_int(timestamp);
	}

	void OutPacket::write_point(Point<int16_t> position)
	{
		write_short(position.x());
		write_short(position.y());
	}

	void OutPacket::write_string(const std::string& str)
	{
		int16_t length = static_cast<int16_t>(str.length());

		write_short(length);

		for (int16_t i = 0; i < length; i++)
			write_byte(str[i]);
	}

	void OutPacket::write_random()
	{
		auto randomizer = Randomizer();
		int32_t rand = randomizer.next_int(INT_MAX);

		write_int(rand);
	}

	void OutPacket::write_hardware_info()
	{
		std::string macs = Configuration::get().get_macs().c_str();
		std::string hwid = Configuration::get().get_hwid().c_str();

		write_string(macs);
		write_string(hwid);
	}

	int32_t OutPacket::hex_to_dec(std::string hexVal)
	{
		int32_t len = strlen(hexVal.c_str());
		int32_t base = 1;
		int32_t dec_val = 0;

		for (int32_t i = len - 1; i >= 0; i--)
		{
			if (hexVal[i] >= '0' && hexVal[i] <= '9')
			{
				dec_val += (hexVal[i] - 48) * base;
				base = base * 16;
			}
			else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
			{
				dec_val += (hexVal[i] - 55) * base;
				base = base * 16;
			}
		}

		return dec_val;
	}
}