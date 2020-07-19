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
#include "UILoginNotice.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UILoginNotice::UILoginNotice(uint16_t message, std::function<void()> okhandler, std::function<void()> cancelhandler)
		: okhandler(okhandler), cancelhandler(cancelhandler)
	{
		multiple = false;

		nl::node Notice = nl::nx::ui["Login.img"]["Notice"];
		nl::node backgrnd;

		switch (message)
		{
			case Message::NAME_IN_USE:
			case Message::ILLEGAL_NAME:
			case Message::BLOCKED_ID:
			case Message::INCORRECT_PIC:
				backgrnd = Notice["backgrnd"]["1"];
				break;
			default:
				backgrnd = Notice["backgrnd"]["0"];
				break;
		}

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(Notice["text"][message], Point<int16_t>(17, 13));

		if (message == Message::DELETE_CONFIRMATION)
		{
			multiple = true;

			buttons[Buttons::YES] = std::make_unique<MapleButton>(Notice["BtYes"], Point<int16_t>(70, 106));
			buttons[Buttons::NO] = std::make_unique<MapleButton>(Notice["BtNo"], Point<int16_t>(130, 106));
		}
		else
		{
			buttons[Buttons::YES] = std::make_unique<MapleButton>(Notice["BtYes"], Point<int16_t>(100, 106));
		}

		position = Point<int16_t>(275, 209);
		dimension = Texture(backgrnd).get_dimensions();
	}

	UILoginNotice::UILoginNotice(uint16_t message, std::function<void()> okhandler) : UILoginNotice(message, okhandler,
																									[]()
																									{})
	{}

	UILoginNotice::UILoginNotice(uint16_t message) : UILoginNotice(message, []()
	{})
	{}

	void UILoginNotice::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				if (!multiple)
					okhandler();
				else
					cancelhandler();

				deactivate();
			}
			else if (keycode == KeyAction::RETURN)
			{
				okhandler();
				deactivate();
			}
		}
	}

	UIElement::Type UILoginNotice::get_type() const
	{
		return TYPE;
	}

	Button::State UILoginNotice::button_pressed(uint16_t buttonid)
	{
		if (buttonid == Buttons::YES)
			okhandler();
		else if (buttonid == Buttons::NO)
			cancelhandler();

		deactivate();

		return Button::State::NORMAL;
	}

	UIQuitConfirm::UIQuitConfirm()
	{
		nl::node notice = nl::nx::ui["Login.img"]["Notice"];
		nl::node backgrnd = notice["backgrnd"]["0"];

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(notice["text"][UILoginNotice::Message::CONFIRM_EXIT], Point<int16_t>(17, 13));

		buttons[Buttons::BT_OK] = std::make_unique<MapleButton>(notice["BtYes"], Point<int16_t>(70, 106));
		buttons[Buttons::BT_CANCEL] = std::make_unique<MapleButton>(notice["BtNo"], Point<int16_t>(130, 106));

		position = Point<int16_t>(275, 209);
		dimension = Texture(backgrnd).get_dimensions();
	}

	void UIQuitConfirm::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				deactivate();
			}
			else if (keycode == KeyAction::RETURN)
			{
				UI::get().quit();
				deactivate();
			}
		}
	}

	UIElement::Type UIQuitConfirm::get_type() const
	{
		return TYPE;
	}

	Button::State UIQuitConfirm::button_pressed(uint16_t buttonid)
	{
		if (buttonid == BT_OK)
			UI::get().quit();

		deactivate();

		return Button::PRESSED;
	}

	UIClassConfirm::UIClassConfirm(uint16_t selected_class, bool unavailable, std::function<void()> okhandler)
		: okhandler(okhandler)
	{
		nl::node RaceSelect = nl::nx::ui["Login.img"]["RaceSelect_new"];
		nl::node type = unavailable ? RaceSelect["deny"] : RaceSelect["confirm"];
		nl::node backgrnd = type["backgrnd"];
		nl::node race = type["race"][selected_class];

		int16_t backgrnd_x = Texture(backgrnd).get_dimensions().x();
		int16_t race_x = Texture(race).get_dimensions().x();

		int16_t pos_x = (backgrnd_x - race_x) / 2;

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(race, Point<int16_t>(pos_x, 10) + (Point<int16_t>) race["origin"]);

		if (unavailable)
		{
			buttons[Buttons::OK] = std::make_unique<MapleButton>(type["BtOK"]);
		}
		else
		{
			buttons[Buttons::OK] = std::make_unique<MapleButton>(type["BtOK"], Point<int16_t>(62, 107));
			buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(type["BtCancel"], Point<int16_t>(137, 107));
		}

		position = Point<int16_t>(286, 189);
		dimension = Texture(backgrnd).get_dimensions();
	}

	Cursor::State UIClassConfirm::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					btit.second->set_state(Button::State::MOUSEOVER);
				}
				else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));
					}
				}
			}
			else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				btit.second->set_state(Button::State::NORMAL);
			}
		}

		return Cursor::State::LEAF;
	}

	void UIClassConfirm::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
				deactivate();
			else if (keycode == KeyAction::Id::RETURN)
				button_pressed(Buttons::OK);
		}
	}

	UIElement::Type UIClassConfirm::get_type() const
	{
		return TYPE;
	}

	Button::State UIClassConfirm::button_pressed(uint16_t buttonid)
	{
		deactivate();

		if (buttonid == Buttons::OK)
			okhandler();

		return Button::State::NORMAL;
	}

	UIKeySelect::UIKeySelect(std::function<void(bool)> oh, bool l) : okhandler(oh), login(l)
	{
		nl::node KeyType = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"];
		nl::node backgrnd = KeyType["backgrnd"];

		sprites.emplace_back(backgrnd);

		buttons[Buttons::CLOSE] = std::make_unique<MapleButton>(KeyType["btClose"]);
		buttons[Buttons::TYPEA] = std::make_unique<MapleButton>(KeyType["btTypeA"]);
		buttons[Buttons::TYPEB] = std::make_unique<MapleButton>(KeyType["btTypeB"], Point<int16_t>(1, 1));

		if (login)
			buttons[Buttons::CLOSE]->set_active(false);

		position = Point<int16_t>(181, 145);
		dimension = Texture(backgrnd).get_dimensions();
	}

	void UIKeySelect::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && !login)
			if (escape || keycode == KeyAction::Id::RETURN)
				deactivate();
	}

	UIElement::Type UIKeySelect::get_type() const
	{
		return TYPE;
	}

	Button::State UIKeySelect::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			default:
			case Buttons::CLOSE:
				deactivate();
				break;
			case Buttons::TYPEA:
			case Buttons::TYPEB:
			{
				bool alternate = (buttonid == Buttons::TYPEA) ? false : true;

				if (alternate)
					buttons[Buttons::TYPEA]->set_state(Button::State::DISABLED);
				else
					buttons[Buttons::TYPEB]->set_state(Button::State::DISABLED);

				auto onok = [&, alternate]()
				{
					okhandler(alternate);
					deactivate();
				};

				UI::get().emplace<UIKeyConfirm>(alternate, onok, login);
				break;
			}
		}

		return Button::State::DISABLED;
	}

	UIKeyConfirm::UIKeyConfirm(bool alternate, std::function<void()> oh, bool l) : okhandler(oh), login(l)
	{
		nl::node alert = nl::nx::ui["UIWindow2.img"]["KeyConfig"]["KeyType"]["alert"];
		nl::node background = alternate ? alert["alternate"] : alert["default"];

		sprites.emplace_back(background);

		buttons[Buttons::OK] = std::make_unique<MapleButton>(alert["btOk"]);

		position = Point<int16_t>(276, 229);
		dimension = Texture(background).get_dimensions();
	}

	void UIKeyConfirm::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (keycode == KeyAction::Id::RETURN)
			{
				confirm();
			}
			else if (!login && escape)
			{
				deactivate();

				UI::get().remove(UIElement::Type::LOGINNOTICE);
			}
		}
	}

	UIElement::Type UIKeyConfirm::get_type() const
	{
		return TYPE;
	}

	Button::State UIKeyConfirm::button_pressed(uint16_t buttonid)
	{
		confirm();

		return Button::State::NORMAL;
	}

	void UIKeyConfirm::confirm()
	{
		okhandler();
		deactivate();

		UI::get().remove(UIElement::Type::LOGINNOTICE);
	}
}