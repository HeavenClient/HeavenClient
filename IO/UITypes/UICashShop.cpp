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
#include "UICashShop.h"

#include "../UI.h"
#include "../Window.h"

#include "../Components/MapleButton.h"

#include "../../Gameplay/Stage.h"

#include "../../Net/Packets/GameplayPackets.h"
#include "../../Net/Packets/LoginPackets.h"

#include "Timer.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	UICashShop::UICashShop() : preview_index(0), menu_index(1), promotion_index(0), mvp_grade(1), mvp_exp(
		0.07f), list_offset(0)
	{
		nl::node CashShop = nl::nx::ui["CashShop.img"];
		nl::node Base = CashShop["Base"];
		nl::node backgrnd = Base["backgrnd"];
		nl::node BestNew = Base["BestNew"];
		nl::node Preview = Base["Preview"];
		nl::node CSTab = CashShop["CSTab"];
		nl::node CSGLChargeNX = CSTab["CSGLChargeNX"];
		nl::node CSStatus = CashShop["CSStatus"];
		nl::node CSPromotionBanner = CashShop["CSPromotionBanner"];
		nl::node CSMVPBanner = CashShop["CSMVPBanner"];
		nl::node CSItemSearch = CashShop["CSItemSearch"];
		nl::node CSChar = CashShop["CSChar"];
		nl::node CSList = CashShop["CSList"];
		nl::node CSEffect = CashShop["CSEffect"];

		sprites.emplace_back(backgrnd);
		sprites.emplace_back(BestNew, Point<int16_t>(139, 346));

		BestNew_dim = Texture(BestNew).get_dimensions();

		for (size_t i = 0; i < 3; i++)
			preview_sprites[i] = Preview[i];

		for (size_t i = 0; i < 3; i++)
			buttons[Buttons::BtPreview1 + i] = std::make_unique<TwoSpriteButton>(Base["Tab"]["Disable"][i],
																				 Base["Tab"]["Enable"][i],
																				 Point<int16_t>(957 + (i * 17), 46));

		buttons[Buttons::BtPreview1]->set_state(Button::State::PRESSED);

		buttons[Buttons::BtExit] = std::make_unique<MapleButton>(CSTab["BtExit"], Point<int16_t>(5, 728));
		buttons[Buttons::BtChargeNX] = std::make_unique<MapleButton>(CSGLChargeNX["BtChargeNX"],
																	 Point<int16_t>(5, 554));
		buttons[Buttons::BtChargeRefresh] = std::make_unique<MapleButton>(CSGLChargeNX["BtChargeRefresh"],
																		  Point<int16_t>(92, 554));

		for (size_t i = 0; i < 9; i++)
			menu_tabs[i] = CSTab["Tab"][i];

		buttons[Buttons::BtChargeRefresh] = std::make_unique<MapleButton>(CSGLChargeNX["BtChargeRefresh"],
																		  Point<int16_t>(92, 554));
		buttons[Buttons::BtWish] = std::make_unique<MapleButton>(CSStatus["BtWish"], Point<int16_t>(226, 6));
		buttons[Buttons::BtMileage] = std::make_unique<MapleButton>(CSStatus["BtMileage"], Point<int16_t>(869, 4));
		buttons[Buttons::BtHelp] = std::make_unique<MapleButton>(CSStatus["BtHelp"], Point<int16_t>(997, 4));
		buttons[Buttons::BtCoupon] = std::make_unique<MapleButton>(CSStatus["BtCoupon"], Point<int16_t>(950, 4));

		Charset tab;

		job_label = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::SUPERNOVA, "Illium");
		name_label = Text(Text::Font::A11B, Text::Alignment::LEFT, Color::Name::WHITE, "ShomeiZekkou");

		promotion_pos = Point<int16_t>(138, 40);
		sprites.emplace_back(CSPromotionBanner["shadow"], promotion_pos);

		promotion_sprites.emplace_back(CSPromotionBanner["basic"]);

		buttons[Buttons::BtNext] = std::make_unique<MapleButton>(CSPromotionBanner["BtNext"], promotion_pos);
		buttons[Buttons::BtPrev] = std::make_unique<MapleButton>(CSPromotionBanner["BtPrev"], promotion_pos);

		for (size_t i = 0; i < 7; i++)
			mvp_sprites[i] = CSMVPBanner["grade"][i];

		mvp_pos = Point<int16_t>(63, 681);
		buttons[Buttons::BtDetailPackage] = std::make_unique<MapleButton>(CSMVPBanner["BtDetailPackage"], mvp_pos);
		buttons[Buttons::BtNonGrade] = std::make_unique<MapleButton>(CSMVPBanner["BtNonGrade"], mvp_pos);

		buttons[Buttons::BtDetailPackage]->set_active(mvp_grade);
		buttons[Buttons::BtNonGrade]->set_active(!mvp_grade);

		mvp_gauge = Gauge(
			Gauge::Type::CASHSHOP,
			CSMVPBanner["gage"][0],
			CSMVPBanner["gage"][2],
			CSMVPBanner["gage"][1],
			84,
			0.0f
		);

		Point<int16_t> search_pos = Point<int16_t>(0, 36);
		sprites.emplace_back(CSItemSearch["backgrnd"], search_pos);
		sprites.emplace_back(CSItemSearch["search"], search_pos + Point<int16_t>(35, 8));

		buttons[Buttons::BtBuyAvatar] = std::make_unique<MapleButton>(CSChar["BtBuyAvatar"], Point<int16_t>(642, 305));
		buttons[Buttons::BtDefaultAvatar] = std::make_unique<MapleButton>(CSChar["BtDefaultAvatar"],
																		  Point<int16_t>(716, 305));
		buttons[Buttons::BtInventory] = std::make_unique<MapleButton>(CSChar["BtInventory"], Point<int16_t>(938, 305));
		buttons[Buttons::BtSaveAvatar] = std::make_unique<MapleButton>(CSChar["BtSaveAvatar"],
																	   Point<int16_t>(864, 305));
		buttons[Buttons::BtTakeoffAvatar] = std::make_unique<MapleButton>(CSChar["BtTakeoffAvatar"],
																		  Point<int16_t>(790, 305));

		charge_charset = Charset(CSGLChargeNX["Number"], Charset::Alignment::RIGHT);

		item_base = CSList["Base"];
		item_line = Base["line"];
		item_none = Base["noItem"];

		for (nl::node item_label : CSEffect)
			item_labels.emplace_back(item_label);

		items.push_back({5220000, Item::Label::HOT, 34000, 11});
		items.push_back({5220000, Item::Label::HOT, 34000, 11});
		items.push_back({5220000, Item::Label::HOT, 0, 0});
		items.push_back({5220000, Item::Label::HOT, 0, 0});
		items.push_back({5220000, Item::Label::HOT, 10000, 11});
		items.push_back({5220000, Item::Label::NEW, 0, 0});
		items.push_back({5220000, Item::Label::SALE, 7000, 0});
		items.push_back({5220000, Item::Label::NEW, 13440, 0});
		items.push_back({5220000, Item::Label::NEW, 7480, 0});
		items.push_back({5220000, Item::Label::NEW, 7480, 0});
		items.push_back({5220000, Item::Label::NEW, 7480, 0});
		items.push_back({5220000, Item::Label::NONE, 12000, 11});
		items.push_back({5220000, Item::Label::NONE, 22000, 11});
		items.push_back({5220000, Item::Label::NONE, 0, 0});
		items.push_back({5220000, Item::Label::NONE, 0, 0});
		items.push_back({5220000, Item::Label::MASTER, 0, 15});

		for (size_t i = 0; i < MAX_ITEMS; i++)
		{
			div_t div = std::div(i, 7);

			buttons[Buttons::BtBuy + i] = std::make_unique<MapleButton>(CSList["BtBuy"], Point<int16_t>(146, 523) +
																						 Point<int16_t>(124 * div.rem,
																										205 *
																										div.quot));

			item_name[i] = Text(Text::Font::A11B, Text::Alignment::CENTER, Color::Name::MINESHAFT);
			item_price[i] = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::GRAY);
			item_discount[i] = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::SILVERCHALICE);
			item_percent[i] = Text(Text::Font::A11M, Text::Alignment::CENTER, Color::Name::TORCHRED);
		}

		Point<int16_t> slider_pos = Point<int16_t>(1007, 372);

		list_slider = Slider(
			Slider::Type::THIN_MINESHAFT,
			Range<int16_t>(slider_pos.y(), slider_pos.y() + 381),
			slider_pos.x(),
			2,
			7,
			[&](bool upwards)
			{
				int16_t shift = upwards ? -7 : 7;
				bool above = list_offset >= 0;
				bool below = list_offset + shift < items.size();

				if (above && below)
				{
					list_offset += shift;

					update_items();
				}
			}
		);

		update_items();

		dimension = Texture(backgrnd).get_dimensions();
	}

	void UICashShop::draw(float inter) const
	{
		preview_sprites[preview_index].draw(position + Point<int16_t>(644, 65), inter);

		UIElement::draw_sprites(inter);

		menu_tabs[menu_index].draw(position + Point<int16_t>(0, 63), inter);

		Point<int16_t> label_pos = position + Point<int16_t>(4, 3);
		job_label.draw(label_pos);

		size_t length = job_label.width();
		name_label.draw(label_pos + Point<int16_t>(length + 10, 0));

		promotion_sprites[promotion_index].draw(position + promotion_pos, inter);

		mvp_sprites[mvp_grade].draw(position + mvp_pos, inter);
		mvp_gauge.draw(position + mvp_pos);

		Point<int16_t> charge_pos = position + Point<int16_t>(107, 388);

		charge_charset.draw("0", charge_pos + Point<int16_t>(0, 30 * 1));
		charge_charset.draw("3,300", charge_pos + Point<int16_t>(0, 30 * 2));
		charge_charset.draw("0", charge_pos + Point<int16_t>(0, 30 * 3));
		charge_charset.draw("8,698,565", charge_pos + Point<int16_t>(0, 30 * 4));
		charge_charset.draw("0", charge_pos + Point<int16_t>(0, 30 * 5));

		if (items.size() > 0)
			item_line.draw(position + Point<int16_t>(139, 566), inter);
		else
			item_none.draw(position + Point<int16_t>(137, 372) +
						   Point<int16_t>(BestNew_dim.x() / 2, list_slider.getvertical().length() / 2) -
						   item_none.get_dimensions() / 2, inter);

		for (size_t i = 0; i < MAX_ITEMS; i++)
		{
			int16_t index = i + list_offset;

			if (index < items.size())
			{
				div_t div = std::div(i, 7);
				Item item = items[index];

				item_base.draw(position + Point<int16_t>(137, 372) + Point<int16_t>(124 * div.rem, 205 * div.quot),
							   inter);
				item.draw(
					DrawArgument(position + Point<int16_t>(164, 473) + Point<int16_t>(124 * div.rem, 205 * div.quot),
								 2.0f, 2.0f));

				if (item.label != Item::Label::NONE)
					item_labels[item.label + 1].draw(
						position + Point<int16_t>(152, 372) + Point<int16_t>(124 * div.rem, 205 * div.quot), inter);

				item_name[i].draw(position + Point<int16_t>(192, 480) + Point<int16_t>(124 * div.rem, 205 * div.quot));

				if (item_discount[i].get_text() == "")
				{
					item_price[i].draw(
						position + Point<int16_t>(195, 499) + Point<int16_t>(124 * div.rem, 205 * div.quot));
				}
				else
				{
					item_price[i].draw(
						position + Point<int16_t>(196, 506) + Point<int16_t>(124 * div.rem, 205 * div.quot));

					item_discount[i].draw(
						position + Point<int16_t>(185, 495) + Point<int16_t>(124 * div.rem, 205 * div.quot));
					item_percent[i].draw(position + Point<int16_t>(198 + (item_discount[i].width() / 2), 495) +
										 Point<int16_t>(124 * div.rem, 205 * div.quot));
				}
			}
		}

		list_slider.draw(position);

		UIElement::draw_buttons(inter);
	}

	void UICashShop::update()
	{
		UIElement::update();

		mvp_gauge.update(mvp_exp);
	}

	Button::State UICashShop::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::BtPreview1:
			case Buttons::BtPreview2:
			case Buttons::BtPreview3:
				buttons[preview_index]->set_state(Button::State::NORMAL);

				preview_index = buttonid;
				return Button::State::PRESSED;
			case Buttons::BtExit:
			{
				uint16_t width = Setting<Width>::get().load();
				uint16_t height = Setting<Height>::get().load();

				Constants::Constants::get().set_viewwidth(width);
				Constants::Constants::get().set_viewheight(height);

				float fadestep = 0.025f;

				Window::get().fadeout(
					fadestep,
					[]()
					{
						GraphicsGL::get().clear();
						ChangeMapPacket().dispatch();
					}
				);

				GraphicsGL::get().lock();
				Stage::get().clear();
				Timer::get().start();

				return Button::State::NORMAL;
			}
			case Buttons::BtNext:
			{
				size_t size = promotion_sprites.size() - 1;

				promotion_index++;

				if (promotion_index > size)
					promotion_index = 0;

				return Button::State::NORMAL;
			}
			case Buttons::BtPrev:
			{
				size_t size = promotion_sprites.size() - 1;

				promotion_index--;

				if (promotion_index < 0)
					promotion_index = size;

				return Button::State::NORMAL;
			}
			case Buttons::BtChargeNX:
			{
				std::string url = Configuration::get().get_chargenx();

//todo: open url on linux
#ifdef _WIN32
				ShellExecute(NULL, (LPCWSTR) "open", (LPCWSTR) url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif

				return Button::State::NORMAL;
			}
			default:
				break;
		}

		if (buttonid >= Buttons::BtBuy)
		{
			int16_t index = buttonid - Buttons::BtBuy + list_offset;

			Item item = items[index];

			// TODO: Purchase item

			return Button::State::NORMAL;
		}

		return Button::State::DISABLED;
	}

	Cursor::State UICashShop::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Point<int16_t> cursor_relative = cursorpos - position;

		if (list_slider.isenabled())
		{
			Cursor::State state = list_slider.send_cursor(cursor_relative, clicked);

			if (state != Cursor::State::IDLE)
				return state;
		}

		return UIElement::send_cursor(clicked, cursorpos);
	}

	UIElement::Type UICashShop::get_type() const
	{
		return TYPE;
	}

	void UICashShop::exit_cashshop()
	{
		UI& ui = UI::get();
		ui.change_state(UI::State::GAME);

		Stage& stage = Stage::get();
		Player& player = stage.get_player();

		PlayerLoginPacket(player.get_oid()).dispatch();

		int32_t mapid = player.get_stats().get_mapid();
		uint8_t portalid = player.get_stats().get_portal();

		stage.load(mapid, portalid);
		stage.transfer_player();

		ui.enable();
		Timer::get().start();
		GraphicsGL::get().unlock();
	}

	void UICashShop::update_items()
	{
		for (size_t i = 0; i < MAX_ITEMS; i++)
		{
			int16_t index = i + list_offset;
			bool found_item = index < items.size();

			buttons[Buttons::BtBuy + i]->set_active(found_item);

			std::string name = "";
			std::string price_text = "";
			std::string discount_text = "";
			std::string percent_text = "";

			if (found_item)
			{
				Item item = items[index];

				name = item.get_name();

				int32_t price = item.get_price();
				price_text = std::to_string(price);

				if (item.discount_price > 0 && price > 0)
				{
					discount_text = price_text;

					uint32_t discount = item.discount_price;
					price_text = std::to_string(discount);

					float_t percent = (float) discount / price;
					std::string percent_str = std::to_string(percent);
					percent_text = "(" + percent_str.substr(2, 1) + "%)";
				}

				string_format::split_number(price_text);
				string_format::split_number(discount_text);

				price_text += " NX";

				if (discount_text != "")
					discount_text += " NX";

				if (item.count > 0)
					price_text += "(" + std::to_string(item.count) + ")";
			}

			item_name[i].change_text(name);
			item_price[i].change_text(price_text);
			item_discount[i].change_text(discount_text);
			item_percent[i].change_text(percent_text);

			string_format::format_with_ellipsis(item_name[i], 92);
		}
	}
}