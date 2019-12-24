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

#include "../Components/TextTooltip.h"

namespace ms
{
	class UIRegion : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::REGION;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIRegion();

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void clear_tooltip();

		enum Buttons : uint16_t
		{
			NA,
			EU,
			EXIT
		};

		Rectangle<int16_t> na_rect;
		Rectangle<int16_t> eu_rect;
	};
}