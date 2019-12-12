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
#include "UIKeyConfig.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIKeyConfig::UIKeyConfig() : UIDragElement<PosKEYCONFIG>(Point<int16_t>(0, 0)), dirty(false)
	{
		keyboard = &UI::get().get_keyboard();
		staged_keys = keyboard->get_maplekeys();

		nl::node KeyConfig = nl::nx::ui["StatusBar3.img"]["KeyConfig"];

		icon = KeyConfig["icon"];
		key = KeyConfig["key"];

		nl::node backgrnd = KeyConfig["backgrnd"];
		Texture bg = backgrnd;
		Point<int16_t> bg_dimensions = bg.get_dimensions();

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(KeyConfig["backgrnd2"]);
		sprites.emplace_back(KeyConfig["backgrnd3"]);

		nl::node BtClose3 = nl::nx::ui["Basic.img"]["BtClose3"];
		buttons[Buttons::CLOSE] = std::make_unique<MapleButton>(BtClose3, Point<int16_t>(bg_dimensions.x() - 18, 3));
		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(KeyConfig["button:Cancel"]);
		buttons[Buttons::DEFAULT] = std::make_unique<MapleButton>(KeyConfig["button:Default"]);
		buttons[Buttons::DELETE] = std::make_unique<MapleButton>(KeyConfig["button:Delete"]);
		buttons[Buttons::KEYSETTING] = std::make_unique<MapleButton>(KeyConfig["button:keySetting"]);
		buttons[Buttons::OK] = std::make_unique<MapleButton>(KeyConfig["button:OK"]);

		dimension = bg_dimensions;
		dragarea = Point<int16_t>(bg_dimensions.x(), 20);

		load_keys_pos();
		load_unbound_actions_pos();
		load_key_textures();
		load_action_icons();
	}

	void UIKeyConfig::load_keys_pos()
	{
		int16_t slot_width = 33;
		int16_t slot_width_lg = 98;
		int16_t slot_height = 33;

		int16_t row_y = 126;
		int16_t row_special_y = row_y - slot_height - 5;

		int16_t row_quickslot_x = 535;

		int16_t row_one_x = 31;
		int16_t row_two_x = 80;
		int16_t row_three_x = 96;
		int16_t row_four_x = 55;
		int16_t row_five_x = 39;

		int16_t row_special_x = row_one_x;

		keys_pos[KeyConfig::Key::ESCAPE] = Point<int16_t>(row_one_x, row_special_y);

		row_special_x += slot_width * 2;

		for (size_t i = KeyConfig::Key::F1; i <= KeyConfig::Key::F12; i++)
		{
			KeyConfig::Key id = KeyConfig::actionbyid(i);

			keys_pos[id] = Point<int16_t>(row_special_x, row_special_y);

			row_special_x += slot_width;

			if (id == KeyConfig::Key::F4 || id == KeyConfig::Key::F8)
				row_special_x += 17;
		}

		keys_pos[KeyConfig::Key::SCROLL_LOCK] = Point<int16_t>(row_quickslot_x + (slot_width * 1), row_special_y);

		keys_pos[KeyConfig::Key::GRAVE_ACCENT] = Point<int16_t>(row_one_x + (slot_width * 0), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM1] = Point<int16_t>(row_one_x + (slot_width * 1), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM2] = Point<int16_t>(row_one_x + (slot_width * 2), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM3] = Point<int16_t>(row_one_x + (slot_width * 3), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM4] = Point<int16_t>(row_one_x + (slot_width * 4), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM5] = Point<int16_t>(row_one_x + (slot_width * 5), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM6] = Point<int16_t>(row_one_x + (slot_width * 6), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM7] = Point<int16_t>(row_one_x + (slot_width * 7), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM8] = Point<int16_t>(row_one_x + (slot_width * 8), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM9] = Point<int16_t>(row_one_x + (slot_width * 9), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::NUM0] = Point<int16_t>(row_one_x + (slot_width * 10), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::MINUS] = Point<int16_t>(row_one_x + (slot_width * 11), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::EQUAL] = Point<int16_t>(row_one_x + (slot_width * 12), row_y + (slot_height * 0));

		for (size_t i = KeyConfig::Key::Q; i <= KeyConfig::Key::RIGHT_BRACKET; i++)
		{
			KeyConfig::Key id = KeyConfig::actionbyid(i);

			keys_pos[id] = Point<int16_t>(row_two_x + (slot_width * (i - KeyConfig::Key::Q)), row_y + (slot_height * 1));
		}

		row_two_x += 9;

		keys_pos[KeyConfig::Key::BACKSLASH] = Point<int16_t>(row_two_x + (slot_width * 12), row_y + (slot_height * 1));

		for (size_t i = KeyConfig::Key::A; i <= KeyConfig::Key::APOSTROPHE; i++)
		{
			KeyConfig::Key id = KeyConfig::actionbyid(i);

			keys_pos[id] = Point<int16_t>(row_three_x + (slot_width * (i - KeyConfig::Key::A)), row_y + (slot_height * 2));
		}

		keys_pos[KeyConfig::Key::LEFT_SHIFT] = Point<int16_t>(row_four_x + (slot_width * 0), row_y + (slot_height * 3));

		row_four_x += 24;

		for (size_t i = KeyConfig::Key::Z; i <= KeyConfig::Key::PERIOD; i++)
		{
			KeyConfig::Key id = KeyConfig::actionbyid(i);

			keys_pos[id] = Point<int16_t>(row_four_x + (slot_width * (i - KeyConfig::Key::Z + 1)), row_y + (slot_height * 3));
		}

		row_four_x += 24;

		keys_pos[KeyConfig::Key::RIGHT_SHIFT] = Point<int16_t>(row_four_x + (slot_width * 11), row_y + (slot_height * 3));

		keys_pos[KeyConfig::Key::LEFT_CONTROL] = Point<int16_t>(row_five_x + (slot_width_lg * 0), row_y + (slot_height * 4));
		keys_pos[KeyConfig::Key::LEFT_ALT] = Point<int16_t>(row_five_x + (slot_width_lg * 1), row_y + (slot_height * 4));

		row_five_x += 24;

		keys_pos[KeyConfig::Key::SPACE] = Point<int16_t>(row_five_x + (slot_width_lg * 2), row_y + (slot_height * 4));

		row_five_x += 27;

		keys_pos[KeyConfig::Key::RIGHT_ALT] = Point<int16_t>(row_five_x + (slot_width_lg * 3), row_y + (slot_height * 4));

		row_five_x += 2;

		keys_pos[KeyConfig::Key::RIGHT_CONTROL] = Point<int16_t>(row_five_x + (slot_width_lg * 4), row_y + (slot_height * 4));

		keys_pos[KeyConfig::Key::INSERT] = Point<int16_t>(row_quickslot_x + (slot_width * 0), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::HOME] = Point<int16_t>(row_quickslot_x + (slot_width * 1), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::PAGE_UP] = Point<int16_t>(row_quickslot_x + (slot_width * 2), row_y + (slot_height * 0));
		keys_pos[KeyConfig::Key::DELETE] = Point<int16_t>(row_quickslot_x + (slot_width * 0), row_y + (slot_height * 1));
		keys_pos[KeyConfig::Key::END] = Point<int16_t>(row_quickslot_x + (slot_width * 1), row_y + (slot_height * 1));
		keys_pos[KeyConfig::Key::PAGE_DOWN] = Point<int16_t>(row_quickslot_x + (slot_width * 2), row_y + (slot_height * 1));
	}

	void UIKeyConfig::load_unbound_actions_pos()
	{
		int16_t row_x = 26;
		int16_t row_y = 307;

		int16_t slot_width = 36;
		int16_t slot_height = 36;

		unbound_actions_pos[KeyAction::Id::MAPLECHAT] = Point<int16_t>(row_x + (slot_width * 0), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::TOGGLECHAT] = Point<int16_t>(row_x + (slot_width * 1), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::WHISPER] = Point<int16_t>(row_x + (slot_width * 2), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::MEDALS] = Point<int16_t>(row_x + (slot_width * 3), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::BOSSPARTY] = Point<int16_t>(row_x + (slot_width * 4), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::PROFESSION] = Point<int16_t>(row_x + (slot_width * 5), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::EQUIPMENT] = Point<int16_t>(row_x + (slot_width * 6), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::ITEMS] = Point<int16_t>(row_x + (slot_width * 7), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::CHARINFO] = Point<int16_t>(row_x + (slot_width * 8), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::MENU] = Point<int16_t>(row_x + (slot_width * 9), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::QUICKSLOTS] = Point<int16_t>(row_x + (slot_width * 10), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::PICKUP] = Point<int16_t>(row_x + (slot_width * 11), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::SIT] = Point<int16_t>(row_x + (slot_width * 12), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::ATTACK] = Point<int16_t>(row_x + (slot_width * 13), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::JUMP] = Point<int16_t>(row_x + (slot_width * 14), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::INTERACT_HARVEST] = Point<int16_t>(row_x + (slot_width * 15), row_y + (slot_height * 0));
		unbound_actions_pos[KeyAction::Id::SOULWEAPON] = Point<int16_t>(row_x + (slot_width * 16), row_y + (slot_height * 0));

		unbound_actions_pos[KeyAction::Id::SAY] = Point<int16_t>(row_x + (slot_width * 0), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::PARTYCHAT] = Point<int16_t>(row_x + (slot_width * 1), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::FRIENDSCHAT] = Point<int16_t>(row_x + (slot_width * 2), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::ITEMPOT] = Point<int16_t>(row_x + (slot_width * 3), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::EVENT] = Point<int16_t>(row_x + (slot_width * 4), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::SILENTCRUSADE] = Point<int16_t>(row_x + (slot_width * 5), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::STATS] = Point<int16_t>(row_x + (slot_width * 6), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::SKILLS] = Point<int16_t>(row_x + (slot_width * 7), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::QUESTLOG] = Point<int16_t>(row_x + (slot_width * 8), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::CHANGECHANNEL] = Point<int16_t>(row_x + (slot_width * 9), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::GUILD] = Point<int16_t>(row_x + (slot_width * 10), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::PARTY] = Point<int16_t>(row_x + (slot_width * 11), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::NOTIFIER] = Point<int16_t>(row_x + (slot_width * 12), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::FRIENDS] = Point<int16_t>(row_x + (slot_width * 13), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::WORLDMAP] = Point<int16_t>(row_x + (slot_width * 14), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::MINIMAP] = Point<int16_t>(row_x + (slot_width * 15), row_y + (slot_height * 1));
		unbound_actions_pos[KeyAction::Id::KEYBINDINGS] = Point<int16_t>(row_x + (slot_width * 16), row_y + (slot_height * 1));

		unbound_actions_pos[KeyAction::Id::GUILDCHAT] = Point<int16_t>(row_x + (slot_width * 0), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::ALLIANCECHAT] = Point<int16_t>(row_x + (slot_width * 1), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::BATTLEANALYSIS] = Point<int16_t>(row_x + (slot_width * 2), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::GUIDE] = Point<int16_t>(row_x + (slot_width * 3), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::ENHANCEEQUIP] = Point<int16_t>(row_x + (slot_width * 4), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::MONSTERCOLLECTION] = Point<int16_t>(row_x + (slot_width * 5), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::MANAGELEGION] = Point<int16_t>(row_x + (slot_width * 6), row_y + (slot_height * 2));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 7), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::MAPLENEWS] = Point<int16_t>(row_x + (slot_width * 8), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::CASHSHOP] = Point<int16_t>(row_x + (slot_width * 9), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::MAINMENU] = Point<int16_t>(row_x + (slot_width * 10), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::SCREENSHOT] = Point<int16_t>(row_x + (slot_width * 11), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::PICTUREMODE] = Point<int16_t>(row_x + (slot_width * 12), row_y + (slot_height * 2));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 13), row_y + (slot_height * 2));
		unbound_actions_pos[KeyAction::Id::MUTE] = Point<int16_t>(row_x + (slot_width * 14), row_y + (slot_height * 2));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 15), row_y + (slot_height * 2));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 16), row_y + (slot_height * 2));

		unbound_actions_pos[KeyAction::Id::FACE1] = Point<int16_t>(row_x + (slot_width * 0), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE2] = Point<int16_t>(row_x + (slot_width * 1), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE3] = Point<int16_t>(row_x + (slot_width * 2), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE4] = Point<int16_t>(row_x + (slot_width * 3), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE5] = Point<int16_t>(row_x + (slot_width * 4), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE6] = Point<int16_t>(row_x + (slot_width * 5), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::FACE7] = Point<int16_t>(row_x + (slot_width * 6), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::MAPLEACHIEVEMENT] = Point<int16_t>(row_x + (slot_width * 7), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::MONSTERBOOK] = Point<int16_t>(row_x + (slot_width * 8), row_y + (slot_height * 3));
		unbound_actions_pos[KeyAction::Id::TOSPOUSE] = Point<int16_t>(row_x + (slot_width * 9), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 10), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 11), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 12), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 13), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 14), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 15), row_y + (slot_height * 3));
		//unbound_actions_pos[KeyAction::Id::LENGTH] = Point<int16_t>(row_x + (slot_width * 16), row_y + (slot_height * 3));
	}

	void UIKeyConfig::load_key_textures()
	{
		key_textures[KeyConfig::Key::ESCAPE] = key[1];
		key_textures[KeyConfig::Key::NUM1] = key[2];
		key_textures[KeyConfig::Key::NUM2] = key[3];
		key_textures[KeyConfig::Key::NUM3] = key[4];
		key_textures[KeyConfig::Key::NUM4] = key[5];
		key_textures[KeyConfig::Key::NUM5] = key[6];
		key_textures[KeyConfig::Key::NUM6] = key[7];
		key_textures[KeyConfig::Key::NUM7] = key[8];
		key_textures[KeyConfig::Key::NUM8] = key[9];
		key_textures[KeyConfig::Key::NUM9] = key[10];
		key_textures[KeyConfig::Key::NUM0] = key[11];
		key_textures[KeyConfig::Key::MINUS] = key[12];
		key_textures[KeyConfig::Key::EQUAL] = key[13];

		key_textures[KeyConfig::Key::Q] = key[16];
		key_textures[KeyConfig::Key::W] = key[17];
		key_textures[KeyConfig::Key::E] = key[18];
		key_textures[KeyConfig::Key::R] = key[19];
		key_textures[KeyConfig::Key::T] = key[20];
		key_textures[KeyConfig::Key::Y] = key[21];
		key_textures[KeyConfig::Key::U] = key[22];
		key_textures[KeyConfig::Key::I] = key[23];
		key_textures[KeyConfig::Key::O] = key[24];
		key_textures[KeyConfig::Key::P] = key[25];
		key_textures[KeyConfig::Key::LEFT_BRACKET] = key[26];
		key_textures[KeyConfig::Key::RIGHT_BRACKET] = key[27];

		key_textures[KeyConfig::Key::LEFT_CONTROL] = key[29];
		key_textures[KeyConfig::Key::RIGHT_CONTROL] = key[29];

		key_textures[KeyConfig::Key::A] = key[30];
		key_textures[KeyConfig::Key::S] = key[31];
		key_textures[KeyConfig::Key::D] = key[32];
		key_textures[KeyConfig::Key::F] = key[33];
		key_textures[KeyConfig::Key::G] = key[34];
		key_textures[KeyConfig::Key::H] = key[35];
		key_textures[KeyConfig::Key::J] = key[36];
		key_textures[KeyConfig::Key::K] = key[37];
		key_textures[KeyConfig::Key::L] = key[38];
		key_textures[KeyConfig::Key::SEMICOLON] = key[39];
		key_textures[KeyConfig::Key::APOSTROPHE] = key[40];
		key_textures[KeyConfig::Key::GRAVE_ACCENT] = key[41];

		key_textures[KeyConfig::Key::LEFT_SHIFT] = key[42];
		key_textures[KeyConfig::Key::RIGHT_SHIFT] = key[42];

		key_textures[KeyConfig::Key::BACKSLASH] = key[43];
		key_textures[KeyConfig::Key::Z] = key[44];
		key_textures[KeyConfig::Key::X] = key[45];
		key_textures[KeyConfig::Key::C] = key[46];
		key_textures[KeyConfig::Key::V] = key[47];
		key_textures[KeyConfig::Key::B] = key[48];
		key_textures[KeyConfig::Key::N] = key[49];
		key_textures[KeyConfig::Key::M] = key[50];
		key_textures[KeyConfig::Key::COMMA] = key[51];
		key_textures[KeyConfig::Key::PERIOD] = key[52];

		key_textures[KeyConfig::Key::LEFT_ALT] = key[56];
		key_textures[KeyConfig::Key::RIGHT_ALT] = key[56];

		key_textures[KeyConfig::Key::SPACE] = key[57];

		key_textures[KeyConfig::Key::F1] = key[59];
		key_textures[KeyConfig::Key::F2] = key[60];
		key_textures[KeyConfig::Key::F3] = key[61];
		key_textures[KeyConfig::Key::F4] = key[62];
		key_textures[KeyConfig::Key::F5] = key[63];
		key_textures[KeyConfig::Key::F6] = key[64];
		key_textures[KeyConfig::Key::F7] = key[65];
		key_textures[KeyConfig::Key::F8] = key[66];
		key_textures[KeyConfig::Key::F9] = key[67];
		key_textures[KeyConfig::Key::F10] = key[68];

		key_textures[KeyConfig::Key::SCROLL_LOCK] = key[70];
		key_textures[KeyConfig::Key::HOME] = key[71];

		key_textures[KeyConfig::Key::PAGE_UP] = key[73];

		key_textures[KeyConfig::Key::END] = key[79];

		key_textures[KeyConfig::Key::PAGE_DOWN] = key[81];
		key_textures[KeyConfig::Key::INSERT] = key[82];
		key_textures[KeyConfig::Key::DELETE] = key[83];

		key_textures[KeyConfig::Key::F11] = key[87];
		key_textures[KeyConfig::Key::F12] = key[88];
	}

	void UIKeyConfig::load_action_icons()
	{
		action_icons[KeyAction::Id::EQUIPMENT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::EQUIPMENT), icon[0], -1);
		action_icons[KeyAction::Id::ITEMS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::ITEMS), icon[1], -1);
		action_icons[KeyAction::Id::STATS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::STATS), icon[2], -1);
		action_icons[KeyAction::Id::SKILLS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SKILLS), icon[3], -1);
		action_icons[KeyAction::Id::FRIENDS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FRIENDS), icon[4], -1);
		action_icons[KeyAction::Id::WORLDMAP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::WORLDMAP), icon[5], -1);
		action_icons[KeyAction::Id::MAPLECHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MAPLECHAT), icon[6], -1);
		action_icons[KeyAction::Id::MINIMAP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MINIMAP), icon[7], -1);
		action_icons[KeyAction::Id::QUESTLOG] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::QUESTLOG), icon[8], -1);
		action_icons[KeyAction::Id::KEYBINDINGS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::KEYBINDINGS), icon[9], -1);
		action_icons[KeyAction::Id::SAY] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SAY), icon[10], -1);
		action_icons[KeyAction::Id::WHISPER] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::WHISPER), icon[11], -1);
		action_icons[KeyAction::Id::PARTYCHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::PARTYCHAT), icon[12], -1);
		action_icons[KeyAction::Id::FRIENDSCHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FRIENDSCHAT), icon[13], -1);
		action_icons[KeyAction::Id::MENU] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MENU), icon[14], -1);
		action_icons[KeyAction::Id::QUICKSLOTS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::QUICKSLOTS), icon[15], -1);
		action_icons[KeyAction::Id::TOGGLECHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::TOGGLECHAT), icon[16], -1);
		action_icons[KeyAction::Id::GUILD] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::GUILD), icon[17], -1);
		action_icons[KeyAction::Id::GUILDCHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::GUILDCHAT), icon[18], -1);
		action_icons[KeyAction::Id::PARTY] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::PARTY), icon[19], -1);
		action_icons[KeyAction::Id::NOTIFIER] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::NOTIFIER), icon[20], -1);
		action_icons[KeyAction::Id::MAPLENEWS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MAPLENEWS), icon[21], -1);
		action_icons[KeyAction::Id::CASHSHOP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::CASHSHOP), icon[22], -1);
		action_icons[KeyAction::Id::ALLIANCECHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::ALLIANCECHAT), icon[23], -1);
		action_icons[KeyAction::Id::MANAGELEGION] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MANAGELEGION), icon[25], -1);
		action_icons[KeyAction::Id::MEDALS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MEDALS), icon[26], -1);
		action_icons[KeyAction::Id::BOSSPARTY] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::BOSSPARTY), icon[27], -1);
		action_icons[KeyAction::Id::PROFESSION] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::PROFESSION), icon[29], -1);
		action_icons[KeyAction::Id::ITEMPOT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::ITEMPOT), icon[30], -1);
		action_icons[KeyAction::Id::EVENT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::EVENT), icon[31], -1);
		action_icons[KeyAction::Id::SILENTCRUSADE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SILENTCRUSADE), icon[33], -1);
		//action_icons[KeyAction::Id::BITS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::BITS), icon[34], -1);
		action_icons[KeyAction::Id::BATTLEANALYSIS] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::BATTLEANALYSIS), icon[35], -1);
		action_icons[KeyAction::Id::GUIDE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::GUIDE), icon[39], -1);
		//action_icons[KeyAction::Id::VIEWERSCHAT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::VIEWERSCHAT), icon[40], -1);
		action_icons[KeyAction::Id::ENHANCEEQUIP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::ENHANCEEQUIP), icon[41], -1);
		action_icons[KeyAction::Id::MONSTERCOLLECTION] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MONSTERCOLLECTION), icon[42], -1);
		action_icons[KeyAction::Id::SOULWEAPON] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SOULWEAPON), icon[43], -1);
		action_icons[KeyAction::Id::CHARINFO] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::CHARINFO), icon[44], -1);
		action_icons[KeyAction::Id::CHANGECHANNEL] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::CHANGECHANNEL), icon[45], -1);
		action_icons[KeyAction::Id::MAINMENU] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MAINMENU), icon[46], -1);
		action_icons[KeyAction::Id::SCREENSHOT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SCREENSHOT), icon[47], -1);
		action_icons[KeyAction::Id::PICTUREMODE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::PICTUREMODE), icon[48], -1);
		action_icons[KeyAction::Id::MAPLEACHIEVEMENT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MAPLEACHIEVEMENT), icon[49], -1);
		action_icons[KeyAction::Id::PICKUP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::PICKUP), icon[50], -1);
		action_icons[KeyAction::Id::SIT] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SIT), icon[51], -1);
		action_icons[KeyAction::Id::ATTACK] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::ATTACK), icon[52], -1);
		action_icons[KeyAction::Id::JUMP] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::JUMP), icon[53], -1);
		action_icons[KeyAction::Id::INTERACT_HARVEST] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::INTERACT_HARVEST), icon[54], -1);
		action_icons[KeyAction::Id::FACE1] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE1), icon[100], -1);
		action_icons[KeyAction::Id::FACE2] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE2), icon[101], -1);
		action_icons[KeyAction::Id::FACE3] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE3), icon[102], -1);
		action_icons[KeyAction::Id::FACE4] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE4), icon[103], -1);
		action_icons[KeyAction::Id::FACE5] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE5), icon[104], -1);
		action_icons[KeyAction::Id::FACE6] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE6), icon[105], -1);
		action_icons[KeyAction::Id::FACE7] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::FACE7), icon[106], -1);
		//action_icons[KeyAction::Id::MAPLESTORAGE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MAPLESTORAGE), icon[200], -1);
		//action_icons[KeyAction::Id::SAFEMODE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::SAFEMODE), icon[201], -1);
		action_icons[KeyAction::Id::MUTE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MUTE), icon[202], -1);
		action_icons[KeyAction::Id::MONSTERBOOK] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::MONSTERBOOK), icon[1000], -1);
		action_icons[KeyAction::Id::TOSPOUSE] = std::make_unique<Icon>(std::make_unique<KeyMapIcon>(KeyAction::Id::TOSPOUSE), icon[1001], -1);
	}

	void UIKeyConfig::stage_keymap(Point<int16_t> cursorposition, Keyboard::Mapping keymap)
	{
		KeyConfig::Key key = key_by_position(cursorposition);

		if (std::find(bound_actions.begin(), bound_actions.end(), keymap.action) != bound_actions.end())
		{
			for (auto const& it : staged_keys)
			{
				Keyboard::Mapping staged_map = it.second;

				// TODO: compare whole map
				if (staged_map.action == keymap.action)
				{
					if (it.first == KeyConfig::Key::LEFT_CONTROL || it.first == KeyConfig::Key::RIGHT_CONTROL)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_CONTROL);
						staged_keys.erase(KeyConfig::Key::RIGHT_CONTROL);
					}
					else if (it.first == KeyConfig::Key::LEFT_ALT || it.first == KeyConfig::Key::RIGHT_ALT)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_ALT);
						staged_keys.erase(KeyConfig::Key::RIGHT_ALT);
					}
					else if (it.first == KeyConfig::Key::LEFT_SHIFT || it.first == KeyConfig::Key::RIGHT_SHIFT)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_SHIFT);
						staged_keys.erase(KeyConfig::Key::RIGHT_SHIFT);
					}
					else
					{
						staged_keys.erase(it.first);
					}

					break;
				}
			}
		}
		else
		{
			bound_actions.emplace_back(keymap.action);
		}

		Keyboard::Mapping staged_keymap = staged_keys[key];
		// TODO: does this check need to be changed?
		if (staged_keymap.type != KeyType::Id::NONE && staged_keymap.action != keymap.action)
			unstage_keymap(staged_keymap);

		if (key == KeyConfig::Key::LEFT_CONTROL || key == KeyConfig::Key::RIGHT_CONTROL)
		{
			staged_keys[KeyConfig::Key::LEFT_CONTROL] = keymap;
			staged_keys[KeyConfig::Key::RIGHT_CONTROL] = keymap;
		}
		else if (key == KeyConfig::Key::LEFT_ALT || key == KeyConfig::Key::RIGHT_ALT)
		{
			staged_keys[KeyConfig::Key::LEFT_ALT] = keymap;
			staged_keys[KeyConfig::Key::RIGHT_ALT] = keymap;
		}
		else if (key == KeyConfig::Key::LEFT_SHIFT || key == KeyConfig::Key::RIGHT_SHIFT)
		{
			staged_keys[KeyConfig::Key::LEFT_SHIFT] = keymap;
			staged_keys[KeyConfig::Key::RIGHT_SHIFT] = keymap;
		}
		else
		{
			staged_keys[key] = keymap;
		}

		// TODO: old logic would effectively always set dirty = true, is this correct though?
		dirty = true;
	}

	void UIKeyConfig::unstage_keymap(Keyboard::Mapping keymap)
	{
		auto iter = std::find(bound_actions.begin(), bound_actions.end(), keymap);

		if (iter != bound_actions.end())
		{
			bound_actions.erase(iter);

			for (auto const& it : staged_keys)
			{
				Keyboard::Mapping map = it.second;

				// TODO: compare whole map
				if (map.action == keymap.action)
				{
					if (it.first == KeyConfig::Key::LEFT_CONTROL || it.first == KeyConfig::Key::RIGHT_CONTROL)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_CONTROL);
						staged_keys.erase(KeyConfig::Key::RIGHT_CONTROL);

						dirty = true;
					}
					else if (it.first == KeyConfig::Key::LEFT_ALT || it.first == KeyConfig::Key::RIGHT_ALT)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_ALT);
						staged_keys.erase(KeyConfig::Key::RIGHT_ALT);

						dirty = true;
					}
					else if (it.first == KeyConfig::Key::LEFT_SHIFT || it.first == KeyConfig::Key::RIGHT_SHIFT)
					{
						staged_keys.erase(KeyConfig::Key::LEFT_SHIFT);
						staged_keys.erase(KeyConfig::Key::RIGHT_SHIFT);

						dirty = true;
					}
					else
					{
						staged_keys.erase(it.first);

						dirty = true;
					}

					break;
				}
			}
		}
	}

	KeyConfig::Key UIKeyConfig::key_by_position(Point<int16_t> cursorpos) const
	{
		for (auto iter : keys_pos)
		{
			Rectangle<int16_t> icon_rect = Rectangle<int16_t>(
				position + iter.second,
				position + iter.second + Point<int16_t>(32, 32)
				);

			if (icon_rect.contains(cursorpos))
				return iter.first;
		}

		return KeyConfig::Key::LENGTH;
	}

	UIKeyConfig::KeyMapIcon::KeyMapIcon(Keyboard::Mapping km) : keymap(km) {}

	UIKeyConfig::KeyMapIcon::KeyMapIcon(KeyAction::Id action) {
		KeyType::Id type = get_keytype(action);
		keymap = Keyboard::Mapping(type, action);
	}

	void UIKeyConfig::KeyMapIcon::drop_on_bindings(Point<int16_t> cursorposition, bool remove) const
	{
		auto keyconfig = UI::get().get_element<UIKeyConfig>();

		if (remove)
			keyconfig->unstage_keymap(keymap);
		else
			keyconfig->stage_keymap(cursorposition, keymap);
	}

	KeyType::Id UIKeyConfig::KeyMapIcon::get_keytype(KeyAction::Id action) const
	{
		switch (action)
		{
		case KeyAction::Id::EQUIPMENT:
		case KeyAction::Id::ITEMS:
		case KeyAction::Id::STATS:
		case KeyAction::Id::SKILLS:
		case KeyAction::Id::FRIENDS:
		case KeyAction::Id::WORLDMAP:
		case KeyAction::Id::MAPLECHAT:
		case KeyAction::Id::MINIMAP:
		case KeyAction::Id::QUESTLOG:
		case KeyAction::Id::KEYBINDINGS:
		case KeyAction::Id::TOGGLECHAT:
		case KeyAction::Id::WHISPER:
		case KeyAction::Id::SAY:
		case KeyAction::Id::PARTYCHAT:
		case KeyAction::Id::MENU:
		case KeyAction::Id::QUICKSLOTS:
		case KeyAction::Id::GUILD:
		case KeyAction::Id::FRIENDSCHAT:
		case KeyAction::Id::PARTY:
		case KeyAction::Id::NOTIFIER:
		case KeyAction::Id::CASHSHOP:
		case KeyAction::Id::GUILDCHAT:
		case KeyAction::Id::MEDALS:
		case KeyAction::Id::BITS:
		case KeyAction::Id::ALLIANCECHAT:
		case KeyAction::Id::MAPLENEWS:
		case KeyAction::Id::MANAGELEGION:
		case KeyAction::Id::PROFESSION:
		case KeyAction::Id::BOSSPARTY:
		case KeyAction::Id::ITEMPOT:
		case KeyAction::Id::EVENT:
		case KeyAction::Id::SILENTCRUSADE:
		case KeyAction::Id::BATTLEANALYSIS:
		case KeyAction::Id::GUIDE:
		case KeyAction::Id::VIEWERSCHAT:
		case KeyAction::Id::ENHANCEEQUIP:
		case KeyAction::Id::MONSTERCOLLECTION:
		case KeyAction::Id::SOULWEAPON:
		case KeyAction::Id::CHARINFO:
		case KeyAction::Id::CHANGECHANNEL:
		case KeyAction::Id::MAINMENU:
		case KeyAction::Id::SCREENSHOT:
		case KeyAction::Id::PICTUREMODE:
		case KeyAction::Id::MAPLEACHIEVEMENT:
			return KeyType::Id::MENU;
		case KeyAction::Id::PICKUP:
		case KeyAction::Id::SIT:
		case KeyAction::Id::ATTACK:
		case KeyAction::Id::JUMP:
			return KeyType::Id::ACTION;
		case KeyAction::Id::INTERACT_HARVEST:
		case KeyAction::Id::MAPLESTORAGE:
		case KeyAction::Id::SAFEMODE:
		case KeyAction::Id::MUTE:
		case KeyAction::Id::MONSTERBOOK:
		case KeyAction::Id::TOSPOUSE:
			return KeyType::Id::MENU;
		case KeyAction::Id::FACE1:
		case KeyAction::Id::FACE2:
		case KeyAction::Id::FACE3:
		case KeyAction::Id::FACE4:
		case KeyAction::Id::FACE5:
		case KeyAction::Id::FACE6:
		case KeyAction::Id::FACE7:
			return KeyType::Id::FACE;
		case KeyAction::Id::LEFT:
		case KeyAction::Id::RIGHT:
		case KeyAction::Id::UP:
		case KeyAction::Id::DOWN:
		case KeyAction::Id::BACK:
		case KeyAction::Id::TAB:
		case KeyAction::Id::RETURN:
		case KeyAction::Id::ESCAPE:
		case KeyAction::Id::SPACE:
		case KeyAction::Id::DELETE:
		case KeyAction::Id::HOME:
		case KeyAction::Id::END:
		case KeyAction::Id::COPY:
		case KeyAction::Id::PASTE:
		case KeyAction::Id::LENGTH:
		default:
			return KeyType::Id::NONE;
		}
	}
}
