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
#include "UIRaceSelect.h"
#include "UILoginNotice.h"
#include "UICharSelect.h"
#include "UIExplorerCreation.h"
#include "UICygnusCreation.h"
#include "UIAranCreation.h"

#include "../UI.h"
#include "../Configuration.h"

#include "../Components/MapleButton.h"
#include "../Components/AreaButton.h"
#include "../Audio/Audio.h"

#include <cctype>

#include <nlnx/nx.hpp>

namespace ms
{
	UIRaceSelect::UIRaceSelect() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600))
	{
		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Common = Login["Common"];
		nl::node RaceSelect = Login["RaceSelect_new"];

		screen_adj = Point<int16_t>(0, -Constants::VIEWYOFFSET);

		Point<int16_t> make_pos = RaceSelect["make"]["pos"];
		Point<int16_t> make_posZero = RaceSelect["make"]["posZero"];

		pos = Point<int16_t>(std::abs(make_pos.x()), std::abs(make_pos.y()));
		posZero = Point<int16_t>(std::abs(make_posZero.x()), std::abs(make_posZero.y()));

		uint8_t selected_list = 1;

		order = RaceSelect["order"][selected_list];
		hotlist = RaceSelect["hotList"][selected_list];
		newlist = RaceSelect["newList"][selected_list];
		bgm = RaceSelect["bgm"];

		hotlabel = RaceSelect["hotLabel"];
		hotlabelZero = RaceSelect["hotLabel2"];
		newlabel = RaceSelect["newLabel"];
		hotbtn = RaceSelect["hot"];
		newbtn = RaceSelect["new"];

		class_index[0] = order[0];
		class_index[1] = order[1];
		class_index[2] = order[2];
		class_index[3] = order[3];
		class_index[4] = order[4];
		mouseover[0] = true;
		mouseover[1] = false;
		mouseover[2] = false;
		mouseover[3] = false;
		mouseover[4] = false;

		class_isdisabled[Classes::RESISTANCE] = true;
		class_isdisabled[Classes::EXPLORER] = false;
		class_isdisabled[Classes::CYGNUSKNIGHTS] = false;
		class_isdisabled[Classes::ARAN] = false;
		class_isdisabled[Classes::EVAN] = true;
		class_isdisabled[Classes::MERCEDES] = true;
		class_isdisabled[Classes::DEMON] = true;
		class_isdisabled[Classes::PHANTOM] = true;
		class_isdisabled[Classes::DUALBLADE] = true;
		class_isdisabled[Classes::MIHILE] = true;
		class_isdisabled[Classes::LUMINOUS] = true;
		class_isdisabled[Classes::KAISER] = true;
		class_isdisabled[Classes::ANGELICBUSTER] = true;
		class_isdisabled[Classes::CANNONEER] = true;
		class_isdisabled[Classes::XENON] = true;
		class_isdisabled[Classes::ZERO] = true;
		class_isdisabled[Classes::SHADE] = true;
		class_isdisabled[Classes::JETT] = true;
		class_isdisabled[Classes::HAYATO] = true;
		class_isdisabled[Classes::KANNA] = true;
		class_isdisabled[Classes::CHASE] = true;
		class_isdisabled[Classes::PINKBEAN] = true;
		class_isdisabled[Classes::KINESIS] = true;
		class_isdisabled[Classes::CADENA] = true;
		class_isdisabled[Classes::ILLIUM] = true;
		class_isdisabled[Classes::ARK] = true;

		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 290));

		back = RaceSelect["Back"]["1"]["0"];
		backZero = RaceSelect["Back"]["2"]["0"];
		back_ani = RaceSelect["BackAni"];
		class_details_background = RaceSelect["Back1"]["0"]["0"];
		class_details_backgroundZero = RaceSelect["Back1"]["1"]["0"];

		for (size_t i = 0; i < CLASS_COUNT; i++)
		{
			class_normal[i][false] = RaceSelect["button"][i]["normal"]["0"];
			class_normal[i][true] = RaceSelect["button"][i]["mouseOver"]["0"];

			class_disabled[i][false] = RaceSelect["buttonDisabled"][i]["normal"]["0"];
			class_disabled[i][true] = RaceSelect["buttonDisabled"][i]["mouseOver"]["0"];

			class_background[i] = RaceSelect["Back0"][i]["0"];
			class_details[i] = RaceSelect["Back2"][i]["0"];
			class_title[i] = RaceSelect["Back3"][i]["0"];
		}

		buttons[Buttons::BACK] = std::make_unique<MapleButton>(Common["BtStart"], Point<int16_t>(0, 505));
		buttons[Buttons::MAKE] = std::make_unique<MapleButton>(RaceSelect["make"]);
		buttons[Buttons::LEFT] = std::make_unique<MapleButton>(RaceSelect["leftArrow"], Point<int16_t>(41, 448));
		buttons[Buttons::RIGHT] = std::make_unique<MapleButton>(RaceSelect["rightArrow"], Point<int16_t>(718, 448));

		for (size_t i = 0; i <= Buttons::CLASS0; i++)
			buttons[Buttons::CLASS0 + i] = std::make_unique<AreaButton>(get_class_pos(i), class_normal[0][true].get_dimensions());

		index_shift = 0;
		selected_index = 0;
		selected_class = class_index[selected_index];

		buttons[Buttons::LEFT]->set_state(Button::State::DISABLED);

		Sound(Sound::Name::RACESELECT).play();
	}

	void UIRaceSelect::draw(float inter) const
	{
		if (selected_class == Classes::ZERO)
			backZero.draw(position + screen_adj);
		else
			back.draw(position + screen_adj);

		UIElement::draw_sprites(inter);

		version.draw(position + Point<int16_t>(707, -9));

		if (selected_class == Classes::KANNA || selected_class == Classes::CHASE)
		{
			if (selected_class == Classes::ZERO)
				class_details_backgroundZero.draw(position + screen_adj);
			else
				class_details_background.draw(position + Point<int16_t>(0, -Constants::VIEWYOFFSET));

			class_background[selected_class].draw(position + screen_adj);
		}
		else
		{
			class_background[selected_class].draw(position + screen_adj);

			if (selected_class == Classes::ZERO)
				class_details_backgroundZero.draw(position + screen_adj);
			else
				class_details_background.draw(position + Point<int16_t>(0, -Constants::VIEWYOFFSET));
		}

		class_details[selected_class].draw(position + screen_adj);
		class_title[selected_class].draw(position + screen_adj);

		for each (auto node in hotlist)
		{
			if (node.get_integer() == selected_class)
			{
				if (selected_class == Classes::ZERO)
					hotlabelZero.draw(position + screen_adj, inter);
				else
					hotlabel.draw(position + screen_adj, inter);

				break;
			}
		}

		for each (auto node in newlist)
		{
			if (node.get_integer() == selected_class)
			{
				newlabel.draw(position + screen_adj, inter);
				break;
			}
		}

		for (size_t i = 0; i < INDEX_COUNT; i++)
		{
			Point<int16_t> button_pos = get_class_pos(i);

			class_isdisabled[class_index[i]] ? class_disabled[class_index[i]][mouseover[i]].draw(position + button_pos) : class_normal[class_index[i]][mouseover[i]].draw(position + button_pos);

			for each (auto node in hotlist)
			{
				if (node.get_integer() == class_index[i])
				{
					hotbtn.draw(position + button_pos, inter);
					break;
				}
			}

			for each (auto node in newlist)
			{
				if (node.get_integer() == selected_class)
				{
					newbtn.draw(position + button_pos, inter);
					break;
				}
			}
		}

		UIElement::draw_buttons(inter);

		back_ani.draw(position + screen_adj, inter);
	}

	void UIRaceSelect::update()
	{
		UIElement::update();

		hotlabel.update();
		hotlabelZero.update();
		newlabel.update();
		hotbtn.update();
		newbtn.update();

		if (selected_class == Classes::ZERO)
			buttons[Buttons::MAKE]->set_position(position + posZero + screen_adj);
		else
			buttons[Buttons::MAKE]->set_position(position + pos + screen_adj);

		back_ani.update();

		for each (auto node in bgm)
		{
			uint8_t name = std::stoi(node.name());

			if (name == selected_class)
			{
				std::string found_bgm = node["bgm"];
				std::size_t found_img = found_bgm.find(".img");

				if (found_img == std::string::npos)
				{
					std::size_t found_slash = found_bgm.find('/');

					if (found_slash != std::string::npos)
					{
						found_bgm.insert(found_slash, ".img");

						Music(found_bgm).play();
					}
				}
				else
				{
					Music(found_bgm).play();
				}

				break;
			}
			else
			{
				Music("BgmUI.img/Title").play();
				break;
			}
		}
	}

	Cursor::State UIRaceSelect::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					if (btit.first >= Buttons::CLASS0)
						mouseover[btit.first - Buttons::CLASS0] = true;

					btit.second->set_state(Button::State::MOUSEOVER);
				}
				else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));
					}
					else
					{
						if (btit.first >= Buttons::CLASS0)
							mouseover[btit.first - Buttons::CLASS0] = true;
					}
				}
			}
			else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				if (btit.first >= Buttons::CLASS0)
					mouseover[btit.first - Buttons::CLASS0] = false;

				btit.second->set_state(Button::State::NORMAL);
			}
		}

		return Cursor::State::LEAF;
	}

	void UIRaceSelect::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				show_charselect();
			}
			else if (keycode == KeyAction::Id::LEFT || keycode == KeyAction::Id::DOWN)
			{
				if (buttons[Buttons::LEFT]->get_state() == Button::State::NORMAL)
					button_pressed(Buttons::LEFT);
			}
			else if (keycode == KeyAction::Id::RIGHT || keycode == KeyAction::Id::UP)
			{
				if (buttons[Buttons::RIGHT]->get_state() == Button::State::NORMAL)
					button_pressed(Buttons::RIGHT);
			}
			else if (keycode == KeyAction::Id::RETURN)
			{
				button_pressed(Buttons::MAKE);
			}
		}
	}

	bool UIRaceSelect::check_name(std::string name) const
	{
		nl::node ForbiddenName = nl::nx::etc["ForbiddenName.img"];

		for each (std::string forbiddenName in ForbiddenName)
		{
			std::string lName = to_lower(name);
			std::string fName = to_lower(forbiddenName);

			if (lName.find(fName) != std::string::npos)
				return false;
		}

		return true;
	}

	void UIRaceSelect::send_naming_result(bool nameused)
	{
		if (selected_class == Classes::EXPLORER)
		{
			if (auto explorercreation = UI::get().get_element<UIExplorerCreation>())
				explorercreation->send_naming_result(nameused);
		}
		else if (selected_class == Classes::CYGNUSKNIGHTS)
		{
			if (auto cygnuscreation = UI::get().get_element<UICygnusCreation>())
				cygnuscreation->send_naming_result(nameused);
		}
		else if (selected_class == Classes::ARAN)
		{
			if (auto arancreation = UI::get().get_element<UIAranCreation>())
				arancreation->send_naming_result(nameused);
		}
	}

	Button::State UIRaceSelect::button_pressed(uint16_t buttonid)
	{
		if (buttonid == Buttons::BACK)
		{
			show_charselect();

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::MAKE)
		{
			auto okhandler = [&]()
			{
				if (!class_isdisabled[selected_class])
				{
					Sound(Sound::Name::SCROLLUP).play();

					deactivate();

					if (selected_class == Classes::EXPLORER)
						UI::get().emplace<UIExplorerCreation>();
					else if (selected_class == Classes::CYGNUSKNIGHTS)
						UI::get().emplace<UICygnusCreation>();
					else if (selected_class == Classes::ARAN)
						UI::get().emplace<UIAranCreation>();
				}
			};

			UI::get().emplace<UIClassConfirm>(selected_class, class_isdisabled[selected_class], okhandler);

			return Button::State::NORMAL;
		}
		else if (buttonid == Buttons::LEFT)
		{
			uint8_t new_index = selected_index - 1;

			int size = sizeof(class_index) / sizeof(class_index[0]);

			int selected = class_index[selected_index - index_shift];
			auto selected_itr = std::find(class_index, class_index + size, selected);

			if (selected_itr != std::end(class_index))
			{
				auto button_index = std::distance(class_index, selected_itr);

				if (button_index == 0)
				{
					index_shift--;

					class_index[0] = order[new_index + 4 - Buttons::CLASS0];
					class_index[1] = order[new_index + 5 - Buttons::CLASS0];
					class_index[2] = order[new_index + 6 - Buttons::CLASS0];
					class_index[3] = order[new_index + 7 - Buttons::CLASS0];
					class_index[4] = order[new_index + 8 - Buttons::CLASS0];
				}
			}

			select_class(new_index);

			return Button::State::IDENTITY;
		}
		else if (buttonid == Buttons::RIGHT)
		{
			uint8_t new_index = selected_index + 1;

			int size = sizeof(class_index) / sizeof(class_index[0]);

			int selected = class_index[selected_index - index_shift];
			auto selected_itr = std::find(class_index, class_index + size, selected);

			if (selected_itr != std::end(class_index))
			{
				auto button_index = std::distance(class_index, selected_itr);

				if (button_index == 4)
				{
					index_shift++;

					class_index[0] = order[new_index + 0 - Buttons::CLASS0];
					class_index[1] = order[new_index + 1 - Buttons::CLASS0];
					class_index[2] = order[new_index + 2 - Buttons::CLASS0];
					class_index[3] = order[new_index + 3 - Buttons::CLASS0];
					class_index[4] = order[new_index + 4 - Buttons::CLASS0];
				}
			}

			select_class(new_index);

			return Button::State::IDENTITY;
		}
		else if (buttonid >= Buttons::CLASS0)
		{
			auto index = buttonid - Buttons::CLASS0 + index_shift;

			select_class(index);

			return Button::State::IDENTITY;
		}
		else
		{
			return Button::State::NORMAL;
		}
	}

	void UIRaceSelect::select_class(uint8_t index)
	{
		uint8_t previous_index = selected_index;
		selected_index = index;

		if (previous_index != selected_index)
		{
			Sound(Sound::Name::RACESELECT).play();

			int size = sizeof(class_index) / sizeof(class_index[0]);

			int previous = class_index[previous_index - index_shift];
			auto previous_itr = std::find(class_index, class_index + size, previous);

			if (previous_itr != std::end(class_index))
			{
				auto button_index = std::distance(class_index, previous_itr);

				mouseover[previous_index - index_shift] = false;
				buttons[button_index + Buttons::CLASS0]->set_state(Button::State::NORMAL);
			}

			int selected = class_index[selected_index - index_shift];
			auto selected_itr = std::find(class_index, class_index + size, selected);

			if (selected_itr != std::end(class_index))
			{
				auto button_index = std::distance(class_index, selected_itr);

				selected_class = class_index[button_index];
				mouseover[selected_index - index_shift] = true;
			}
		}
		else
		{
			button_pressed(Buttons::MAKE);
		}

		if (selected_index > 0)
			buttons[Buttons::LEFT]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::LEFT]->set_state(Button::State::DISABLED);

		if (selected_index < CLASS_COUNT - 2)
			buttons[Buttons::RIGHT]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::RIGHT]->set_state(Button::State::DISABLED);
	}

	void UIRaceSelect::show_charselect()
	{
		Sound(Sound::Name::SCROLLUP).play();

		deactivate();

		if (auto charselect = UI::get().get_element<UICharSelect>())
			charselect->makeactive();
	}

	Point<int16_t> UIRaceSelect::get_class_pos(size_t index) const
	{
		uint16_t x_adj = index * 126;

		return Point<int16_t>(95 + x_adj, 420);
	}

	std::string UIRaceSelect::to_lower(std::string value) const
	{
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		return value;
	}
}