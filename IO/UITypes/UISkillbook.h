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
#include "../UIDragElement.h"

#include "../Components/Slider.h"
#include "../Components/Charset.h"
#include "../Components/StatefulIcon.h"
#include "../Character/CharStats.h"
#include "../Character/Skillbook.h"
#include "../Graphics/Text.h"

namespace ms
{
	class UISkillbook : public UIDragElement<PosSKILL>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::SKILLBOOK;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UISkillbook(const CharStats& stats, const Skillbook& skillbook);

		void draw(float alpha) const override;

		void toggle_active() override;
		void doubleclick(Point<int16_t> cursorpos) override;
		bool remove_cursor(bool clicked, Point<int16_t> cursorpos) override;
		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;

		void update_stat(Maplestat::Id stat, int16_t value);
		void update_skills(int32_t skill_id);
		bool is_skillpoint_enabled();

	protected:
		Button::State button_pressed(uint16_t id) override;

	private:
		void change_job(uint16_t id);
		void change_sp();
		void change_tab(uint16_t new_tab);
		void change_offset(uint16_t new_offset);

		void show_skill(int32_t skill_id);
		void clear_tooltip();

		bool can_raise(int32_t skill_id) const;
		void send_spup(uint16_t row);
		void spend_sp(int32_t skill_id);

		Job::Level joblevel_by_tab(uint16_t tab) const;
		SkillIconDeprecated* icon_by_position(Point<int16_t> cursorpos);

		void close();
		bool check_required(int32_t id) const;

		void set_macro(bool enabled);
		void set_skillpoint(bool enabled);

		enum Buttons : uint16_t
		{
			BT_CLOSE,
			BT_HYPER,
			BT_GUILDSKILL,
			BT_RIDE,
			BT_MACRO,
			BT_MACRO_OK,
			BT_CANCLE,
			BT_OKAY,
			BT_SPDOWN,
			BT_SPMAX,
			BT_SPUP,
			BT_TAB0,
			BT_TAB1,
			BT_TAB2,
			BT_TAB3,
			BT_TAB4,
			BT_SPUP0,
			BT_SPUP1,
			BT_SPUP2,
			BT_SPUP3,
			BT_SPUP4,
			BT_SPUP5,
			BT_SPUP6,
			BT_SPUP7,
			BT_SPUP8,
			BT_SPUP9,
			BT_SPUP10,
			BT_SPUP11
		};

		static constexpr int16_t ROWS = 12;
		static constexpr int16_t ROW_HEIGHT = 40;
		static constexpr int16_t ROW_WIDTH = 143;
		static constexpr Point<int16_t> SKILL_OFFSET = Point<int16_t>(11, 93);
		static constexpr Point<int16_t> ICON_OFFSET = Point<int16_t>(2, 34);
		static constexpr Point<int16_t> LINE_OFFSET = Point<int16_t>(0, 37);

		const CharStats& stats;
		const Skillbook& skillbook;

		Slider slider;
		Texture skille;
		Texture skilld;
		Texture skillb;
		Texture line;
		Texture bookicon;
		Text booktext;
		Text splabel;

		Job job;
		int16_t sp;
		int16_t beginner_sp;

		uint16_t tab;
		uint16_t skillcount;
		uint16_t offset;

		std::vector<SkillIconDeprecated> icons;
		bool grabbing;

		Point<int16_t> bg_dimensions;

		bool macro_enabled;
		Texture macro_backgrnd;
		Texture macro_backgrnd2;
		Texture macro_backgrnd3;

		bool sp_enabled;
		Texture sp_backgrnd;
		Texture sp_backgrnd2;
		Texture sp_backgrnd3;
		Charset sp_before;
		Charset sp_after;
		std::string sp_before_text;
		std::string sp_after_text;
		Text sp_used;
		Text sp_remaining;
		Text sp_name;
		Texture sp_skill;
		int32_t sp_id;
		int32_t sp_masterlevel;

		class SkillIcon : public StatefulIcon::Type
		{
		public:
			SkillIcon(int32_t skill_id);

			void drop_on_stage() const override {}
			void drop_on_equips(Equipslot::Id) const override {}
			bool drop_on_items(InventoryType::Id, Equipslot::Id, int16_t, bool) const override { return true; }
			void drop_on_bindings(Point<int16_t> cursorposition, bool remove) const override;
			void set_count(int16_t) override {}

		private:
			int32_t skill_id;
		};

		class SkillMeta
		{
		public:
			SkillMeta(int32_t id, int32_t level);

			int32_t get_id() const;
			int32_t get_level() const;
			UISkillbook::SkillIcon get_icon() const;

		private:
			int32_t id;
			int32_t level;
			UISkillbook::SkillIcon icon;
			Text name_text;
			Text level_text;
		};
	};
}