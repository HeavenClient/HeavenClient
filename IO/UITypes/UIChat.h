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

#include "../UIDragElement.h"

#include "../Components/Charset.h"
#include "../Graphics/Text.h"

namespace ms
{
	class UIChat : public UIDragElement<PosMAPLECHAT>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CHAT;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIChat();

		void draw(float inter) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void close();

		enum Buttons : uint16_t
		{
			CLOSE,
			CHAT_DUO,
			CHAT_FRIEND,
			CHAT_RANDOM
		};

		bool show_weekly;
		Point<int16_t> rank_shift;
		Point<int16_t> name_shift;
		Point<int16_t> origin_left;
		Point<int16_t> origin_right;
		Charset charset;
		Text name_left;
		Text name_right;
	};

	class UIRank : public UIDragElement<PosMAPLECHAT>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::CHATRANK;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIRank();

		void draw(float inter) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void close();

		enum Buttons : uint16_t
		{
			CLOSE
		};

		Point<int16_t> rank_shift;
		Point<int16_t> name_shift;
		Point<int16_t> origin_left;
		Point<int16_t> origin_right;
		Charset charset;
		Text name_left;
		Text name_right;
	};
}