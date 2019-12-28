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
#include "PacketSwitch.h"

#include "Handlers/CommonHandlers.h"
#include "Handlers/LoginHandlers.h"
#include "Handlers/SetfieldHandlers.h"
#include "Handlers/PlayerHandlers.h"
#include "Handlers/AttackHandlers.h"
#include "Handlers/MapobjectHandlers.h"
#include "Handlers/InventoryHandlers.h"
#include "Handlers/MessagingHandlers.h"
#include "Handlers/NpcInteractionHandlers.h"
#include "Handlers/TestingHandlers.h"

#include "../Console.h"
#include "../Configuration.h"

namespace ms
{
	// Opcodes for InPackets.
	enum Opcode : uint16_t
	{
		// Login 1
		LOGIN_RESULT = 0,
		SERVERLIST = 10,
		CHARLIST = 11,
		SERVER_IP = 12,
		CHARNAME_RESPONSE = 13,
		ADD_NEWCHAR_ENTRY = 14,
		DELCHAR_RESPONSE = 15,
		PING = 17,

		// Player 1
		APPLY_BUFF = 20,

		// Login 2
		SELECT_WORLD = 26,
		RECOMMENDED_WORLDS = 27,
		CHECK_SPW_RESULT = 28,

		// Inventory 1
		MODIFY_INVENTORY = 29,

		// Player 2
		CHANGE_STATS = 31,
		GIVE_BUFF = 32,
		CANCEL_BUFF = 33,
		RECALCULATE_STATS = 35,
		UPDATE_SKILL = 36,

		// Messaging 1
		SHOW_STATUS_INFO = 39,
		MEMO_RESULT = 41,
		ENABLE_REPORT = 47,

		//Inventory 2
		GATHER_RESULT = 52,
		SORT_RESULT = 53,

		// Player 3
		UPDATE_GENDER = 58,
		BUDDY_LIST = 63,
		GUILD_OPERATION = 65,

		// Messaging 2
		SERVER_MESSAGE = 68,
		WEEK_EVENT_MESSAGE = 77,

		FIELD_SET_VARIABLE = 92,
		FAMILY_PRIV_LIST = 100,
		CANCEL_RENAME_BY_OTHER = 120,
		SCRIPT_PROGRESS_MESSAGE = 122,
		RECEIVE_POLICE = 123,
		SKILL_MACROS = 124,
		SET_FIELD = 125,
		FIELD_EFFECT = 138,
		FIELD_OBSTACLE_ONOFF_LIST = 140,
		ADMIN_RESULT = 144,
		CLOCK = 147,

		// Mapobject
		SPAWN_CHAR = 160,
		REMOVE_CHAR = 161,

		// Messaging
		CHAT_RECEIVED = 162,
		SCROLL_RESULT = 167,

		// Mapobject
		SPAWN_PET = 168,
		CHAR_MOVED = 185,

		// Attack
		ATTACKED_CLOSE = 186,
		ATTACKED_RANGED = 187,
		ATTACKED_MAGIC = 188,

		FACIAL_EXPRESSION = 193,
		SHOW_ITEM_EFFECT = 194,
		SHOW_CHAIR = 196,
		UPDATE_CHARLOOK = 197,
		SHOW_FOREIGN_EFFECT = 198,
		GIVE_FOREIGN_BUFF = 199,
		CANCEL_FOREIGN_BUFF = 200,
		SHOW_ITEM_GAIN_INCHAT = 206, // TODO: Rename this (Terribly named)
		UPDATE_QUEST_INFO = 211,
		LOCK_UI = 221,
		TOGGLE_UI = 222,

		// Player
		ADD_COOLDOWN = 234,

