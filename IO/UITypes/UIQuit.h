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

#include "IO/UIElement.h"

#include "IO/Components/Charset.h"
#include "Character/CharStats.h"
#include "Graphics/Text.h"
#include "Graphics/Geometry.h"

namespace ms
{
	class UIQuit : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::QUIT;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UIQuit(const CharStats& stats);

		void draw(float inter) const override;

		void update() override;

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		const CharStats& stats;

		std::string pad_time(int64_t time);

		float getexppercent(uint16_t level, int64_t exp) const;

		void close();

		enum Buttons : uint16_t
		{
			NO,
			YES
		};

		Point<int16_t> screen_adj;
		ColorBox background;

		/// Time
		int64_t minutes;
		int64_t hours;

		Charset time_minutes;
		Point<int16_t> time_minutes_pos;
		std::string time_minutes_text;

		Charset time_hours;
		Point<int16_t> time_hours_pos;
		std::string time_hours_text;

		int64_t time_number_width;

		Point<int16_t> time_lt;
		Point<int16_t> time_rb;

		/// Level
		Sprite levelupEffect;

		uint16_t uplevel;

		Charset levelBefore;
		Point<int16_t> levelBeforePos;
		std::string levelBeforeText;

		uint16_t cur_level;

		Charset levelAfter;
		Point<int16_t> levelAfterPos;
		std::string levelAfterText;

		int64_t levelNumberWidth;
		Point<int16_t> level_adj;

		/// Experience
		Text expBefore;
		Point<int16_t> expBeforePos;

		Text expAfter;
		Point<int16_t> expAfterPos;

		Point<int16_t> exp_adj;
	};
}