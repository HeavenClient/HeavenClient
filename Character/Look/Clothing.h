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
#pragma once

#include "BodyDrawInfo.h"
#include "EquipSlot.h"

#include "../../Graphics/Texture.h"

namespace ms
{
	class Clothing
	{
	public:
		enum Layer
		{
			CAPE,
			SHOES,
			PANTS,
			TOP,
			MAIL,
			MAILARM,
			EARRINGS,
			FACEACC,
			EYEACC,
			PENDANT,
			BELT,
			MEDAL,
			RING,
			CAP,
			CAP_BELOW_BODY,
			CAP_OVER_HAIR,
			GLOVE,
			WRIST,
			GLOVE_OVER_HAIR,
			WRIST_OVER_HAIR,
			GLOVE_OVER_BODY,
			WRIST_OVER_BODY,
			SHIELD,
			BACKSHIELD,
			SHIELD_BELOW_BODY,
			SHIELD_OVER_HAIR,
			WEAPON,
			BACKWEAPON,
			WEAPON_BELOW_ARM,
			WEAPON_BELOW_BODY,
			WEAPON_OVER_HAND,
			WEAPON_OVER_BODY,
			WEAPON_OVER_GLOVE,
			NUM_LAYERS
		};

		// Construct a new equip.
		Clothing(int32_t itemid, const BodyDrawInfo &drawinfo);

		// Draw the equip.
		void draw(Stance::Id stance, Layer layer, uint8_t frame, const DrawArgument &args) const;

		// Check if a part of the equip lies on the specified layer while in the specified stance.
		bool contains_layer(Stance::Id stance, Layer layer) const;

		// Return whether the equip is invisible
		bool is_transparent() const;

		// Return whether this equip uses twohanded stances.
		bool is_twohanded() const;

		// Return the item id.
		int32_t get_id() const;

		// Return the equip slot for this cloth.
		EquipSlot::Id get_eqslot() const;

		// Return the standing stance to use while equipped.
		Stance::Id get_stand() const;

		// Return the walking stance to use while equipped.
		Stance::Id get_walk() const;

		// Return the vslot, used to distinguish some layering types.
		const std::string &get_vslot() const;

	private:
		EnumMap<Stance::Id, EnumMap<Layer, std::unordered_multimap<uint8_t, Texture>, Layer::NUM_LAYERS>> stances;
		int32_t itemid;
		EquipSlot::Id eqslot;
		Stance::Id walk;
		Stance::Id stand;
		std::string vslot;
		bool twohanded;
		bool transparent;

		static const std::unordered_map<std::string, Layer> sublayernames;
	};
}