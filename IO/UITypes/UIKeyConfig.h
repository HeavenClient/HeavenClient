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

#include "../UIDragElement.h"
#include "../KeyAction.h"
#include "../KeyConfig.h"
#include "../Keyboard.h"

#include "../Template/EnumMap.h"

namespace ms
{
	// TODO: reorganize functions based on ordering in .cpp file
	class UIKeyConfig : public UIDragElement<PosKEYCONFIG>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::KEYCONFIG;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIKeyConfig();

		void draw(float inter) const override;
		//void update() override;

		//void send_key(int32_t keycode, bool pressed, bool escape) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		//bool send_icon(const Icon& icon, Point<int16_t> cursorpos) override;

		void stage_mapping(Point<int16_t> cursorposition, Keyboard::Mapping);
		void unstage_mapping(Keyboard::Mapping);

		void close();

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void load_keys_pos();
		void load_unbound_actions_pos();
		void load_key_textures();
		void load_action_mappings();
		void load_action_icons();
		void load_skill_icons();

		void safe_close();

		void bind_action_keys();
		void save_staged_mappings();
		void clear();
		void reset();

		Texture get_skill_texture(int32_t skill_id) const;
		//int32_t get_key_from_action(KeyAction::Id action) const;
		// TODO: take note - I'm treating key_by_position as the old logic from all_keys_*
		KeyConfig::Key key_by_position(Point<int16_t> position) const;
		KeyAction::Id unbound_action_by_position(Point<int16_t> position) const;
		Keyboard::Mapping get_staged_mapping(int32_t keycode) const;
		//KeyType::Id get_keytype(KeyAction::Id action) const;
		bool UIKeyConfig::is_action_mapping(Keyboard::Mapping mapping) const;

		static KeyType::Id get_keytype(KeyAction::Id action)
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

		enum Buttons : uint16_t
		{
			CLOSE,
			CANCEL,
			DEFAULT,
			DELETE,
			KEYSETTING,
			OK
		};

		class MappingIcon : public Icon::Type
		{
		public:
			MappingIcon(Keyboard::Mapping);
			MappingIcon(KeyAction::Id keyId);

			void drop_on_stage() const override {}
			void drop_on_equips(Equipslot::Id) const override {}
			bool drop_on_items(InventoryType::Id, Equipslot::Id, int16_t, bool) const override { return true; }
			void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const override;
			void set_count(int16_t) override {}

		private:
			Keyboard::Mapping mapping;
		};

		bool dirty;

		Keyboard* keyboard = nullptr;

		nl::node key;
		nl::node icon;

		EnumMap<KeyConfig::Key, Texture> key_textures;
		EnumMap<KeyConfig::Key, Point<int16_t>> keys_pos;

		EnumMap<KeyAction::Id, std::unique_ptr<Icon>> action_icons;
		EnumMap<KeyAction::Id, Point<int16_t>> unbound_actions_pos;

		std::map<int32_t, std::unique_ptr<Icon>> skill_icons;

		// Used to determine if mapping belongs to predefined action, e.g. attack, pick up, faces, etc.
		std::vector<Keyboard::Mapping> action_mappings;

		// TODO: consider renaming staged_actions, but I don't really mind it like this
		std::vector<KeyAction::Id> bound_actions;
		std::map<int32_t, Keyboard::Mapping> staged_mappings;

