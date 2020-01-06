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
#include "InventoryHandlers.h"

#include "Helpers/ItemParser.h"

#include "../Gameplay/Stage.h"
#include "../IO/UI.h"
#include "../IO/Messages.h"

#include "../Character/Inventory/Inventory.h"
#include "../IO/UITypes/UIEquipInventory.h"
#include "../IO/UITypes/UIItemInventory.h"
#include "../IO/UITypes/UIKeyConfig.h"
#include "../IO/UITypes/UIShop.h"

namespace ms
{
	void GatherResultHandler::handle(InPacket&) const
	{
		if (auto iteminventory = UI::get().get_element<UIItemInventory>())
			iteminventory->set_sort(true);
	}

	void SortResultHandler::handle(InPacket&) const
	{
		if (auto iteminventory = UI::get().get_element<UIItemInventory>())
		{
			iteminventory->set_sort(false);
			iteminventory->clear_new();
		}
	}

	void ModifyInventoryHandler::handle(InPacket& recv) const
	{
		recv.read_bool(); // 'updatetick'

		Inventory& inventory = Stage::get()
			.get_player()
			.get_inventory();

		struct Mod
		{
			int8_t mode;
			InventoryType::Id type;
			int16_t pos;
			int16_t arg;
		};

		std::vector<Mod> mods;

		int8_t size = recv.read_byte();

		for (int8_t i = 0; i < size; i++)
		{
			Mod mod;
			mod.mode = recv.read_byte();
			mod.type = InventoryType::by_value(recv.read_byte());
			mod.pos = recv.read_short();

			mod.arg = 0;

			switch (mod.mode)
			{
			case Inventory::Modification::ADD:
				ItemParser::parse_item(recv, mod.type, mod.pos, inventory);

				if (auto keyconfig = UI::get().get_element<UIKeyConfig>())
				{
					int16_t count_now = inventory.get_item_count(mod.type, mod.pos);
					keyconfig->update_item_count(mod.type, mod.pos, count_now);
				}

				break;
			case Inventory::Modification::CHANGECOUNT:
			{
				mod.arg = recv.read_short();

				int16_t count_before = inventory.get_item_count(mod.type, mod.pos);
				int16_t count_now = mod.arg;

				inventory.modify(mod.type, mod.pos, mod.mode, mod.arg, Inventory::Movement::MOVE_NONE);

				if (auto keyconfig = UI::get().get_element<UIKeyConfig>())
					keyconfig->update_item_count(mod.type, mod.pos, count_now - count_before);

				if (count_before < count_now)
					mod.mode = Inventory::Modification::ADDCOUNT;
			}
			break;
			case Inventory::Modification::SWAP:
				mod.arg = recv.read_short();
				break;
			case Inventory::Modification::REMOVE:
				if (auto keyconfig = UI::get().get_element<UIKeyConfig>())
				{
					int16_t count_before = inventory.get_item_count(mod.type, mod.pos);
					keyconfig->update_item_count(mod.type, mod.pos, -1 * count_before);
				}

				inventory.modify(mod.type, mod.pos, mod.mode, mod.arg, Inventory::Movement::MOVE_INTERNAL);
				break;
			}

			mods.push_back(mod);
		}

		Inventory::Movement move = (recv.length() > 0) ?
			Inventory::movementbyvalue(recv.read_byte()) :
			Inventory::Movement::MOVE_INTERNAL;

		for (const Mod& mod : mods)
		{
			if (mod.mode == 2)
				inventory.modify(mod.type, mod.pos, mod.mode, mod.arg, move);

			if (auto shop = UI::get().get_element<UIShop>())
				shop->modify(mod.type);

			auto eqinvent = UI::get().get_element<UIEquipInventory>();
			auto itinvent = UI::get().get_element<UIItemInventory>();

			switch (move)
			{
			case Inventory::Movement::MOVE_INTERNAL:
				switch (mod.type)
				{
				case InventoryType::Id::EQUIPPED:
					if (eqinvent)
						eqinvent->modify(mod.pos, mod.mode, mod.arg);

					Stage::get().get_player().change_equip(-mod.pos);
					Stage::get().get_player().change_equip(-mod.arg);
					break;
				case InventoryType::Id::EQUIP:
				case InventoryType::Id::USE:
				case InventoryType::Id::SETUP:
				case InventoryType::Id::ETC:
				case InventoryType::Id::CASH:
					if (itinvent)
						itinvent->modify(mod.type, mod.pos, mod.mode, mod.arg);

					break;
				}

				break;
			case Inventory::Movement::MOVE_EQUIP:
			case Inventory::Movement::MOVE_UNEQUIP:
				if (mod.pos < 0)
				{
					if (eqinvent)
						eqinvent->modify(-mod.pos, 3, 0);

					if (itinvent)
						itinvent->modify(InventoryType::Id::EQUIP, mod.arg, mod.mode, 0);

					Stage::get().get_player().change_equip(-mod.pos);
				}
				else if (mod.arg < 0)
				{
					if (eqinvent)
						eqinvent->modify(-mod.arg, 0, 0);

					if (itinvent)
						itinvent->modify(InventoryType::Id::EQUIP, mod.pos, Inventory::Modification::REMOVE, 0);

					Stage::get().get_player().change_equip(-mod.arg);
				}

				break;
			}
		}

		Stage::get().get_player().recalc_stats(true);
		UI::get().enable();
	}
}