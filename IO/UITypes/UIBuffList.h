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

#include "../Components/IconCover.h"

namespace ms
{
	class BuffIcon
	{
	public:
		BuffIcon(int32_t buff, int32_t dur);

		void draw(Point<int16_t> position, float alpha) const;

		bool update();

	private:
		static const uint16_t FLASH_TIME = 3'000;

		Texture icon;
		IconCover cover;
		int32_t buffid;
		int32_t duration;
		Linear<float> opacity;
		float opcstep;
	};


	class UIBuffList : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::BUFFLIST;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIBuffList();

		void draw(float inter) const override;

		void update() override;

		void update_screen(int16_t new_width, int16_t new_height) override;

		Cursor::State send_cursor(bool pressed, Point<int16_t> position) override;

		UIElement::Type get_type() const override;

		void add_buff(int32_t buffid, int32_t duration);

	private:
		std::unordered_map<int32_t, BuffIcon> icons;
	};
}