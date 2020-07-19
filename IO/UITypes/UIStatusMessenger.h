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

#include "../../Graphics/Text.h"

#include <deque>

namespace ms
{
	class StatusInfo
	{
	public:
		StatusInfo(const std::string& str, Color::Name color);

		void draw(Point<int16_t> position, float alpha) const;

		bool update();

	private:
		Text text;
		Text shadow;
		Linear<float> opacity;

		// 8 seconds
		static constexpr int64_t FADE_DURATION = 8'000;
	};


	class UIStatusMessenger : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::STATUSMESSENGER;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIStatusMessenger();

		void draw(float alpha) const override;

		void update() override;

		void update_screen(int16_t new_width, int16_t new_height) override;

		UIElement::Type get_type() const override;

		void show_status(Color::Name color, const std::string& message);

	private:
		static constexpr size_t MAX_MESSAGES = 6;

		std::deque<StatusInfo> statusinfos;
	};
}