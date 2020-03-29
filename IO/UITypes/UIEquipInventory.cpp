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
#include "UIEquipInventory.h"

#include "../UI.h"

#include "../Components/MapleButton.h"
#include "../Data/ItemData.h"
#include "../Audio/Audio.h"

#include "../Net/Packets/InventoryPackets.h"
#include "../IO/UITypes/UIItemInventory.h"

#include <nlnx/nx.hpp>

namespace ms
{
	UIEquipInventory::UIEquipInventory(const Inventory &invent)
			: UIDragElement<PosEQINV>(), inventory(invent), tab(Buttons::BT_TAB1), hasPendantSlot(false),
			  hasPocketSlot(false)
	{
		// Column 1
		iconpositions[Equipslot::Id::RING1] = Point<int16_t>(14, 50);
		iconpositions[Equipslot::Id::RING2] = Point<int16_t>(14, 91);
		iconpositions[Equipslot::Id::RING3] = Point<int16_t>(14, 132);
		iconpositions[Equipslot::Id::RING4] = Point<int16_t>(14, 173);
		iconpositions[Equipslot::Id::POCKET] = Point<int16_t>(14, 214);
		iconpositions[Equipslot::Id::BOOK] = Point<int16_t>(14, 255);

		// Column 2
		//iconpositions[Equipslot::Id::NONE] = Point<int16_t>(55, 50);
		iconpositions[Equipslot::Id::PENDANT2] = Point<int16_t>(55, 91);
		iconpositions[Equipslot::Id::PENDANT1] = Point<int16_t>(55, 132);
		iconpositions[Equipslot::Id::WEAPON] = Point<int16_t>(55, 173);
		iconpositions[Equipslot::Id::BELT] = Point<int16_t>(55, 214);
		//iconpositions[Equipslot::Id::NONE] = Point<int16_t>(55, 255);

		// Column 3
		iconpositions[Equipslot::Id::HAT] = Point<int16_t>(96, 50);
		iconpositions[Equipslot::Id::FACE] = Point<int16_t>(96, 91);
		iconpositions[Equipslot::Id::EYEACC] = Point<int16_t>(96, 132);
		iconpositions[Equipslot::Id::TOP] = Point<int16_t>(96, 173);
		iconpositions[Equipslot::Id::BOTTOM] = Point<int16_t>(96, 214);
		iconpositions[Equipslot::Id::SHOES] = Point<int16_t>(96, 255);

		// Column 4
		//iconpositions[Equipslot::Id::NONE] = Point<int16_t>(137, 50);
		//iconpositions[Equipslot::Id::NONE] = Point<int16_t>(137, 91);
		iconpositions[Equipslot::Id::EARACC] = Point<int16_t>(137, 132);
		iconpositions[Equipslot::Id::SHOULDER] = Point<int16_t>(137, 173);
		iconpositions[Equipslot::Id::GLOVES] = Point<int16_t>(137, 214);
		iconpositions[Equipslot::Id::ANDROID] = Point<int16_t>(137, 255);

		// Column 5
		iconpositions[Equipslot::Id::EMBLEM] = Point<int16_t>(178, 50);
		iconpositions[Equipslot::Id::BADGE] = Point<int16_t>(178, 91);
		iconpositions[Equipslot::Id::MEDAL] = Point<int16_t>(178, 132);
		iconpositions[Equipslot::Id::SUBWEAPON] = Point<int16_t>(178, 173);
		iconpositions[Equipslot::Id::CAPE] = Point<int16_t>(178, 214);
		iconpositions[Equipslot::Id::HEART] = Point<int16_t>(178, 255);

		//iconpositions[Equipslot::Id::SHIELD] = Point<int16_t>(142, 124);
		//iconpositions[Equipslot::Id::TAMEDMOB] = Point<int16_t>(142, 91);
		//iconpositions[Equipslot::Id::SADDLE] = Point<int16_t>(76, 124);

		tab_source[Buttons::BT_TAB0] = "Equip";
		tab_source[Buttons::BT_TAB1] = "Cash";
		tab_source[Buttons::BT_TAB2] = "Pet";
		tab_source[Buttons::BT_TAB3] = "Android";

		nl::node close = nl::nx::ui["Basic.img"]["BtClose3"];
		nl::node Equip = nl::nx::ui["UIWindow4.img"]["Equip"];

		background[Buttons::BT_TAB0] = Equip[tab_source[Buttons::BT_TAB0]]["backgrnd"];
		background[Buttons::BT_TAB1] = Equip[tab_source[Buttons::BT_TAB1]]["backgrnd"];
		background[Buttons::BT_TAB2] = Equip[tab_source[Buttons::BT_TAB2]]["backgrnd"];
		background[Buttons::BT_TAB3] = Equip[tab_source[Buttons::BT_TAB3]]["backgrnd"];

		for (uint16_t i = Buttons::BT_TAB0; i < Buttons::BT_TABE; i++)
			for (auto slot : Equip[tab_source[i]]["Slots"])
				if (slot.name().find("_") == std::string::npos)
					Slots[i].emplace_back(slot);

		nl::node EquipGL = nl::nx::ui["UIWindowGL.img"]["Equip"];
		nl::node backgrnd = Equip["backgrnd"];

		Point<int16_t> bg_dimensions = Texture(backgrnd).get_dimensions();

		sprites.emplace_back(EquipGL["Totem"]["backgrnd"], Point<int16_t>(-56, 0));
		sprites.emplace_back(backgrnd);
		sprites.emplace_back(Equip["backgrnd2"]);

		tabbar = Equip["tabbar"];
		disabled = Equip[tab_source[Buttons::BT_TAB0]]["disabled"];
		disabled2 = Equip[tab_source[Buttons::BT_TAB0]]["disabled2"];

		buttons[Buttons::BT_CLOSE] = std::make_unique<MapleButton>(close, Point<int16_t>(bg_dimensions.x() - 19, 5));
		buttons[Buttons::BT_SLOT] = std::make_unique<MapleButton>(Equip[tab_source[Buttons::BT_TAB0]]["BtSlot"]);
		buttons[Buttons::BT_EFFECT] = std::make_unique<MapleButton>(EquipGL["Equip"]["btEffect"]);
		buttons[Buttons::BT_SALON] = std::make_unique<MapleButton>(EquipGL["Equip"]["btSalon"]);
		buttons[Buttons::BT_CONSUMESETTING] = std::make_unique<MapleButton>(
				Equip[tab_source[Buttons::BT_TAB2]]["BtConsumeSetting"]);
		buttons[Buttons::BT_EXCEPTION] = std::make_unique<MapleButton>(
				Equip[tab_source[Buttons::BT_TAB2]]["BtException"]);
		buttons[Buttons::BT_SHOP] = std::make_unique<MapleButton>(Equip[tab_source[Buttons::BT_TAB3]]["BtShop"]);

		buttons[Buttons::BT_CONSUMESETTING]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_EXCEPTION]->set_state(Button::State::DISABLED);
		buttons[Buttons::BT_SHOP]->set_state(Button::State::DISABLED);

