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

#include "../../Character/Char.h"

namespace ms
{
	class UICharInfo : public UIDragElement<PosCHARINFO>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CHARINFO;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UICharInfo(int32_t cid);

		void draw(float inter) const override;

		void update() override;

		Button::State button_pressed(uint16_t buttonid) override;

		bool is_in_range(Point<int16_t> cursorpos) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

		void update_stats(int32_t character_id, int16_t job_id, int8_t level, int16_t fame, std::string guild,
						  std::string alliance);

	private:
		void show_bottom_window(uint16_t buttonid);

		void show_right_window(uint16_t buttonid);

		enum Buttons : uint16_t
		{
			BtClose,
			BtCollect,
			BtDamage,
			BtFamily,
			BtItem,
			BtParty,
			BtPersonality,
			BtPet,
			BtPopDown,
			BtPopUp,
			BtRide,
			BtTrad,
			BtFriend,
			BtVisit,
			BtArrayGet,
			BtArrayName,
			BtFAQ,
			BtRegist
		};

		/// Main Window
		Text name;
		Text job;
		Text level;
		Text fame;
		Text guild;
		Text alliance;

		Char* target_character;

		/// Sub Windows
		Point<int16_t> bottom_window_adj;
		Point<int16_t> right_window_adj;

		/// Farm
		Text farm_name;
		Sprite loading;
		bool is_loading;
		uint16_t timestep;
		Charset farm_level;
		Point<int16_t> farm_dim;
		Point<int16_t> farm_adj;
		std::string farm_level_text;

		/// Personality
		bool personality_enabled;
		std::vector<Sprite> personality_sprites;
		BoolPair<std::vector<Sprite>> personality_sprites_enabled;
		Point<int16_t> personality_dimensions;

		/// Collect
		bool collect_enabled;
		std::vector<Sprite> collect_sprites;
		Point<int16_t> collect_dimensions;
		Point<int16_t> collect_adj;
		Sprite default_medal;
		Text medal_text;
		Text medal_total;

		/// Damage
		bool damage_enabled;
		std::vector<Sprite> damage_sprites;
		Point<int16_t> damage_dimensions;

		/// Item
		bool item_enabled;
		std::vector<Sprite> item_sprites;
		Point<int16_t> item_dimensions;
	};
}