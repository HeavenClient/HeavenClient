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
#include "Icon.h"

#include "Charset.h"

#include "../../Audio/Audio.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	Icon::Icon(std::unique_ptr<Type> t, Texture tx, int16_t c) : type(std::move(t)), texture(tx), count(c)
	{
		texture.shift(Point<int16_t>(0, 32));
		showcount = c > -1;
		dragged = false;
	}

	Icon::Icon() : Icon(std::make_unique<NullType>(), {}, -1)
	{}

	void Icon::draw(Point<int16_t> position) const
	{
		float opacity = dragged ? 0.5f : 1.0f;
		get_texture().draw(DrawArgument(position, opacity));

		if (showcount)
		{
			static const Charset countset = Charset(nl::nx::ui["Basic.img"]["ItemNo"], Charset::Alignment::LEFT);
			countset.draw(std::to_string(count), position + Point<int16_t>(0, 20));
		}
	}

	void Icon::dragdraw(Point<int16_t> cursorpos) const
	{
		if (dragged && texture.is_valid())
			texture.draw(DrawArgument(cursorpos - cursoroffset, 0.5f));
	}

	void Icon::drop_on_stage() const
	{
		type->drop_on_stage();
	}

	void Icon::drop_on_equips(EquipSlot::Id eqslot) const
	{
		type->drop_on_equips(eqslot);
	}

	bool Icon::drop_on_items(InventoryType::Id tab, EquipSlot::Id eqslot, int16_t slot, bool equip) const
	{
		if (!texture.is_valid())
			return false;

		bool remove_icon = type->drop_on_items(tab, eqslot, slot, equip);

		if (remove_icon)
			Sound(Sound::Name::DRAGEND).play();

		return remove_icon;
	}

	void Icon::drop_on_bindings(Point<int16_t> cursorposition, bool remove) const
	{
		type->drop_on_bindings(cursorposition, remove);
	}

	void Icon::start_drag(Point<int16_t> offset)
	{
		cursoroffset = offset;
		dragged = true;

		Sound(Sound::Name::DRAGSTART).play();
	}

	void Icon::reset()
	{
		dragged = false;
	}

	// Allows for Icon extensibility
	// Use this instead of referencing texture directly
	Texture Icon::get_texture() const
	{
		return texture;
	}

	void Icon::set_count(int16_t c)
	{
		count = c;
		type->set_count(c);
	}

	Icon::IconType Icon::get_type()
	{
		return type->get_type();
	}

	int16_t Icon::get_count() const
	{
		return count;
	}

	bool Icon::get_drag()
	{
		return dragged;
	}
}