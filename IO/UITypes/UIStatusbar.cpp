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
#include "UIStatusbar.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Gameplay/Stage.h"
#include "../UITypes/UIQuestLog.h"
#include "../UITypes/UIUserList.h"
#include "../UITypes/UIStatsinfo.h"
#include "../UITypes/UISkillbook.h"
#include "../UITypes/UIEquipInventory.h"
#include "../UITypes/UIItemInventory.h"
#include "../UITypes/UIChannel.h"
#include "../UITypes/UIJoypad.h"
#include "../UITypes/UIEvent.h"
#include "../UITypes/UIKeyConfig.h"
#include "../UITypes/UIChat.h"
#include "../UITypes/UIOptionMenu.h"
#include "../UITypes/UIQuit.h"
#include "../Character/ExpTable.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIStatusbar::UIStatusbar(const CharStats& st) : stats(st)
	{
		quickslot_active = false;
		quickslot_adj = Point<int16_t>(QUICKSLOT_MAX, 0);
		VWIDTH = Constants::Constants::get().get_viewwidth();
		VHEIGHT = Constants::Constants::get().get_viewheight();

		menu_active = false;
		setting_active = false;
		community_active = false;
		character_active = false;
		event_active = false;

		std::string stat = "status";

		if (VWIDTH == 800)
			stat += "800";

		nl::node mainBar = nl::nx::ui["StatusBar3.img"]["mainBar"];
		nl::node status = mainBar[stat];
		nl::node EXPBar = mainBar["EXPBar"];
		nl::node EXPBarRes = EXPBar[VWIDTH];
		nl::node menu = mainBar["menu"];
		nl::node quickSlot = mainBar["quickSlot"];
		nl::node submenu = mainBar["submenu"];

		exp_pos = Point<int16_t>(0, 87);

		sprites.emplace_back(EXPBar["backgrnd"], DrawArgument(Point<int16_t>(0, 87), Point<int16_t>(VWIDTH, 0)));
		sprites.emplace_back(EXPBarRes["layer:back"], exp_pos);

		int16_t exp_max = VWIDTH - 16;

		expbar = Gauge(
			EXPBarRes.resolve("layer:gauge"),
			EXPBarRes.resolve("layer:cover"),
			EXPBar.resolve("layer:effect"),
			exp_max, 0.0f
		);

		int16_t pos_adj = 0;

		if (VWIDTH == 1280)
			pos_adj = 87;
		else if (VWIDTH == 1366)
			pos_adj = 171;
		else if (VWIDTH == 1920)
			pos_adj = 448;

		if (VWIDTH == 1024)
			quickslot_min = 1;
		else
			quickslot_min = 0;

		if (VWIDTH == 800)
		{
			hpmp_pos = Point<int16_t>(412, 40);
			hpset_pos = Point<int16_t>(530, 70);
			mpset_pos = Point<int16_t>(528, 86);
			statset_pos = Point<int16_t>(427, 111);
			levelset_pos = Point<int16_t>(461, 48);
			namelabel_pos = Point<int16_t>(487, 40);
			quickslot_pos = Point<int16_t>(579, 0);

			// Menu
			menu_pos = Point<int16_t>(682, -280);
			setting_pos = menu_pos + Point<int16_t>(0, 168);
			community_pos = menu_pos + Point<int16_t>(-26, 196);
			character_pos = menu_pos + Point<int16_t>(-61, 168);
			event_pos = menu_pos + Point<int16_t>(-94, 252);
		}
		else
		{
			hpmp_pos = Point<int16_t>(416 + pos_adj, 40);
			hpset_pos = Point<int16_t>(550 + pos_adj, 70);
			mpset_pos = Point<int16_t>(546 + pos_adj, 86);
			statset_pos = Point<int16_t>(539 + pos_adj, 111);
			levelset_pos = Point<int16_t>(465 + pos_adj, 48);
			namelabel_pos = Point<int16_t>(493 + pos_adj, 40);
			quickslot_pos = Point<int16_t>(628 + pos_adj, 37);

			// Menu
			menu_pos = Point<int16_t>(720 + pos_adj, -280);
			setting_pos = menu_pos + Point<int16_t>(0, 168);
			community_pos = menu_pos + Point<int16_t>(-26, 196);
			character_pos = menu_pos + Point<int16_t>(-61, 168);
			event_pos = menu_pos + Point<int16_t>(-94, 252);
		}

		if (VWIDTH == 1280)
		{
			statset_pos = Point<int16_t>(580 + pos_adj, 111);
			quickslot_pos = Point<int16_t>(622 + pos_adj, 37);

			// Menu
			menu_pos += Point<int16_t>(-7, 0);
			setting_pos += Point<int16_t>(-7, 0);
			community_pos += Point<int16_t>(-7, 0);
			character_pos += Point<int16_t>(-7, 0);
			event_pos += Point<int16_t>(-7, 0);
		}
		else if (VWIDTH == 1366)
		{
			quickslot_pos = Point<int16_t>(623 + pos_adj, 37);

			// Menu
			menu_pos += Point<int16_t>(-5, 0);
			setting_pos += Point<int16_t>(-5, 0);
			community_pos += Point<int16_t>(-5, 0);
			character_pos += Point<int16_t>(-5, 0);
			event_pos += Point<int16_t>(-5, 0);
		}
		else if (VWIDTH == 1920)
		{
			quickslot_pos = Point<int16_t>(900 + pos_adj, 37);

			// Menu
			menu_pos += Point<int16_t>(272, 0);
			setting_pos += Point<int16_t>(272, 0);
			community_pos += Point<int16_t>(272, 0);
			character_pos += Point<int16_t>(272, 0);
			event_pos += Point<int16_t>(272, 0);
		}

		hpmp_sprites.emplace_back(status["backgrnd"], hpmp_pos - Point<int16_t>(1, 0));
		hpmp_sprites.emplace_back(status["layer:cover"], hpmp_pos - Point<int16_t>(1, 0));

		if (VWIDTH == 800)
			hpmp_sprites.emplace_back(status["layer:Lv"], hpmp_pos);
		else
			hpmp_sprites.emplace_back(status["layer:Lv"], hpmp_pos - Point<int16_t>(1, 0));

		int16_t hpmp_max = 139;

		if (VWIDTH > 800)
			hpmp_max += 30;

		hpbar = Gauge(status.resolve("gauge/hp/layer:0"), hpmp_max, 0.0f);
		mpbar = Gauge(status.resolve("gauge/mp/layer:0"), hpmp_max, 0.0f);

		statset = Charset(EXPBar["number"], Charset::Alignment::RIGHT);
		hpmpset = Charset(status["gauge"]["number"], Charset::Alignment::RIGHT);
		levelset = Charset(status["lvNumber"], Charset::Alignment::LEFT);

		namelabel = OutlinedText(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::GALLERY, Color::Name::TUNA);

		quickslot[0] = quickSlot["backgrnd"];
		quickslot[1] = quickSlot["layer:cover"];

		Point<int16_t> buttonPos = Point<int16_t>(591 + pos_adj, 73);

		if (VWIDTH == 1024)
			buttonPos += Point<int16_t>(38, 0);
		else if (VWIDTH == 1280)
			buttonPos += Point<int16_t>(31, 0);
		else if (VWIDTH == 1366)
			buttonPos += Point<int16_t>(33, 0);
		else if (VWIDTH == 1920)
			buttonPos += Point<int16_t>(310, 0);

		buttons[Buttons::BT_CASHSHOP] = std::make_unique<MapleButton>(menu["button:CashShop"], buttonPos);
		buttons[Buttons::BT_MENU] = std::make_unique<MapleButton>(menu["button:Menu"], buttonPos);
		buttons[Buttons::BT_OPTIONS] = std::make_unique<MapleButton>(menu["button:Setting"], buttonPos);
		buttons[Buttons::BT_CHARACTER] = std::make_unique<MapleButton>(menu["button:Character"], buttonPos);
		buttons[Buttons::BT_COMMUNITY] = std::make_unique<MapleButton>(menu["button:Community"], buttonPos);
		buttons[Buttons::BT_EVENT] = std::make_unique<MapleButton>(menu["button:Event"], buttonPos);

		if (quickslot_active && VWIDTH > 800)
		{
			buttons[Buttons::BT_CASHSHOP]->set_active(false);
			buttons[Buttons::BT_MENU]->set_active(false);
			buttons[Buttons::BT_OPTIONS]->set_active(false);
			buttons[Buttons::BT_CHARACTER]->set_active(false);
			buttons[Buttons::BT_COMMUNITY]->set_active(false);
			buttons[Buttons::BT_EVENT]->set_active(false);
		}

		std::string fold = "button:Fold";
		std::string extend = "button:Extend";

		if (VWIDTH == 800)
		{
			fold += "800";
			extend += "800";
		}

		if (VWIDTH == 1366)
			quickslot_qs_adj = Point<int16_t>(213, 0);
		else
			quickslot_qs_adj = Point<int16_t>(211, 0);

		if (VWIDTH == 800)
		{
			Point<int16_t> quickslot_qs = Point<int16_t>(579, 0);

			buttons[Buttons::BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], quickslot_qs);
			buttons[Buttons::BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], quickslot_qs + quickslot_qs_adj);
		}
		else if (VWIDTH == 1024)
		{
			Point<int16_t> quickslot_qs = Point<int16_t>(627 + pos_adj, 37);

			buttons[Buttons::BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], quickslot_qs);
			buttons[Buttons::BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], quickslot_qs + quickslot_qs_adj);
		}
		else if (VWIDTH == 1280)
		{
			Point<int16_t> quickslot_qs = Point<int16_t>(621 + pos_adj, 37);

			buttons[Buttons::BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], quickslot_qs);
			buttons[Buttons::BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], quickslot_qs + quickslot_qs_adj);
		}
		else if (VWIDTH == 1366)
		{
			Point<int16_t> quickslot_qs = Point<int16_t>(623 + pos_adj, 37);

			buttons[Buttons::BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], quickslot_qs);
			buttons[Buttons::BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], quickslot_qs + quickslot_qs_adj);
		}
		else if (VWIDTH == 1920)
		{
			Point<int16_t> quickslot_qs = Point<int16_t>(900 + pos_adj, 37);

			buttons[Buttons::BT_FOLD_QS] = std::make_unique<MapleButton>(quickSlot[fold], quickslot_qs);
			buttons[Buttons::BT_EXTEND_QS] = std::make_unique<MapleButton>(quickSlot[extend], quickslot_qs + quickslot_qs_adj);
		}

		if (quickslot_active)
			buttons[Buttons::BT_EXTEND_QS]->set_active(false);
		else
			buttons[Buttons::BT_FOLD_QS]->set_active(false);