		std::map<int32_t, Keyboard::Mapping> alternate_keys = {
		   { KeyConfig::Key::ESCAPE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MAINMENU) },
		   { KeyConfig::Key::F1, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE1) },
		   { KeyConfig::Key::F2, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE2) },
		   { KeyConfig::Key::F3, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE3) },
		   { KeyConfig::Key::F5, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE4) },
		   { KeyConfig::Key::F6, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE5) },
		   { KeyConfig::Key::F7, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE6) },
		   { KeyConfig::Key::F8, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE7) },
		   { KeyConfig::Key::SCROLL_LOCK, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SCREENSHOT) },
		   { KeyConfig::Key::GRAVE_ACCENT, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::CASHSHOP) },
		   { KeyConfig::Key::INSERT, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SAY) },
		   { KeyConfig::Key::HOME, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PARTYCHAT) },
		   { KeyConfig::Key::PAGE_UP, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::FRIENDSCHAT) },
		   { KeyConfig::Key::T, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::BOSSPARTY) },
		   { KeyConfig::Key::Y, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ITEMPOT) },
		   { KeyConfig::Key::U, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::EQUIPMENT) },
		   { KeyConfig::Key::I, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ITEMS) },
		   { KeyConfig::Key::P, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PARTY) },
		   { KeyConfig::Key::LEFT_BRACKET, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MENU) },
		   { KeyConfig::Key::RIGHT_BRACKET, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::QUICKSLOTS) },
		   { KeyConfig::Key::BACKSLASH, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::KEYBINDINGS) },
		   { KeyConfig::Key::DELETE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::GUILDCHAT) },
		   { KeyConfig::Key::END, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ALLIANCECHAT) },
		   { KeyConfig::Key::G, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::GUILD) },
		   { KeyConfig::Key::H, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::WHISPER) },
		   { KeyConfig::Key::J, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::QUESTLOG) },
		   { KeyConfig::Key::K, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SKILLS) },
		   { KeyConfig::Key::L, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::NOTIFIER) },
		   { KeyConfig::Key::SEMICOLON, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MEDALS) },
		   { KeyConfig::Key::APOSTROPHE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::TOGGLECHAT) },
		   { KeyConfig::Key::Z, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::PICKUP) },
		   { KeyConfig::Key::X, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::SIT) },
		   { KeyConfig::Key::C, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::STATS) },
		   { KeyConfig::Key::V, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::EVENT) },
		   { KeyConfig::Key::B, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PROFESSION) },
		   { KeyConfig::Key::N, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::WORLDMAP) },
		   { KeyConfig::Key::M, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MINIMAP) },
		   { KeyConfig::Key::PERIOD, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::FRIENDS) },
		   { KeyConfig::Key::LEFT_CONTROL, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::ATTACK) },
		   { KeyConfig::Key::LEFT_ALT, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::JUMP) },
		   { KeyConfig::Key::SPACE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::INTERACT_HARVEST) },
		   { KeyConfig::Key::RIGHT_ALT, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::JUMP) },
		   { KeyConfig::Key::RIGHT_CONTROL, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::ATTACK) }
		};

		std::map<int32_t, Keyboard::Mapping> basic_keys = {
		   { KeyConfig::Key::ESCAPE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MAINMENU) },
		   { KeyConfig::Key::F1, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE1) },
		   { KeyConfig::Key::F2, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE2) },
		   { KeyConfig::Key::F3, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE3) },
		   { KeyConfig::Key::F5, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE4) },
		   { KeyConfig::Key::F6, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE5) },
		   { KeyConfig::Key::F7, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE6) },
		   { KeyConfig::Key::F8, Keyboard::Mapping(KeyType::Id::FACE, KeyAction::Id::FACE7) },
		   { KeyConfig::Key::SCROLL_LOCK, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SCREENSHOT) },
		   { KeyConfig::Key::GRAVE_ACCENT, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::CASHSHOP) },
		   { KeyConfig::Key::NUM1, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SAY) },
		   { KeyConfig::Key::NUM2, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PARTYCHAT) },
		   { KeyConfig::Key::NUM3, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::FRIENDSCHAT) },
		   { KeyConfig::Key::NUM4, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::GUILDCHAT) },
		   { KeyConfig::Key::NUM5, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ALLIANCECHAT) },
		   { KeyConfig::Key::Q, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::QUESTLOG) },
		   { KeyConfig::Key::W, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::WORLDMAP) },
		   { KeyConfig::Key::E, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::EQUIPMENT) },
		   { KeyConfig::Key::R, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::FRIENDS) },
		   { KeyConfig::Key::T, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::BOSSPARTY) },
		   { KeyConfig::Key::Y, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ITEMPOT) },
		   { KeyConfig::Key::U, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::GUIDE) },
		   { KeyConfig::Key::I, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ITEMS) },
		   { KeyConfig::Key::O, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::ENHANCEEQUIP) },
		   { KeyConfig::Key::P, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PARTY) },
		   { KeyConfig::Key::LEFT_BRACKET, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MENU) },
		   { KeyConfig::Key::RIGHT_BRACKET, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::QUICKSLOTS) },
		   { KeyConfig::Key::BACKSLASH, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::KEYBINDINGS) },
		   { KeyConfig::Key::S, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::STATS) },
		   { KeyConfig::Key::G, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::GUILD) },
		   { KeyConfig::Key::H, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::WHISPER) },
		   { KeyConfig::Key::K, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::SKILLS) },
		   { KeyConfig::Key::L, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::NOTIFIER) },
		   { KeyConfig::Key::SEMICOLON, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MEDALS) },
		   { KeyConfig::Key::APOSTROPHE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::TOGGLECHAT) },
		   { KeyConfig::Key::Z, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::PICKUP) },
		   { KeyConfig::Key::X, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::SIT) },
		   { KeyConfig::Key::C, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MAPLECHAT) },
		   { KeyConfig::Key::V, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::EVENT) },
		   { KeyConfig::Key::B, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::PROFESSION) },
		   { KeyConfig::Key::M, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::MINIMAP) },
		   { KeyConfig::Key::LEFT_CONTROL, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::ATTACK) },
		   { KeyConfig::Key::LEFT_ALT, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::JUMP) },
		   { KeyConfig::Key::SPACE, Keyboard::Mapping(KeyType::Id::MENU, KeyAction::Id::INTERACT_HARVEST) },
		   { KeyConfig::Key::RIGHT_ALT, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::JUMP) },
		   { KeyConfig::Key::RIGHT_CONTROL, Keyboard::Mapping(KeyType::Id::ACTION, KeyAction::Id::ATTACK) }
		};
	};
}
