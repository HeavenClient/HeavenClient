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

#include "../UIElement.h"

#include "../Components/Charset.h"
#include "../Components/Gauge.h"

#include "../../Character/CharStats.h"
#include "../../Graphics/SpecialText.h"

namespace ms
{
	class UIStatusBar : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::STATUSBAR;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		enum MenuType
		{
			MENU,
			SETTING,
			COMMUNITY,
			CHARACTER,
			EVENT
		};

		UIStatusBar(const CharStats &stats);

		void draw(float alpha) const override;

		void update() override;

		bool is_in_range(Point<int16_t> cursorpos) const override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		void toggle_qs();

		void toggle_menu();

		void remove_menus();

		bool is_menu_active();

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		static constexpr int16_t QUICKSLOT_MAX = 211;

		float getexppercent() const;

		float gethppercent() const;

		float getmppercent() const;

		void toggle_qs(bool quick_slot_active);

		void toggle_setting();

		void toggle_community();

		void toggle_character();

		void toggle_event();

		void remove_active_menu(MenuType type);

		Point<int16_t> get_quickslot_pos() const;

		enum Buttons : uint16_t
		{
			BT_CASHSHOP,
			BT_MENU,
			BT_OPTIONS,
			BT_CHARACTER,
			BT_COMMUNITY,
			BT_EVENT,
			BT_FOLD_QS,
			BT_EXTEND_QS,
			BT_MENU_QUEST,
			BT_MENU_MEDAL,
			BT_MENU_UNION,
			BT_MENU_MONSTER_COLLECTION,
			BT_MENU_AUCTION,
			BT_MENU_MONSTER_LIFE,
			BT_MENU_BATTLE,
			BT_MENU_ACHIEVEMENT,
			BT_MENU_FISHING,
			BT_MENU_HELP,
			BT_MENU_CLAIM,
			BT_SETTING_CHANNEL,
			BT_SETTING_OPTION,
			BT_SETTING_KEYS,
			BT_SETTING_JOYPAD,
			BT_SETTING_QUIT,
			BT_COMMUNITY_FRIENDS,
			BT_COMMUNITY_PARTY,
			BT_COMMUNITY_GUILD,
			BT_COMMUNITY_MAPLECHAT,
			BT_CHARACTER_INFO,
			BT_CHARACTER_STAT,
			BT_CHARACTER_SKILL,
			BT_CHARACTER_EQUIP,
			BT_CHARACTER_ITEM,
			BT_EVENT_SCHEDULE,
			BT_EVENT_DAILY
		};

		const CharStats &stats;

		Gauge expbar;
		Gauge hpbar;
		Gauge mpbar;
		Charset statset;
		Charset hpmpset;
		Charset levelset;
		Texture quickslot[2];
		Texture menutitle[5];
		Texture menubackground[3];
		OutlinedText namelabel;
		std::vector<Sprite> hpmp_sprites;

		Point<int16_t> exp_pos;
		Point<int16_t> hpmp_pos;
		Point<int16_t> hpset_pos;
		Point<int16_t> mpset_pos;
		Point<int16_t> statset_pos;
		Point<int16_t> levelset_pos;
		Point<int16_t> namelabel_pos;
		Point<int16_t> quickslot_pos;
		Point<int16_t> quickslot_adj;
		Point<int16_t> quickslot_qs_adj;
		Point<int16_t> menu_pos;
		Point<int16_t> setting_pos;
		Point<int16_t> community_pos;
		Point<int16_t> character_pos;
		Point<int16_t> event_pos;
		int16_t quickslot_min;
		int16_t position_x;
		int16_t position_y;

		bool quickslot_active;
		int16_t VWIDTH;
		int16_t VHEIGHT;

		bool menu_active;
		bool setting_active;
		bool community_active;
		bool character_active;
		bool event_active;
	};
}