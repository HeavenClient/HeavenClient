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

#include "../Components/ChatBalloon.h"

#include "../../Net/Login.h"

namespace ms
{
	class UIWorldSelect : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::WORLDSELECT;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIWorldSelect();

		void draw(float alpha) const override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		void draw_world();

		void add_world(World world);

		void add_recommended_world(RecommendedWorld world);

		void change_world(World selectedWorld);

		void remove_selected();

		void set_region(uint8_t value);

		uint16_t get_worldbyid(uint16_t worldid);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void enter_world();

		void toggle_recommended(bool active);

		void clear_selected_world();

		uint16_t get_next_world(uint16_t id, bool upward);

		enum Buttons : uint16_t
		{
			BT_WORLD0,
			BT_WORLD1,
			BT_WORLD2,
			BT_WORLD3,
			BT_WORLD4,
			BT_CHANNEL0,
			BT_CHANNEL1,
			BT_CHANNEL2,
			BT_CHANNEL3,
			BT_CHANNEL4,
			BT_CHANNEL5,
			BT_CHANNEL6,
			BT_CHANNEL7,
			BT_CHANNEL8,
			BT_CHANNEL9,
			BT_CHANNEL10,
			BT_CHANNEL11,
			BT_CHANNEL12,
			BT_CHANNEL13,
			BT_CHANNEL14,
			BT_CHANNEL15,
			BT_CHANNEL16,
			BT_CHANNEL17,
			BT_CHANNEL18,
			BT_CHANNEL19,
			BT_ENTERWORLD,
			BT_VIEWALL,
			BT_VIEWRECOMMENDED,
			BT_VIEWRECOMMENDED_SELECT,
			BT_VIEWRECOMMENDED_CANCEL,
			BT_VIEWRECOMMENDED_PREV,
			BT_VIEWRECOMMENDED_NEXT,
			BT_CHANGEREGION,
			BT_QUITGAME
		};

		enum Worlds : uint16_t
		{
			SCANIA,
			BERA,
			BROA,
			WINDIA,
			KHAINI,
			BELLOCAN,
			MARDIA,
			KRADIA,
			YELLONDE,
			DEMETHOS,
			GALICIA,
			ELNIDO,
			ZENITH,
			ARCANIA,
			CHAOS,
			NOVA,
			RENEGADES,
			AURORA,
			ELYSIUM1,
			KOREAN1 = 29,
			LUNA,
			ELYSIUM2,
			LAB = 40,
			KOREAN2 = 43,
			KOREAN3,
			REBOOT1,
			REBOOT2,
			PINKBEAN = 48,
			BURNING,
			KOREAN4,
			KOREAN5,
			TESPIA = 100
		};

		Text version;
		Text recommended_message;
		Texture worlds_background;
		Texture channels_background;
		Point<int16_t> worldsrc_pos;
		Point<int16_t> channelsrc_pos;
		ChatBalloonHorizontal chatballoon;

		uint8_t worldid;
		uint8_t recommended_worldid;
		uint8_t channelid;
		uint8_t worldcount;
		uint8_t recommended_worldcount;

		std::vector<World> worlds;
		std::vector<RecommendedWorld> recommended_worlds;
		std::vector<Texture> world_textures;
		std::vector<Texture> recommended_world_textures;
		std::vector<Texture> recommended_textures;
		std::map<uint16_t, uint16_t> world_map;

		bool world_selected;
		bool use_recommended;
		bool show_recommended;
		bool draw_chatballoon;

		nl::node worldselect;
		nl::node worldsrc;
		nl::node channelsrc;
	};
}