		nl::node Tab = Equip["Tab"];

		for (uint16_t i = Buttons::BT_TAB0; i < Buttons::BT_TABE; i++)
			buttons[Buttons::BT_TAB0 + i] = std::make_unique<TwoSpriteButton>(Tab["disabled"][i], Tab["enabled"][i],
																			  Point<int16_t>(0, 3));

		dimension = bg_dimensions;
		dragarea = Point<int16_t>(bg_dimensions.x(), 20);

		load_icons();
		change_tab(Buttons::BT_TAB0);
	}

	void UIEquipInventory::draw(float alpha) const
	{
		UIElement::draw(alpha);

		background[tab].draw(position);
		tabbar.draw(position);

		for (auto slot : Slots[tab])
			slot.draw(position);

		if (tab == Buttons::BT_TAB0)
		{
			if (!hasPendantSlot)
				disabled.draw(position + iconpositions[Equipslot::Id::PENDANT2]);

			if (!hasPocketSlot)
				disabled.draw(position + iconpositions[Equipslot::Id::POCKET]);

			for (auto iter : icons)
				if (iter.second)
					iter.second->draw(position + iconpositions[iter.first] + Point<int16_t>(4, 4));
		} else if (tab == Buttons::BT_TAB2)
		{
			disabled2.draw(position + Point<int16_t>(113, 57));
			disabled2.draw(position + Point<int16_t>(113, 106));
			disabled2.draw(position + Point<int16_t>(113, 155));
		}
	}

	Button::State UIEquipInventory::button_pressed(uint16_t id)
	{
		switch (id)
		{
			case Buttons::BT_CLOSE:
				toggle_active();
				break;
			case Buttons::BT_TAB0:
			case Buttons::BT_TAB1:
			case Buttons::BT_TAB2:
			case Buttons::BT_TAB3:
				change_tab(id);

				return Button::State::IDENTITY;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UIEquipInventory::update_slot(Equipslot::Id slot)
	{
		if (int32_t item_id = inventory.get_item_id(InventoryType::Id::EQUIPPED, slot))
		{
			const Texture &texture = ItemData::get(item_id).get_icon(false);

			icons[slot] = std::make_unique<Icon>(
					std::make_unique<EquipIcon>(slot),
					texture,
					-1
			);
		} else if (icons[slot])
		{
			icons[slot].release();
		}

		clear_tooltip();
	}

	void UIEquipInventory::load_icons()
	{
		icons.clear();

		for (auto iter : Equipslot::values)
			update_slot(iter);
	}

	Cursor::State UIEquipInventory::send_cursor(bool pressed, Point<int16_t> cursorpos)
	{
		Cursor::State dstate = UIDragElement::send_cursor(pressed, cursorpos);

		if (dragged)
		{
			clear_tooltip();

			return dstate;
		}

		Equipslot::Id slot = slot_by_position(cursorpos);

		if (auto icon = icons[slot].get())
		{
			if (pressed)
			{
				icon->start_drag(cursorpos - position - iconpositions[slot]);

				UI::get().drag_icon(icon);

				clear_tooltip();

				return Cursor::State::GRABBING;
			} else
			{
				show_equip(slot);

				return Cursor::State::CANGRAB;
			}
		} else
		{
			clear_tooltip();

			return Cursor::State::IDLE;
		}
	}

	void UIEquipInventory::send_key(int32_t keycode, bool pressed, bool escape)
	{
		if (pressed)
		{
			if (escape)
			{
				toggle_active();
			} else if (keycode == KeyAction::Id::TAB)
			{
				uint16_t newtab = tab + 1;

				if (newtab >= Buttons::BT_TABE)
					newtab = Buttons::BT_TAB0;

				change_tab(newtab);
			}
		}
	}

	UIElement::Type UIEquipInventory::get_type() const
	{
		return TYPE;
	}

	void UIEquipInventory::doubleclick(Point<int16_t> cursorpos)
	{
		Equipslot::Id slot = slot_by_position(cursorpos);

		if (icons[slot])
			if (int16_t freeslot = inventory.find_free_slot(InventoryType::Id::EQUIP))
				UnequipItemPacket(slot, freeslot).dispatch();
	}

	bool UIEquipInventory::send_icon(const Icon &icon, Point<int16_t> cursorpos)
	{
		if (Equipslot::Id slot = slot_by_position(cursorpos))
			icon.drop_on_equips(slot);

		return true;
	}

	void UIEquipInventory::toggle_active()
	{
		clear_tooltip();

		UIElement::toggle_active();
	}

	void UIEquipInventory::modify(int16_t pos, int8_t mode, int16_t arg)
	{
		Equipslot::Id eqpos = Equipslot::by_id(pos);
		Equipslot::Id eqarg = Equipslot::by_id(arg);

		switch (mode)
		{
			case 0:
			case 3:
				update_slot(eqpos);
				break;
			case 2:
				update_slot(eqpos);
				update_slot(eqarg);
				break;
		}
	}

	void UIEquipInventory::show_equip(Equipslot::Id slot)
	{
		UI::get().show_equip(Tooltip::Parent::EQUIPINVENTORY, slot);
	}

	void UIEquipInventory::clear_tooltip()
	{
		UI::get().clear_tooltip(Tooltip::Parent::EQUIPINVENTORY);
	}

	Equipslot::Id UIEquipInventory::slot_by_position(Point<int16_t> cursorpos) const
	{
		if (tab != Buttons::BT_TAB0)
			return Equipslot::Id::NONE;

		for (auto iter : iconpositions)
		{
			Rectangle<int16_t> iconrect = Rectangle<int16_t>(
					position + iter.second,
					position + iter.second + Point<int16_t>(32, 32)
			);

			if (iconrect.contains(cursorpos))
				return iter.first;
		}

		return Equipslot::Id::NONE;
	}

	void UIEquipInventory::change_tab(uint16_t tabid)
	{
		uint8_t oldtab = tab;
		tab = tabid;

		if (oldtab != tab)
		{
			clear_tooltip();

			buttons[oldtab]->set_state(Button::State::NORMAL);
			buttons[tab]->set_state(Button::State::PRESSED);

			if (tab == Buttons::BT_TAB0)
				buttons[Buttons::BT_SLOT]->set_active(true);
			else
				buttons[Buttons::BT_SLOT]->set_active(false);

			if (tab == Buttons::BT_TAB2)
			{
				buttons[Buttons::BT_CONSUMESETTING]->set_active(true);
				buttons[Buttons::BT_EXCEPTION]->set_active(true);
			} else
			{
				buttons[Buttons::BT_CONSUMESETTING]->set_active(false);
				buttons[Buttons::BT_EXCEPTION]->set_active(false);
			}

			if (tab == Buttons::BT_TAB3)
				buttons[Buttons::BT_SHOP]->set_active(true);
			else
				buttons[Buttons::BT_SHOP]->set_active(false);
		}
	}

	UIEquipInventory::EquipIcon::EquipIcon(int16_t s)
	{
		source = s;
	}

	void UIEquipInventory::EquipIcon::drop_on_stage() const
	{
		Sound(Sound::Name::DRAGEND).play();
	}

	void UIEquipInventory::EquipIcon::drop_on_equips(Equipslot::Id slot) const
	{
		if (source == slot)
			Sound(Sound::Name::DRAGEND).play();
	}

	bool UIEquipInventory::EquipIcon::drop_on_items(InventoryType::Id tab, Equipslot::Id eqslot, int16_t slot,
													bool equip) const
	{
		if (tab != InventoryType::Id::EQUIP)
		{
			if (auto iteminventory = UI::get().get_element<UIItemInventory>())
			{
				if (iteminventory->is_active())
				{
					iteminventory->change_tab(InventoryType::Id::EQUIP);
					return false;
				}
			}
		}

		if (equip)
		{
			if (eqslot == source)
				EquipItemPacket(slot, eqslot).dispatch();
		} else
		{
			UnequipItemPacket(source, slot).dispatch();
		}

		return true;
	}

	Icon::IconType UIEquipInventory::EquipIcon::get_type()
	{
		return Icon::IconType::EQUIP;
	}
}