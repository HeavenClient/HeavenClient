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
#include "UIChat.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UIChat::UIChat() : UIDragElement<PosMAPLECHAT>()
	{
		show_weekly = Configuration::get().get_show_weekly();

		nl::node socialChatEnter = nl::nx::ui["UIWindow2.img"]["socialChatEnter"];

		nl::node backgrnd = socialChatEnter["backgrnd"];
		nl::node backgrnd4 = socialChatEnter["backgrnd4"];
		nl::node backgrnd5 = socialChatEnter["backgrnd5"];

		rank_shift = Point<int16_t>(86, 130);
		name_shift = Point<int16_t>(50, 5);

		origin_left = Texture(backgrnd4).get_origin();
		origin_right = Texture(backgrnd5).get_origin();

		origin_left = Point<int16_t>(std::abs(origin_left.x()), std::abs(origin_left.y()));
		origin_right = Point<int16_t>(std::abs(origin_right.x()), std::abs(origin_right.y()));

		sprites.emplace_back(socialChatEnter["ribbon"]);
		sprites.emplace_back(backgrnd);
		sprites.emplace_back(socialChatEnter["backgrnd2"]);
		sprites.emplace_back(socialChatEnter["backgrnd3"]);
		sprites.emplace_back(backgrnd4);
		sprites.emplace_back(backgrnd5);

		buttons[Buttons::CLOSE] = std::make_unique<MapleButton>(socialChatEnter["btX"]);
		buttons[Buttons::CHAT_DUO] = std::make_unique<MapleButton>(socialChatEnter["duoChat"]);
		buttons[Buttons::CHAT_FRIEND] = std::make_unique<MapleButton>(socialChatEnter["groupChatFrd"]);
		buttons[Buttons::CHAT_RANDOM] = std::make_unique<MapleButton>(socialChatEnter["groupChatRnd"]);

		charset = Charset(socialChatEnter["number"], Charset::Alignment::RIGHT);

		name_left = Text(Text::Font::A12B, Text::Alignment::CENTER, Color::Name::WHITE);
		name_right = Text(Text::Font::A12B, Text::Alignment::CENTER, Color::Name::WHITE);

		dimension = Texture(backgrnd).get_dimensions();

		if (show_weekly)
			UI::get().emplace<UIRank>();
	}

	void UIChat::draw(float inter) const
	{
		UIElement::draw(inter);

		charset.draw("0", position + origin_left + rank_shift);
		charset.draw("0", position + origin_right + rank_shift);

		name_left.draw(position + origin_left + name_shift);
		name_right.draw(position + origin_right + name_shift);
	}

	void UIChat::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
			close();
	}

	UIElement::Type UIChat::get_type() const
	{
		return TYPE;
	}

	Button::State UIChat::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::CLOSE:
				close();
				break;
			case Buttons::CHAT_DUO:
				break;
			case Buttons::CHAT_FRIEND:
				break;
			case Buttons::CHAT_RANDOM:
				break;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UIChat::close()
	{
		deactivate();
	}

	UIRank::UIRank() : UIDragElement<PosMAPLECHAT>()
	{
		Configuration::get().set_show_weekly(false);

		nl::node socialRank = nl::nx::ui["UIWindow2.img"]["socialRank"];

		nl::node backgrnd = socialRank["backgrnd"];
		nl::node backgrnd4 = socialRank["backgrnd4"];
		nl::node backgrnd5 = socialRank["backgrnd5"];

		rank_shift = Point<int16_t>(86, 130);
		name_shift = Point<int16_t>(52, 4);

		origin_left = Texture(backgrnd4).get_origin();
		origin_right = Texture(backgrnd5).get_origin();

		origin_left = Point<int16_t>(std::abs(origin_left.x()) - 1, std::abs(origin_left.y()));
		origin_right = Point<int16_t>(std::abs(origin_right.x()), std::abs(origin_right.y()));

		sprites.emplace_back(socialRank["ribbon"]);
		sprites.emplace_back(backgrnd);
		sprites.emplace_back(socialRank["backgrnd2"]);
		sprites.emplace_back(socialRank["backgrnd3"]);
		sprites.emplace_back(backgrnd4);
		sprites.emplace_back(backgrnd5);

		buttons[Buttons::CLOSE] = std::make_unique<MapleButton>(socialRank["btX"]);

		charset = Charset(socialRank["number"], Charset::Alignment::RIGHT);

		name_left = Text(Text::Font::A12B, Text::Alignment::CENTER, Color::Name::WHITE);
		name_right = Text(Text::Font::A12B, Text::Alignment::CENTER, Color::Name::WHITE);

		dimension = Texture(backgrnd).get_dimensions();
		position = position + Point<int16_t>(211, 124);
	}

	void UIRank::draw(float inter) const
	{
		UIElement::draw(inter);

		charset.draw("0", position + origin_left + rank_shift);
		charset.draw("0", position + origin_right + rank_shift);

		name_left.draw(position + origin_left + name_shift);
		name_right.draw(position + origin_right + name_shift);
	}

	void UIRank::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
			close();
	}

	UIElement::Type UIRank::get_type() const
	{
		return TYPE;
	}

	Button::State UIRank::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::CLOSE:
				close();
				break;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UIRank::close()
	{
		deactivate();
	}
}