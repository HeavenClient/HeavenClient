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

#include <cstdint>

namespace ms
{
	namespace KeyAction
	{
		// Maple-specific keycodes, sent via the Keymap Packet.
		enum Id : int32_t
		{
			NONE = 0,
			EQUIPMENT = 1,
			ITEMS = 2,
			STATS = 3,
			SKILLS = 4,
			FRIENDS = 5,
			WORLDMAP = 6,
			MAPLECHAT = 7,
			MINIMAP = 8,
			QUESTLOG = 9,
			KEYBINDINGS = 10,
			TOGGLECHAT = 11,
			WHISPER = 12,
			SAY = 13,
			PARTYCHAT = 14,
			MENU = 15,
			QUICKSLOTS = 16,
			GUILD = 18,
			FRIENDSCHAT = 19,
			PARTY = 20,
			NOTIFIER = 21,
			CASHSHOP = 24,
			GUILDCHAT = 25,
			MEDALS = 28,
			BITS,
			ALLIANCECHAT,
			MAPLENEWS,
			MANAGELEGION,
			PROFESSION,
			BOSSPARTY,
			ITEMPOT,
			EVENT,
			SILENTCRUSADE,
			BATTLEANALYSIS,
			GUIDE,
			VIEWERSCHAT,
			ENHANCEEQUIP,
			MONSTERCOLLECTION,
			SOULWEAPON,
			CHARINFO,
			CHANGECHANNEL,
			MAINMENU,
			SCREENSHOT,
			PICTUREMODE,
			MAPLEACHIEVEMENT,
			PICKUP = 51,
			SIT = 52,
			ATTACK = 53,
			JUMP = 54,
			INTERACT_HARVEST = 55,
			MAPLESTORAGE,
			SAFEMODE,
			MUTE,
			MONSTERBOOK,
			TOSPOUSE,
			FACE1 = 100,
			FACE2 = 101,
			FACE3 = 102,
			FACE4 = 103,
			FACE5 = 104,
			FACE6 = 105,
			FACE7 = 106,
			// Static keys
				LEFT,
			RIGHT,
			UP,
			DOWN,
			BACK,
			TAB,
			RETURN,
			ESCAPE,
			SPACE,
			DELETE,
			HOME,
			END,
			COPY,
			PASTE,
			LENGTH
		};

		inline Id actionbyid(int32_t id)
		{
			return static_cast<Id>(id);
		}
	}
}