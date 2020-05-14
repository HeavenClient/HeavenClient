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
#include "UIQuit.h"

#include "Timer.h"
#include "../UI.h"
#include "../Window.h"

#include "../Components/MapleButton.h"

#include "../../Character/ExpTable.h"
#include "../../Gameplay/Stage.h"
#include "../../Net/Session.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UIQuit::UIQuit(const CharStats& st) : screen_adj(212, 114), stats(st)
	{
		nl::node askReward = nl::nx::ui["UIWindow6.img"]["askReward"];
		nl::node userLog = askReward["userLog"];
		nl::node exp = userLog["exp"];
		nl::node level = userLog["level"];
		nl::node time = userLog["time"];
		nl::node backgrnd = userLog["backgrnd"];

		sprites.emplace_back(backgrnd, -screen_adj);

		buttons[Buttons::NO] = std::make_unique<MapleButton>(askReward["btNo"], Point<int16_t>(0, 37));
		buttons[Buttons::YES] = std::make_unique<MapleButton>(askReward["btYes"], Point<int16_t>(0, 37));

		Stage& stage = Stage::get();

		/// Time
		int64_t uptime = stage.get_uptime() / 1000 / 1000;
		minutes = uptime / 60;
		hours = minutes / 60;

		minutes -= hours * 60;

		time_minutes = Charset(time["number"], Charset::Alignment::LEFT);
		time_minutes_pos = time["posM"];
		time_minutes_text = pad_time(minutes);

		time_hours = Charset(time["number"], Charset::Alignment::LEFT);
		time_hours_pos = time["posH"];
		time_hours_text = pad_time(hours);

		time_number_width = time["numberWidth"];

		time_lt = time["tooltip"]["lt"];
		time_rb = time["tooltip"]["rb"];

		/// Level
		levelupEffect = level["levelupEffect"];

		uplevel = stage.get_uplevel();

		levelBefore = Charset(level["number"], Charset::Alignment::LEFT);
		levelBeforePos = level["posBefore"];
		levelBeforeText = std::to_string(uplevel);

		cur_level = stats.get_stat(MapleStat::Id::LEVEL);

		levelAfter = Charset(level["number"], Charset::Alignment::LEFT);
		levelAfterPos = level["posAfter"];
		levelAfterText = std::to_string(cur_level);

		levelNumberWidth = level["numberWidth"];

		level_adj = Point<int16_t>(40, 0);

		/// Experience
		int64_t upexp = stage.get_upexp();
		float expPercentBefore = getexppercent(uplevel, upexp);
		std::string expBeforeString = std::to_string(100 * expPercentBefore);
		std::string expBeforeText = expBeforeString.substr(0, expBeforeString.find('.') + 3) + '%';

		expBefore = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::WHITE, expBeforeText);
		expBeforePos = exp["posBefore"];

		int64_t cur_exp = stats.get_exp();
		float expPercentAfter = getexppercent(cur_level, cur_exp);
		std::string expAfterString = std::to_string(100 * expPercentAfter);
		std::string expAfterText = expAfterString.substr(0, expAfterString.find('.') + 3) + '%';

		expAfter = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::ELECTRICLIME, expAfterText);
		expAfterPos = exp["posAfter"];

		exp_adj = Point<int16_t>(0, 6);

		int16_t width = Constants::Constants::get().get_viewwidth();
		int16_t height = Constants::Constants::get().get_viewheight();

		background = ColorBox(width, height, Color::Name::BLACK, 0.5f);
		position = Point<int16_t>(width / 2, height / 2);
		dimension = Texture(backgrnd).get_dimensions();
	}

	void UIQuit::draw(float inter) const
	{
		background.draw(Point<int16_t>(0, 0));

		UIElement::draw(inter);

		time_minutes.draw(time_minutes_text, time_number_width, position + time_minutes_pos - screen_adj);
		time_hours.draw(time_hours_text, time_number_width, position + time_hours_pos - screen_adj);

		levelBefore.draw(levelBeforeText, levelNumberWidth, position + levelBeforePos + level_adj - screen_adj);
		levelAfter.draw(levelAfterText, levelNumberWidth, position + levelAfterPos + level_adj - screen_adj);

		if (cur_level > uplevel)
			levelupEffect.draw(position - screen_adj, inter);

		expBefore.draw(position + expBeforePos - exp_adj - screen_adj);
		expAfter.draw(position + expAfterPos - exp_adj - screen_adj);
	}

	void UIQuit::update()
	{
		UIElement::update();

		levelupEffect.update();
	}

	Cursor::State UIQuit::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		auto lt = position + time_lt - screen_adj;
		auto rb = position + time_rb - screen_adj;

		auto bounds = Rectangle<int16_t>(lt, rb);

		if (bounds.contains(cursorpos))
			UI::get().show_text(Tooltip::Parent::TEXT,
								std::to_string(hours) + "Hour " + std::to_string(minutes) + "Minute");
		else
			UI::get().clear_tooltip(Tooltip::Parent::TEXT);

		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UIQuit::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
				close();
			else if (keycode == KeyAction::Id::RETURN)
				button_pressed(Buttons::YES);
		}
	}

	UIElement::Type UIQuit::get_type() const
	{
		return TYPE;
	}

	Button::State UIQuit::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::NO:
				deactivate();
				break;
			case Buttons::YES:
			{
				Constants::Constants::get().set_viewwidth(800);
				Constants::Constants::get().set_viewheight(600);

				float fadestep = 0.025f;

				Window::get().fadeout(
					fadestep,
					[]()
					{
						GraphicsGL::get().clear();

						UI::get().change_state(UI::State::LOGIN);
						UI::get().set_scrollnotice("");
						Session::get().reconnect();

						UI::get().enable();
						Timer::get().start();
						GraphicsGL::get().unlock();
					}
				);

				GraphicsGL::get().lock();
				Stage::get().clear();
				Timer::get().start();
			}
				break;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	std::string UIQuit::pad_time(int64_t time)
	{
		std::string ctime = std::to_string(time);
		size_t length = ctime.length();

		if (length > 2)
			return "99";

		return std::string(2 - length, '0') + ctime;
	}

	float UIQuit::getexppercent(uint16_t level, int64_t exp) const
	{
		if (level >= ExpTable::LEVELCAP)
			return 0.0f;

		return static_cast<float>(
			static_cast<double>(exp) / ExpTable::values[level]
		);
	}

	void UIQuit::close()
	{
		deactivate();

		UI::get().clear_tooltip(Tooltip::Parent::TEXT);
	}
}