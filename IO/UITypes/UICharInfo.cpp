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
#include "UICharInfo.h"

#include "../Components/MapleButton.h"

#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/PlayerInteractionPackets.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UICharInfo::UICharInfo(int32_t cid)
		: UIDragElement<PosCHARINFO>(), is_loading(true), timestep(Constants::TIMESTEP), personality_enabled(
		false), collect_enabled(false), damage_enabled(false), item_enabled(false)
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		nl::node UserInfo = nl::nx::ui["UIWindow2.img"]["UserInfo"];
		nl::node character = UserInfo["character"];
		nl::node backgrnd = character["backgrnd"];

		/// Main Window
		sprites.emplace_back(backgrnd);
		sprites.emplace_back(character["backgrnd2"]);
		sprites.emplace_back(character["name"]);

		Point<int16_t> backgrnd_dim = Texture(backgrnd).get_dimensions();
		Point<int16_t> close_dimensions = Point<int16_t>(backgrnd_dim.x() - 21, 6);

		buttons[Buttons::BtClose] = std::make_unique<MapleButton>(close, close_dimensions);
		buttons[Buttons::BtCollect] = std::make_unique<MapleButton>(character["BtCollect"]);
		buttons[Buttons::BtDamage] = std::make_unique<MapleButton>(character["BtDamage"]);
		buttons[Buttons::BtFamily] = std::make_unique<MapleButton>(character["BtFamily"]);
		buttons[Buttons::BtItem] = std::make_unique<MapleButton>(character["BtItem"]);
		buttons[Buttons::BtParty] = std::make_unique<MapleButton>(character["BtParty"]);
		buttons[Buttons::BtPersonality] = std::make_unique<MapleButton>(character["BtPersonality"]);
		buttons[Buttons::BtPet] = std::make_unique<MapleButton>(character["BtPet"]);
		buttons[Buttons::BtPopDown] = std::make_unique<MapleButton>(character["BtPopDown"]);
		buttons[Buttons::BtPopUp] = std::make_unique<MapleButton>(character["BtPopUp"]);
		buttons[Buttons::BtRide] = std::make_unique<MapleButton>(character["BtRide"]);
		buttons[Buttons::BtTrad] = std::make_unique<MapleButton>(character["BtTrad"]);

		name = Text(Text::Font::A12M, Text::Alignment::CENTER, Color::Name::WHITE);
		job = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		level = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		fame = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		guild = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		alliance = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);

		// TODO: Check if player has a mount or pet, disable if they don't
		buttons[Buttons::BtPet]->set_state(Button::State::DISABLED);
		buttons[Buttons::BtRide]->set_state(Button::State::DISABLED);

		/// Farm
		nl::node farm = UserInfo["farm"];
		nl::node farm_backgrnd = farm["backgrnd"];

		loading = farm["loading"];

		farm_dim = Texture(farm_backgrnd).get_dimensions();
		farm_adj = Point<int16_t>(-farm_dim.x(), 0);

		sprites.emplace_back(farm_backgrnd, farm_adj);
		sprites.emplace_back(farm["backgrnd2"], farm_adj);
		sprites.emplace_back(farm["default"], farm_adj);
		sprites.emplace_back(farm["cover"], farm_adj);

		buttons[Buttons::BtFriend] = std::make_unique<MapleButton>(farm["btFriend"], farm_adj);
		buttons[Buttons::BtVisit] = std::make_unique<MapleButton>(farm["btVisit"], farm_adj);

		farm_name = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::SUPERNOVA);
		farm_level = Charset(farm["number"], Charset::Alignment::LEFT);

