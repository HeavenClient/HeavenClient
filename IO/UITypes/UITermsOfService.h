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

#include "../Components/Slider.h"

#include "../../Graphics/Text.h"

namespace ms
{
	class UITermsOfService : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::TOS;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UITermsOfService(std::function<void()> okhandler);

		void draw(float inter) const override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void update_accept(uint16_t offset);

		enum Buttons : uint16_t
		{
			OK,
			CANCEL
		};

		Text text;
		Slider slider;
		std::function<void()> okhandler;
		uint16_t offset;
		int16_t unit_rows;
		int16_t max_rows;
	};
}