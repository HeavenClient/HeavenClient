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
#include "UIAranCreation.h"

#include "UICharSelect.h"
#include "UILoginNotice.h"
#include "UIRaceSelect.h"

#include "../UI.h"

#include "../Components/MapleButton.h"

#include "../../Configuration.h"

#include "../../Audio/Audio.h"
#include "../../Data/ItemData.h"

#include "../../Net/Packets/CharCreationPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UIAranCreation::UIAranCreation() : UIElement(Point<int16_t>(0, 0), Point<int16_t>(800, 600))
	{
		gender = false;
		charSet = false;
		named = false;

		std::string version_text = Configuration::get().get_version();
		version = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::LEMONGRASS, "Ver. " + version_text);

		nl::node Login = nl::nx::ui["Login.img"];
		nl::node Common = Login["Common"];
		nl::node CustomizeChar = Login["CustomizeChar"]["2000"];
		nl::node back = nl::nx::map001["Back"]["login.img"]["back"];
		nl::node board = CustomizeChar["board"];
		nl::node genderSelect = CustomizeChar["genderSelect"];
		nl::node frame = nl::nx::mapLatest["Obj"]["login.img"]["Common"]["frame"]["2"]["0"];

		sky = back["2"];
		cloud = back["27"];

		sprites.emplace_back(back["33"], Point<int16_t>(256, 299));
		sprites.emplace_back(back["34"], Point<int16_t>(587, 157));
		sprites_gender_select.emplace_back(board["genderTop"], Point<int16_t>(491, 168));
		sprites_gender_select.emplace_back(board["boardMid"], Point<int16_t>(491, 220));
		sprites_gender_select.emplace_back(board["boardBottom"], Point<int16_t>(491, 313));
		sprites_lookboard.emplace_back(CustomizeChar["charSet"], Point<int16_t>(473, 103));

		for (size_t i = 0; i <= 6; i++)
			sprites_lookboard.emplace_back(CustomizeChar["avatarSel"][i]["normal"], Point<int16_t>(504, 187 + (i * 18)));

		buttons[Buttons::BT_CHARC_GENDER_M] = std::make_unique<MapleButton>(genderSelect["male"], Point<int16_t>(439, 106));
		buttons[Buttons::BT_CHARC_GEMDER_F] = std::make_unique<MapleButton>(genderSelect["female"], Point<int16_t>(437, 106));
		buttons[Buttons::BT_CHARC_SKINL] = std::make_unique<MapleButton>(CustomizeChar["BtLeft"], Point<int16_t>(562, 187 + (2 * 18)));
		buttons[Buttons::BT_CHARC_SKINR] = std::make_unique<MapleButton>(CustomizeChar["BtRight"], Point<int16_t>(699, 187 + (2 * 18)));
		buttons[Buttons::BT_CHARC_OK] = std::make_unique<MapleButton>(CustomizeChar["BtYes"], Point<int16_t>(520, 397));
		buttons[Buttons::BT_CHARC_CANCEL] = std::make_unique<MapleButton>(CustomizeChar["BtNo"], Point<int16_t>(594, 397));

		buttons[Buttons::BT_CHARC_SKINL]->set_active(false);
		buttons[Buttons::BT_CHARC_SKINR]->set_active(false);

		nameboard = CustomizeChar["charName"];
		namechar = Textfield(Text::Font::A13M, Text::Alignment::LEFT, Color::Name::WHITE, Rectangle<int16_t>(Point<int16_t>(524, 196), Point<int16_t>(630, 253)), 12);

		sprites.emplace_back(frame, Point<int16_t>(400, 300));
		sprites.emplace_back(Common["frame"], Point<int16_t>(400, 300));
		sprites.emplace_back(Common["step"]["3"], Point<int16_t>(40, 0));

		buttons[Buttons::BT_BACK] = std::make_unique<MapleButton>(Login["Common"]["BtStart"], Point<int16_t>(0, 515));

		namechar.set_state(Textfield::DISABLED);

		namechar.set_enter_callback(
				[&](std::string)
				{
					button_pressed(Buttons::BT_CHARC_OK);
				}
		);

		namechar.set_key_callback(
				KeyAction::Id::ESCAPE,
				[&]()
				{
					button_pressed(Buttons::BT_CHARC_CANCEL);
				}
		);

		facename = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		hairname = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		bodyname = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		topname = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		botname = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		shoename = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);
		wepname = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::BLACK);

		nl::node mkinfo = nl::nx::etc["MakeCharInfo.img"]["Info"];

		for (size_t i = 0; i < 2; i++)
		{
			bool f;
			nl::node CharGender;

			if (i == 0)
			{
				f = true;
				CharGender = mkinfo["CharFemale"];
			}
			else
			{
				f = false;
				CharGender = mkinfo["CharMale"];
			}

			for (auto node : CharGender)
			{
				int num = stoi(node.name());

				for (auto idnode : node)
				{
					int32_t value = idnode;

					switch (num)
					{
						case 0:
							faces[f].push_back(value);
							break;
						case 1:
							hairs[f].push_back(value);
							break;
						case 2:
							haircolors[f].push_back(static_cast<uint8_t>(value));
							break;
						case 3:
							skins[f].push_back(static_cast<uint8_t>(value));
							break;
						case 4:
							tops[f].push_back(value);
							break;
						case 5:
							bots[f].push_back(value);
							break;
						case 6:
							shoes[f].push_back(value);
							break;
						case 7:
							weapons[f].push_back(value);
							break;
					}
				}
			}
		}

		female = false;
		randomize_look();

		newchar.set_direction(true);

		cloudfx = 200.0f;
	}

	void UIAranCreation::draw(float inter) const
	{
		for (size_t i = 0; i < 2; i++)
			for (size_t k = 0; k < 800; k += sky.width())
				sky.draw(Point<int16_t>(k, (400 * i) - 100));

		int16_t cloudx = static_cast<int16_t>(cloudfx) % 800;
		cloud.draw(Point<int16_t>(cloudx - cloud.width(), 310));
		cloud.draw(Point<int16_t>(cloudx, 310));
		cloud.draw(Point<int16_t>(cloudx + cloud.width(), 310));

		if (!gender)
		{
			for (size_t i = 0; i < sprites_gender_select.size(); i++)
			{
				if (i == 1)
				{
					for (size_t f = 0; f <= 4; f++)
						sprites_gender_select[i].draw(position + Point<int16_t>(0, 24 * f), inter);
				}
				else
				{
					sprites_gender_select[i].draw(position, inter);
				}
			}

			UIElement::draw(inter);

			newchar.draw(Point<int16_t>(394, 339), inter);
		}
		else
		{
			if (!charSet)
			{
				UIElement::draw_sprites(inter);

				for (auto& sprite : sprites_lookboard)
					sprite.draw(position, inter);

				facename.draw(Point<int16_t>(647, 183 + (0 * 18)));
				hairname.draw(Point<int16_t>(647, 183 + (1 * 18)));
				bodyname.draw(Point<int16_t>(647, 183 + (2 * 18)));
				topname.draw(Point<int16_t>(647, 183 + (3 * 18)));
				botname.draw(Point<int16_t>(647, 183 + (4 * 18)));
				shoename.draw(Point<int16_t>(647, 183 + (5 * 18)));
				wepname.draw(Point<int16_t>(647, 183 + (6 * 18)));

				newchar.draw(Point<int16_t>(394, 339), inter);

				UIElement::draw_buttons(inter);
			}
			else
			{
				if (!named)
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(Point<int16_t>(489, 106));

					namechar.draw(position);
					newchar.draw(Point<int16_t>(394, 339), inter);

					UIElement::draw_buttons(inter);
				}
				else
				{
					UIElement::draw_sprites(inter);

					nameboard.draw(Point<int16_t>(489, 106));

					UIElement::draw_buttons(inter);

					for (auto& sprite : sprites_keytype)
						sprite.draw(position, inter);
				}
			}
		}

		version.draw(position + Point<int16_t>(707, 4));
	}

	void UIAranCreation::update()
	{
		if (!gender)
		{
			for (auto& sprite : sprites_gender_select)
				sprite.update();

			newchar.update(Constants::TIMESTEP);
		}
		else
		{
			if (!charSet)
			{
				for (auto& sprite : sprites_lookboard)
					sprite.update();

				newchar.update(Constants::TIMESTEP);
			}
			else
			{
				if (!named)
				{
					namechar.update(position);
					newchar.update(Constants::TIMESTEP);
				}
				else
				{
					for (auto& sprite : sprites_keytype)
						sprite.update();

					namechar.set_state(Textfield::State::DISABLED);
				}
			}
		}

		UIElement::update();

		cloudfx += 0.25f;
	}

	Cursor::State UIAranCreation::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		if (namechar.get_state() == Textfield::State::NORMAL)
		{
			if (namechar.get_bounds().contains(cursorpos))
			{
				if (clicked)
				{
					namechar.set_state(Textfield::State::FOCUSED);

					return Cursor::State::CLICKING;
				}
				else
				{
					return Cursor::State::IDLE;
				}
			}
		}

		return UIElement::send_cursor(clicked, cursorpos);
	}

	void UIAranCreation::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
				button_pressed(Buttons::BT_CHARC_CANCEL);
			else if (keycode == KeyAction::Id::RETURN)
				button_pressed(Buttons::BT_CHARC_OK);
		}
	}

	UIElement::Type UIAranCreation::get_type() const
	{
		return TYPE;
	}

	void UIAranCreation::send_naming_result(bool nameused)
	{
		if (!named)
		{
			if (!nameused)
			{
				named = true;

				std::string cname = namechar.get_text();
				int32_t cface = faces[female][face];
				int32_t chair = hairs[female][hair];
				uint8_t chairc = haircolors[female][haircolor];
				uint8_t cskin = skins[female][skin];
				int32_t ctop = tops[female][top];
				int32_t cbot = bots[female][bot];
				int32_t cshoe = shoes[female][shoe];
				int32_t cwep = weapons[female][weapon];

				CreateCharPacket(cname, 2, cface, chair, chairc, cskin, ctop, cbot, cshoe, cwep, female).dispatch();

				auto onok = [&](bool alternate)
				{
					Sound(Sound::Name::SCROLLUP).play();

					UI::get().remove(UIElement::Type::LOGINNOTICE_CONFIRM);
					UI::get().remove(UIElement::Type::LOGINNOTICE);
					UI::get().remove(UIElement::Type::CLASSCREATION);
					UI::get().remove(UIElement::Type::RACESELECT);

					if (auto charselect = UI::get().get_element<UICharSelect>())
						charselect->post_add_character();
				};

				UI::get().emplace<UIKeySelect>(onok, true);
			}
			else
			{
				auto onok = [&]()
				{
					namechar.set_state(Textfield::State::FOCUSED);

					buttons[Buttons::BT_CHARC_OK]->set_state(Button::State::NORMAL);
					buttons[Buttons::BT_CHARC_CANCEL]->set_state(Button::State::NORMAL);
				};

				UI::get().emplace<UILoginNotice>(UILoginNotice::Message::NAME_IN_USE, onok);
			}
		}
	}

	Button::State UIAranCreation::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::BT_CHARC_OK:
			{
				if (!gender)
				{
					gender = true;

					buttons[Buttons::BT_CHARC_GENDER_M]->set_active(false);
					buttons[Buttons::BT_CHARC_GEMDER_F]->set_active(false);

					buttons[Buttons::BT_CHARC_SKINL]->set_active(true);
					buttons[Buttons::BT_CHARC_SKINR]->set_active(true);

					buttons[Buttons::BT_CHARC_OK]->set_position(Point<int16_t>(533, 368));
					buttons[Buttons::BT_CHARC_CANCEL]->set_position(Point<int16_t>(607, 368));

					return Button::State::NORMAL;
				}
				else
				{
					if (!charSet)
					{
						charSet = true;

						buttons[Buttons::BT_CHARC_SKINL]->set_active(false);
						buttons[Buttons::BT_CHARC_SKINR]->set_active(false);

						buttons[Buttons::BT_CHARC_OK]->set_position(Point<int16_t>(523, 243));
						buttons[Buttons::BT_CHARC_CANCEL]->set_position(Point<int16_t>(597, 243));

						namechar.set_state(Textfield::State::FOCUSED);

						return Button::State::NORMAL;
					}
					else
					{
						if (!named)
						{
							std::string name = namechar.get_text();

							if (name.size() <= 0)
							{
								return Button::State::NORMAL;
							}
							else if (name.size() >= 4)
							{
								namechar.set_state(Textfield::State::DISABLED);

								buttons[Buttons::BT_CHARC_OK]->set_state(Button::State::DISABLED);
								buttons[Buttons::BT_CHARC_CANCEL]->set_state(Button::State::DISABLED);

								if (auto raceselect = UI::get().get_element<UIRaceSelect>())
								{
									if (raceselect->check_name(name))
									{
										NameCharPacket(name).dispatch();

										return Button::State::IDENTITY;
									}
								}

								std::function<void()> okhandler = [&]()
								{
									namechar.set_state(Textfield::State::FOCUSED);

									buttons[Buttons::BT_CHARC_OK]->set_state(Button::State::NORMAL);
									buttons[Buttons::BT_CHARC_CANCEL]->set_state(Button::State::NORMAL);
								};

								UI::get().emplace<UILoginNotice>(UILoginNotice::Message::ILLEGAL_NAME, okhandler);

								return Button::State::NORMAL;
							}
							else
							{
								namechar.set_state(Textfield::State::DISABLED);

								buttons[Buttons::BT_CHARC_OK]->set_state(Button::State::DISABLED);
								buttons[Buttons::BT_CHARC_CANCEL]->set_state(Button::State::DISABLED);

								std::function<void()> okhandler = [&]()
								{
									namechar.set_state(Textfield::State::FOCUSED);

									buttons[Buttons::BT_CHARC_OK]->set_state(Button::State::NORMAL);
									buttons[Buttons::BT_CHARC_CANCEL]->set_state(Button::State::NORMAL);
								};

								UI::get().emplace<UILoginNotice>(UILoginNotice::Message::ILLEGAL_NAME, okhandler);

								return Button::State::IDENTITY;
							}
						}
						else
						{
							return Button::State::NORMAL;
						}
					}
				}
			}
			case BT_BACK:
			{
				Sound(Sound::Name::SCROLLUP).play();

				UI::get().remove(UIElement::Type::CLASSCREATION);
				UI::get().emplace<UIRaceSelect>();

				return Button::State::NORMAL;
			}
			case Buttons::BT_CHARC_CANCEL:
			{
				if (charSet)
				{
					charSet = false;

					buttons[Buttons::BT_CHARC_SKINL]->set_active(true);
					buttons[Buttons::BT_CHARC_SKINR]->set_active(true);

					buttons[Buttons::BT_CHARC_OK]->set_position(Point<int16_t>(533, 368));
					buttons[Buttons::BT_CHARC_CANCEL]->set_position(Point<int16_t>(607, 368));

					namechar.set_state(Textfield::State::DISABLED);

					return Button::State::NORMAL;
				}
				else
				{
					if (gender)
					{
						gender = false;

						buttons[Buttons::BT_CHARC_GENDER_M]->set_active(true);
						buttons[Buttons::BT_CHARC_GEMDER_F]->set_active(true);

						buttons[Buttons::BT_CHARC_SKINL]->set_active(false);
						buttons[Buttons::BT_CHARC_SKINR]->set_active(false);

						buttons[Buttons::BT_CHARC_OK]->set_position(Point<int16_t>(520, 397));
						buttons[Buttons::BT_CHARC_CANCEL]->set_position(Point<int16_t>(594, 397));

						return Button::State::NORMAL;
					}
					else
					{
						button_pressed(Buttons::BT_BACK);

						return Button::State::NORMAL;
					}
				}
			}
			case Buttons::BT_CHARC_SKINL:
			{
				skin = (skin > 0) ? skin - 1 : skins[female].size() - 1;
				newchar.set_body(skins[female][skin]);
				bodyname.change_text(newchar.get_body()->get_name());

				return Button::State::NORMAL;
			}
			case Buttons::BT_CHARC_SKINR:
			{
				skin = (skin < skins[female].size() - 1) ? skin + 1 : 0;
				newchar.set_body(skins[female][skin]);
				bodyname.change_text(newchar.get_body()->get_name());

				return Button::State::NORMAL;
			}
			case Buttons::BT_CHARC_GENDER_M:
			{
				if (female)
				{
					female = false;
					randomize_look();
				}

				return Button::State::NORMAL;
			}
			case Buttons::BT_CHARC_GEMDER_F:
			{
				if (!female)
				{
					female = true;
					randomize_look();
				}

				return Button::State::NORMAL;
			}
		}

		return Button::State::PRESSED;
	}

	void UIAranCreation::randomize_look()
	{
		hair = 0;
		face = 0;
		skin = randomizer.next_int(skins[female].size());
		haircolor = 0;
		top = 0;
		bot = 0;
		shoe = 0;
		weapon = 0;

		newchar.set_body(skins[female][skin]);
		newchar.set_face(faces[female][face]);
		newchar.set_hair(hairs[female][hair] + haircolors[female][haircolor]);
		newchar.add_equip(tops[female][top]);
		newchar.add_equip(bots[female][bot]);
		newchar.add_equip(shoes[female][shoe]);
		newchar.add_equip(weapons[female][weapon]);

		bodyname.change_text(newchar.get_body()->get_name());
		facename.change_text(newchar.get_face()->get_name());
		hairname.change_text(newchar.get_hair()->get_name());
		topname.change_text(get_equipname(EquipSlot::Id::TOP));
		botname.change_text(get_equipname(EquipSlot::Id::BOTTOM));
		shoename.change_text(get_equipname(EquipSlot::Id::SHOES));
		wepname.change_text(get_equipname(EquipSlot::Id::WEAPON));
	}

	const std::string& UIAranCreation::get_equipname(EquipSlot::Id slot) const
	{
		if (int32_t item_id = newchar.get_equips().get_equip(slot))
		{
			return ItemData::get(item_id).get_name();
		}
		else
		{
			static const std::string& nullstr = "Missing name.";

			return nullstr;
		}
	}
}