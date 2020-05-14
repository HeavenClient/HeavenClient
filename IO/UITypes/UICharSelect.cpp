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
#include "UICharSelect.h"
#include "UILoginNotice.h"
#include "UIRaceSelect.h"
#include "UISoftKey.h"
#include "UIWorldSelect.h"

#include "../UI.h"

#include "../Components/AreaButton.h"
#include "../Components/MapleButton.h"

#include "../../Configuration.h"

#include "../../Audio/Audio.h"
#include "../../Character/Job.h"

#include "../../Net/Packets/SelectCharPackets.h"

#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UICharSelect::UICharSelect(std::vector<CharEntry> c, int8_t char_count, int32_t s, int8_t rp) : UIElement(
		Point<int16_t>(0, 0), Point<int16_t>(800, 600)),
		characters(c),
		characters_count(
			char_count),
		slots(s),
		require_pic(rp)
	{
		burning_character = true;

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		pagepos = Point<int16_t>(247, 462);
		worldpos = Point<int16_t>(586, 46);
		charinfopos = Point<int16_t>(671, 339);

		Point<int16_t> character_sel_pos = Point<int16_t>(601, 393);
		Point<int16_t> character_new_pos = Point<int16_t>(200, 495);
		Point<int16_t> character_del_pos = Point<int16_t>(316, 495);

		selected_character = Setting<DefaultCharacter>::get().load();
		selected_page = selected_character / PAGESIZE;
		page_count = std::ceil((double) slots / (double) PAGESIZE);

		tab = nl::nx::ui["Basic.img"]["Cursor"]["18"]["0"];

		tab_index = 0;
		tab_active = false;
		tab_move = false;

		Point<int16_t> tab_adj = Point<int16_t>(86, 5);

		tab_pos[0] = character_sel_pos + tab_adj + Point<int16_t>(47, 3);
		tab_pos[1] = character_new_pos + tab_adj;
		tab_pos[2] = character_del_pos + tab_adj;

		tab_move_pos = 0;

		tab_map[0] = Buttons::CHARACTER_SELECT;
		tab_map[1] = Buttons::CHARACTER_NEW;
		tab_map[2] = Buttons::CHARACTER_DELETE;

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Common = Login["Common"];
		nl::node CharSelect = Login["CharSelect"];
		nl::node selectWorld = Common["selectWorld"];
		nl::node selectedWorld = CharSelect["selectedWorld"];
		nl::node pageNew = CharSelect["pageNew"];

		world_dimensions = Texture(selectWorld).get_dimensions();

		uint16_t world;
		uint8_t world_id = Configuration::get().get_worldid();
		uint8_t channel_id = Configuration::get().get_channelid();

		if (auto worldselect = UI::get().get_element<UIWorldSelect>())
			world = worldselect->get_worldbyid(world_id);

		world_sprites.emplace_back(selectWorld, worldpos);
		world_sprites.emplace_back(selectedWorld["icon"][world], worldpos - Point<int16_t>(12, -1));
		world_sprites.emplace_back(selectedWorld["name"][world], worldpos - Point<int16_t>(8, 1));
		world_sprites.emplace_back(selectedWorld["ch"][channel_id], worldpos - Point<int16_t>(0, 1));

		nl::node map = nl::nx::map001["Back"]["login.img"];
		nl::node ani = map["ani"];

		nl::node frame = nl::nx::mapLatest["Obj"]["login.img"]["Common"]["frame"]["2"]["0"];

		sprites.emplace_back(map["back"]["13"], Point<int16_t>(392, 297));
		sprites.emplace_back(ani["17"], Point<int16_t>(151, 283));
		sprites.emplace_back(ani["18"], Point<int16_t>(365, 252));
		sprites.emplace_back(ani["19"], Point<int16_t>(191, 208));
		sprites.emplace_back(frame, Point<int16_t>(400, 300));
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));
		sprites.emplace_back(Common["step"]["2"], Point<int16_t>(40, 0));

		burning_notice = Common["Burning"]["BurningNotice"];
		burning_count = Text(Text::Font::A12B, Text::Alignment::LEFT, Color::Name::CREAM, "1");

		charinfo = CharSelect["charInfo"];
		charslot = CharSelect["charSlot"]["0"];
		pagebase = pageNew["base"]["0"];
		pagenumber = Charset(pageNew["number"], Charset::Alignment::LEFT);
		pagenumberpos = pageNew["numberpos"];

		signpost[0] = CharSelect["adventure"]["0"];
		signpost[1] = CharSelect["knight"]["0"];
		signpost[2] = CharSelect["aran"]["0"];

		nametag = CharSelect["nameTag"];

		buttons[Buttons::CHARACTER_SELECT] = std::make_unique<MapleButton>(CharSelect["BtSelect"], character_sel_pos);
		buttons[Buttons::CHARACTER_NEW] = std::make_unique<MapleButton>(CharSelect["BtNew"], character_new_pos);
		buttons[Buttons::CHARACTER_DELETE] = std::make_unique<MapleButton>(CharSelect["BtDelete"], character_del_pos);
		buttons[Buttons::PAGELEFT] = std::make_unique<MapleButton>(CharSelect["pageL"], Point<int16_t>(98, 491));
		buttons[Buttons::PAGERIGHT] = std::make_unique<MapleButton>(CharSelect["pageR"], Point<int16_t>(485, 491));
		buttons[Buttons::CHANGEPIC] = std::make_unique<MapleButton>(Common["BtChangePIC"], Point<int16_t>(0, 80));
		buttons[Buttons::RESETPIC] = std::make_unique<MapleButton>(Login["WorldSelect"]["BtResetPIC"],
																   Point<int16_t>(0, 115));
		buttons[Buttons::EDITCHARLIST] = std::make_unique<MapleButton>(CharSelect["EditCharList"]["BtCharacter"],
																	   Point<int16_t>(-1, 47));
		buttons[Buttons::BACK] = std::make_unique<MapleButton>(Common["BtStart"], Point<int16_t>(0, 515));

		for (size_t i = 0; i < PAGESIZE; i++)
			buttons[Buttons::CHARACTER_SLOT0 + i] = std::make_unique<AreaButton>(get_character_slot_pos(i, 105, 144),
																				 Point<int16_t>(50, 90));

		if (require_pic == 0)
		{
			buttons[Buttons::CHANGEPIC]->set_active(false);
			buttons[Buttons::RESETPIC]->set_active(false);
		}

		levelset = Charset(CharSelect["lv"], Charset::Alignment::CENTER);
		namelabel = OutlinedText(Text::Font::A14B, Text::Alignment::CENTER, Color::Name::WHITE,
								 Color::Name::IRISHCOFFEE);

		for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
			infolabels[i] = OutlinedText(Text::Font::A11M, Text::Alignment::RIGHT, Color::Name::WHITE,
										 Color::Name::TOBACCOBROWN);

		for (auto& entry : characters)
		{
			charlooks.emplace_back(entry.look);
			nametags.emplace_back(nametag, Text::Font::A12M, entry.stats.name);
		}

		emptyslot_effect = CharSelect["character"]["0"];
		emptyslot = CharSelect["character"]["1"]["0"];

		selectedslot_effect[0] = CharSelect["effect"][0];
		selectedslot_effect[1] = CharSelect["effect"][1];

		charslotlabel = OutlinedText(Text::Font::A12M, Text::Alignment::LEFT, Color::Name::WHITE,
									 Color::Name::JAMBALAYA);
		charslotlabel.change_text(get_slot_text());

		update_buttons();

		if (characters_count > 0)
		{
			if (selected_character < characters_count)
				update_selected_character();
			else
				select_last_slot();
		}

		if (Configuration::get().get_auto_login())
		{
			SelectCharPicPacket(
				Configuration::get().get_auto_pic(),
				Configuration::get().get_auto_cid()
			).dispatch();
		}

		dimension = Point<int16_t>(800, 600);
	}

	void UICharSelect::draw(float inter) const
	{
		UIElement::draw_sprites(inter);

		version.draw(position + Point<int16_t>(707, 4));

		charslot.draw(position + Point<int16_t>(589, 106 - charslot_y));
		charslotlabel.draw(position + Point<int16_t>(702, 111 - charslot_y));

		for (Sprite sprite : world_sprites)
			sprite.draw(position, inter);

		std::string total = pad_number_with_leading_zero(page_count);
		std::string current = pad_number_with_leading_zero(selected_page + 1);

		std::list<uint8_t> fliplist = {2, 3, 6, 7};

		for (uint8_t i = 0; i < PAGESIZE; i++)
		{
			uint8_t index = i + selected_page * PAGESIZE;
			bool flip_character = std::find(fliplist.begin(), fliplist.end(), i) != fliplist.end();
			bool selectedslot = index == selected_character;

			if (index < characters_count)
			{
				Point<int16_t> charpos = get_character_slot_pos(i, 130, 234);
				DrawArgument chararg = DrawArgument(charpos, flip_character);

				nametags[index].draw(charpos + Point<int16_t>(2, 1));

				const StatsEntry& character_stats = characters[index].stats;

				if (selectedslot)
				{
					selectedslot_effect[1].draw(charpos + Point<int16_t>(-5, 16), inter);

					int8_t lvy = -115;
					Point<int16_t> pos_adj = Point<int16_t>(662, 365);

					charinfo.draw(position + charinfopos);

					std::string levelstr = std::to_string(character_stats.stats[MapleStat::Id::LEVEL]);
					int16_t lvx = levelset.draw(levelstr, pos_adj + Point<int16_t>(12, lvy));
					levelset.draw('l', pos_adj + Point<int16_t>(1 - lvx / 2, lvy));

					namelabel.draw(pos_adj + Point<int16_t>(10, -103));

					for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
					{
						Point<int16_t> labelpos = pos_adj + get_infolabel_pos(i);
						infolabels[i].draw(labelpos);
					}
				}

				uint8_t j = 0;
				uint16_t job = character_stats.stats[MapleStat::Id::JOB];

				if (job >= 0 && job < 1000)
					j = 0;
				else if (job >= 1000 && job < 2000)
					j = 1;
				else if (job >= 2000 && job < 2200)
					j = 2;
				else
					j = 0;

				signpost[j].draw(chararg);
				charlooks[index].draw(chararg, inter);

				if (selectedslot)
					selectedslot_effect[0].draw(charpos + Point<int16_t>(-5, -298), inter);
			}
			else if (i < slots)
			{
				Point<int16_t> emptyslotpos = get_character_slot_pos(i, 130, 234);

				emptyslot_effect.draw(emptyslotpos, inter);
				emptyslot.draw(DrawArgument(emptyslotpos, flip_character));
			}
		}

		UIElement::draw_buttons(inter);

		if (tab_active)
			tab.draw(position + tab_pos[tab_index] + Point<int16_t>(0, tab_move_pos));

		if (burning_character)
		{
			burning_notice.draw(position + Point<int16_t>(190, 502), inter);
			burning_count.draw(position + Point<int16_t>(149, 464));
		}

		pagebase.draw(position + pagepos);
		pagenumber.draw(current.substr(0, 1), position + pagepos + Point<int16_t>(pagenumberpos[0]));
		pagenumber.draw(current.substr(1, 1), position + pagepos + Point<int16_t>(pagenumberpos[1]));
		pagenumber.draw(total.substr(0, 1), position + pagepos + Point<int16_t>(pagenumberpos[2]));
		pagenumber.draw(total.substr(1, 1), position + pagepos + Point<int16_t>(pagenumberpos[3]));
	}

	void UICharSelect::update()
	{
		UIElement::update();

		if (show_timestamp)
		{
			if (timestamp > 0)
			{
				timestamp -= Constants::TIMESTEP;

				if (timestamp <= 176)
					charslot_y += 1;
			}
		}
		else
		{
			if (timestamp <= 176)
			{
				timestamp += Constants::TIMESTEP;

				if (charslot_y >= 0)
					charslot_y -= 1;
			}
		}

		if (tab_move && tab_move_pos < 4)
			tab_move_pos += 1;

		if (tab_move && tab_move_pos == 4)
			tab_move = false;

		if (!tab_move && tab_move_pos > 0)
			tab_move_pos -= 1;

		for (auto& charlook : charlooks)
			charlook.update(Constants::TIMESTEP);

		for (auto& effect : selectedslot_effect)
			effect.update();

		emptyslot_effect.update();

		if (burning_character)
			burning_notice.update();
	}

	void UICharSelect::doubleclick(Point<int16_t> cursorpos)
	{
		uint16_t button_index = selected_character + Buttons::CHARACTER_SLOT0;
		auto& btit = buttons[button_index];

		if (btit->is_active() && btit->bounds(position).contains(cursorpos) &&
			btit->get_state() == Button::State::NORMAL && button_index >= Buttons::CHARACTER_SLOT0)
			button_pressed(Buttons::CHARACTER_SELECT);
	}

	Cursor::State UICharSelect::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Rectangle<int16_t> charslot_bounds = Rectangle<int16_t>(
			worldpos,
			worldpos + world_dimensions
		);

		if (charslot_bounds.contains(cursorpos))
		{
			if (clicked)
			{
				show_timestamp = !show_timestamp;

				return Cursor::State::CLICKING;
			}
		}

		Cursor::State ret = clicked ? Cursor::State::CLICKING : Cursor::State::IDLE;

		for (auto& btit : buttons)
		{
			if (btit.second->is_active() && btit.second->bounds(position).contains(cursorpos))
			{
				if (btit.second->get_state() == Button::State::NORMAL)
				{
					Sound(Sound::Name::BUTTONOVER).play();

					btit.second->set_state(Button::State::MOUSEOVER);
					ret = Cursor::State::CANCLICK;
				}
				else if (btit.second->get_state() == Button::State::MOUSEOVER)
				{
					if (clicked)
					{
						Sound(Sound::Name::BUTTONCLICK).play();

						btit.second->set_state(button_pressed(btit.first));

						if (tab_active && btit.first == tab_map[tab_index])
							btit.second->set_state(Button::State::MOUSEOVER);

						ret = Cursor::State::IDLE;
					}
					else
					{
						if (!tab_active || btit.first != tab_map[tab_index])
							ret = Cursor::State::CANCLICK;
					}
				}
			}
			else if (btit.second->get_state() == Button::State::MOUSEOVER)
			{
				if (!tab_active || btit.first != tab_map[tab_index])
					btit.second->set_state(Button::State::NORMAL);
			}
		}

		return ret;
	}

	void UICharSelect::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				button_pressed(Buttons::BACK);
			}
			else if (keycode == KeyAction::Id::RETURN)
			{
				if (tab_active)
				{
					uint16_t btn_index = tab_map[tab_index];

					auto& btn = buttons[btn_index];
					auto state = btn->get_state();

					if (state != Button::State::DISABLED)
						button_pressed(btn_index);
				}
				else
				{
					button_pressed(Buttons::CHARACTER_SELECT);
				}
			}
			else
			{
				if (keycode == KeyAction::Id::TAB)
				{
					uint8_t prev_tab = tab_index;

					if (!tab_active)
					{
						tab_active = true;

						if (!buttons[Buttons::CHARACTER_SELECT]->is_active())
							tab_index++;
					}
					else
					{
						tab_index++;

						if (tab_index > 2)
						{
							tab_active = false;
							tab_index = 0;
						}
					}

					tab_move = true;
					tab_move_pos = 0;

					auto& prev_btn = buttons[tab_map[prev_tab]];
					auto prev_state = prev_btn->get_state();

					if (prev_state != Button::State::DISABLED)
						prev_btn->set_state(Button::State::NORMAL);

					if (tab_active)
					{
						auto& btn = buttons[tab_map[tab_index]];
						auto state = btn->get_state();

						if (state != Button::State::DISABLED)
							btn->set_state(Button::State::MOUSEOVER);
					}
				}
				else
				{
					uint8_t selected_index = selected_character;
					uint8_t index_total = std::min(characters_count,
												   static_cast<int8_t>((selected_page + 1) * PAGESIZE));

					uint8_t COLUMNS = 4;
					uint8_t columns = std::min(index_total, COLUMNS);

					uint8_t rows = std::floor((index_total - 1) / COLUMNS) + 1;

					int32_t current_col = 0;

					if (columns > 0)
					{
						div_t div = std::div(selected_index, columns);
						current_col = div.rem;
					}

					if (keycode == KeyAction::Id::UP)
					{
						uint8_t next_index = (selected_index - COLUMNS < 0 ? (selected_index - COLUMNS) + rows * COLUMNS
																		   : selected_index - COLUMNS);

						if (next_index == selected_character)
							return;

						if (next_index >= index_total)
							button_pressed(next_index - COLUMNS + Buttons::CHARACTER_SLOT0);
						else
							button_pressed(next_index + Buttons::CHARACTER_SLOT0);
					}
					else if (keycode == KeyAction::Id::DOWN)
					{
						uint8_t next_index = (selected_index + COLUMNS >= index_total ? current_col : selected_index +
																									  COLUMNS);

						if (next_index == selected_character)
							return;

						if (next_index > index_total)
							button_pressed(next_index + COLUMNS + Buttons::CHARACTER_SLOT0);
						else
							button_pressed(next_index + Buttons::CHARACTER_SLOT0);
					}
					else if (keycode == KeyAction::Id::LEFT)
					{
						if (selected_index != 0)
						{
							selected_index--;

							if (selected_index >= (selected_page + 1) * PAGESIZE - PAGESIZE)
								button_pressed(selected_index + Buttons::CHARACTER_SLOT0);
							else
								button_pressed(Buttons::PAGELEFT);
						}
					}
					else if (keycode == KeyAction::Id::RIGHT)
					{
						if (selected_index != characters_count - 1)
						{
							selected_index++;

							if (selected_index < index_total)
								button_pressed(selected_index + Buttons::CHARACTER_SLOT0);
							else
								button_pressed(Buttons::PAGERIGHT);
						}
					}
				}
			}
		}
	}

	UIElement::Type UICharSelect::get_type() const
	{
		return TYPE;
	}

	void UICharSelect::add_character(CharEntry&& character)
	{
		charlooks.emplace_back(character.look);
		nametags.emplace_back(nametag, Text::Font::A13M, character.stats.name);
		characters.emplace_back(std::forward<CharEntry>(character));

		characters_count++;
	}

	void UICharSelect::post_add_character()
	{
		bool page_matches = (characters_count - 1) / PAGESIZE == selected_page;

		if (!page_matches)
			button_pressed(Buttons::PAGERIGHT);

		update_buttons();

		if (characters_count > 1)
			select_last_slot();
		else
			update_selected_character();

		makeactive();

		charslotlabel.change_text(get_slot_text());
	}

	void UICharSelect::remove_character(int32_t id)
	{
		for (size_t i = 0; i < characters.size(); i++)
		{
			if (characters[i].id == id)
			{
				charlooks.erase(charlooks.begin() + i);
				nametags.erase(nametags.begin() + i);
				characters.erase(characters.begin() + i);

				characters_count--;

				if (selected_page > 0)
				{
					bool page_matches = (characters_count - 1) / PAGESIZE == selected_page;

					if (!page_matches)
						button_pressed(Buttons::PAGELEFT);
				}

				update_buttons();

				if (selected_character < characters_count)
					update_selected_character();
				else
					select_last_slot();

				return;
			}
		}
	}

	const CharEntry& UICharSelect::get_character(int32_t id)
	{
		for (auto& character : characters)
			if (character.id == id)
				return character;

		std::cout << "Invalid character id: [" << id << "]" << std::endl;

		static const CharEntry null_character = {{}, {}, 0};

		return null_character;
	}

	Button::State UICharSelect::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::CHARACTER_SELECT:
			{
				if (characters.size() > 0)
				{
					Setting<DefaultCharacter>::get().save(selected_character);
					int32_t id = characters[selected_character].id;

					switch (require_pic)
					{
						case 0:
						{
							std::function<void()> onok = [&]()
							{
								request_pic();
							};

							UI::get().emplace<UILoginNotice>(UILoginNotice::Message::PIC_REQ, onok);
							break;
						}
						case 1:
						{
							std::function<void(const std::string&)> onok = [id](const std::string& pic)
							{
								SelectCharPicPacket(pic, id).dispatch();
							};

							UI::get().emplace<UISoftKey>(onok);
							break;
						}
						case 2:
						{
							SelectCharPacket(id).dispatch();
							break;
						}
					}
				}

				break;
			}
			case Buttons::CHARACTER_NEW:
			{
				Sound(Sound::Name::SCROLLUP).play();

				deactivate();

				tab_index = 0;
				tab_active = false;
				tab_move = false;
				tab_move_pos = 0;

				UI::get().emplace<UIRaceSelect>();
				break;
			}
			case Buttons::CHARACTER_DELETE:
			{
				int32_t id = characters[selected_character].id;

				switch (require_pic)
				{
					case 0:
					{
						std::function<void()> onok = [&]()
						{
							charslotlabel.change_text(get_slot_text());
						};

						UI::get().emplace<UILoginNotice>(UILoginNotice::Message::CHAR_DEL_FAIL_NO_PIC, onok);
						break;
					}
					case 1:
					{
						std::function<void()> oncancel = [&]()
						{
							charslotlabel.change_text(get_slot_text());
						};

						std::function<void()> onok = [&, id, oncancel]()
						{
							std::function<void(const std::string&)> onok = [&, id](const std::string& pic)
							{
								DeleteCharPicPacket(pic, id).dispatch();
								charslotlabel.change_text(get_slot_text());
							};

							UI::get().emplace<UISoftKey>(onok, oncancel);
						};

						const StatsEntry& character_stats = characters[selected_character].stats;
						uint16_t cjob = character_stats.stats[MapleStat::Id::JOB];

						if (cjob < 1000)
							UI::get().emplace<UILoginNotice>(UILoginNotice::Message::DELETE_CONFIRMATION, onok,
															 oncancel);
						else
							UI::get().emplace<UILoginNotice>(UILoginNotice::Message::CASH_ITEMS_CONFIRM_DELETION, onok);

						break;
					}
					case 2:
					{
						DeleteCharPacket(id).dispatch();
						charslotlabel.change_text(get_slot_text());
						break;
					}
				}

				break;
			}
			case Buttons::PAGELEFT:
			{
				uint8_t previous_page = selected_page;

				if (selected_page > 0)
					selected_page--;
				else if (characters_count > PAGESIZE)
					selected_page = page_count - 1;

				if (previous_page != selected_page)
					update_buttons();

				select_last_slot();
				break;
			}
			case Buttons::PAGERIGHT:
			{
				uint8_t previous_page = selected_page;

				if (selected_page < page_count - 1)
					selected_page++;
				else
					selected_page = 0;

				if (previous_page != selected_page)
				{
					update_buttons();

					button_pressed(Buttons::CHARACTER_SLOT0);
				}

				break;
			}
			case Buttons::CHANGEPIC:
			{
				break;
			}
			case Buttons::RESETPIC:
			{
				std::string url = Configuration::get().get_resetpic();

#ifdef _WIN32
				ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
				break;
			}
			case Buttons::EDITCHARLIST:
			{
				break;
			}
			case Buttons::BACK:
			{
				deactivate();

				Sound(Sound::Name::SCROLLUP).play();

				if (auto worldselect = UI::get().get_element<UIWorldSelect>())
					worldselect->makeactive();

				break;
			}
			default:
			{
				if (buttonid >= Buttons::CHARACTER_SLOT0)
				{
					uint8_t previous_character = selected_character;
					selected_character = buttonid - Buttons::CHARACTER_SLOT0 + selected_page * PAGESIZE;

					if (previous_character != selected_character)
					{
						if (previous_character < characters_count)
						{
							charlooks[previous_character].set_stance(Stance::Id::STAND1);
							nametags[previous_character].set_selected(false);
						}

						if (selected_character < characters_count)
							update_selected_character();
					}
				}

				break;
			}
		}

		return Button::State::NORMAL;
	}

	void UICharSelect::update_buttons()
	{
		for (uint8_t i = 0; i < PAGESIZE; i++)
		{
			uint8_t index = i + selected_page * PAGESIZE;

			if (index < characters_count)
				buttons[Buttons::CHARACTER_SLOT0 + i]->set_state(Button::State::NORMAL);
			else
				buttons[Buttons::CHARACTER_SLOT0 + i]->set_state(Button::State::DISABLED);
		}

		if (characters_count >= slots)
			buttons[Buttons::CHARACTER_NEW]->set_state(Button::State::DISABLED);
		else
			buttons[Buttons::CHARACTER_NEW]->set_state(Button::State::NORMAL);

		bool character_found = false;

		for (int8_t i = PAGESIZE - 1; i >= 0; i--)
		{
			uint8_t index = i + selected_page * PAGESIZE;

			if (index < characters_count)
			{
				character_found = true;

				break;
			}
		}

		buttons[Buttons::CHARACTER_SELECT]->set_active(character_found);
		buttons[Buttons::CHARACTER_DELETE]->set_state(
			character_found ? Button::State::NORMAL : Button::State::DISABLED);
	}

	void UICharSelect::update_selected_character()
	{
		Sound(Sound::Name::CHARSELECT).play();

		charlooks[selected_character].set_stance(Stance::Id::WALK1);
		nametags[selected_character].set_selected(true);

		const StatsEntry& character_stats = characters[selected_character].stats;

		namelabel.change_text(character_stats.name);

		for (size_t i = 0; i < InfoLabel::NUM_LABELS; i++)
			infolabels[i].change_text(get_infolabel(i, character_stats));
	}

	void UICharSelect::select_last_slot()
	{
		for (int8_t i = PAGESIZE - 1; i >= 0; i--)
		{
			uint8_t index = i + selected_page * PAGESIZE;

			if (index < characters_count)
			{
				button_pressed(i + Buttons::CHARACTER_SLOT0);

				return;
			}
		}
	}

	std::string UICharSelect::get_slot_text()
	{
		show_timestamp = true;
		timestamp = 7 * 1000;
		charslot_y = 0;

		return pad_number_with_leading_zero(characters_count) + "/" + pad_number_with_leading_zero(slots);
	}

	std::string UICharSelect::pad_number_with_leading_zero(uint8_t value) const
	{
		std::string return_val = std::to_string(value);
		return_val.insert(return_val.begin(), 2 - return_val.length(), '0');

		return return_val;
	}

	Point<int16_t> UICharSelect::get_character_slot_pos(size_t index, uint16_t x_adj, uint16_t y_adj) const
	{
		int16_t x = 125 * (index % 4);
		int16_t y = 200 * (index > 3);

		return Point<int16_t>(x + x_adj, y + y_adj);
	}

	Point<int16_t> UICharSelect::get_infolabel_pos(size_t index) const
	{
		switch (index)
		{
			case InfoLabel::JOB:
				return Point<int16_t>(73, -71);
			case InfoLabel::STR:
				return Point<int16_t>(1, -47);
			case InfoLabel::DEX:
				return Point<int16_t>(1, -24);
			case InfoLabel::INT:
				return Point<int16_t>(72, -47);
			case InfoLabel::LUK:
				return Point<int16_t>(72, -24);
			case InfoLabel::NUM_LABELS:
				break;
			default:
				break;
		}

		return Point<int16_t>();
	}

	std::string UICharSelect::get_infolabel(size_t index, StatsEntry character_stats) const
	{
		switch (index)
		{
			case InfoLabel::JOB:
				return Job(character_stats.stats[MapleStat::Id::JOB]).get_name();
			case InfoLabel::STR:
				return std::to_string(character_stats.stats[MapleStat::Id::STR]);
			case InfoLabel::DEX:
				return std::to_string(character_stats.stats[MapleStat::Id::DEX]);
			case InfoLabel::INT:
				return std::to_string(character_stats.stats[MapleStat::Id::INT]);
			case InfoLabel::LUK:
				return std::to_string(character_stats.stats[MapleStat::Id::LUK]);
			case InfoLabel::NUM_LABELS:
				break;
			default:
				break;
		}

		return "";
	}

	void UICharSelect::request_pic()
	{
		std::function<void(const std::string&)> enterpic = [&](const std::string& entered_pic)
		{
			std::function<void(const std::string&)> verifypic = [&, entered_pic](const std::string& verify_pic)
			{
				if (entered_pic == verify_pic)
				{
					RegisterPicPacket(
						characters[selected_character].id,
						entered_pic
					).dispatch();
				}
				else
				{
					std::function<void()> onreturn = [&]()
					{
						request_pic();
					};

					UI::get().emplace<UILoginNotice>(UILoginNotice::Message::PASSWORD_IS_INCORRECT, onreturn);
				}
			};

			UI::get().emplace<UISoftKey>(verifypic, []()
			{}, "Please re-enter your new PIC.", Point<int16_t>(24, 0));
		};

		UI::get().emplace<UISoftKey>(enterpic, []()
		{}, "Your new PIC must at least be 6 characters long.", Point<int16_t>(24, 0));
	}
}