#pragma region Menu
		menubackground[0] = submenu["backgrnd"]["0"];
		menubackground[1] = submenu["backgrnd"]["1"];
		menubackground[2] = submenu["backgrnd"]["2"];

		buttons[Buttons::BT_MENU_ACHIEVEMENT] = std::make_unique<MapleButton>(submenu["menu"]["button:achievement"], menu_pos);
		buttons[Buttons::BT_MENU_AUCTION] = std::make_unique<MapleButton>(submenu["menu"]["button:auction"], menu_pos);
		buttons[Buttons::BT_MENU_BATTLE] = std::make_unique<MapleButton>(submenu["menu"]["button:battleStats"], menu_pos);
		buttons[Buttons::BT_MENU_CLAIM] = std::make_unique<MapleButton>(submenu["menu"]["button:Claim"], menu_pos);
		buttons[Buttons::BT_MENU_FISHING] = std::make_unique<MapleButton>(submenu["menu"]["button:Fishing"], menu_pos + Point<int16_t>(3, 1));
		buttons[Buttons::BT_MENU_HELP] = std::make_unique<MapleButton>(submenu["menu"]["button:Help"], menu_pos);
		buttons[Buttons::BT_MENU_MEDAL] = std::make_unique<MapleButton>(submenu["menu"]["button:medal"], menu_pos);
		buttons[Buttons::BT_MENU_MONSTER_COLLECTION] = std::make_unique<MapleButton>(submenu["menu"]["button:monsterCollection"], menu_pos);
		buttons[Buttons::BT_MENU_MONSTER_LIFE] = std::make_unique<MapleButton>(submenu["menu"]["button:monsterLife"], menu_pos);
		buttons[Buttons::BT_MENU_QUEST] = std::make_unique<MapleButton>(submenu["menu"]["button:quest"], menu_pos);
		buttons[Buttons::BT_MENU_UNION] = std::make_unique<MapleButton>(submenu["menu"]["button:union"], menu_pos);

		buttons[Buttons::BT_SETTING_CHANNEL] = std::make_unique<MapleButton>(submenu["setting"]["button:channel"], setting_pos);
		buttons[Buttons::BT_SETTING_QUIT] = std::make_unique<MapleButton>(submenu["setting"]["button:GameQuit"], setting_pos);
		buttons[Buttons::BT_SETTING_JOYPAD] = std::make_unique<MapleButton>(submenu["setting"]["button:JoyPad"], setting_pos);
		buttons[Buttons::BT_SETTING_KEYS] = std::make_unique<MapleButton>(submenu["setting"]["button:keySetting"], setting_pos);
		buttons[Buttons::BT_SETTING_OPTION] = std::make_unique<MapleButton>(submenu["setting"]["button:option"], setting_pos);

		buttons[Buttons::BT_COMMUNITY_PARTY] = std::make_unique<MapleButton>(submenu["community"]["button:bossParty"], community_pos);
		buttons[Buttons::BT_COMMUNITY_FRIENDS] = std::make_unique<MapleButton>(submenu["community"]["button:friends"], community_pos);
		buttons[Buttons::BT_COMMUNITY_GUILD] = std::make_unique<MapleButton>(submenu["community"]["button:guild"], community_pos);
		buttons[Buttons::BT_COMMUNITY_MAPLECHAT] = std::make_unique<MapleButton>(submenu["community"]["button:mapleChat"], community_pos);

		buttons[Buttons::BT_CHARACTER_INFO] = std::make_unique<MapleButton>(submenu["character"]["button:character"], character_pos);
		buttons[Buttons::BT_CHARACTER_EQUIP] = std::make_unique<MapleButton>(submenu["character"]["button:Equip"], character_pos);
		buttons[Buttons::BT_CHARACTER_ITEM] = std::make_unique<MapleButton>(submenu["character"]["button:Item"], character_pos);
		buttons[Buttons::BT_CHARACTER_SKILL] = std::make_unique<MapleButton>(submenu["character"]["button:Skill"], character_pos);
		buttons[Buttons::BT_CHARACTER_STAT] = std::make_unique<MapleButton>(submenu["character"]["button:Stat"], character_pos);

		buttons[Buttons::BT_EVENT_DAILY] = std::make_unique<MapleButton>(submenu["event"]["button:dailyGift"], event_pos);
		buttons[Buttons::BT_EVENT_SCHEDULE] = std::make_unique<MapleButton>(submenu["event"]["button:schedule"], event_pos);

		for (size_t i = Buttons::BT_MENU_QUEST; i <= Buttons::BT_EVENT_DAILY; i++)
			buttons[i]->set_active(false);

		menutitle[0] = submenu["title"]["character"];
		menutitle[1] = submenu["title"]["community"];
		menutitle[2] = submenu["title"]["event"];
		menutitle[3] = submenu["title"]["menu"];
		menutitle[4] = submenu["title"]["setting"];
