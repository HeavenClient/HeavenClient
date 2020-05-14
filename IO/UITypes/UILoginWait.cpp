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
#include "UILoginWait.h"

#include "../Components/MapleButton.h"

#include "../../Net/Session.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UILoginWait::UILoginWait() : UILoginWait([]()
											 {})
	{}

	UILoginWait::UILoginWait(std::function<void()> okhandler) : okhandler(okhandler)
	{
		nl::node Loading = nl::nx::ui["Login.img"]["Notice"]["Loading"];
		nl::node backgrnd = Loading["backgrnd"];

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(Loading["circle"], Point<int16_t>(127, 70));

		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Loading["BtCancel"], Point<int16_t>(101, 106));

		position = Point<int16_t>(276, 229);
		dimension = Texture(backgrnd).get_dimensions();
	}

	UIElement::Type UILoginWait::get_type() const
	{
		return TYPE;
	}

	void UILoginWait::close()
	{
		deactivate();
		okhandler();
	}

	std::function<void()> UILoginWait::get_handler()
	{
		return okhandler;
	}

	Button::State UILoginWait::button_pressed(uint16_t id)
	{
		Session::get().reconnect();

		close();

		return Button::State::NORMAL;
	}
}