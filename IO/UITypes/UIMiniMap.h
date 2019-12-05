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

#include "../Graphics/Text.h"
#include "../Character/Charstats.h"
#include "../Gameplay/Stage.h"

#include "../Components/MapleButton.h"
#include "../Components/AreaButton.h"
#include "../Components/Slider.h"
#include "../Components/TextTooltip.h"

namespace ms
{
	class UIMiniMap : public UIDragElement<PosMINIMAP>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::MINIMAP;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIMiniMap(const CharStats& stats);

		void draw(float alpha) const override;
		void update() override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> pos) override;
		void UIMiniMap::send_scroll(double yoffset) override;
		void send_key(int32_t keycode, bool pressed, bool escape) override;
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		static constexpr int16_t center_start_x = 64;
		static constexpr Point<int16_t> window_ul_pos = Point<int16_t>(0, 0);
		static constexpr int16_t btn_min_y = 4;
		static constexpr int16_t ml_mr_y = 17;
		static constexpr int16_t max_adj = 40;
		const CharStats& stats;

		void update_buttons();
		void toggle_buttons();
		void update_text();
		void update_canvas();
		void draw_movable_markers(Point<int16_t> init_pos, float alpha) const;
		void update_static_markers();

		void set_npclist_active(bool active);

		void update_dimensions();

		void update_npclist();

		void draw_npclist(Point<int16_t> minimap_dims, float alpha) const;

		void select_npclist(int16_t choice);

		nl::node get_map_node_name();

		// Returns the name of the node, under which the argument mapid is in.
		std::string get_map_category(int mapid);

		enum Buttons
		{
			BT_MIN,
			BT_MAX,
			BT_SMALL,
			BT_BIG,
			BT_MAP,
			BT_NPC
		};

		enum Type
		{
			MIN,
			NORMAL,
			MAX
		};

		// Constants
		int32_t mapid;
		int8_t type;
		bool simpleMode;
		bool big_map;
		bool has_map;
		int16_t scale;
		nl::node Map;
		nl::node MiniMap;
		nl::node marker;
		Texture map_sprite;
		Animation player_marker;
		int16_t combined_text_width;
		int16_t middle_right_x;
		int16_t bt_min_width;
		int16_t bt_max_width;
		int16_t bt_map_width;
		std::vector<Sprite> min_sprites;
		std::vector<Sprite> normal_sprites;
		std::vector<Sprite> max_sprites;
		std::vector<std::pair<std::string, Point<int16_t>>> static_marker_info;
		int16_t map_draw_origin_x, map_draw_origin_y;
		Point<int16_t> center_offset;
		Point<int16_t> min_dimensions;
		Point<int16_t> normal_dimensions;
		Point<int16_t> max_dimensions;
		TextTooltip hover_tooltip;
		Text combined_text;
		Text region_text;
		Text town_text;

		bool tooltip_enabled;
		Point<int16_t> tooltip_pos;

		bool listNpc_enabled;
		nl::node listNpc;
		std::vector<Sprite> listNpc_sprites;
		std::vector<MapObject*> listNpc_list;
		std::vector<Text> listNpc_names;

		Point<int16_t> listNpc_dimensions;

		const int16_t m_start = 36;
		const int16_t listNpc_item_height = 17;
		const int16_t listNpc_item_width = 128;
		const int16_t listNpc_text_width = 114; 

		Slider listNpc_slider;
		int16_t listNpc_offset;
		int16_t selected;
		Animation selected_marker;
	};
}