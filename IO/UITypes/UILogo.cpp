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
#include "UILogo.h"

#include "UILogin.h"

#include "../../Configuration.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UILogo::UILogo() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600))
	{
		Music("BgmUI.img/NxLogo").play_once();

		nexon_ended = false;
		wizet_ended = false;
		user_clicked = false;

		nl::node Logo = nl::nx::ui["Logo.img"];

		Nexon = Logo["Nexon"];
		Wizet = Logo["Wizet"];
		WizetEnd = Logo["Wizet"]["40"];
	}

	void UILogo::draw(float inter) const
	{
		if (!user_clicked)
		{
			if (!nexon_ended)
			{
				Nexon.draw(position + Point<int16_t>(440, 360), inter);
			}
			else
			{
				if (!wizet_ended)
					Wizet.draw(position + Point<int16_t>(263, 195), inter);
				else
					WizetEnd.draw(position + Point<int16_t>(263, 195));
			}
		}
		else
		{
			WizetEnd.draw(position + Point<int16_t>(263, 195));
		}
	}

	void UILogo::update()
	{
		if (!nexon_ended)
		{
			nexon_ended = Nexon.update();
		}
		else
		{
			if (!wizet_ended)
			{
				wizet_ended = Wizet.update();
			}
			else
			{
				Configuration::get().set_start_shown(true);

				UI::get().remove(UIElement::Type::START);
				UI::get().emplace<UILogin>();
			}
		}
	}

	Cursor::State UILogo::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Cursor::State ret = clicked ? Cursor::State::CLICKING : Cursor::State::IDLE;

		if (clicked && !user_clicked)
			user_clicked = true;

		return ret;
	}

	UIElement::Type UILogo::get_type() const
	{
		return TYPE;
	}
}