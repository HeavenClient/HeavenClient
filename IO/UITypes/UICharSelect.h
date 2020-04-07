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
#include "../Components/NameTag.h"

#include "../../Character/Look/CharLook.h"

#include <list>

namespace ms
{
	// The character selection screen
	class UICharSelect : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CHARSELECT;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UICharSelect(std::vector<CharEntry> characters, int8_t characters_count, int32_t slots, int8_t require_pic);

		void draw(float inter) const override;

		void update() override;

		void doubleclick(Point<int16_t> cursorpos) override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		void add_character(CharEntry &&character);

		void post_add_character();

		void remove_character(int32_t id);

		const CharEntry &get_character(int32_t id);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void update_buttons();

		void update_selected_character();

		void select_last_slot();

		std::string get_slot_text();

		std::string pad_number_with_leading_zero(uint8_t value) const;

		Point<int16_t> get_character_slot_pos(size_t index, uint16_t x_adj, uint16_t y_adj) const;

		Point<int16_t> get_infolabel_pos(size_t index) const;

		std::string get_infolabel(size_t index, StatsEntry character_stats) const;

		void request_pic();

		void check_pic(const std::string entered_pic) const;

		static constexpr uint8_t PAGESIZE = 8;

		enum Buttons : uint16_t
		{
			CHARACTER_SELECT,
			CHARACTER_NEW,
			CHARACTER_DELETE,
			PAGELEFT,
			PAGERIGHT,
			CHANGEPIC,
			RESETPIC,
			EDITCHARLIST,
			BACK,
			CHARACTER_SLOT0
		};

		std::vector<CharEntry> characters;
		int8_t characters_count;
		int32_t slots;
		int8_t require_pic;
		Text version;
		Point<int16_t> pagepos;
		Point<int16_t> worldpos;
		Point<int16_t> charinfopos;
		uint8_t selected_character;
		uint8_t selected_page;
		uint8_t page_count;
		Texture tab;
		uint8_t tab_index;
		bool tab_active;
		bool tab_move;
		Point<int16_t> tab_pos[3];
		int16_t tab_move_pos;
		std::map<uint8_t, uint16_t> tab_map;
		Point<int16_t> world_dimensions;
		Animation burning_notice;
		Text burning_count;
		std::vector<Sprite> world_sprites;
		Texture charinfo;
		Texture charslot;
		Texture pagebase;
		Charset pagenumber;
		nl::node pagenumberpos;
		Texture signpost[3];
		nl::node nametag;
		Charset levelset;
		OutlinedText namelabel;
		std::vector<CharLook> charlooks;
		std::vector<NameTag> nametags;
		Animation emptyslot_effect;
		Texture emptyslot;
		Animation selectedslot_effect[2];
		OutlinedText chatslotlabel;
		int16_t timestamp;
		uint16_t charslot_y;
		bool show_timestamp;
		bool burning_character;

		enum InfoLabel : uint8_t
		{
			JOB,
			STR,
			DEX,
			INT,
			LUK,
			NUM_LABELS
		};

		OutlinedText infolabels[UICharSelect::InfoLabel::NUM_LABELS];
	};
}