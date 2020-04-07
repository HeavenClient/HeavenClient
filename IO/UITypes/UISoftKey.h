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

#include "../Components/Textfield.h"
#include "../Components/TextTooltip.h"

#include "../../Util/Randomizer.h"

namespace ms
{
	// Keyboard which is used via the mouse. The game uses this for pic/pin input.
	class UISoftKey : public UIElement
	{
	public:
		using OkCallback = std::function<void(const std::string &entered)>;
		using CancelCallback = std::function<void()>;

		static constexpr Type TYPE = UIElement::Type::SOFTKEYBOARD;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UISoftKey(OkCallback ok_callback, CancelCallback cancel_callback, std::string tooltip_text,
				  Point<int16_t> tooltip_pos);

		UISoftKey(OkCallback ok_callback, CancelCallback cancel_callback, std::string tooltip_text);

		UISoftKey(OkCallback ok_callback, CancelCallback cancel_callback);

		UISoftKey(OkCallback ok_callback);

		void draw(float inter) const override;

		void update() override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void shufflekeys();

		void show_text(std::string text);

		void clear_tooltip();

		Point<int16_t> keypos(uint8_t num) const;

		enum Buttons : uint16_t
		{
			NEXT,
			DEL,
			CANCEL,
			OK,
			TAB0,
			TAB1,
			TAB2,
			NUM0,
			NUM1,
			NUM2,
			NUM3,
			NUM4,
			NUM5,
			NUM6,
			NUM7,
			NUM8,
			NUM9
		};

		static constexpr size_t MIN_SIZE = 6;
		static constexpr size_t MAX_SIZE = 16;
		static constexpr uint8_t NUM_KEYS = 10;

		OkCallback ok_callback;
		CancelCallback cancel_callback;
		Textfield entry;
		Randomizer random;
		TextTooltip tetooltip;
		Optional<Tooltip> tooltip;
		Point<int16_t> tooltipposition;
		int16_t timestamp;
	};
}