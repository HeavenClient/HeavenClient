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
#include "UIStatsinfo.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../UITypes/UINotice.h"
#include "../Character/Player.h"
#include "../Gameplay/Stage.h"

#include "../Net/Packets/PlayerPackets.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIStatsinfo::UIStatsinfo(const CharStats& st) : UIDragElement<PosSTATS>(Point<int16_t>(212, 20)), stats(st)
	{
		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		nl::node Stat = nl::nx::ui["UIWindow4.img"]["Stat"];
		nl::node main = Stat["main"];
		nl::node detail = Stat["detail"];
		nl::node abilityTitle = detail["abilityTitle"];
		nl::node metierLine = detail["metierLine"];

		sprites.emplace_back(main["backgrnd"]);
		sprites.emplace_back(main["backgrnd2"]);
		sprites.emplace_back(main["backgrnd3"]);

		textures_detail.emplace_back(detail["backgrnd"]);
		textures_detail.emplace_back(detail["backgrnd2"]);
		textures_detail.emplace_back(detail["backgrnd3"]);
		textures_detail.emplace_back(detail["backgrnd4"]);

		abilities[Ability::RARE] = abilityTitle["rare"]["0"];
		abilities[Ability::EPIC] = abilityTitle["epic"]["0"];
		abilities[Ability::UNIQUE] = abilityTitle["unique"]["0"];
		abilities[Ability::LEGENDARY] = abilityTitle["legendary"]["0"];
		abilities[Ability::NONE] = abilityTitle["normal"]["0"];

		inner_ability[true] = metierLine["activated"]["0"];
		inner_ability[false] = metierLine["disabled"]["0"];

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(190, 6));
		buttons[Buttons::BT_HP] = std::make_unique<MapleButton>(main["BtHpUp"]);
		buttons[Buttons::BT_MP] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 18));		// TODO: "BtMpUp" not Working
		buttons[Buttons::BT_STR] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 87));	// TODO: "BtStrUp" not working
		buttons[Buttons::BT_DEX] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 105));	// TODO: "BtDexUp" not working
		buttons[Buttons::BT_INT] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 123));	// TODO: "BtIntUp" not working
		buttons[Buttons::BT_LUK] = std::make_unique<MapleButton>(main["BtHpUp"], Point<int16_t>(0, 141));	// TODO: "BtLukUp" not working
		buttons[Buttons::BT_AUTO] = std::make_unique<MapleButton>(main["BtAuto"]);
		buttons[Buttons::BT_HYPERSTATOPEN] = std::make_unique<MapleButton>(main["BtHyperStatOpen"]);
		buttons[Buttons::BT_HYPERSTATCLOSE] = std::make_unique<MapleButton>(main["BtHyperStatClose"]);
		buttons[Buttons::BT_DETAILOPEN] = std::make_unique<MapleButton>(main["BtDetailOpen"]);
		buttons[Buttons::BT_DETAILCLOSE] = std::make_unique<MapleButton>(main["BtDetailClose"]);
		buttons[Buttons::BT_ABILITY] = std::make_unique<MapleButton>(detail["BtAbility"], Point<int16_t>(212, 0));
		buttons[Buttons::BT_DETAIL_DETAILCLOSE] = std::make_unique<MapleButton>(detail["BtHpUp"], Point<int16_t>(212, 0));

		buttons[Buttons::BT_HYPERSTATOPEN]->set_active(false);
		buttons[Buttons::BT_DETAILCLOSE]->set_active(false);
		buttons[Buttons::BT_ABILITY]->set_active(false);
		buttons[Buttons::BT_ABILITY]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_DETAIL_DETAILCLOSE]->set_active(false);

		update_ap();

		// Normal
		for (size_t i = StatLabel::NAME; i <= LUK; i++)
			statlabels[i] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);

		statlabels[StatLabel::AP] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);

		statoffsets[StatLabel::NAME] = Point<int16_t>(73, 26);
		statoffsets[StatLabel::JOB] = Point<int16_t>(74, 44);
		statoffsets[StatLabel::GUILD] = Point<int16_t>(74, 63);
		statoffsets[StatLabel::FAME] = Point<int16_t>(74, 80);
		statoffsets[StatLabel::DAMAGE] = Point<int16_t>(74, 98);
		statoffsets[StatLabel::HP] = Point<int16_t>(74, 116);
		statoffsets[StatLabel::MP] = Point<int16_t>(74, 134);
		statoffsets[StatLabel::AP] = Point<int16_t>(91, 175);
		statoffsets[StatLabel::STR] = Point<int16_t>(73, 204);
		statoffsets[StatLabel::DEX] = Point<int16_t>(73, 222);
		statoffsets[StatLabel::INT] = Point<int16_t>(73, 240);
		statoffsets[StatLabel::LUK] = Point<int16_t>(73, 258);

		// Detailed
		statlabels[StatLabel::DAMAGE_DETAILED] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		statlabels[StatLabel::DAMAGE_BONUS] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::BOSS_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::FINAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::IGNORE_DEFENSE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::CRITICAL_RATE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::CRITICAL_DAMAGE] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		statlabels[StatLabel::STATUS_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::KNOCKBACK_RESISTANCE] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::DEFENSE] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);
		statlabels[StatLabel::SPEED] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::JUMP] = Text(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::EMPEROR);
		statlabels[StatLabel::HONOR] = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::EMPEROR);

		statoffsets[StatLabel::DAMAGE_DETAILED] = Point<int16_t>(73, 38);
		statoffsets[StatLabel::DAMAGE_BONUS] = Point<int16_t>(100, 56);
		statoffsets[StatLabel::BOSS_DAMAGE] = Point<int16_t>(196, 56);
		statoffsets[StatLabel::FINAL_DAMAGE] = Point<int16_t>(100, 74);
		statoffsets[StatLabel::IGNORE_DEFENSE] = Point<int16_t>(196, 74);
		statoffsets[StatLabel::CRITICAL_RATE] = Point<int16_t>(100, 92);
		statoffsets[StatLabel::CRITICAL_DAMAGE] = Point<int16_t>(73, 110);
		statoffsets[StatLabel::STATUS_RESISTANCE] = Point<int16_t>(100, 128);
		statoffsets[StatLabel::KNOCKBACK_RESISTANCE] = Point<int16_t>(196, 128);
		statoffsets[StatLabel::DEFENSE] = Point<int16_t>(73, 146);
		statoffsets[StatLabel::SPEED] = Point<int16_t>(100, 164);
		statoffsets[StatLabel::JUMP] = Point<int16_t>(196, 164);
		statoffsets[StatLabel::HONOR] = Point<int16_t>(73, 283);

		update_all_stats();
		update_stat(Maplestat::Id::JOB);
		update_stat(Maplestat::Id::FAME);

		dimension = Point<int16_t>(212, 318);
		showdetail = false;
	}

	void UIStatsinfo::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		if (showdetail)
		{
			Point<int16_t> detail_pos(position + Point<int16_t>(212, 0));

			textures_detail[0].draw(detail_pos + Point<int16_t>(0, -1));
			textures_detail[1].draw(detail_pos);
			textures_detail[2].draw(detail_pos);
			textures_detail[3].draw(detail_pos);

			abilities[Ability::NONE].draw(DrawArgument(detail_pos));

			inner_ability[false].draw(detail_pos);
			inner_ability[false].draw(detail_pos + Point<int16_t>(0, 19));
			inner_ability[false].draw(detail_pos + Point<int16_t>(0, 38));
		}

		size_t last = showdetail ? StatLabel::NUM_LABELS : StatLabel::NUM_NORMAL;

		for (size_t i = 0; i < last; i++)
		{
			Point<int16_t> labelpos = position + statoffsets[i];

			if (i >= StatLabel::NUM_NORMAL)
				labelpos.shift_x(213);

			statlabels[i].draw(labelpos);
		}

		UIElement::draw_buttons(alpha);
	}

	void UIStatsinfo::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed && escape)
			active = false;
	}

	bool UIStatsinfo::is_in_range(Point<int16_t> cursorpos) const
	{
		Point<int16_t> pos_adj;

		if (showdetail)
			pos_adj = Point<int16_t>(211, 25);
		else
			pos_adj = Point<int16_t>(0, 0);

		auto bounds = Rectangle<int16_t>(position, position + dimension + pos_adj);
		return bounds.contains(cursorpos);
	}

	void UIStatsinfo::update_all_stats()
	{
		update_simple(AP, Maplestat::Id::AP);

		if (hasap ^ (stats.get_stat(Maplestat::Id::AP) > 0))
			update_ap();

		statlabels[StatLabel::NAME].change_text(stats.get_name());
		statlabels[StatLabel::GUILD].change_text("-");
		statlabels[StatLabel::HP].change_text(std::to_string(stats.get_stat(Maplestat::Id::HP)) + " / " + std::to_string(stats.get_total(Equipstat::Id::HP)));
		statlabels[StatLabel::MP].change_text(std::to_string(stats.get_stat(Maplestat::Id::MP)) + " / " + std::to_string(stats.get_total(Equipstat::Id::MP)));

		update_basevstotal(StatLabel::STR, Maplestat::Id::STR, Equipstat::Id::STR);
		update_basevstotal(StatLabel::DEX, Maplestat::Id::DEX, Equipstat::Id::DEX);
		update_basevstotal(StatLabel::INT, Maplestat::Id::INT, Equipstat::Id::INT);
		update_basevstotal(StatLabel::LUK, Maplestat::Id::LUK, Equipstat::Id::LUK);

		statlabels[StatLabel::DAMAGE].change_text(std::to_string(stats.get_mindamage()) + " ~ " + std::to_string(stats.get_maxdamage()));

		if (stats.is_damage_buffed())
			statlabels[StatLabel::DAMAGE].change_color(Color::Name::RED);
		else
			statlabels[StatLabel::DAMAGE].change_color(Color::Name::EMPEROR);

		statlabels[StatLabel::DAMAGE_DETAILED].change_text(std::to_string(stats.get_mindamage()) + " ~ " + std::to_string(stats.get_maxdamage()));
		statlabels[StatLabel::DAMAGE_BONUS].change_text("0%");
		statlabels[StatLabel::BOSS_DAMAGE].change_text(std::to_string(static_cast<int32_t>(stats.get_bossdmg() * 100)) + "%");
		statlabels[StatLabel::FINAL_DAMAGE].change_text("0%");
		statlabels[StatLabel::IGNORE_DEFENSE].change_text(std::to_string(static_cast<int32_t>(stats.get_ignoredef())) + "%");
		statlabels[StatLabel::CRITICAL_RATE].change_text(std::to_string(static_cast<int32_t>(stats.get_critical() * 100)) + "%");
		statlabels[StatLabel::CRITICAL_DAMAGE].change_text("0.00%");
		statlabels[StatLabel::STATUS_RESISTANCE].change_text(std::to_string(static_cast<int32_t>(stats.get_resistance())));
		statlabels[StatLabel::KNOCKBACK_RESISTANCE].change_text("0%");

		update_buffed(StatLabel::DEFENSE, Equipstat::Id::WDEF);

		statlabels[StatLabel::SPEED].change_text(std::to_string(stats.get_total(Equipstat::Id::SPEED)) + "%");
		statlabels[StatLabel::JUMP].change_text(std::to_string(stats.get_total(Equipstat::Id::JUMP)) + "%");
		statlabels[StatLabel::HONOR].change_text(std::to_string(stats.get_honor()));
	}

	void UIStatsinfo::update_stat(Maplestat::Id stat)
	{
		switch (stat)
		{
		case Maplestat::Id::JOB:
			statlabels[StatLabel::JOB].change_text(stats.get_jobname());
			break;
		case Maplestat::Id::FAME:
			update_simple(StatLabel::FAME, Maplestat::Id::FAME);
			break;
		}
	}

	Button::State UIStatsinfo::button_pressed(uint16_t id)
	{
		const Player& player = Stage::get().get_player();

		switch (id)
		{
		case Buttons::BT_CLOSE:
			deactivate();
			break;
		case Buttons::BT_HP:
			send_apup(Maplestat::Id::HP);
			break;
		case Buttons::BT_MP:
			send_apup(Maplestat::Id::MP);
			break;
		case Buttons::BT_STR:
			send_apup(Maplestat::Id::STR);
			break;
		case Buttons::BT_DEX:
			send_apup(Maplestat::Id::DEX);
			break;
		case Buttons::BT_INT:
			send_apup(Maplestat::Id::INT);
			break;
		case Buttons::BT_LUK:
			send_apup(Maplestat::Id::LUK);
			break;
		case Buttons::BT_AUTO:
		{
			uint16_t autostr = 0;
			uint16_t autodex = 0;
			uint16_t autoint = 0;
			uint16_t autoluk = 0;
			uint16_t nowap = stats.get_stat(Maplestat::Id::AP);
			Equipstat::Id id = player.get_stats().get_job().get_primary(player.get_weapontype());

			switch (id)
			{
			case Equipstat::Id::STR:
				autostr = nowap;
				break;
			case Equipstat::Id::DEX:
				autodex = nowap;
				break;
			case Equipstat::Id::INT:
				autoint = nowap;
				break;
			case Equipstat::Id::LUK:
				autoluk = nowap;
				break;
			}

			std::string message =
				"Your AP will be distributed as follows:\\r"
				"\\nSTR : +" + std::to_string(autostr) +
				"\\nDEX : +" + std::to_string(autodex) +
				"\\nINT : +" + std::to_string(autoint) +
				"\\nLUK : +" + std::to_string(autoluk) +
				"\\r\\n";

			std::function<void(bool)> yesnohandler = [&, autostr, autodex, autoint, autoluk](bool yes)
			{
				if (yes)
				{
					if (autostr > 0)
						for (size_t i = 0; i < autostr; i++)
							send_apup(Maplestat::Id::STR);

					if (autodex > 0)
						for (size_t i = 0; i < autodex; i++)
							send_apup(Maplestat::Id::DEX);

					if (autoint > 0)
						for (size_t i = 0; i < autoint; i++)
							send_apup(Maplestat::Id::INT);

					if (autoluk > 0)
						for (size_t i = 0; i < autoluk; i++)
							send_apup(Maplestat::Id::LUK);
				}
			};

			UI::get().emplace<UIYesNo>(message, yesnohandler, Text::Alignment::LEFT);
		}
		break;
		case Buttons::BT_HYPERSTATOPEN:
			break;
		case Buttons::BT_HYPERSTATCLOSE:
		{
			if (player.get_level() < 140)
				UI::get().emplace<UIOk>("You can use the Hyper Stat at Lv. 140 and above.", [](bool) {});
		}
		break;
		case Buttons::BT_DETAILOPEN:
			set_detail(true);
			break;
		case Buttons::BT_DETAILCLOSE:
		case Buttons::BT_DETAIL_DETAILCLOSE:
			set_detail(false);
			break;
		case Buttons::BT_ABILITY:
			break;
		default:
			break;
		}

		return Button::State::NORMAL;
	}

	void UIStatsinfo::send_apup(Maplestat::Id stat) const
	{
		SpendApPacket(stat).dispatch();
		UI::get().disable();
	}

	void UIStatsinfo::set_detail(bool enabled)
	{
		showdetail = enabled;

		buttons[Buttons::BT_DETAILOPEN]->set_active(!enabled);
		buttons[Buttons::BT_DETAILCLOSE]->set_active(enabled);
		buttons[Buttons::BT_ABILITY]->set_active(enabled);
		buttons[Buttons::BT_DETAIL_DETAILCLOSE]->set_active(enabled);
	}

	void UIStatsinfo::update_ap()
	{
		bool nowap = stats.get_stat(Maplestat::Id::AP) > 0;
		Button::State newstate = nowap ? Button::State::NORMAL : Button::State::DISABLED;

		for (int i = Buttons::BT_HP; i <= Buttons::BT_AUTO; i++)
			buttons[i]->set_state(newstate);

		hasap = nowap;
	}

	void UIStatsinfo::update_simple(StatLabel label, Maplestat::Id stat)
	{
		statlabels[label].change_text(std::to_string(stats.get_stat(stat)));
	}

	void UIStatsinfo::update_basevstotal(StatLabel label, Maplestat::Id bstat, Equipstat::Id tstat)
	{
		int32_t base = stats.get_stat(bstat);
		int32_t total = stats.get_total(tstat);
		int32_t delta = total - base;

		std::string stattext = std::to_string(total);

		if (delta)
		{
			stattext += " (" + std::to_string(base);

			if (delta > 0)
				stattext += "+" + std::to_string(delta);
			else if (delta < 0)
				stattext += "-" + std::to_string(-delta);

			stattext += ")";
		}

		statlabels[label].change_text(stattext);
	}

	void UIStatsinfo::update_buffed(StatLabel label, Equipstat::Id stat)
	{
		int32_t total = stats.get_total(stat);
		int32_t delta = stats.get_buffdelta(stat);

		std::string stattext = std::to_string(total);

		if (delta)
		{
			stattext += " (" + std::to_string(total - delta);

			if (delta > 0)
			{
				stattext += "+" + std::to_string(delta);

				statlabels[label].change_color(Color::Name::RED);
			}
			else if (delta < 0)
			{
				stattext += "-" + std::to_string(-delta);

				statlabels[label].change_color(Color::Name::BLUE);
			}

			stattext += ")";
		}

		statlabels[label].change_text(stattext);
	}
}