#pragma endregion

		if (VWIDTH == 800)
		{
			position = Point<int16_t>(0, 480);
			position_x = 410;
			position_y = position.y();
			dimension = Point<int16_t>(VWIDTH - position_x, 140);
		}
		else if (VWIDTH == 1024)
		{
			position = Point<int16_t>(0, 648);
			position_x = 410;
			position_y = position.y() + 42;
			dimension = Point<int16_t>(VWIDTH - position_x, 75);
		}
		else if (VWIDTH == 1280)
		{
			position = Point<int16_t>(0, 600);
			position_x = 500;
			position_y = position.y() + 42;
			dimension = Point<int16_t>(VWIDTH - position_x, 75);
		}
		else if (VWIDTH == 1366)
		{
			position = Point<int16_t>(0, 648);
			position_x = 585;
			position_y = position.y() + 42;
			dimension = Point<int16_t>(VWIDTH - position_x, 75);
		}
		else if (VWIDTH == 1920)
		{
			position = Point<int16_t>(0, 960 + (VHEIGHT - 1080));
			position_x = 860;
			position_y = position.y() + 40;
			dimension = Point<int16_t>(VWIDTH - position_x, 80);
		}
	}

	void UIStatusbar::draw(float alpha) const
	{
		UIElement::draw_sprites(alpha);

		for (size_t i = 0; i <= Buttons::BT_EVENT; i++)
			buttons.at(i)->draw(position);

		hpmp_sprites[0].draw(position, alpha);

		expbar.draw(position + exp_pos);
		hpbar.draw(position + hpmp_pos);
		mpbar.draw(position + hpmp_pos);

		hpmp_sprites[1].draw(position, alpha);
		hpmp_sprites[2].draw(position, alpha);

		int16_t level = stats.get_stat(Maplestat::Id::LEVEL);
		int16_t hp = stats.get_stat(Maplestat::Id::HP);
		int16_t mp = stats.get_stat(Maplestat::Id::MP);
		int32_t maxhp = stats.get_total(Equipstat::Id::HP);
		int32_t maxmp = stats.get_total(Equipstat::Id::MP);
		int64_t exp = stats.get_exp();

		std::string expstring = std::to_string(100 * getexppercent());

		statset.draw(
			std::to_string(exp) + "[" + expstring.substr(0, expstring.find('.') + 3) + "%]",
			position + statset_pos
		);

		hpmpset.draw(
			"[" + std::to_string(hp) + "/" + std::to_string(maxhp) + "]",
			position + hpset_pos
		);

		hpmpset.draw(
			"[" + std::to_string(mp) + "/" + std::to_string(maxmp) + "]",
			position + mpset_pos
		);

		levelset.draw(
			std::to_string(level),
			position + levelset_pos
		);

		namelabel.draw(position + namelabel_pos);

		buttons.at(Buttons::BT_FOLD_QS)->draw(position + quickslot_adj);
		buttons.at(Buttons::BT_EXTEND_QS)->draw(position + quickslot_adj - quickslot_qs_adj);

		if (VWIDTH > 800 && VWIDTH < 1366)
		{
			quickslot[0].draw(position + quickslot_pos + Point<int16_t>(-1, 0) + quickslot_adj);
			quickslot[1].draw(position + quickslot_pos + Point<int16_t>(-1, 0) + quickslot_adj);
		}
		else
		{
			quickslot[0].draw(position + quickslot_pos + quickslot_adj);
			quickslot[1].draw(position + quickslot_pos + quickslot_adj);
		}

#pragma region Menu
		Point<int16_t> pos_adj = Point<int16_t>(0, 0);

		if (quickslot_active)
		{
			if (VWIDTH == 800)
				pos_adj += Point<int16_t>(0, -73);
			else
				pos_adj += Point<int16_t>(0, -31);
		}

		Point<int16_t> pos;
		uint8_t button_count, menutitle_index;

		if (character_active)
		{
			pos = character_pos;
			button_count = 5;
			menutitle_index = 0;
		}
		else if (community_active)
		{
			pos = community_pos;
			button_count = 4;
			menutitle_index = 1;
		}
		else if (event_active)
		{
			pos = event_pos;
			button_count = 2;
			menutitle_index = 2;
		}
		else if (menu_active)
		{
			pos = menu_pos;
			button_count = 11;
			menutitle_index = 3;
		}
		else if (setting_active)
		{
			pos = setting_pos;
			button_count = 5;
			menutitle_index = 4;
		}
		else
		{
			return;
		}

		Point<int16_t> mid_pos = Point<int16_t>(0, 29);

		uint16_t end_y = std::floor(28.2 * button_count);

		if (menu_active)
			end_y -= 1;

		uint16_t mid_y = end_y - mid_pos.y();

		menubackground[0].draw(position + pos + pos_adj);
		menubackground[1].draw(DrawArgument(position + pos + pos_adj) + DrawArgument(mid_pos, Point<int16_t>(0, mid_y)));
		menubackground[2].draw(position + pos + pos_adj + Point<int16_t>(0, end_y));

		menutitle[menutitle_index].draw(position + pos + pos_adj);

		for (size_t i = Buttons::BT_MENU_QUEST; i <= Buttons::BT_EVENT_DAILY; i++)
			buttons.at(i)->draw(position);
#pragma endregion
	}

	void UIStatusbar::update()
	{
		UIElement::update();

		for each (auto sprite in hpmp_sprites)
			sprite.update();

		expbar.update(getexppercent());
		hpbar.update(gethppercent());
		mpbar.update(getmppercent());

		namelabel.change_text(stats.get_name());

		Point<int16_t> pos_adj = get_quickslot_pos();

		if (quickslot_active)
		{
			if (quickslot_adj.x() > quickslot_min)
			{
				int16_t new_x = quickslot_adj.x() - Constants::TIMESTEP;

				if (new_x < quickslot_min)
					quickslot_adj.set_x(quickslot_min);
				else
					quickslot_adj.shift_x(-Constants::TIMESTEP);
			}
		}
		else
		{
			if (quickslot_adj.x() < QUICKSLOT_MAX)
			{
				int16_t new_x = quickslot_adj.x() + Constants::TIMESTEP;

				if (new_x > QUICKSLOT_MAX)
					quickslot_adj.set_x(QUICKSLOT_MAX);
				else
					quickslot_adj.shift_x(Constants::TIMESTEP);
			}
		}

		for (size_t i = Buttons::BT_MENU_QUEST; i <= Buttons::BT_MENU_CLAIM; i++)
		{
			Point<int16_t> menu_adj = Point<int16_t>(0, 0);

			if (i == Buttons::BT_MENU_FISHING)
				menu_adj = Point<int16_t>(3, 1);

			buttons[i]->set_position(menu_pos + menu_adj + pos_adj);
		}

		for (size_t i = Buttons::BT_SETTING_CHANNEL; i <= Buttons::BT_SETTING_QUIT; i++)
			buttons[i]->set_position(setting_pos + pos_adj);

		for (size_t i = Buttons::BT_COMMUNITY_FRIENDS; i <= Buttons::BT_COMMUNITY_MAPLECHAT; i++)
			buttons[i]->set_position(community_pos + pos_adj);

		for (size_t i = Buttons::BT_CHARACTER_INFO; i <= Buttons::BT_CHARACTER_ITEM; i++)
			buttons[i]->set_position(character_pos + pos_adj);

		for (size_t i = Buttons::BT_EVENT_SCHEDULE; i <= Buttons::BT_EVENT_DAILY; i++)
			buttons[i]->set_position(event_pos + pos_adj);
	}

	Button::State UIStatusbar::button_pressed(uint16_t id)
	{
		switch (id)
		{
		case Buttons::BT_CASHSHOP:
			break;
		case Buttons::BT_MENU:
			toggle_menu();
			break;
		case Buttons::BT_OPTIONS:
			toggle_setting();
			break;
		case Buttons::BT_CHARACTER:
			toggle_character();
			break;
		case Buttons::BT_COMMUNITY:
			toggle_community();
			break;
		case Buttons::BT_EVENT:
			toggle_event();
			break;
		case Buttons::BT_FOLD_QS:
			toggle_qs(false);
			break;
		case Buttons::BT_EXTEND_QS:
			toggle_qs(true);
			break;
		case Buttons::BT_MENU_QUEST:
			UI::get().emplace<UIQuestLog>(
				Stage::get().get_player().get_quests()
				);

			remove_menus();
			break;
		case Buttons::BT_MENU_MEDAL:
		case Buttons::BT_MENU_UNION:
		case Buttons::BT_MENU_MONSTER_COLLECTION:
		case Buttons::BT_MENU_AUCTION:
		case Buttons::BT_MENU_MONSTER_LIFE:
		case Buttons::BT_MENU_BATTLE:
		case Buttons::BT_MENU_ACHIEVEMENT:
		case Buttons::BT_MENU_FISHING:
		case Buttons::BT_MENU_HELP:
		case Buttons::BT_MENU_CLAIM:
			remove_menus();
			break;
		case Buttons::BT_SETTING_CHANNEL:
			UI::get().emplace<UIChannel>();

			remove_menus();
			break;
		case Buttons::BT_SETTING_OPTION:
			UI::get().emplace<UIOptionMenu>();

			remove_menus();
			break;
		case Buttons::BT_SETTING_KEYS:
			UI::get().emplace<UIKeyConfig>(
				Stage::get().get_player().get_inventory(),
				Stage::get().get_player().get_skills()
				);

			remove_menus();
			break;
		case Buttons::BT_SETTING_JOYPAD:
			UI::get().emplace<UIJoypad>();

			remove_menus();
			break;
		case Buttons::BT_SETTING_QUIT:
			UI::get().emplace<UIQuit>(stats);

			remove_menus();
			break;
		case Buttons::BT_COMMUNITY_FRIENDS:
		case Buttons::BT_COMMUNITY_PARTY:
		{
			auto userlist = UI::get().get_element<UIUserList>();
			auto tab = (id == Buttons::BT_COMMUNITY_FRIENDS) ? UIUserList::Tab::FRIEND : UIUserList::Tab::PARTY;

			if (!userlist)
			{
				UI::get().emplace<UIUserList>(tab);
			}
			else
			{
				auto cur_tab = userlist->get_tab();
				auto is_active = userlist->is_active();

				if (cur_tab == tab)
				{
					if (is_active)
						userlist->deactivate();
					else
						userlist->makeactive();
				}
				else
				{
					if (!is_active)
						userlist->makeactive();

					userlist->change_tab(tab);
				}
			}

			remove_menus();
		}
		break;
		case Buttons::BT_COMMUNITY_GUILD:
			remove_menus();
			break;
		case Buttons::BT_COMMUNITY_MAPLECHAT:
			UI::get().emplace<UIChat>();

			remove_menus();
			break;
		case Buttons::BT_CHARACTER_INFO:
			remove_menus();
			break;
		case Buttons::BT_CHARACTER_STAT:
			UI::get().emplace<UIStatsinfo>(
				Stage::get().get_player().get_stats()
				);

			remove_menus();
			break;
		case Buttons::BT_CHARACTER_SKILL:
			UI::get().emplace<UISkillbook>(
				Stage::get().get_player().get_stats(),
				Stage::get().get_player().get_skills()
				);

			remove_menus();
			break;
		case Buttons::BT_CHARACTER_EQUIP:
			UI::get().emplace<UIEquipInventory>(
				Stage::get().get_player().get_inventory()
				);

			remove_menus();
			break;
		case Buttons::BT_CHARACTER_ITEM:
			UI::get().emplace<UIItemInventory>(
				Stage::get().get_player().get_inventory()
				);

			remove_menus();
			break;
		case Buttons::BT_EVENT_SCHEDULE:
			UI::get().emplace<UIEvent>();

			remove_menus();
			break;
		case Buttons::BT_EVENT_DAILY:
			remove_menus();
			break;
		}

		return Button::State::NORMAL;
	}

	void UIStatusbar::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				if (!menu_active && !setting_active && !community_active && !character_active && !event_active)
					toggle_setting();
				else
					remove_menus();
			}
			else if (keycode == KeyAction::Id::RETURN)
			{
				for (size_t i = Buttons::BT_MENU_QUEST; i <= Buttons::BT_EVENT_DAILY; i++)
					if (buttons[i]->get_state() == Button::State::MOUSEOVER)
						button_pressed(i);
			}
			else if (keycode == KeyAction::Id::UP || keycode == KeyAction::Id::DOWN)
			{
				uint16_t min_id, max_id;

				if (menu_active)
				{
					min_id = Buttons::BT_MENU_QUEST;
					max_id = Buttons::BT_MENU_CLAIM;
				}
				else if (setting_active)
				{
					min_id = Buttons::BT_SETTING_CHANNEL;
					max_id = Buttons::BT_SETTING_QUIT;
				}
				else if (community_active)
				{
					min_id = Buttons::BT_COMMUNITY_FRIENDS;
					max_id = Buttons::BT_COMMUNITY_MAPLECHAT;
				}
				else if (character_active)
				{
					min_id = Buttons::BT_CHARACTER_INFO;
					max_id = Buttons::BT_CHARACTER_ITEM;
				}
				else if (event_active)
				{
					min_id = Buttons::BT_EVENT_SCHEDULE;
					max_id = Buttons::BT_EVENT_DAILY;
				}

				uint16_t id = min_id;

				for (size_t i = min_id; i <= max_id; i++)
				{
					if (buttons[i]->get_state() != Button::State::NORMAL)
					{
						id = i;

						buttons[i]->set_state(Button::State::NORMAL);
						break;
					}
				}

				if (keycode == KeyAction::Id::DOWN)
				{
					if (id < max_id)
						id++;
					else
						id = min_id;
				}
				else if (keycode == KeyAction::Id::UP)
				{
					if (id > min_id)
						id--;
					else
						id = max_id;
				}

				buttons[id]->set_state(Button::State::MOUSEOVER);
			}
		}
	}

	bool UIStatusbar::is_in_range(Point<int16_t> cursorpos) const
	{
		Point<int16_t> pos;
		Rectangle<int16_t> bounds;

		if (!character_active && !community_active && !event_active && !menu_active && !setting_active)
		{
			pos = Point<int16_t>(position_x, position_y);
			bounds = Rectangle<int16_t>(pos, pos + dimension);
		}
		else
		{
			uint8_t button_count;
			int16_t pos_y_adj;

			if (character_active)
			{
				pos = character_pos;
				button_count = 5;
				pos_y_adj = 248;
			}
			else if (community_active)
			{
				pos = community_pos;
				button_count = 4;
				pos_y_adj = 301;
			}
			else if (event_active)
			{
				pos = event_pos;
				button_count = 2;
				pos_y_adj = 417;
			}
			else if (menu_active)
			{
				pos = menu_pos;
				button_count = 11;
				pos_y_adj = -90;
			}
			else if (setting_active)
			{
				pos = setting_pos;
				button_count = 5;
				pos_y_adj = 248;
			}

			pos_y_adj += VHEIGHT - 600;

			Point<int16_t> pos_adj = const_cast<UIStatusbar*>(this)->get_quickslot_pos();
			pos = Point<int16_t>(pos.x(), std::abs(pos.y()) + pos_y_adj) + pos_adj;

			uint16_t end_y = std::floor(28.2 * button_count);

			bounds = Rectangle<int16_t>(pos, pos + Point<int16_t>(113, end_y + 35));
		}

		return bounds.contains(cursorpos);
	}

	UIElement::Type UIStatusbar::get_type() const
	{
		return TYPE;
	}

	void UIStatusbar::toggle_qs()
	{
		if (!menu_active && !setting_active && !community_active && !character_active && !event_active)
			toggle_qs(!quickslot_active);
	}

	void UIStatusbar::toggle_qs(bool quick_slot_active)
	{
		if (quickslot_active == quick_slot_active)
			return;

		quickslot_active = quick_slot_active;
		buttons[Buttons::BT_FOLD_QS]->set_active(quickslot_active);
		buttons[Buttons::BT_EXTEND_QS]->set_active(!quickslot_active);

		if (VWIDTH > 800)
		{
			buttons[Buttons::BT_CASHSHOP]->set_active(!quickslot_active);
			buttons[Buttons::BT_MENU]->set_active(!quickslot_active);
			buttons[Buttons::BT_OPTIONS]->set_active(!quickslot_active);
			buttons[Buttons::BT_CHARACTER]->set_active(!quickslot_active);
			buttons[Buttons::BT_COMMUNITY]->set_active(!quickslot_active);
			buttons[Buttons::BT_EVENT]->set_active(!quickslot_active);
		}
	}

	void UIStatusbar::toggle_menu()
	{
		remove_active_menu(MenuType::MENU);

		menu_active = !menu_active;

		buttons[Buttons::BT_MENU_ACHIEVEMENT]->set_active(menu_active);
		buttons[Buttons::BT_MENU_AUCTION]->set_active(menu_active);
		buttons[Buttons::BT_MENU_BATTLE]->set_active(menu_active);
		buttons[Buttons::BT_MENU_CLAIM]->set_active(menu_active);
		buttons[Buttons::BT_MENU_FISHING]->set_active(menu_active);
		buttons[Buttons::BT_MENU_HELP]->set_active(menu_active);
		buttons[Buttons::BT_MENU_MEDAL]->set_active(menu_active);
		buttons[Buttons::BT_MENU_MONSTER_COLLECTION]->set_active(menu_active);
		buttons[Buttons::BT_MENU_MONSTER_LIFE]->set_active(menu_active);
		buttons[Buttons::BT_MENU_QUEST]->set_active(menu_active);
		buttons[Buttons::BT_MENU_UNION]->set_active(menu_active);

		if (menu_active)
		{
			buttons[Buttons::BT_MENU_QUEST]->set_state(Button::State::MOUSEOVER);

			Sound(Sound::Name::DLGNOTICE).play();
		}
	}

	void UIStatusbar::toggle_setting()
	{
		remove_active_menu(MenuType::SETTING);

		setting_active = !setting_active;

		buttons[Buttons::BT_SETTING_CHANNEL]->set_active(setting_active);
		buttons[Buttons::BT_SETTING_QUIT]->set_active(setting_active);
		buttons[Buttons::BT_SETTING_JOYPAD]->set_active(setting_active);
		buttons[Buttons::BT_SETTING_KEYS]->set_active(setting_active);
		buttons[Buttons::BT_SETTING_OPTION]->set_active(setting_active);

		if (setting_active)
		{
			buttons[Buttons::BT_SETTING_CHANNEL]->set_state(Button::State::MOUSEOVER);

			Sound(Sound::Name::DLGNOTICE).play();
		}
	}

	void UIStatusbar::toggle_community()
	{
		remove_active_menu(MenuType::COMMUNITY);

		community_active = !community_active;

		buttons[Buttons::BT_COMMUNITY_PARTY]->set_active(community_active);
		buttons[Buttons::BT_COMMUNITY_FRIENDS]->set_active(community_active);
		buttons[Buttons::BT_COMMUNITY_GUILD]->set_active(community_active);
		buttons[Buttons::BT_COMMUNITY_MAPLECHAT]->set_active(community_active);

		if (community_active)
		{
			buttons[Buttons::BT_COMMUNITY_FRIENDS]->set_state(Button::State::MOUSEOVER);

			Sound(Sound::Name::DLGNOTICE).play();
		}
	}

	void UIStatusbar::toggle_character()
	{
		remove_active_menu(MenuType::CHARACTER);

		character_active = !character_active;

		buttons[Buttons::BT_CHARACTER_INFO]->set_active(character_active);
		buttons[Buttons::BT_CHARACTER_EQUIP]->set_active(character_active);
		buttons[Buttons::BT_CHARACTER_ITEM]->set_active(character_active);
		buttons[Buttons::BT_CHARACTER_SKILL]->set_active(character_active);
		buttons[Buttons::BT_CHARACTER_STAT]->set_active(character_active);

		if (character_active)
		{
			buttons[Buttons::BT_CHARACTER_INFO]->set_state(Button::State::MOUSEOVER);

			Sound(Sound::Name::DLGNOTICE).play();
		}
	}

	void UIStatusbar::toggle_event()
	{
		remove_active_menu(MenuType::EVENT);

		event_active = !event_active;

		buttons[Buttons::BT_EVENT_DAILY]->set_active(event_active);
		buttons[Buttons::BT_EVENT_SCHEDULE]->set_active(event_active);

		if (event_active)
		{
			buttons[Buttons::BT_EVENT_SCHEDULE]->set_state(Button::State::MOUSEOVER);

			Sound(Sound::Name::DLGNOTICE).play();
		}
	}

	void UIStatusbar::remove_menus()
	{
		if (menu_active)
			toggle_menu();
		else if (setting_active)
			toggle_setting();
		else if (community_active)
			toggle_community();
		else if (character_active)
			toggle_character();
		else if (event_active)
			toggle_event();
	}

	void UIStatusbar::remove_active_menu(MenuType type)
	{
		for (size_t i = Buttons::BT_MENU_QUEST; i <= Buttons::BT_EVENT_DAILY; i++)
			buttons[i]->set_state(Button::State::NORMAL);

		if (menu_active && type != MenuType::MENU)
			toggle_menu();
		else if (setting_active && type != MenuType::SETTING)
			toggle_setting();
		else if (community_active && type != MenuType::COMMUNITY)
			toggle_community();
		else if (character_active && type != MenuType::CHARACTER)
			toggle_character();
		else if (event_active && type != MenuType::EVENT)
			toggle_event();
	}

	Point<int16_t> UIStatusbar::get_quickslot_pos()
	{
		if (quickslot_active)
		{
			if (VWIDTH == 800)
				return Point<int16_t>(0, -73);
			else
				return Point<int16_t>(0, -31);
		}

		return Point<int16_t>(0, 0);
	}

	bool UIStatusbar::is_menu_active()
	{
		return menu_active || setting_active || community_active || character_active || event_active;
	}

	float UIStatusbar::getexppercent() const
	{
		int16_t level = stats.get_stat(Maplestat::Id::LEVEL);

		if (level >= ExpTable::LEVELCAP)
			return 0.0f;

		int64_t exp = stats.get_exp();

		return static_cast<float>(
			static_cast<double>(exp) / ExpTable::values[level]
			);
	}

	float UIStatusbar::gethppercent() const
	{
		int16_t hp = stats.get_stat(Maplestat::Id::HP);
		int32_t maxhp = stats.get_total(Equipstat::Id::HP);

		return static_cast<float>(hp) / maxhp;
	}

	float UIStatusbar::getmppercent() const
	{
		int16_t mp = stats.get_stat(Maplestat::Id::MP);
		int32_t maxmp = stats.get_total(Equipstat::Id::MP);

		return static_cast<float>(mp) / maxmp;
	}
}