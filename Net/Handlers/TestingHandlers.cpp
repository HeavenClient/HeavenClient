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
#include "TestingHandlers.h"

#include "../../Gameplay/Stage.h"
#include "../../IO/UI.h"

#include "../../IO/UITypes/UILoginNotice.h"

namespace ms
{
	void CheckSpwResultHandler::handle(InPacket& recv) const
	{
		int reason = recv.read_byte();

		if (reason == 0)
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::INCORRECT_PIC);
		else
			std::cout << "[CheckSpwResultHandler]: Unknown reason: [" << reason << "]" << std::endl;

		UI::get().enable();
	}

	void LatestConnectedWorldHandler::handle(InPacket& recv) const
	{
		uint32_t latestConnectedWorldID = recv.read_int();
		// CUIWorldSelect::SetFocusWorld
		if (latestConnectedWorldID < 254 || latestConnectedWorldID > 255) {
			//SetFocusWorld(CUIWorldSelect *this, int nWorldID);
		}
	}

	void FieldEffectHandler::handle(InPacket& recv) const
	{
		int rand = recv.read_byte();

		// Effect
		if (rand == 3)
		{
			std::string path = recv.read_string();

			Stage::get().add_effect(path);

			return;
		}

		std::cout << "[FieldEffectHandler]: Unknown value: [" << rand << "]" << std::endl;
	}

	void AdminResultHandler::handle(InPacket& recv) const
	{
		uint8_t type = recv.read_byte();
		switch (type) {
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x10:
		case 0x1E:
		case 0x12:
			recv.read_byte();
			break;
		case 0x13:
			bool map = recv.read_bool();
			if (map) {
				recv.read_int();
			}
			else {
				recv.read_byte();
			}
		}
	}

	void QuickslotKeyMappedManHandler::handle(InPacket& recv) const
	{
		if (recv.read_bool()) {
			for (uint8_t i = 0; i < recv.length(); i++) {
				recv.read_int();
			}
		}
	}

	void sendAutoHpPotHandler::handle(InPacket& recv) const
	{
		recv.read_int();
	}

	void sendAutoMpPotHandler::handle(InPacket& recv) const
	{
		recv.read_int();
	}

	void updateInventorySlotLimitHandler::handle(InPacket& recv) const
	{
		recv.read_byte();
		recv.read_byte();
	}

	void MapTransferResultHandler::handle(InPacket& recv) const
	{
		uint8_t type = recv.read_byte();
		uint8_t vip = recv.read_bool();
		switch (type) {
		case 2:
		case 3:
			if (vip) {
				recv.read_byte();
				for (int i = 0; i < 10; i++) {
					recv.read_int();
				}
			}
			else {
				recv.read_byte();
				for (int i = 0; i < 5; i++) {
					recv.read_int();
				}
			}
			break;
		case 5:
			/// UI/Login.img/BtCancel
			break;
		case 6:
		case 7:
			/// UI/Login.img/BtOk
			break;
		case 8:
			/// UI/Login.img/BtCancel
			break;
		case 9:
			/// Hacking
			break;
		case 10:
			/// gamemaster
			break;
		case 11:
			/// Botting
			break;
		}
	}

	void SetTamingMobInfoHandler::handle(InPacket& recv) const
	{
		uint32_t charid = recv.read_int();
		/* TODO: Handle MapleMount
		Mount& mount = Stage::get().get_player().get_mount();
		mount.set_level(recv.read_int());
		mount.set_exp(recv.read_int());
		mount.set_tiredness(recv.read_int());
		mount.set_islevelup(recv.read_bool());
		*/
	}

	void SetShowQuestCompletionHandler::handle(InPacket& recv) const
	{
		int16_t questid = recv.read_short();
		Player& player = Stage::get().get_player();
		/// CreateQuestClear
	}

	void skillBookResultHandler::handle(InPacket& recv) const
	{
		uint32_t chrid = recv.read_int();
		recv.skip(1);
		uint32_t skillid = recv.read_int();
		uint32_t masterlevel = recv.read_int();
		uint8_t canuse = recv.read_bool();
		uint8_t success = recv.read_bool();
		/// TODO
	}
	
	/* No need!
	0 : You have succesfully reported the user. 
	1 : Unable to locate the user. 
	2 : You may only report users 10 times a day. 
	3 : You have been reported to the GM's by a user. 
	4 : Your request did not go through for unknown reasons. Please try again later.
	*/
	void SueCharacterResultHandler::handle(InPacket& recv) const
	{
		uint8_t mode = recv.read_byte();
		switch (mode) {
		case 0: 
			// UI/Login/Gender/text/0
			break;
		case 1: 
			// UI/Login/Gender/text/1
			break;
		case 2: 
			// UI/Login/Gender/BtYes
			break;
		case 3: 
			// UI/Login/Gender/BtNo
			break;
		default: 
			// UI/Login.img/Gender/scroll/0
			break;
		}
	}

	void TradeMoneyLimitHandler::handle(InPacket& recv) const
	{
		if (!recv.read_bool()) {
			// %s is under the request \r\nfor transfer to %s.
		}
	}

	void KeymapHandler::handle(InPacket& recv) const
	{
		recv.skip(1);

		for (uint8_t i = 0; i < 90; i++)
		{
			uint8_t type = recv.read_byte();
			int32_t action = recv.read_int();

			UI::get().add_keymapping(i, type, action);
		}
	}

	void TownPortalHandler::handle(InPacket& recv) const
	{
		Stage& stage = Stage::get();
		int32_t townid = recv.read_int();
		int32_t fieldid = recv.read_int();
		Point<int16_t> fieldportal;
		if (townid != 999999999 && fieldid != 999999999) {
			fieldportal = recv.read_point();
		}
		stage.load(townid, fieldid);
		stage.transfer_player();
	}

	void NotifyLevelUpHandler::handle(InPacket& recv) const
	{
		int8_t type = recv.read_byte();
		int32_t level = recv.read_int();
		std::string charname = recv.read_string();
	}

	void FriendResultHandler::handle(InPacket& recv) const
	{
		// TODO
	}

	void ScriptProgressMessageHandler::handle(InPacket& recv) const
	{
		std::string message = recv.read_string();
		// OnQuestProgressUpdated_Script
	}	
	
	void ClockHandler::handle(InPacket& recv) const
	{
		int8_t type = recv.read_byte();
		if (type == 0) {
			setEventTime(recv);
		}
		else if (type == 1) {
			setClockTime(recv);
		}
		else if (type == 2) {
			setClock(recv);
		}
	}

	void ClockHandler::setClock(InPacket& recv) const
	{
		int32_t time = recv.read_int();
	}

	void ClockHandler::setClockTime(InPacket& recv) const
	{
		int8_t hour = recv.read_byte();
		int8_t min = recv.read_byte();
		int8_t sec = recv.read_byte();
	}

	void ClockHandler::setEventTime(InPacket& recv) const
	{
		int32_t duration = recv.read_int();
	}
}