#pragma region BottomWindow
		bottom_window_adj = Point<int16_t>(0, backgrnd_dim.y() + 1);

		/// Personality
		nl::node personality = UserInfo["personality"];
		nl::node personality_backgrnd = personality["backgrnd"];

		personality_sprites.emplace_back(personality_backgrnd, bottom_window_adj);
		personality_sprites.emplace_back(personality["backgrnd2"], bottom_window_adj);

		personality_sprites_enabled[true].emplace_back(personality["backgrnd3"], bottom_window_adj);
		personality_sprites_enabled[true].emplace_back(personality["backgrnd4"], bottom_window_adj);
		personality_sprites_enabled[true].emplace_back(personality["center"], bottom_window_adj);
		personality_sprites_enabled[false].emplace_back(personality["before30level"], bottom_window_adj);

		personality_dimensions = Texture(personality_backgrnd).get_dimensions();

		/// Collect
		nl::node collect = UserInfo["collect"];
		nl::node collect_backgrnd = collect["backgrnd"];

		collect_sprites.emplace_back(collect_backgrnd, bottom_window_adj);
		collect_sprites.emplace_back(collect["backgrnd2"], bottom_window_adj);

		default_medal = collect["icon1"];

		buttons[Buttons::BtArrayGet] = std::make_unique<MapleButton>(collect["BtArrayGet"], bottom_window_adj);
		buttons[Buttons::BtArrayName] = std::make_unique<MapleButton>(collect["BtArrayName"], bottom_window_adj);

		medal_text = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR, "Junior Adventurer");
		medal_total = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR, "2");

		collect_dimensions = Texture(collect_backgrnd).get_dimensions();

		/// Damage
		nl::node damage = UserInfo["damage"];
		nl::node damage_backgrnd = damage["backgrnd"];

		damage_sprites.emplace_back(damage_backgrnd, bottom_window_adj);
		damage_sprites.emplace_back(damage["backgrnd2"], bottom_window_adj);
		damage_sprites.emplace_back(damage["backgrnd3"], bottom_window_adj);

		buttons[Buttons::BtFAQ] = std::make_unique<MapleButton>(damage["BtFAQ"], bottom_window_adj);
		buttons[Buttons::BtRegist] = std::make_unique<MapleButton>(damage["BtRegist"], bottom_window_adj);

		damage_dimensions = Texture(damage_backgrnd).get_dimensions();
#pragma endregion

#pragma region RightWindow
		right_window_adj = Point<int16_t>(backgrnd_dim.x(), 0);

		/// Item
		nl::node item = UserInfo["item"];
		nl::node item_backgrnd = item["backgrnd"];

		item_sprites.emplace_back(item_backgrnd, right_window_adj);
		item_sprites.emplace_back(item["backgrnd2"], right_window_adj);

		item_dimensions = Texture(item_backgrnd).get_dimensions();