		// Mapobject
		SPAWN_MOB = 236,
		KILL_MOB = 237,
		SPAWN_MOB_C = 238,
		MOB_MOVED = 239,
		MOVE_MOB_RESPONSE = 240,
		SHOW_MOB_HP = 250,
		SPAWN_NPC = 257,
		SPAWN_NPC_C = 259,
		MAKE_NPC_SCRIPTED = 263,
		DROP_LOOT = 268,
		REMOVE_LOOT = 269,
		HIT_REACTOR = 277,
		SPAWN_REACTOR = 279,
		REMOVE_REACTOR = 280,

		// NPC Interaction
		NPC_DIALOGUE = 304,
		OPEN_NPC_SHOP = 305,
		CONFIRM_SHOP_TRANSACTION = 306,
		PLAYER_INTERACTION = 314,
		KEYMAP = 335,
		AUTO_HP_POT = 336,
		AUTO_MP_POT = 337
	};

	PacketSwitch::PacketSwitch()
	{
		// Common handlers
		emplace<PING, PingHandler>();

		// Login handlers
		emplace<LOGIN_RESULT, LoginResultHandler>();
		emplace<SERVERLIST, ServerlistHandler>();
		emplace<RECOMMENDED_WORLDS, RecommendedWorldsHandler>();
		emplace<CHARLIST, CharlistHandler>();
		emplace<CHARNAME_RESPONSE, CharnameResponseHandler>();
		emplace<ADD_NEWCHAR_ENTRY, AddNewCharEntryHandler>();
		emplace<DELCHAR_RESPONSE, DeleteCharResponseHandler>();
		emplace<SERVER_IP, ServerIPHandler>();

		// 'Setfield' handlers
		emplace<SET_FIELD, SetfieldHandler>();

		// MapObject handlers
		emplace<SPAWN_CHAR, SpawnCharHandler>();
		emplace<CHAR_MOVED, CharMovedHandler>();
		emplace<UPDATE_CHARLOOK, UpdateCharLookHandler>();
		emplace<SHOW_FOREIGN_EFFECT, ShowForeignEffectHandler>();
		emplace<REMOVE_CHAR, RemoveCharHandler>();
		emplace<SPAWN_PET, SpawnPetHandler>();
		emplace<SPAWN_NPC, SpawnNpcHandler>();
		emplace<SPAWN_NPC_C, SpawnNpcControllerHandler>();
		emplace<SPAWN_MOB, SpawnMobHandler>();
		emplace<SPAWN_MOB_C, SpawnMobControllerHandler>();
		emplace<MOB_MOVED, MobMovedHandler>();
		emplace<SHOW_MOB_HP, ShowMobHpHandler>();
		emplace<KILL_MOB, KillMobHandler>();
		emplace<DROP_LOOT, DropLootHandler>();
		emplace<REMOVE_LOOT, RemoveLootHandler>();
		emplace<HIT_REACTOR, HitReactorHandler>();
		emplace<SPAWN_REACTOR, SpawnReactorHandler>();
		emplace<REMOVE_REACTOR, RemoveReactorHandler>();

		// Attack handlers
		emplace<ATTACKED_CLOSE, CloseAttackHandler>();
		emplace<ATTACKED_RANGED, RangedAttackHandler>();
		emplace<ATTACKED_MAGIC, MagicAttackHandler>();

		// Player handlers
		emplace<KEYMAP, KeymapHandler>();
		emplace<SKILL_MACROS, SkillMacrosHandler>();
		emplace<CHANGE_STATS, ChangeStatsHandler>();
		emplace<GIVE_BUFF, ApplyBuffHandler>();
		emplace<CANCEL_BUFF, CancelBuffHandler>();
		emplace<RECALCULATE_STATS, RecalculateStatsHandler>();
		emplace<UPDATE_SKILL, UpdateSkillHandler>();
		emplace<ADD_COOLDOWN, AddCooldownHandler>();

		// Messaging handlers
		emplace<SHOW_STATUS_INFO, ShowStatusInfoHandler>();
		emplace<CHAT_RECEIVED, ChatReceivedHandler>();
		emplace<SCROLL_RESULT, ScrollResultHandler>();
		emplace<SERVER_MESSAGE, ServerMessageHandler>();
		emplace<WEEK_EVENT_MESSAGE, WeekEventMessageHandler>();
		emplace<SHOW_ITEM_GAIN_INCHAT, ShowItemGainInChatHandler>();

		// Inventory Handlers
		emplace<MODIFY_INVENTORY, ModifyInventoryHandler>();
		emplace<GATHER_RESULT, GatherResultHandler>();
		emplace<SORT_RESULT, SortResultHandler>();

		// Npc Interaction Handlers
		emplace<NPC_DIALOGUE, NpcDialogueHandler>();
		emplace<OPEN_NPC_SHOP, OpenNpcShopHandler>();

		// TODO: Handle packets below correctly
		emplace<MOVE_MOB_RESPONSE, NullHandler>();
		emplace<MEMO_RESULT, NullHandler>();
		emplace<ENABLE_REPORT, NullHandler>();
		emplace<BUDDY_LIST, NullHandler>();
		emplace<GUILD_OPERATION, NullHandler>();
		emplace<FAMILY_PRIV_LIST, NullHandler>();
		emplace<SCRIPT_PROGRESS_MESSAGE, NullHandler>();
		emplace<RECEIVE_POLICE, NullHandler>();
		emplace<MAKE_NPC_SCRIPTED, NullHandler>();

		// Ignored
		emplace<SELECT_WORLD, NullHandler>();
		emplace<UPDATE_GENDER, NullHandler>();

		// New handlers for testing only
		// Once these are handled properly, they need moved to a proper file
		emplace<CHECK_SPW_RESULT, CheckSpwResultHandler>();
		emplace<FIELD_EFFECT, FieldEffectHandler>();
		emplace<FIELD_OBSTACLE_ONOFF_LIST, FieldObstacleOnOffListHandler>();
		emplace<ADMIN_RESULT, AdminResultHandler>();
		emplace<FACIAL_EXPRESSION, FacialExpressionHandler>();
		emplace<GIVE_FOREIGN_BUFF, GiveForeignBuffHandler>();
		emplace<CANCEL_FOREIGN_BUFF, CancelForeignBuffHandler>();
		emplace<UPDATE_QUEST_INFO, UpdateQuestInfoHandler>();
		emplace<LOCK_UI, LockUiHandler>();
		emplace<TOGGLE_UI, ToggleUiHandler>();
		emplace<CONFIRM_SHOP_TRANSACTION, ConfirmShopTransactionHandler>();
		emplace<PLAYER_INTERACTION, PlayerInteractionHandler>();
		emplace<AUTO_HP_POT, AutoHpPotHandler>();
		emplace<AUTO_MP_POT, AutoMpPotHandler>();
	}

	void PacketSwitch::forward(const int8_t* bytes, size_t length) const
	{
		// Wrap the bytes with a parser.
		InPacket recv = { bytes, length };
		// Read the opcode to determine handler responsible.
		uint16_t opcode = recv.read_short();

		if (Configuration::get().get_show_packets())
			std::cout << "Received Packet: " << std::to_string(opcode) << std::endl;

		if (opcode < NUM_HANDLERS)
		{
			if (auto & handler = handlers[opcode])
			{
				// Handler ok. Packet is passed on.
				try
				{
					handler->handle(recv);
				}
				catch (const PacketError& err)
				{
					// Notice about an error.
					warn(err.what(), opcode);
				}
			}
			else
			{
				// Warn about an unhandled packet.
				warn(MSG_UNHANDLED, opcode);
			}
		}
		else
		{
			// Warn about a packet with opcode out of bounds.
			warn(MSG_OUTOFBOUNDS, opcode);
		}
	}

	void PacketSwitch::warn(const std::string& message, size_t opcode) const
	{
		Console::get().print(message + ", Opcode: " + std::to_string(opcode));
	}
}