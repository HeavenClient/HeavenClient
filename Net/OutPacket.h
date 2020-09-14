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

#include "../Template/Point.h"

#include <vector>

namespace ms
{
	// A packet to be sent to the server
	// Used as a base class to create specific packets
	class OutPacket
	{
	public:
		// Construct a packet by writing its opcode
		OutPacket(int16_t opcode);

		void dispatch();

		// Opcodes for OutPackets associated with version 83 of the game
		enum Opcode : uint16_t
		{
			LOGIN_PASSWORD = 1,
			GUEST_LOGIN = 2,
			SERVERLIST_REREQUEST = 4,
			CHARLIST_REQUEST = 5,
			SERVERSTATUS_REQUEST = 6,
			ACCEPT_TOS = 7,
			SET_GENDER = 8,
			AFTER_LOGIN = 9,
			REGISTER_PIN = 10,
			SERVERLIST_REQUEST = 11,
			PLAYER_DC = 12,
			VIEW_ALL_CHAR = 13,
			PICK_ALL_CHAR = 14,
			NAME_TRANSFER = 16,
			WORLD_TRANSFER = 18,
			CHAR_SELECT = 19,
			PLAYER_LOGGEDIN = 20,
			CHECK_CHAR_NAME = 21,
			CREATE_CHAR = 22,
			DELETE_CHAR = 23,
			PONG = 24,
			CLIENT_START_ERROR = 25,
			CLIENT_ERROR = 26,
			STRANGE_DATA = 27,
			RELOG = 28,
			REGISTER_PIC = 29,
			CHAR_SELECT_WITH_PIC = 30,
			VIEW_ALL_PIC_REGISTER = 31,
			VIEW_ALL_WITH_PIC = 32,
			LOGIN_START = 35, // custom name
			CHANGE_MAP = 38,
			CHANGE_CHANNEL = 39,
			ENTER_CASHSHOP = 40,
			MOVE_PLAYER = 41,
			CANCEL_CHAIR = 42,
			USE_CHAIR = 43,
			CLOSE_RANGE_ATTACK = 44,
			RANGED_ATTACK = 45,
			MAGIC_ATTACK = 46,
			TOUCH_MONSTER_ATTACK = 47,
			TAKE_DAMAGE = 48,
			GENERAL_CHAT = 49,
			CLOSE_CHALKBOARD = 50,
			FACE_EXPRESSION = 51,
			USE_ITEMEFFECT = 52,
			USE_DEATHITEM = 53,
			MOB_BANISH_PLAYER = 56,
			MONSTER_BOOK_COVER = 57,
			NPC_TALK = 58,
			REMOTE_STORE = 59,
			NPC_TALK_MORE = 60,
			NPC_SHOP = 61,
			STORAGE = 62,
			HIRED_MERCHANT_REQUEST = 63,
			FREDRICK_ACTION = 64,
			DUEY_ACTION = 65,
			OWL_ACTION = 66,
			OWL_WARP = 67,
			ADMIN_SHOP = 68,
			ITEM_SORT = 69,
			ITEM_SORT2 = 70,
			ITEM_MOVE = 71,
			USE_ITEM = 72,
			CANCEL_ITEM_EFFECT = 73,
			USE_SUMMON_BAG = 75,
			PET_FOOD = 76,
			USE_MOUNT_FOOD = 77,
			SCRIPTED_ITEM = 78,
			USE_CASH_ITEM = 79,
			USE_CATCH_ITEM = 81,
			USE_SKILL_BOOK = 82,
			USE_TELEPORT_ROCK = 84,
			USE_RETURN_SCROLL = 85,
			USE_UPGRADE_SCROLL = 86,
			DISTRIBUTE_AP = 87,
			AUTO_DISTRIBUTE_AP = 88,
			HEAL_OVER_TIME = 89,
			DISTRIBUTE_SP = 90,
			SPECIAL_MOVE = 91,
			CANCEL_BUFF = 92,
			SKILL_EFFECT = 93,
			MESO_DROP = 94,
			GIVE_FAME = 95,
			CHAR_INFO_REQUEST = 97,
			SPAWN_PET = 98,
			CANCEL_DEBUFF = 99,
			CHANGE_MAP_SPECIAL = 100,
			USE_INNER_PORTAL = 101,
			TROCK_ADD_MAP = 102,
			REPORT = 106,
			QUEST_ACTION = 107,
			GRENADE_EFFECT = 109,
			SKILL_MACRO = 110,
			USE_ITEM_REWARD = 112,
			MAKER_SKILL = 113,
			USE_REMOTE = 116,
			WATER_OF_LIFE = 117,
			ADMIN_CHAT = 118,
			MULTI_CHAT = 119,
			WHISPER = 120,
			SPOUSE_CHAT = 121,
			MESSENGER = 122,
			PLAYER_INTERACTION = 123,
			PARTY_OPERATION = 124,
			DENY_PARTY_REQUEST = 125,
			GUILD_OPERATION = 126,
			DENY_GUILD_REQUEST = 127,
			ADMIN_COMMAND = 128,
			ADMIN_LOG = 129,
			BUDDYLIST_MODIFY = 130,
			NOTE_ACTION = 131,
			USE_DOOR = 133,
			CHANGE_KEYMAP = 135,
			RPS_ACTION = 136,
			RING_ACTION = 137,
			WEDDING_ACTION = 138,
			WEDDING_TALK = 139,
			WEDDING_TALK_MORE = 139,
			ALLIANCE_OPERATION = 143,
			DENY_ALLIANCE_REQUEST = 144,
			OPEN_FAMILY_PEDIGREE = 145,
			OPEN_FAMILY = 146,
			ADD_FAMILY = 147,
			SEPARATE_FAMILY_BY_SENIOR = 148,
			SEPARATE_FAMILY_BY_JUNIOR = 149,
			ACCEPT_FAMILY = 150,
			USE_FAMILY = 151,
			CHANGE_FAMILY_MESSAGE = 152,
			FAMILY_SUMMON_RESPONSE = 153,
			BBS_OPERATION = 155,
			ENTER_MTS = 156,
			USE_SOLOMON_ITEM = 157,
			USE_GACHA_EXP = 158,
			NEW_YEAR_CARD_REQUEST = 159,
			CASHSHOP_SURPRISE = 161,
			CLICK_GUIDE = 162,
			ARAN_COMBO_COUNTER = 163,
			MOVE_PET = 167,
			PET_CHAT = 168,
			PET_COMMAND = 169,
			PET_LOOT = 170,
			PET_AUTO_POT = 171,
			PET_EXCLUDE_ITEMS = 172,
			MOVE_SUMMON = 175,
			SUMMON_ATTACK = 176,
			DAMAGE_SUMMON = 177,
			BEHOLDER = 178,
			MOVE_DRAGON = 181,
			CHANGE_QUICKSLOT = 183,
			MOVE_LIFE = 188,
			AUTO_AGGRO = 189,
			FIELD_DAMAGE_MOB = 191,
			MOB_DAMAGE_MOB_FRIENDLY = 192,
			MONSTER_BOMB = 193,
			MOB_DAMAGE_MOB = 194,
			NPC_ACTION = 197,
			ITEM_PICKUP = 202,
			DAMAGE_REACTOR = 205,
			TOUCHING_REACTOR = 206,
			PLAYER_MAP_TRANSFER = 207,
			MAPLETV = 65534,
			SNOWBALL = 211,
			LEFT_KNOCKBACK = 212,
			COCONUT = 213,
			MATCH_TABLE = 214,
			MONSTER_CARNIVAL = 218,
			PARTY_SEARCH_REGISTER = 220,
			PARTY_SEARCH_START = 222,
			PARTY_SEARCH_UPDATE = 223,
			CHECK_CASH = 228,
			CASHSHOP_OPERATION = 229,
			COUPON_CODE = 230,
			OPEN_ITEMUI = 236,
			CLOSE_ITEMUI = 237,
			USE_ITEMUI = 238,
			MTS_OPERATION = 253,
			USE_MAPLELIFE = 256,
			USE_HAMMER = 260
		};

	protected:
		// Skip a number of bytes (filled with zeros)
		void skip(size_t count);
		// Write a byte
		void write_byte(int8_t ch);
		// Write a short
		void write_short(int16_t sh);
		// Write an int
		void write_int(int32_t in);
		// Write a long
		void write_long(int64_t lg);

		// Write a point
		// One short for x and one for y
		void write_point(Point<int16_t> point);
		// Write a timestamp as an integer
		void write_time();
		// Write a string
		// Writes the length as a short and then each individual character as a byte
		void write_string(const std::string& str);
		// Write a random int
		void write_random();

		// Write the MACS and then write the HWID of the computer
		void write_hardware_info();
		// Function to convert hexadecimal to decimal
		int32_t hex_to_dec(std::string hexVal);

	private:
		std::vector<int8_t> bytes;
		int16_t opcode;
	};
}