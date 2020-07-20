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
#include "UILogin.h"

#include "UILoginNotice.h"
#include "UILoginWait.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Audio/Audio.h"

#include "../../Net/Packets/LoginPackets.h"

#include <windows.h>

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UILogin::UILogin() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600)), signboard_pos(Point<int16_t>(389, 333))
	{
		LoginStartPacket().dispatch();

		std::string LoginMusicNewtro = Configuration::get().get_login_music_newtro();

		Music(LoginMusicNewtro).play();

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		nl::node map001 = nl::nx::map001["Back"]["login.img"];
		nl::node back = map001["back"];
		nl::node ani = map001["ani"];

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Title = Login["Title"];
		nl::node Common = Login["Common"];

		nl::node prettyLogo = nl::nx::mapPretty["Back"]["login.img"]["ani"]["16"];
		nl::node frame = nl::nx::mapLatest["Obj"]["login.img"]["Common"]["frame"]["2"]["0"];

		sprites.emplace_back(back["11"], Point<int16_t>(400, 300));
		sprites.emplace_back(ani["17"], Point<int16_t>(165, 276));
		sprites.emplace_back(ani["18"], Point<int16_t>(301, 245));
		sprites.emplace_back(ani["19"], Point<int16_t>(374, 200));
		sprites.emplace_back(ani["19"], Point<int16_t>(348, 161));
		sprites.emplace_back(back["35"], Point<int16_t>(399, 260));
		sprites.emplace_back(prettyLogo, Point<int16_t>(409, 144));
		sprites.emplace_back(Title["signboard"], signboard_pos);
		sprites.emplace_back(frame, Point<int16_t>(400, 300));
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));

		buttons[Buttons::BT_LOGIN] = std::make_unique<MapleButton>(Title["BtLogin"], signboard_pos + Point<int16_t>(62, -51));
		buttons[Buttons::BT_SAVEID] = std::make_unique<MapleButton>(Title["BtLoginIDSave"], signboard_pos + Point<int16_t>(-89, 5));
		buttons[Buttons::BT_IDLOST] = std::make_unique<MapleButton>(Title["BtLoginIDLost"], signboard_pos + Point<int16_t>(-17, 5));
		buttons[Buttons::BT_PASSLOST] = std::make_unique<MapleButton>(Title["BtPasswdLost"], signboard_pos + Point<int16_t>(55, 5));
		buttons[Buttons::BT_REGISTER] = std::make_unique<MapleButton>(Title["BtNew"], signboard_pos + Point<int16_t>(-101, 25));
		buttons[Buttons::BT_HOMEPAGE] = std::make_unique<MapleButton>(Title["BtHomePage"], signboard_pos + Point<int16_t>(-29, 25));
		buttons[Buttons::BT_QUIT] = std::make_unique<MapleButton>(Title["BtQuit"], signboard_pos + Point<int16_t>(43, 25));

		checkbox[false] = Title["check"]["0"];
		checkbox[true] = Title["check"]["1"];

		background = ColorBox(dimension.x(), dimension.y(), Color::Name::BLACK, 1.0f);

		Point<int16_t> textbox_pos = signboard_pos + Point<int16_t>(-96, -51);
		Point<int16_t> textbox_dim = Point<int16_t>(150, 24);
		int16_t textbox_limit = 12;

#pragma region Account
		account = Textfield(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::JAMBALAYA, Color::Name::SMALT, 0.75f, Rectangle<int16_t>(textbox_pos, textbox_pos + textbox_dim), textbox_limit);

		account.set_key_callback
		(
			KeyAction::Id::TAB, [&]
			{
				account.set_state(Textfield::State::NORMAL);
				password.set_state(Textfield::State::FOCUSED);
			}
		);

		account.set_enter_callback
		(
			[&](std::string msg)
			{
				login();
			}
		);

		accountbg = Title["ID"];
#pragma endregion

#pragma region Password
		textbox_pos.shift_y(26);

		password = Textfield(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::JAMBALAYA, Color::Name::PRUSSIANBLUE, 0.85f, Rectangle<int16_t>(textbox_pos, textbox_pos + textbox_dim), textbox_limit);

		password.set_key_callback
		(
			KeyAction::Id::TAB, [&]
			{
				account.set_state(Textfield::State::FOCUSED);
				password.set_state(Textfield::State::NORMAL);
			}
		);

		password.set_enter_callback
		(
			[&](std::string msg)
			{
				login();
			}
		);

		password.set_cryptchar('*');
		passwordbg = Title["PW"];