#pragma endregion


		dimension = backgrnd_dim;
		dragarea = Point<int16_t>(dimension.x(), 20);

		target_character = Stage::get().get_character(cid).get();

		CharInfoRequestPacket(cid).dispatch();
	}

	void UICharInfo::draw(float inter) const
	{
		UIElement::draw_sprites(inter);

		for (size_t i = 0; i < Buttons::BtArrayGet; i++)
			if (const auto button = buttons.at(i).get())
				button->draw(position);

		/// Main Window
		int16_t row_height = 18;
		Point<int16_t> text_pos = position + Point<int16_t>(153, 65);

		target_character->draw_preview(position + Point<int16_t>(63, 129), inter);

		name.draw(position + Point<int16_t>(59, 131));
		level.draw(text_pos + Point<int16_t>(0, row_height * 0));
		job.draw(text_pos + Point<int16_t>(0, row_height * 1));
		fame.draw(text_pos + Point<int16_t>(0, row_height * 2));
		guild.draw(text_pos + Point<int16_t>(0, row_height * 3) + Point<int16_t>(0, 1));
		alliance.draw(text_pos + Point<int16_t>(0, row_height * 4));

		/// Farm
		Point<int16_t> farm_pos = position + farm_adj;

		if (is_loading)
			loading.draw(farm_pos, inter);

		farm_name.draw(farm_pos + Point<int16_t>(136, 51));
		farm_level.draw(farm_level_text, farm_pos + Point<int16_t>(126, 34));

		/// Personality
		if (personality_enabled)
		{
			for (Sprite sprite : personality_sprites)
				sprite.draw(position, inter);

			bool show_personality = (target_character->get_level() >= 30);

			for (Sprite sprite : personality_sprites_enabled[show_personality])
				sprite.draw(position, inter);
		}

		/// Collect
		if (collect_enabled)
		{
			for (Sprite sprite : collect_sprites)
				sprite.draw(position, inter);

			for (size_t i = 0; i < 15; i++)
			{
				div_t div = std::div(i, 5);
				default_medal.draw(
					position + bottom_window_adj + Point<int16_t>(61, 66) + Point<int16_t>(38 * div.rem, 38 * div.quot),
					inter);
			}

			for (size_t i = Buttons::BtArrayGet; i < Buttons::BtFAQ; i++)
				if (const auto button = buttons.at(i).get())
					button->draw(position);

			Point<int16_t> text_pos = Point<int16_t>(121, 8);

			medal_text.draw(position + bottom_window_adj + text_pos);
			medal_total.draw(position + bottom_window_adj + text_pos + Point<int16_t>(0, 19));
		}

		/// Damage
		if (damage_enabled)
		{
			for (Sprite sprite : damage_sprites)
				sprite.draw(position, inter);

			for (size_t i = Buttons::BtFAQ; i < buttons.size(); i++)
				if (const auto button = buttons.at(i).get())
					button->draw(position);
		}

		/// Item
		if (item_enabled)
			for (Sprite sprite : item_sprites)
				sprite.draw(position, inter);
	}

	void UICharInfo::update()
	{
		if (timestep >= Constants::TIMESTEP * UCHAR_MAX)
		{
			is_loading = false;
		}
		else
		{
			loading.update();
			timestep += Constants::TIMESTEP;
		}
	}

	Button::State UICharInfo::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::BtClose:
				deactivate();
				return Button::State::NORMAL;
			case Buttons::BtFamily:
			case Buttons::BtParty:
				break;
			case Buttons::BtItem:
				show_right_window(buttonid);
				return Button::State::NORMAL;
			case Buttons::BtCollect:
			case Buttons::BtPersonality:
			case Buttons::BtRide:
			case Buttons::BtPet:
			case Buttons::BtDamage:
				show_bottom_window(buttonid);
				return Button::State::NORMAL;
			case Buttons::BtPopDown:
			case Buttons::BtPopUp:
			case Buttons::BtTrad:
			case Buttons::BtFriend:
			case Buttons::BtVisit:
			default:
				break;
		}

		return Button::State::DISABLED;
	}

	bool UICharInfo::is_in_range(Point<int16_t> cursorpos) const
	{
		Rectangle<int16_t> bounds = Rectangle<int16_t>(position, position + dimension);

		Rectangle<int16_t> farm_bounds = Rectangle<int16_t>(position, position + farm_dim);
		farm_bounds.shift(farm_adj);

		Rectangle<int16_t> bottom_bounds = Rectangle<int16_t>(Point<int16_t>(0, 0), Point<int16_t>(0, 0));
		Rectangle<int16_t> right_bounds = Rectangle<int16_t>(Point<int16_t>(0, 0), Point<int16_t>(0, 0));

		int16_t cur_x = cursorpos.x();
		int16_t cur_y = cursorpos.y();

		if (personality_enabled)
		{
			bottom_bounds = Rectangle<int16_t>(position, position + personality_dimensions);
			bottom_bounds.shift(bottom_window_adj);
		}

		if (collect_enabled)
		{
			bottom_bounds = Rectangle<int16_t>(position, position + collect_dimensions);
			bottom_bounds.shift(bottom_window_adj);
		}

		if (damage_enabled)
		{
			bottom_bounds = Rectangle<int16_t>(position, position + damage_dimensions);
			bottom_bounds.shift(bottom_window_adj);
		}

		if (item_enabled)
		{
			right_bounds = Rectangle<int16_t>(position, position + item_dimensions);
			right_bounds.shift(right_window_adj);
		}

		return bounds.contains(cursorpos) || farm_bounds.contains(cursorpos) || bottom_bounds.contains(cursorpos) ||
			   right_bounds.contains(cursorpos);
	}

	void UICharInfo::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
			deactivate();
	}

	UIElement::Type UICharInfo::get_type() const
	{
		return TYPE;
	}

	void
	UICharInfo::update_stats(int32_t character_id, int16_t job_id, int8_t lv, int16_t f, std::string g, std::string a)
	{
		int32_t player_id = Stage::get().get_player().get_oid();

		if (character_id == player_id)
		{
			buttons[Buttons::BtParty]->set_state(Button::State::DISABLED);
			buttons[Buttons::BtPopDown]->set_state(Button::State::DISABLED);
			buttons[Buttons::BtPopUp]->set_state(Button::State::DISABLED);
			buttons[Buttons::BtFriend]->set_state(Button::State::DISABLED);
		}

		Job character_job = Job(job_id);

		name.change_text(target_character->get_name());
		job.change_text(character_job.get_name());
		level.change_text(std::to_string(lv));
		fame.change_text(std::to_string(f));
		guild.change_text((g == "" ? "-" : g));
		alliance.change_text(a);

		farm_name.change_text("");
		farm_level_text = "1";
	}

	void UICharInfo::show_bottom_window(uint16_t buttonid)
	{
		personality_enabled = false;
		collect_enabled = false;
		damage_enabled = false;

		switch (buttonid)
		{
			case Buttons::BtPersonality:
				personality_enabled = true;
				break;
			case Buttons::BtCollect:
				collect_enabled = true;
				break;
			case Buttons::BtDamage:
				damage_enabled = true;
				break;
		}
	}

	void UICharInfo::show_right_window(uint16_t buttonid)
	{
		item_enabled = false;

		switch (buttonid)
		{
			case Buttons::BtItem:
				item_enabled = true;
				break;
		}
	}
}