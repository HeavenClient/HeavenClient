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
#include "UIMiniMap.h"
#include "UIWorldMap.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../Gameplay/Maplemap/Npc.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIMiniMap::UIMiniMap(const CharStats& st) : UIDragElement<PosMINIMAP>(Point<int16_t>(128, 20)), stats(st), 
		big_map(true), has_map(false), listNpc_enabled(false), listNpc_dimensions(Point<int16_t>(150, 170)), listNpc_offset(0), selected(-1)
	{
		type = Setting<MiniMapType>::get().load();
		simpleMode = Setting<MiniMapSimpleMode>::get().load();

		std::string node = simpleMode ? "MiniMapSimpleMode" : "MiniMap";
		MiniMap = nl::nx::ui["UIWindow2.img"][node];
		listNpc = nl::nx::ui["UIWindow2.img"]["MiniMap"]["ListNpc"];

		buttons[Buttons::BT_MIN] = std::make_unique<MapleButton>(MiniMap["BtMin"], Point<int16_t>(195, -6));
		buttons[Buttons::BT_MAX] = std::make_unique<MapleButton>(MiniMap["BtMax"], Point<int16_t>(209, -6));
		buttons[Buttons::BT_SMALL] = std::make_unique<MapleButton>(MiniMap["BtSmall"], Point<int16_t>(223, -6));
		buttons[Buttons::BT_BIG] = std::make_unique<MapleButton>(MiniMap["BtBig"], Point<int16_t>(223, -6));
		buttons[Buttons::BT_MAP] = std::make_unique<MapleButton>(MiniMap["BtMap"], Point<int16_t>(237, -6));
		buttons[Buttons::BT_NPC] = std::make_unique<MapleButton>(MiniMap["BtNpc"], Point<int16_t>(276, -6));

		region_text = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::WHITE);
		town_text = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::WHITE);
		combined_text = Text(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE);

		marker = Setting<MiniMapDefaultHelpers>::get().load() ? nl::nx::ui["UIWindow2.img"]["MiniMapSimpleMode"]["DefaultHelper"] : nl::nx::map["MapHelper.img"]["minimap"];

		player_marker = Animation(marker["user"]);
		selected_marker = Animation(MiniMap["iconNpc"]);

		//position.shift_y(-Constants::VIEWYOFFSET);
	}

	void UIMiniMap::draw(float alpha) const
	{
		if (type == Type::MIN)
		{
			for (auto sprite : min_sprites)
				sprite.draw(position, alpha);

			combined_text.draw(position + Point<int16_t>(7, -3));
		}
		else if (type == Type::NORMAL)
		{
			for (auto sprite : normal_sprites)
				sprite.draw(position, alpha);

			if (has_map)
			{
				for (auto sprite : static_marker_sprites)
					sprite.draw(position, alpha);

				draw_movable_markers(position, alpha);
				
				if (listNpc_enabled)
				{
					draw_npclist(normal_dimensions, alpha);
				}
			}
		}
		else
		{
			for (auto sprite : max_sprites)
				sprite.draw(position, alpha);

			region_text.draw(position + Point<int16_t>(48, 14));
			town_text.draw(position + Point<int16_t>(48, 28));

			if (has_map)
			{
				for (auto sprite : static_marker_sprites)
					sprite.draw(position + Point<int16_t>(0, max_adj), alpha);

				draw_movable_markers(position + Point<int16_t>(0, max_adj), alpha);

				if (listNpc_enabled) {
					draw_npclist(max_dimensions, alpha);
				}
			}
		}

		UIElement::draw(alpha);
	}

	void UIMiniMap::update()
	{
		int32_t mid = Stage::get().get_mapid();

		if (mid != mapid)
		{
			mapid = mid;
			Map = get_map_node_name();

			if (!Map["miniMap"])
				has_map = false;
			else
				has_map = true;

			scale = std::pow(2, (int)Map["miniMap"]["mag"]);
			center_offset = Point<int16_t>(Map["miniMap"]["centerX"], Map["miniMap"]["centerY"]);
			update_text();
			update_buttons();
			update_canvas();
			update_static_markers();
			toggle_buttons();
			update_npclist();
		}

		if (type == Type::MIN)
		{
			for (auto sprite : min_sprites)
				sprite.update();
		}
		else if (type == Type::NORMAL)
		{
			for (auto sprite : normal_sprites)
				sprite.update();
		}
		else
		{
			for (auto sprite : max_sprites)
				sprite.update();
		}

		if (has_map)
			for (auto sprite : static_marker_sprites)
				sprite.update();

		if (listNpc_enabled)
			for each (Sprite s in listNpc_sprites)
				s.update();

		if (selected >= 0)
		{
			selected_marker.update();
		}

		UIElement::update();
	}

	Cursor::State UIMiniMap::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(clicked, cursorpos);

		if (dragged)
			return dstate;

		Point<int16_t> cursor_relative = cursorpos - position;

		if (listNpc_slider.isenabled())
		{
			if (Cursor::State new_state = listNpc_slider.send_cursor(cursor_relative, clicked))
			{
				return new_state;
			}
		}

		if (listNpc_enabled && clicked)
		{
			Point<int16_t> clicked_point = cursor_relative - Point<int16_t>(10 + (type == Type::MAX ? max_dimensions : normal_dimensions).x(), 23);
			Rectangle<int16_t> list_bounds(0, listNpc_item_width, 0, listNpc_item_height * 8);
			if (list_bounds.contains(clicked_point))
			{
				int16_t clicked_item = listNpc_offset + clicked_point.y() / listNpc_item_height;
				select_npclist(clicked_item < listNpc_names.size() ? clicked_item :  -1);
			}
		}

		return Cursor::State::IDLE;
	}

	void UIMiniMap::send_scroll (double yoffset)
	{
		if (listNpc_enabled && listNpc_slider.isenabled())
			listNpc_slider.send_scroll(yoffset);
	}

	void UIMiniMap::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (type < Type::MAX)
			type++;
		else
			type = Type::MIN;

		toggle_buttons();
	}

	Button::State UIMiniMap::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
		case BT_MIN:
			type -= 1;
			toggle_buttons();
			return type == Type::MIN ? Button::State::DISABLED : Button::State::NORMAL;
		case BT_MAX:
			type += 1;
			toggle_buttons();
			return type == Type::MAX ? Button::State::DISABLED : Button::State::NORMAL;
		case BT_SMALL:
		case BT_BIG:
			big_map = !big_map;
			// TODO: toggle scrolling map
			toggle_buttons();
			break;
		case BT_MAP:
			UI::get().emplace<UIWorldMap>();
			break;
		case BT_NPC:
			// TODO: make NPC submenu
			set_npclist_active(!listNpc_enabled);
			break;
		}

		return Button::State::NORMAL;
	}

	void UIMiniMap::update_buttons()
	{
		// Add one pixel for a space to the right of each button
		bt_min_width = buttons[Buttons::BT_MIN]->width() + 1;
		bt_max_width = buttons[Buttons::BT_MAX]->width() + 1;
		bt_map_width = buttons[Buttons::BT_MAP]->width() + 1;

		combined_text_width = combined_text.width();
	}

	void UIMiniMap::toggle_buttons()
	{
		int16_t bt_min_x;

		if (type == Type::MIN)
		{
			buttons[Buttons::BT_MAP]->set_active(true);
			buttons[Buttons::BT_MAX]->set_active(true);
			buttons[Buttons::BT_MIN]->set_active(true);
			buttons[Buttons::BT_NPC]->set_active(false);
			buttons[Buttons::BT_SMALL]->set_active(false);
			buttons[Buttons::BT_BIG]->set_active(false);

			buttons[Buttons::BT_MIN]->set_state(Button::State::DISABLED);
			buttons[Buttons::BT_MAX]->set_state(Button::State::NORMAL);

			bt_min_x = combined_text_width + 11;

			buttons[Buttons::BT_MIN]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_min_width;

			buttons[Buttons::BT_MAX]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_max_width;

			buttons[Buttons::BT_MAP]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			min_dimensions = Point<int16_t>(bt_min_x + bt_map_width + 7, 20);

			update_dimensions();

			dragarea = dimension;

			set_npclist_active(false);
		}
		else
		{
			buttons[Buttons::BT_MAP]->set_active(true);
			buttons[Buttons::BT_MAX]->set_active(true);
			buttons[Buttons::BT_MIN]->set_active(true);
			buttons[Buttons::BT_NPC]->set_active(true);

			if (big_map)
			{
				buttons[Buttons::BT_BIG]->set_active(false);
				buttons[Buttons::BT_SMALL]->set_active(true);
			}
			else
			{
				buttons[Buttons::BT_BIG]->set_active(true);
				buttons[Buttons::BT_SMALL]->set_active(false);
			}

			buttons[Buttons::BT_MIN]->set_state(Button::State::NORMAL);

			bt_min_x = middle_right_x - (bt_min_width + buttons[Buttons::BT_SMALL]->width() + 1 + bt_max_width + bt_map_width + buttons[Buttons::BT_NPC]->width());

			buttons[Buttons::BT_MIN]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_max_width;

			buttons[Buttons::BT_MAX]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_max_width;

			buttons[Buttons::BT_SMALL]->set_position(Point<int16_t>(bt_min_x, btn_min_y));
			buttons[Buttons::BT_BIG]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_max_width;

			buttons[Buttons::BT_MAP]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			bt_min_x += bt_map_width;

			buttons[Buttons::BT_NPC]->set_position(Point<int16_t>(bt_min_x, btn_min_y));

			if (type == Type::MAX)
			{
				buttons[Buttons::BT_MAX]->set_state(Button::State::DISABLED);
			}
			else
			{
				buttons[Buttons::BT_MAX]->set_state(Button::State::NORMAL);
			}

			set_npclist_active(listNpc_enabled);

			dragarea = Point<int16_t>(dimension.x(), 20);
		}
	}

	void UIMiniMap::update_text()
	{
		nl::node Map = nl::nx::string["Map.img"];
		nl::node foundMap;

		for (auto node = Map.begin(); node != Map.end(); ++node)
		{
			foundMap = node.resolve(std::to_string(mapid));

			if (foundMap.size() > 0)
			{
				std::string mapName = foundMap["mapName"];
				std::string streetName = foundMap["streetName"];
				std::string combined = mapName + ": " + streetName;

				combined_text.change_text(combined);
				region_text.change_text(mapName);
				town_text.change_text(streetName);
				break;
			}
		}
	}

	void UIMiniMap::update_canvas()
	{
		min_sprites.clear();
		normal_sprites.clear();
		max_sprites.clear();

		nl::node Min, Normal, Max;

		if (simpleMode)
		{
			Min = MiniMap["Window"]["Min"];
			Normal = MiniMap["Window"]["Normal"];
			Max = MiniMap["Window"]["Max"];
		}
		else
		{
			Min = MiniMap["Min"];
			Normal = MiniMap["MinMap"];
			Max = MiniMap["MaxMap"];
		}

		map_sprite = Texture(Map["miniMap"]["canvas"]);
		Point<int16_t> map_dimensions = map_sprite.get_dimensions();
		

		// 48 (Offset for text) + longer text's width + 10 (space for right side border)
		int16_t mark_text_width = 48 + std::max(region_text.width(), town_text.width()) + 10;
		int16_t c_stretch, ur_x_offset, m_stretch, down_y_offset;
		int16_t window_width = std::max(178, std::max((int)mark_text_width, map_dimensions.x() + 20));

		c_stretch = std::max(0, window_width - 128);
		ur_x_offset = center_start_x + c_stretch;
		map_draw_origin_x = std::max(10, window_width / 2 - map_dimensions.x() / 2);

		if (map_dimensions.y() <= 20)
		{
			m_stretch = 5;
			down_y_offset = 17 + m_stretch;
			map_draw_origin_y = 10 + m_stretch - map_dimensions.y();
		}
		else
		{
			m_stretch = map_dimensions.y() - 17;
			down_y_offset = 17 + m_stretch;
			map_draw_origin_y = 20;
		}

		middle_right_x = ur_x_offset + 55;

		std::string Left = simpleMode ? "Left" : "w";
		std::string Center = simpleMode ? "Center" : "c";
		std::string Right = simpleMode ? "Right" : "e";

		std::string DownCenter = simpleMode ? "DownCenter" : "s";
		std::string DownLeft = simpleMode ? "DownLeft" : "sw";
		std::string DownRight = simpleMode ? "DownRight" : "se";
		std::string MiddleLeft = simpleMode ? "MiddleLeft" : "w";
		std::string MiddleRight = simpleMode ? "MiddleRight" : "e";
		std::string UpCenter = simpleMode ? "UpCenter" : "n";
		std::string UpLeft = simpleMode ? "UpLeft" : "nw";
		std::string UpRight = simpleMode ? "UpRight" : "ne";

		// SimpleMode's backdrop is opaque, notSimpleMode's is transparent but lightly coloured 
		// UI.wz v208 has normal centre sprite inlinked to bottom right window frame, not sure why?
		nl::node MiddleCenter = simpleMode ? MiniMap["Window"]["Max"]["MiddleCenter"] : MiniMap["MaxMap"]["c"];

		int16_t dl_dr_y = std::max(map_dimensions.y(), (int16_t)10);

		// combined_text_width + 14 (7px buffer on both sides) + 4 (buffer between name and buttons) + 3 buttons' widths - 128 (length of left and right window borders)
		int16_t min_c_stretch = combined_text_width + 18 + bt_min_width + bt_max_width + bt_map_width - 128;

		// Min sprites queue
		min_sprites.emplace_back(Min[Center], DrawArgument(window_ul_pos + Point<int16_t>(center_start_x, 0), Point<int16_t>(min_c_stretch, 0)));
		min_sprites.emplace_back(Min[Left], DrawArgument(window_ul_pos));
		min_sprites.emplace_back(Min[Right], DrawArgument(window_ul_pos + Point<int16_t>(min_c_stretch + center_start_x, 0)));

		// (7,10) is the top left corner of the inner window. 
		// 114 = 128 (width of left and right borders) - 14 (width of middle borders * 2). 27 = height of inner frame drawn on up and down borders
		// Normal sprites queue
		normal_sprites.emplace_back(MiddleCenter, DrawArgument(Point<int16_t>(7, 10), Point<int16_t>(c_stretch + 114, m_stretch + 27)));

		if (has_map)
			normal_sprites.emplace_back(Map["miniMap"]["canvas"], DrawArgument(Point<int16_t>(map_draw_origin_x, map_draw_origin_y)));

		normal_sprites.emplace_back(Normal[MiddleLeft], DrawArgument(Point<int16_t>(0, ml_mr_y), Point<int16_t>(0, m_stretch)));
		normal_sprites.emplace_back(Normal[MiddleRight], DrawArgument(Point<int16_t>(middle_right_x, ml_mr_y), Point<int16_t>(0, m_stretch)));
		normal_sprites.emplace_back(Normal[UpCenter], DrawArgument(Point<int16_t>(center_start_x, 0) + window_ul_pos, Point<int16_t>(c_stretch, 0)));
		normal_sprites.emplace_back(Normal[UpLeft], window_ul_pos);
		normal_sprites.emplace_back(Normal[UpRight], DrawArgument(Point<int16_t>(ur_x_offset, 0) + window_ul_pos));
		normal_sprites.emplace_back(Normal[DownCenter], DrawArgument(Point<int16_t>(center_start_x, down_y_offset + 18), Point<int16_t>(c_stretch, 0)));
		normal_sprites.emplace_back(Normal[DownLeft], Point<int16_t>(0, down_y_offset));
		normal_sprites.emplace_back(Normal[DownRight], Point<int16_t>(ur_x_offset, down_y_offset));

		normal_dimensions = Point<int16_t>(ur_x_offset + 64, down_y_offset + 27);

		// Max sprites queue
		max_sprites.emplace_back(MiddleCenter, DrawArgument(Point<int16_t>(7, 50), Point<int16_t>(c_stretch + 114, m_stretch + 27)));

		if (has_map)
			max_sprites.emplace_back(Map["miniMap"]["canvas"], DrawArgument(Point<int16_t>(map_draw_origin_x, map_draw_origin_y + max_adj)));

		max_sprites.emplace_back(Max[MiddleLeft], DrawArgument(Point<int16_t>(0, ml_mr_y + max_adj), Point<int16_t>(0, m_stretch)));
		max_sprites.emplace_back(Max[MiddleRight], DrawArgument(Point<int16_t>(middle_right_x, ml_mr_y + max_adj), Point<int16_t>(0, m_stretch)));
		max_sprites.emplace_back(Max[UpCenter], DrawArgument(Point<int16_t>(center_start_x, 0) + window_ul_pos, Point<int16_t>(c_stretch, 0)));
		max_sprites.emplace_back(Max[UpLeft], window_ul_pos);
		max_sprites.emplace_back(Max[UpRight], DrawArgument(Point<int16_t>(ur_x_offset, 0) + window_ul_pos));
		max_sprites.emplace_back(Max[DownCenter], DrawArgument(Point<int16_t>(center_start_x, down_y_offset + max_adj + 18), Point<int16_t>(c_stretch, 0)));
		max_sprites.emplace_back(Max[DownLeft], Point<int16_t>(0, down_y_offset + max_adj));
		max_sprites.emplace_back(Max[DownRight], Point<int16_t>(ur_x_offset, down_y_offset + max_adj));
		max_sprites.emplace_back(nl::nx::map["MapHelper.img"]["mark"][Map["info"]["mapMark"]], DrawArgument(Point<int16_t>(7, 16)));

		max_dimensions = normal_dimensions + Point<int16_t>(0, max_adj);

	}

	nl::node UIMiniMap::get_map_node_name()
	{
		// Create node name from new map id
		std::string mid_string = "000000000.img";
		std::string id_string = std::to_string(mapid);
		mid_string.replace(9 - id_string.length(), id_string.length(), id_string);

		// Get node of new map
		return nl::nx::map["Map"]["Map" + std::to_string(mapid / 100000000)][mid_string];
	}

	void UIMiniMap::draw_movable_markers(Point<int16_t> init_pos, float alpha) const
	{
		if (!has_map)
			return;

		Animation marker_sprite;
		Point<int16_t> sprite_offset;

		// NPCs
		MapObjects* npcs = Stage::get().get_npcs().get_npcs();
		marker_sprite = Animation(marker["npc"]);
		sprite_offset = marker_sprite.get_dimensions() / Point<int16_t>(2, 0);

		for (auto npc = npcs->begin(); npc != npcs->end(); ++npc)
		{
			Point<int16_t> npc_pos = npc->second.get()->get_position();
			marker_sprite.draw((npc_pos + center_offset) / scale - sprite_offset + Point<int16_t>(map_draw_origin_x, map_draw_origin_y) + init_pos, alpha);
		}

		// other characters
		MapObjects* chars = Stage::get().get_chars().get_chars();
		marker_sprite = Animation(marker["another"]);
		sprite_offset = marker_sprite.get_dimensions() / Point<int16_t>(2, 0);

		for (auto chr = chars->begin(); chr != chars->end(); ++chr)
		{
			Point<int16_t> chr_pos = chr->second.get()->get_position();
			marker_sprite.draw((chr_pos + center_offset) / scale - sprite_offset + Point<int16_t>(map_draw_origin_x, map_draw_origin_y) + init_pos, alpha);
		}

		// Player
		Point<int16_t> player_pos = Stage::get().get_player().get_position();
		sprite_offset = player_marker.get_dimensions() / Point<int16_t>(2, 0);
		player_marker.draw((player_pos + center_offset) / scale - sprite_offset + Point<int16_t>(map_draw_origin_x, map_draw_origin_y) + init_pos, alpha);
	}

	void UIMiniMap::update_static_markers()
	{
		static_marker_sprites.clear();

		if (!has_map)
			return;

		Animation marker_sprite;

		// portals
		nl::node portals = Map["portal"];
		marker_sprite = Animation(marker["portal"]);
		Point<int16_t> marker_offset = marker_sprite.get_dimensions() / Point<int16_t>(2, 0);

		for (nl::node portal = portals.begin(); portal != portals.end(); ++portal)
		{
			int portal_type = portal["pt"];

			if (portal_type == 2)
			{
				Point<int16_t> marker_pos = (Point<int16_t>(portal["x"], portal["y"]) + center_offset) / scale - marker_offset + Point<int16_t>(map_draw_origin_x, map_draw_origin_y);
				static_marker_sprites.emplace_back(marker_sprite, marker_pos);
			}
		}
	}

	void UIMiniMap::set_npclist_active(bool active)
	{
		if (type == Type::MIN) {
			listNpc_enabled = false;
			select_npclist(-1);
		}
		else {
			listNpc_enabled = active;
		}

		update_dimensions();
	}

	void UIMiniMap::update_dimensions()
	{
		if (type == Type::MIN)
		{
			dimension = min_dimensions;
		}
		else
		{
			Point<int16_t> base_dims = type == Type::MAX ? max_dimensions : normal_dimensions;
			dimension = base_dims;
			if (listNpc_enabled) {
				dimension += listNpc_dimensions;
				dimension.set_y(std::max(base_dims.y(), listNpc_dimensions.y()));
			}
		}
	}

	void UIMiniMap::update_npclist()
	{
		listNpc_sprites.clear();
		listNpc_names.clear();
		listNpc_list.clear();
		selected = -1;
		listNpc_offset = 0;

		if (simpleMode)
			return;

		MapObjects *npcs = Stage::get().get_npcs().get_npcs();
		for (auto npc = npcs->begin(); npc != npcs->end(); ++npc)
		{
			listNpc_list.emplace_back(npc->second.get());

			Npc* n = static_cast<Npc*>(npc->second.get());
			std::string name = n->get_name();
			if (n->get_func() != "")
				name += " (" + n->get_func() + ")";

			Text name_text = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE, name);

			// Stopgap solution to truncating names until Text supports truncation...
			while (name_text.width() > listNpc_text_width)
			{
				name.pop_back();
				name_text.change_text(name + "..");
			}
			listNpc_names.emplace_back(name_text);
		}

		const Point<int16_t> listNpc_pos = Point<int16_t>(type == Type::MAX ? max_dimensions.x() : normal_dimensions.x(), 0);
		int16_t c_stretch = 20;
		int16_t m_stretch = 102;

		if (listNpc_names.size() > 8)
		{
			listNpc_slider = Slider(Slider::DEFAULT, Range<int16_t>(23, 11 + listNpc_item_height * 8), listNpc_pos.x() + listNpc_item_width + 13, 8, listNpc_names.size(), 
			[&](bool upwards) 
			{
				int16_t shift = upwards ? -1 : 1;
				bool above = listNpc_offset + shift >= 0;
				bool below = listNpc_offset + 8 + shift <= listNpc_names.size();

				if (above && below)
					listNpc_offset += shift;
			});
			c_stretch += 12;
		}
		else
		{
			listNpc_slider.setenabled(false);
			m_stretch = listNpc_item_height * listNpc_names.size() - 34;
		}

		listNpc_sprites.emplace_back(listNpc["c"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x, m_start), Point<int16_t>(c_stretch, m_stretch)));
		listNpc_sprites.emplace_back(listNpc["w"], DrawArgument(listNpc_pos + Point<int16_t>(0, m_start), Point<int16_t>(0, m_stretch)));
		listNpc_sprites.emplace_back(listNpc["e"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x+c_stretch, m_start), Point<int16_t>(0, m_stretch)));
		listNpc_sprites.emplace_back(listNpc["n"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x, 0), Point<int16_t>(c_stretch, 0)));
		listNpc_sprites.emplace_back(listNpc["s"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x, m_start+m_stretch), Point<int16_t>(c_stretch, 0)));
		listNpc_sprites.emplace_back(listNpc["nw"], DrawArgument(listNpc_pos + Point<int16_t>(0, 0)));
		listNpc_sprites.emplace_back(listNpc["ne"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x+c_stretch, 0)));
		listNpc_sprites.emplace_back(listNpc["sw"], DrawArgument(listNpc_pos + Point<int16_t>(0, m_start + m_stretch)));
		listNpc_sprites.emplace_back(listNpc["se"], DrawArgument(listNpc_pos + Point<int16_t>(center_start_x+c_stretch, m_start+m_stretch))); 
		
		listNpc_dimensions = Point<int16_t>(center_start_x * 2 + c_stretch, m_start + m_stretch + 30);

		update_dimensions();
	}

	void UIMiniMap::draw_npclist(Point<int16_t> minimap_dims, float alpha) const
	{
		Animation npc_marker = Animation(marker["npc"]);

		for each (Sprite s in listNpc_sprites)
			s.draw(position, alpha);

		Point<int16_t> listNpc_pos = position + Point<int16_t>(minimap_dims.x() + 10, 23);

		for (int8_t i = 0; i + listNpc_offset < listNpc_list.size() && i < 8; i++)
		{
			if (selected - listNpc_offset == i)
			{
				ColorBox highlight = ColorBox(listNpc_item_width, listNpc_item_height, Color::Name::YELLOW, 1.0f);
				highlight.draw(listNpc_pos);
			}

			npc_marker.draw(DrawArgument(listNpc_pos + Point<int16_t>(2, 7), false, npc_marker.get_dimensions()/2), alpha);
			listNpc_names[listNpc_offset + i].draw(DrawArgument(listNpc_pos + Point<int16_t>(11, -3)));

			listNpc_pos.shift_y(listNpc_item_height);
		}

		if (listNpc_slider.isenabled())
		{
			listNpc_slider.draw(position);
		}

		if (selected >= 0)
		{
			Point<int16_t> npc_pos = (listNpc_list[selected]->get_position() + center_offset) / scale
				+ Point<int16_t>(map_draw_origin_x, map_draw_origin_y - npc_marker.get_dimensions().y() + (type == Type::MAX ? max_adj : 0));
			selected_marker.draw(position + npc_pos, 0.5f);
		}
	}

	void UIMiniMap::select_npclist(int16_t choice)
	{
		if (selected >= 0 && selected < listNpc_names.size())
		{
			listNpc_names[selected].change_color(Color::Name::WHITE);
		}

		if (choice > listNpc_names.size() || choice < 0)
		{
			selected = -1;
		}
		else
		{
			selected = choice != selected ? choice : -1;
			if (selected >= 0)
				listNpc_names[selected].change_color(Color::Name::BLACK);
		}
	}
}