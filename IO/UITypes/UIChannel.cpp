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
#include "UIChannel.h"

#include "../KeyAction.h"

#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIChannel::UIChannel() : UIDragElement<PosCHANNEL>()
	{
		uint8_t selected_world = 1; // TODO: Need to get current world user is on
		current_channel = 9; // TODO: Need to get current channel user is on
		selected_channel = current_channel;
		channel_count = 20; // TODO: Need to get total number of channels on world

		nl::node Channel = nl::nx::ui["UIWindow2.img"]["Channel"];

		nl::node backgrnd = Channel["backgrnd"];
		Texture bg = backgrnd;

		sprites.emplace_back(backgrnd, Point<int16_t>(1, 0));
		sprites.emplace_back(Channel["backgrnd2"]);
		sprites.emplace_back(Channel["backgrnd3"]);
		sprites.emplace_back(Channel["world"][selected_world], Point<int16_t>(16, 30));

		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Channel["BtCancel"]);
		buttons[Buttons::CHANGE] = std::make_unique<MapleButton>(Channel["BtChange"], Point<int16_t>(-20, 0));

		channel[true] = Channel["channel1"];
		channel[false] = Channel["channel0"];

		size_t x = 0;
		size_t y = 0;

		for (size_t i = 0; i < channel_count; i++)
		{
			if (x >= 5)
			{
				x = 0;
				y++;
			}

			ch.emplace_back(Channel["ch"][i], Point<int16_t>(19 + 70 * x, 60 + 20 * y));
			buttons[Buttons::CH + i] = std::make_unique<AreaButton>(Point<int16_t>(11 + 70 * x, 55 + 20 * y),
																	channel[true].get_dimensions());

			if (i == selected_channel)
			{
				current_channel_x = 11 + 70 * x;
				current_channel_y = 55 + 20 * y;
				selected_channel_x = current_channel_x;
				selected_channel_y = current_channel_y;
			}

			x++;
		}

		dimension = bg.get_dimensions();
		dragarea = Point<int16_t>(dimension.x(), 20);
	}

	void UIChannel::draw(float inter) const
	{
		UIElement::draw(inter);

		if (current_channel == selected_channel)
		{
			channel[true].draw(DrawArgument(position.x() + selected_channel_x, position.y() + selected_channel_y));
		} else
		{
			channel[true].draw(DrawArgument(position.x() + selected_channel_x, position.y() + selected_channel_y));
			channel[false].draw(DrawArgument(position.x() + current_channel_x, position.y() + current_channel_y));
		}


		for (auto &sprite : ch)
			sprite.draw(position, inter);
	}

	void UIChannel::update()
	{
		UIElement::update();

		for (auto &sprite : ch)
			sprite.update();
	}

	void UIChannel::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				cancel();
			} else if (keycode == KeyAction::Id::RETURN)
			{
				change_channel();
			} else if (keycode == KeyAction::Id::UP)
			{
				if (selected_channel > 4)
				{
					selected_channel -= 5;
				} else
				{
					for (size_t i = 0; i < 3; i++)
						selected_channel += 5;
				}

				if (selected_channel == current_channel)
				{
					if (selected_channel > 4)
					{
						selected_channel -= 5;
					} else
					{
						for (size_t i = 0; i < 3; i++)
							selected_channel += 5;
					}
				}

				update_selected_channel_position();
			} else if (keycode == KeyAction::Id::DOWN)
			{
				if (selected_channel < 15)
				{
					selected_channel += 5;
				} else
				{
					for (size_t i = 0; i < 3; i++)
						selected_channel -= 5;
				}

				if (selected_channel == current_channel)
				{
					if (selected_channel < 15)
					{
						selected_channel += 5;
					} else
					{
						for (size_t i = 0; i < 3; i++)
							selected_channel -= 5;
					}
				}

				update_selected_channel_position();
			} else if (keycode == KeyAction::Id::LEFT)
			{
				if (selected_channel != 0)
					selected_channel--;
				else
					selected_channel = channel_count - 1;

				if (selected_channel == current_channel)
				{
					if (selected_channel != 0)
						selected_channel--;
					else
						selected_channel = channel_count - 1;
				}

				update_selected_channel_position();
			} else if (keycode == KeyAction::Id::RIGHT)
			{
				if (selected_channel != channel_count - 1)
					selected_channel++;
				else
					selected_channel = 0;

				if (selected_channel == current_channel)
				{
					if (selected_channel != channel_count - 1)
						selected_channel++;
					else
						selected_channel = 0;
				}

				update_selected_channel_position();
			}
		}
	}

	Cursor::State UIChannel::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(clicked, cursorpos);

		if (dragged)
			return dstate;

		Cursor::State ret = clicked ? Cursor::State::CLICKING : Cursor::State::IDLE;

		for (size_t i = 0; i < channel_count + Buttons::CH; i++)
		{
			if (buttons[i]->is_active() && buttons[i]->bounds(position).contains(cursorpos))
			{
				if (buttons[i]->get_state() == Button::State::NORMAL)
				{
					if (i < Buttons::CH)
					{
						Sound(Sound::Name::BUTTONOVER).play();

						buttons[i]->set_state(Button::State::MOUSEOVER);
						ret = Cursor::State::CANCLICK;
					} else
					{
						buttons[i]->set_state(Button::State::MOUSEOVER);
						ret = Cursor::State::IDLE;
					}
				} else if (buttons[i]->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						if (i < Buttons::CH)
							Sound(Sound::Name::BUTTONCLICK).play();

						buttons[i]->set_state(button_pressed(i));

						ret = Cursor::State::IDLE;
					} else
					{
						if (i < Buttons::CH)
							ret = Cursor::State::CANCLICK;
						else
							ret = Cursor::State::IDLE;
					}
				}
			} else if (buttons[i]->get_state() == Button::State::MOUSEOVER)
			{
				buttons[i]->set_state(Button::State::NORMAL);
			}
		}

		return ret;
	}

	UIElement::Type UIChannel::get_type() const
	{
		return TYPE;
	}

	Button::State UIChannel::button_pressed(uint16_t buttonid)
	{
		if (buttonid < Buttons::CH)
		{
			switch (buttonid)
			{
				case Buttons::CANCEL:
					cancel();
					break;
				case Buttons::CHANGE:
					change_channel();
					break;
				default:
					break;
			}
		} else
		{
			if (buttonid - Buttons::CH == current_channel)
				return Button::State::NORMAL;

			selected_channel = buttonid - Buttons::CH;
			update_selected_channel_position();
		}

		return Button::State::NORMAL;
	}

	void UIChannel::cancel()
	{
		deactivate();

		current_channel = 9; // TODO: Need to get current channel user is on
		selected_channel = current_channel;
		selected_channel_x = current_channel_x;
		selected_channel_y = current_channel_y;
	}

	void UIChannel::change_channel()
	{
		// TODO: Send packet to change channel?
		cancel();
	}

	void UIChannel::update_selected_channel_position()
	{
		size_t x = 0;
		size_t y = 0;

		for (size_t i = 0; i < channel_count; i++)
		{
			if (x >= 5)
			{
				x = 0;
				y++;
			}

			if (i == selected_channel)
			{
				selected_channel_x = 11 + 70 * x;
				selected_channel_y = 55 + 20 * y;
				break;
			}

			x++;
		}
	}
}