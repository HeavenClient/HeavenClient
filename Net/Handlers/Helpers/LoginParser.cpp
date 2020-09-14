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
#include "LoginParser.h"

#include "../../Session.h"

namespace ms
{
	Account LoginParser::parse_account(InPacket& recv)
	{
		Account account;

		recv.skip_short();

		account.accid = recv.read_int();
		account.female = recv.read_byte();
		account.admin = recv.read_bool();

		recv.skip_byte(); // Admin
		recv.skip_byte(); // Country Code

		account.name = recv.read_string();

		recv.skip_byte();

		account.muted = recv.read_bool();

		recv.skip_long(); // muted until
		recv.skip_long(); // creation date

		recv.skip_int(); // Remove "Select the world you want to play in"

		account.pin = recv.read_bool(); // 0 - Enabled, 1 - Disabled
		account.pic = recv.read_byte(); // 0 - Register, 1 - Ask, 2 - Disabled

		return account;
	}

	World LoginParser::parse_world(InPacket& recv)
	{
		int8_t worldid = recv.read_byte();

		if (worldid == -1)
			return { {}, {}, {}, 0, 0, worldid };

		std::string name = recv.read_string();
		uint8_t worldstate = recv.read_byte();
		std::string worldeventdesc = recv.read_string();

		recv.read_short(); // WorldEventEXP_WSE
		recv.read_short(); // WorldEventDrop_WSE
		recv.read_byte(); // BlockCharCreation

		std::vector<uint32_t> chloads;
		uint8_t channelcount = recv.read_byte();

		for (uint8_t i = 0; i < channelcount; ++i)
		{
			std::string channelname = recv.read_string(); // channel name
			chloads.push_back(recv.read_int()); // UserNo

			uint8_t worldid = recv.read_byte(); // WorldID
			uint8_t channelid = recv.read_byte(); // ChannelID
			recv.read_bool(); // AdultChannel
		}

		recv.read_short(); // BalloonCount

		return { name, worldeventdesc, chloads, channelcount, worldstate, worldid };
	}

	RecommendedWorld LoginParser::parse_recommended_world(InPacket& recv)
	{
		int32_t wid = recv.read_int();

		if (wid == -1)
			return { {}, wid };

		std::string message = recv.read_string();

		return { message, wid };
	}

	CharEntry LoginParser::parse_charentry(InPacket& recv)
	{
		int32_t dwCharacterID = recv.read_int();
		StatsEntry stats = parse_stats(recv);
		LookEntry look = parse_look(recv);

		recv.read_bool(); // 'rankinfo' bool

		if (recv.read_bool())
		{
			int32_t currank = recv.read_int();
			int32_t rankmv = recv.read_int();
			int32_t curjobrank = recv.read_int();
			int32_t jobrankmv = recv.read_int();
			int8_t rankmc = (rankmv > 0) ? '+' : (rankmv < 0) ? '-' : '=';
			int8_t jobrankmc = (jobrankmv > 0) ? '+' : (jobrankmv < 0) ? '-' : '=';

			stats.rank = std::make_pair(currank, rankmc);
			stats.jobrank = std::make_pair(curjobrank, jobrankmc);
		}

		return { stats, look, dwCharacterID };
	}

	StatsEntry LoginParser::parse_stats(InPacket& recv)
	{
		// TODO: This is similar to CashShopParser.cpp, try and merge these.
		StatsEntry statsEntry;

		statsEntry.name = recv.read_padded_string(13);
		statsEntry.female = recv.read_bool(); // 0 = male, 1 = female
		statsEntry.stats[MapleStat::Id::SKIN] = recv.read_byte();	// skin color
		statsEntry.stats[MapleStat::Id::FACE] = recv.read_int();	// face
		statsEntry.stats[MapleStat::Id::HAIR] = recv.read_int();	// hair
		for (size_t i = 0; i < 3; i++) {
			statsEntry.petids.push_back(recv.read_long());
		}
		statsEntry.stats[MapleStat::Id::LEVEL] = recv.read_short();
		statsEntry.stats[MapleStat::Id::JOB] = recv.read_short();
		statsEntry.stats[MapleStat::Id::STR] = recv.read_short();
		statsEntry.stats[MapleStat::Id::DEX] = recv.read_short();
		statsEntry.stats[MapleStat::Id::INT] = recv.read_short();
		statsEntry.stats[MapleStat::Id::LUK] = recv.read_short();
		statsEntry.hp = recv.read_int();
		statsEntry.maxhp = recv.read_int();
		statsEntry.mp = recv.read_int();
		statsEntry.maxmp = recv.read_int();
		statsEntry.stats[MapleStat::Id::AP] = recv.read_short();
		statsEntry.stats[MapleStat::Id::SP] = recv.read_short();
		statsEntry.exp = recv.read_int();
		statsEntry.stats[MapleStat::Id::FAME] = recv.read_short();
		statsEntry.termexp = recv.read_int(); // TermEXP
		statsEntry.mapid = recv.read_int(); // dwPosMap
		statsEntry.portal = recv.read_byte(); // Portal
		statsEntry.playTime = recv.read_int(); // Playtime

		return statsEntry;
	}

	LookEntry LoginParser::parse_look(InPacket& recv)
	{
		LookEntry look;

		look.female = recv.read_bool();
		look.skin = recv.read_byte();
		look.faceid = recv.read_int();

		recv.read_bool(); // megaphone

		look.hairid = recv.read_int();

		uint8_t eqslot = recv.read_byte();

		while (eqslot != 0xFF)
		{
			look.equips[eqslot] = recv.read_int();
			eqslot = recv.read_byte();
		}

		uint8_t mskeqslot = recv.read_byte();

		while (mskeqslot != 0xFF)
		{
			look.maskedequips[mskeqslot] = recv.read_int();
			mskeqslot = recv.read_byte();
		}

		look.maskedequips[-111] = recv.read_int();

		for (uint8_t i = 0; i < 3; i++)
			look.petids.push_back(recv.read_int());

		return look;
	}

	void LoginParser::parse_login(InPacket& recv)
	{
		recv.skip_byte();

		// Read the IPv4 address in a string
		std::string addrstr;

		for (size_t i = 0; i < 4; i++)
		{
			uint8_t num = static_cast<uint8_t>(recv.read_byte());
			addrstr.append(std::to_string(num));

			if (i < 3)
				addrstr.push_back('.');
		}

		// Read the port address in a string
		std::string portstr = std::to_string(recv.read_short());

		// Attempt to reconnect to the server
		Session::get().reconnect(addrstr.c_str(), portstr.c_str());
	}
}