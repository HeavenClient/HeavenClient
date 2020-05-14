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

#include "../../Template/BoolPair.h"
#include "../../Util/Randomizer.h"

namespace ms
{
	// Keyboard which is used via the mouse. The game uses this for pic/pin input.
	class UISoftKey : public UIElement
	{
	public:
		using OkCallback = std::function<void(const std::string& entered)>;
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

		void deactivate();

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void show_text(std::string text, uint16_t maxwidth = 0, bool formatted = true, int16_t line_adj = 0);

		void clear_tooltip();

		void append_key(std::string key);

		Point<int16_t> keypos(uint16_t index, uint16_t row) const;

		Button::State case_pressed(uint16_t buttonid);

		std::string get_key_map_index(std::string key);

		std::string get_key_from_index(uint16_t index);

		bool check_pic();

		enum Buttons : uint16_t
		{
			BtCancel,
			BtDel,
			BtOK,
			BtShift,
			BtNum0
		};

		static constexpr size_t MIN_TEXT_LEN = 6;
		static constexpr size_t MAX_TEXT_LEN = 16;

		static constexpr uint16_t ROW_MAX = 12;

		static constexpr uint16_t ROW1_KEYS = 10;
		static constexpr uint16_t ROW2_KEYS = 10;
		static constexpr uint16_t ROW3_KEYS = 9;
		static constexpr uint16_t ROW4_KEYS = 7;

		static constexpr uint16_t ROW4_MAX = ROW4_KEYS + 1;
		static constexpr uint16_t CASE_KEYS = ROW_MAX * 2 + ROW4_MAX;

		bool highCase;
		std::string row1keys[ROW_MAX];
		std::string row2keys[ROW_MAX];
		std::string row3keys[ROW_MAX];
		std::string row4keys[ROW4_MAX];
		BoolPair<std::unique_ptr<Button>> BtCaseKeys[CASE_KEYS];

		OkCallback ok_callback;
		CancelCallback cancel_callback;

		Textfield textfield;
		Point<int16_t> textfield_pos;

		int16_t tooltip_timestep;
		TextTooltip tetooltip;
		Optional<Tooltip> tooltip;
		Point<int16_t> tooltip_pos;

		Randomizer random;

		std::map<uint16_t, std::string> KeyMap =
			{
				{0,  "A"},
				{1,  "B"},
				{2,  "C"},
				{3,  "D"},
				{4,  "E"},
				{5,  "F"},
				{6,  "G"},
				{7,  "H"},
				{8,  "I"},
				{9,  "J"},
				{10, "K"},
				{11, "L"},
				{12, "M"},
				{13, "N"},
				{14, "O"},
				{15, "P"},
				{16, "Q"},
				{17, "R"},
				{18, "S"},
				{19, "T"},
				{20, "U"},
				{21, "V"},
				{22, "W"},
				{23, "X"},
				{24, "Y"},
				{25, "Z"}
			};
	};
}