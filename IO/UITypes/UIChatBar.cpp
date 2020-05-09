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
#include "UIChatBar.h"

#include "../Components/MapleButton.h"

#include "../../Net/Packets/MessagingPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIChatBar::UIChatBar() : UIDragElement<PosCHAT>(Point<int16_t>(410, -5))
	{
		chatopen = Setting<Chatopen>::get().load();
		chatopen_persist = chatopen;
		chatfieldopen = false;
		chatrows = 5;
		lastpos = 0;
		rowpos = 0;
		rowmax = -1;

		nl::node chat = nl::nx::ui["StatusBar3.img"]["chat"];
		nl::node ingame = chat["ingame"];
		nl::node view = ingame["view"];
		nl::node input = ingame["input"];
		nl::node chatTarget = chat["common"]["chatTarget"];

		chatspace[0] = view["min"]["top"];
		chatspace[1] = view["min"]["center"];
		chatspace[2] = view["min"]["bottom"];
		chatspace[3] = view["drag"];

		int16_t chattop_y = getchattop(true) - 33;
		closechat = Point<int16_t>(387, 21);

		buttons[Buttons::BT_OPENCHAT] = std::make_unique<MapleButton>(view["btMax"], Point<int16_t>(391, -7));
		buttons[Buttons::BT_CLOSECHAT] = std::make_unique<MapleButton>(view["btMin"],
																	   closechat + Point<int16_t>(0, chattop_y));
		buttons[Buttons::BT_CHAT] = std::make_unique<MapleButton>(input["button:chat"], Point<int16_t>(344, -8));
		buttons[Buttons::BT_LINK] = std::make_unique<MapleButton>(input["button:itemLink"], Point<int16_t>(365, -8));
		buttons[Buttons::BT_HELP] = std::make_unique<MapleButton>(input["button:help"], Point<int16_t>(386, -8));

		buttons[chatopen ? Buttons::BT_OPENCHAT : Buttons::BT_CLOSECHAT]->set_active(false);
		buttons[Buttons::BT_CHAT]->set_active(chatopen ? true : false);
		buttons[Buttons::BT_LINK]->set_active(chatopen ? true : false);
		buttons[Buttons::BT_HELP]->set_active(chatopen ? true : false);

		chattab_x = 6;
		chattab_y = chattop_y;
		chattab_span = 54;

		for (size_t i = 0; i < ChatTab::NUM_CHATTAB; i++)
		{
			buttons[Buttons::BT_TAB_0 + i] = std::make_unique<MapleButton>(view["tab"], Point<int16_t>(
					chattab_x + (i * chattab_span), chattab_y));
			buttons[Buttons::BT_TAB_0 + i]->set_active(chatopen ? true : false);
			chattab_text[ChatTab::CHT_ALL + i] = Text(Text::Font::A12M, Text::Alignment::CENTER, Color::Name::DUSTYGRAY,
													  ChatTabText[i]);
		}

		chattab_text[ChatTab::CHT_ALL].change_color(Color::Name::WHITE);

		buttons[Buttons::BT_TAB_0 + ChatTab::NUM_CHATTAB] = std::make_unique<MapleButton>(view["btAddTab"],
																						  Point<int16_t>(chattab_x +
																										 (ChatTab::NUM_CHATTAB *
																										  chattab_span),
																										 chattab_y));
		buttons[Buttons::BT_TAB_0 + ChatTab::NUM_CHATTAB]->set_active(chatopen ? true : false);

		buttons[Buttons::BT_CHAT_TARGET] = std::make_unique<MapleButton>(chatTarget["all"], Point<int16_t>(5, -8));
		buttons[Buttons::BT_CHAT_TARGET]->set_active(chatopen ? true : false);

		chatenter = input["layer:chatEnter"];
		chatcover = input["layer:backgrnd"];

		chatfield = Textfield(Text::A11M, Text::LEFT, Color::Name::WHITE,
							  Rectangle<int16_t>(Point<int16_t>(62, -9), Point<int16_t>(330, 8)), 0);
		chatfield.set_state(chatopen ? Textfield::State::NORMAL : Textfield::State::DISABLED);

		chatfield.set_enter_callback(
				[&](std::string msg)
				{
					if (msg.size() > 0)
					{
						size_t last = msg.find_last_not_of(' ');

						if (last != std::string::npos)
						{
							msg.erase(last + 1);

							GeneralChatPacket(msg, true).dispatch();

							lastentered.push_back(msg);
							lastpos = lastentered.size();
						} else
						{
							toggle_chatfield();
						}

						chatfield.change_text("");
					} else
					{
						toggle_chatfield();
					}
				}
		);

		chatfield.set_key_callback(
				KeyAction::Id::UP,
				[&]()
				{
					if (lastpos > 0)
					{
						lastpos--;
						chatfield.change_text(lastentered[lastpos]);
					}
				}
		);

		chatfield.set_key_callback(
				KeyAction::Id::DOWN,
				[&]()
				{
					if (lastentered.size() > 0 && lastpos < lastentered.size() - 1)
					{
						lastpos++;
						chatfield.change_text(lastentered[lastpos]);
					}
				}
		);

		chatfield.set_key_callback(
				KeyAction::Id::ESCAPE,
				[&]()
				{
					toggle_chatfield(false);
				}
		);

		//int16_t slider_x = 394;
		//int16_t slider_y = -80;
		//int16_t slider_height = slider_y + 56;
		//int16_t slider_unitrows = chatrows;
		//int16_t slider_rowmax = 1;
		//slider = Slider(Slider::Type::CHATBAR, Range<int16_t>(slider_y, slider_height), slider_x, slider_unitrows, slider_rowmax, [&](bool upwards) {});

		send_chatline("[Welcome] Welcome to MapleStory!!", LineType::YELLOW);

		dimension = Point<int16_t>(410, DIMENSION_Y);

		//if (chatopen)
		//	dimension.shift_y(getchatbarheight());
	}

	void UIChatBar::draw(float inter) const
	{
		UIElement::draw_sprites(inter);

		if (chatopen)
		{
			int16_t chattop = getchattop(chatopen);

			auto pos_adj = chatfieldopen ? Point<int16_t>(0, 0) : Point<int16_t>(0, 28);

			chatspace[0].draw(position + Point<int16_t>(0, chattop) + pos_adj);

			if (chatrows > 1)
				chatspace[1].draw(
						DrawArgument(position + Point<int16_t>(0, -28) + pos_adj, Point<int16_t>(0, 28 + chattop)));

			chatspace[2].draw(position + Point<int16_t>(0, -28) + pos_adj);
			chatspace[3].draw(position + Point<int16_t>(0, -15 + chattop) + pos_adj);

			//slider.draw(position);

			int16_t yshift = chattop;

			for (size_t i = 0; i < chatrows; i++)
			{
				int16_t rowid = rowpos - i;

				if (!rowtexts.count(rowid))
					break;

				int16_t textheight = rowtexts.at(rowid).height() / CHATROWHEIGHT;

				while (textheight > 0)
				{
					yshift += CHATROWHEIGHT;
					textheight--;
				}

				rowtexts.at(rowid).draw(position + Point<int16_t>(9, getchattop(chatopen) - yshift - 21) + pos_adj);
			}
		} else
		{
			auto pos_adj = chatfieldopen ? Point<int16_t>(0, -28) : Point<int16_t>(0, 0);

			chatspace[0].draw(position + Point<int16_t>(0, -1) + pos_adj);
			chatspace[1].draw(position + Point<int16_t>(0, -1) + pos_adj);
			chatspace[2].draw(position + pos_adj);
			chatspace[3].draw(position + Point<int16_t>(0, -16) + pos_adj);

			if (rowtexts.count(rowmax))
				rowtexts.at(rowmax).draw(position + Point<int16_t>(9, -6) + pos_adj);
		}

		if (chatfieldopen)
		{
			chatcover.draw(DrawArgument(position + Point<int16_t>(0, -13), Point<int16_t>(409, 0)));
			chatenter.draw(DrawArgument(position + Point<int16_t>(0, -13), Point<int16_t>(285, 0)));
			chatfield.draw(position + Point<int16_t>(-4, -4));
		}

		UIElement::draw_buttons(inter);

		if (chatopen)
		{
			auto pos_adj = chatopen && !chatfieldopen ? Point<int16_t>(0, 28) : Point<int16_t>(0, 0);

			for (size_t i = 0; i < ChatTab::NUM_CHATTAB; i++)
				chattab_text[ChatTab::CHT_ALL + i].draw(
						position + Point<int16_t>(chattab_x + (i * chattab_span) + 25, chattab_y - 3) + pos_adj);
		}
	}

	void UIChatBar::update()
	{
		UIElement::update();

		auto pos_adj = chatopen && !chatfieldopen ? Point<int16_t>(0, 28) : Point<int16_t>(0, 0);

		for (size_t i = 0; i < ChatTab::NUM_CHATTAB; i++)
			buttons[BT_TAB_0 + i]->set_position(Point<int16_t>(chattab_x + (i * chattab_span), chattab_y) + pos_adj);

		buttons[Buttons::BT_TAB_0 + ChatTab::NUM_CHATTAB]->set_position(
				Point<int16_t>(chattab_x + (ChatTab::NUM_CHATTAB * chattab_span), chattab_y) + pos_adj);
		buttons[Buttons::BT_CLOSECHAT]->set_position(closechat + Point<int16_t>(0, chattab_y) + pos_adj);

		chatfield.update(position);

		for (auto iter : message_cooldowns)
			iter.second -= Constants::TIMESTEP;
	}

	void UIChatBar::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (keycode == KeyAction::Id::RETURN)
				toggle_chatfield();
			else if (escape)
				toggle_chatfield(false);
		}
	}

	bool UIChatBar::is_in_range(Point<int16_t> cursorpos) const
	{
		auto bounds = getbounds(dimension);
		return bounds.contains(cursorpos);
	}

	Cursor::State UIChatBar::send_cursor(bool clicking, Point<int16_t> cursorpos)
	{
		if (chatopen)
		{
			if (Cursor::State new_state = chatfield.send_cursor(cursorpos, clicking))
				return new_state;

			return check_dragtop(clicking, cursorpos);
		} else
		{
			return UIDragElement::send_cursor(clicking, cursorpos);
		}
	}

	UIElement::Type UIChatBar::get_type() const
	{
		return TYPE;
	}

	Cursor::State UIChatBar::check_dragtop(bool clicking, Point<int16_t> cursorpos)
	{
		Rectangle<int16_t> bounds = getbounds(dimension);
		Point<int16_t> bounds_lt = bounds.get_left_top();
		Point<int16_t> bounds_rb = bounds.get_right_bottom();

		int16_t chattab_height = 20;
		int16_t bounds_rb_y = bounds_rb.y();
		int16_t bounds_lt_y = bounds_lt.y() + chattab_height;

		auto chattop_rb = Point<int16_t>(bounds_rb.x() - 1, bounds_rb_y - 27);
		auto chattop = Rectangle<int16_t>(Point<int16_t>(bounds_lt.x() + 1, bounds_lt_y), chattop_rb);

		auto chattopleft = Rectangle<int16_t>(Point<int16_t>(bounds_lt.x(), bounds_lt_y),
											  Point<int16_t>(bounds_lt.x(), chattop_rb.y()));
		auto chattopright = Rectangle<int16_t>(Point<int16_t>(chattop_rb.x() + 1, bounds_lt_y),
											   Point<int16_t>(chattop_rb.x() + 1, chattop_rb.y()));
		auto chatleft = Rectangle<int16_t>(Point<int16_t>(bounds_lt.x(), bounds_lt_y),
										   Point<int16_t>(bounds_lt.x(), bounds_lt_y + bounds_rb_y));
		auto chatright = Rectangle<int16_t>(Point<int16_t>(chattop_rb.x() + 1, bounds_lt_y),
											Point<int16_t>(chattop_rb.x() + 1, bounds_lt_y + bounds_rb_y));

		bool in_chattop = chattop.contains(cursorpos);
		bool in_chattopleft = chattopleft.contains(cursorpos);
		bool in_chattopright = chattopright.contains(cursorpos);
		bool in_chatleft = chatleft.contains(cursorpos);
		bool in_chatright = chatright.contains(cursorpos);

		if (dragchattop)
		{
			if (clicking)
			{
				int16_t ydelta = cursorpos.y() - bounds_rb_y + 10;

				while (ydelta > 0 && chatrows > MINCHATROWS)
				{
					chatrows--;
					ydelta -= CHATROWHEIGHT;
				}

				while (ydelta < 0 && chatrows < MAXCHATROWS)
				{
					chatrows++;
					ydelta += CHATROWHEIGHT;
				}

				//slider.setrows(rowpos, chatrows, rowmax);
				//slider.setvertical(Range<int16_t>(0, CHATROWHEIGHT * chatrows - 14));

				chattab_y = getchattop(chatopen) - 33;
				//dimension.set_y(getchatbarheight());

				return Cursor::State::CLICKING;
			} else
			{
				dragchattop = false;
			}
		} else if (in_chattop)
		{
			if (clicking)
			{
				dragchattop = true;

				return Cursor::State::CLICKING;
			} else
			{
				return Cursor::State::CHATBARVDRAG;
			}
		} else if (in_chattopleft)
		{
			if (clicking)
			{
				//dragchattopleft = true;

				return Cursor::State::CLICKING;
			} else
			{
				return Cursor::State::CHATBARBRTLDRAG;
			}
		} else if (in_chattopright)
		{
			if (clicking)
			{
				//dragchattopright = true;

				return Cursor::State::CLICKING;
			} else
			{
				return Cursor::State::CHATBARBLTRDRAG;
			}
		} else if (in_chatleft)
		{
			if (clicking)
			{
				//dragchatleft = true;

				return Cursor::State::CLICKING;
			} else
			{
				return Cursor::State::CHATBARHDRAG;
			}
		} else if (in_chatright)
		{
			if (clicking)
			{
				//dragchatright = true;

				return Cursor::State::CLICKING;
			} else
			{
				return Cursor::State::CHATBARHDRAG;
			}
		}

		return UIDragElement::send_cursor(clicking, cursorpos);
	}

	bool UIChatBar::indragrange(Point<int16_t> cursorpos) const
	{
		auto bounds = getbounds(dragarea);

		return bounds.contains(cursorpos);
	}

	void UIChatBar::send_chatline(const std::string &line, LineType type)
	{
		rowmax++;
		rowpos = rowmax;

		//slider.setrows(rowpos, chatrows, rowmax);

		Color::Name color;

		switch (type)
		{
			case LineType::RED:
				color = Color::Name::DARKRED;
				break;
			case LineType::BLUE:
				color = Color::Name::MEDIUMBLUE;
				break;
			case LineType::YELLOW:
				color = Color::Name::YELLOW;
				break;
			default:
				color = Color::Name::WHITE;
				break;
		}

		rowtexts.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(rowmax),
				std::forward_as_tuple(Text::Font::A11M, Text::Alignment::LEFT, color, line, 480)
		);
	}

	void UIChatBar::display_message(Messages::Type line, UIChatBar::LineType type)
	{
		if (message_cooldowns[line] > 0)
			return;

		std::string message = Messages::messages[line];
		send_chatline(message, type);

		message_cooldowns[line] = MESSAGE_COOLDOWN;
	}

	void UIChatBar::toggle_chat()
	{
		chatopen_persist = !chatopen_persist;
		toggle_chat(chatopen_persist);
	}

	void UIChatBar::toggle_chat(bool chat_open)
	{
		if (!chat_open && chatopen_persist)
			return;

		chatopen = chat_open;

		if (!chatopen && chatfieldopen)
			toggle_chatfield();

		buttons[Buttons::BT_OPENCHAT]->set_active(!chat_open);
		buttons[Buttons::BT_CLOSECHAT]->set_active(chat_open);

		for (size_t i = 0; i < ChatTab::NUM_CHATTAB; i++)
			buttons[Buttons::BT_TAB_0 + i]->set_active(chat_open);

		buttons[Buttons::BT_TAB_0 + ChatTab::NUM_CHATTAB]->set_active(chat_open);
	}

	void UIChatBar::toggle_chatfield()
	{
		chatfieldopen = !chatfieldopen;
		toggle_chatfield(chatfieldopen);
	}

	void UIChatBar::toggle_chatfield(bool chatfield_open)
	{
		chatfieldopen = chatfield_open;

		toggle_chat(chatfieldopen);

		if (chatfieldopen)
		{
			buttons[Buttons::BT_CHAT]->set_active(true);
			buttons[Buttons::BT_HELP]->set_active(true);
			buttons[Buttons::BT_LINK]->set_active(true);
			buttons[Buttons::BT_CHAT_TARGET]->set_active(true);

			chatfield.set_state(Textfield::State::FOCUSED);

			//dimension.shift_y(getchatbarheight());
		} else
		{
			buttons[Buttons::BT_CHAT]->set_active(false);
			buttons[Buttons::BT_HELP]->set_active(false);
			buttons[Buttons::BT_LINK]->set_active(false);
			buttons[Buttons::BT_CHAT_TARGET]->set_active(false);

			chatfield.set_state(Textfield::State::DISABLED);
			chatfield.change_text("");

			//dimension.set_y(DIMENSION_Y);
		}
	}

	bool UIChatBar::is_chatopen()
	{
		return chatopen;
	}

	bool UIChatBar::is_chatfieldopen()
	{
		return chatfieldopen;
	}

	Button::State UIChatBar::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::BT_OPENCHAT:
			case Buttons::BT_CLOSECHAT:
				toggle_chat();
				break;
			case Buttons::BT_TAB_0:
			case Buttons::BT_TAB_1:
			case Buttons::BT_TAB_2:
			case Buttons::BT_TAB_3:
			case Buttons::BT_TAB_4:
			case Buttons::BT_TAB_5:
				for (size_t i = 0; i < ChatTab::NUM_CHATTAB; i++)
				{
					buttons[Buttons::BT_TAB_0 + i]->set_state(Button::State::NORMAL);
					chattab_text[ChatTab::CHT_ALL + i].change_color(Color::Name::DUSTYGRAY);
				}

				chattab_text[buttonid - Buttons::BT_TAB_0].change_color(Color::Name::WHITE);

				return Button::State::PRESSED;
		}

		Setting<Chatopen>::get().save(chatopen);

		return Button::State::NORMAL;
	}

	int16_t UIChatBar::getchattop(bool chat_open) const
	{
		if (chat_open)
			return getchatbarheight() * -1;
		else
			return -1;
	}

	int16_t UIChatBar::getchatbarheight() const
	{
		return 15 + chatrows * CHATROWHEIGHT;
	}

	Rectangle<int16_t> UIChatBar::getbounds(Point<int16_t> additional_area) const
	{
		int16_t screen_adj = (chatopen) ? 35 : 16;

		auto absp = position + Point<int16_t>(0, getchattop(chatopen));
		auto da = absp + additional_area;

		absp = Point<int16_t>(absp.x(), absp.y() - screen_adj);
		da = Point<int16_t>(da.x(), da.y());

		return Rectangle<int16_t>(absp, da);
	}
}