#pragma endregion

		saveid = Setting<SaveLogin>::get().load();

		if (saveid)
		{
			account.change_text(Setting<DefaultAccount>::get().load());
			password.set_state(Textfield::State::FOCUSED);
		}
		else
		{
			account.set_state(Textfield::State::FOCUSED);
		}
	}

	void UILogin::draw(float alpha) const
	{
		background.draw(position + Point<int16_t>(0, 7));

		UIElement::draw(alpha);

		version.draw(position + Point<int16_t>(707, 4));
		account.draw(position + Point<int16_t>(1, 0));
		password.draw(position + Point<int16_t>(1, 3));

		if (account.get_state() == Textfield::State::NORMAL && account.empty())
			accountbg.draw(position + signboard_pos + Point<int16_t>(-101, -51));

		if (password.get_state() == Textfield::State::NORMAL && password.empty())
			passwordbg.draw(position + signboard_pos + Point<int16_t>(-101, -25));

		checkbox[saveid].draw(position + signboard_pos + Point<int16_t>(-101, 7));
	}

	void UILogin::update()
	{
		if (Configuration::get().get_auto_login())
		{
			auto loginwait = UI::get().get_element<UILoginWait>();

			if (!loginwait || !loginwait->is_active()) {
				UI::get().emplace<UILoginWait>([]() {});

				LoginPacket(
					Configuration::get().get_auto_acc(),
					Configuration::get().get_auto_pass()
				).dispatch();
			}
		}
		UIElement::update();

		account.update(position);
		password.update(position);
	}

	void UILogin::login()
	{
		account.set_state(Textfield::State::DISABLED);
		password.set_state(Textfield::State::DISABLED);

		std::string account_text = account.get_text();
		std::string password_text = password.get_text();

		std::function<void()> okhandler = [&, password_text]()
		{
			account.set_state(Textfield::State::NORMAL);
			password.set_state(Textfield::State::NORMAL);

			if (!password_text.empty())
				password.set_state(Textfield::State::FOCUSED);
			else
				account.set_state(Textfield::State::FOCUSED);
		};

		if (account_text.empty())
		{
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::NOT_REGISTERED, okhandler);
			return;
		}

		if (password_text.length() <= 4)
		{
			UI::get().emplace<UILoginNotice>(UILoginNotice::Message::WRONG_PASSWORD, okhandler);
			return;
		}

		UI::get().emplace<UILoginWait>(okhandler);

		auto loginwait = UI::get().get_element<UILoginWait>();

		if (loginwait && loginwait->is_active())
			LoginPacket(account_text, password_text).dispatch();
	}

	void UILogin::open_url(uint16_t id)
	{
		std::string url;

		switch (id)
		{
			case Buttons::BT_REGISTER:
				url = Configuration::get().get_joinlink();
				break;
			case Buttons::BT_HOMEPAGE:
				url = Configuration::get().get_website();
				break;
			case Buttons::BT_PASSLOST:
				url = Configuration::get().get_findpass();
				break;
			case Buttons::BT_IDLOST:
				url = Configuration::get().get_findid();
				break;
			default:
				return;
		}

		ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}

	Button::State UILogin::button_pressed(uint16_t id)
	{
		switch (id)
		{
			case Buttons::BT_LOGIN:
			{
				login();

				return Button::State::NORMAL;
			}
			case Buttons::BT_REGISTER:
			case Buttons::BT_HOMEPAGE:
			case Buttons::BT_PASSLOST:
			case Buttons::BT_IDLOST:
			{
				open_url(id);

				return Button::State::NORMAL;
			}
			case Buttons::BT_SAVEID:
			{
				saveid = !saveid;

				Setting<SaveLogin>::get().save(saveid);

				return Button::State::MOUSEOVER;
			}
			case Buttons::BT_QUIT:
			{
				UI::get().quit();

				return Button::State::PRESSED;
			}
			default:
			{
				return Button::State::DISABLED;
			}
		}
	}

	Cursor::State UILogin::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (Cursor::State new_state = account.send_cursor(cursorpos, clicked))
			return new_state;

		if (Cursor::State new_state = password.send_cursor(cursorpos, clicked))
			return new_state;

		return UIElement::send_cursor(clicked, cursorpos);
	}

	UIElement::Type UILogin::get_type() const
	{
		return TYPE;
	}
}