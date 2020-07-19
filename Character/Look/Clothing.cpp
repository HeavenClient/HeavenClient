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
#include "Clothing.h"

#include "../../Data/WeaponData.h"

#include <unordered_set>

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	Clothing::Clothing(int32_t id, const BodyDrawInfo& drawinfo) : itemid(id)
	{
		const EquipData& equipdata = EquipData::get(itemid);

		eqslot = equipdata.get_eqslot();

		if (eqslot == EquipSlot::Id::WEAPON)
			twohanded = WeaponData::get(itemid).is_twohanded();
		else
			twohanded = false;

		constexpr size_t NON_WEAPON_TYPES = 15;
		constexpr size_t WEAPON_OFFSET = NON_WEAPON_TYPES + 15;
		constexpr size_t WEAPON_TYPES = 20;

		constexpr Clothing::Layer layers[NON_WEAPON_TYPES] =
			{
				Clothing::Layer::CAP,
				Clothing::Layer::FACEACC,
				Clothing::Layer::EYEACC,
				Clothing::Layer::EARRINGS,
				Clothing::Layer::TOP,
				Clothing::Layer::MAIL,
				Clothing::Layer::PANTS,
				Clothing::Layer::SHOES,
				Clothing::Layer::GLOVE,
				Clothing::Layer::SHIELD,
				Clothing::Layer::CAPE,
				Clothing::Layer::RING,
				Clothing::Layer::PENDANT,
				Clothing::Layer::BELT,
				Clothing::Layer::MEDAL
			};

		Clothing::Layer chlayer;
		size_t index = (itemid / 10000) - 100;

		if (index < NON_WEAPON_TYPES)
			chlayer = layers[index];
		else if (index >= WEAPON_OFFSET && index < WEAPON_OFFSET + WEAPON_TYPES)
			chlayer = Clothing::Layer::WEAPON;
		else
			chlayer = Clothing::Layer::CAPE;

		std::string strid = "0" + std::to_string(itemid);
		std::string category = equipdata.get_itemdata().get_category();
		nl::node src = nl::nx::character[category][strid + ".img"];
		nl::node info = src["info"];

		vslot = std::string(info["vslot"]);

		switch (int32_t standno = info["stand"])
		{
			case 1:
				stand = Stance::Id::STAND1;
				break;
			case 2:
				stand = Stance::Id::STAND2;
				break;
			default:
				stand = twohanded ? Stance::Id::STAND2 : Stance::Id::STAND1;
				break;
		}

		switch (int32_t walkno = info["walk"])
		{
			case 1:
				walk = Stance::Id::WALK1;
				break;
			case 2:
				walk = Stance::Id::WALK2;
				break;
			default:
				walk = twohanded ? Stance::Id::WALK2 : Stance::Id::WALK1;
				break;
		}

		for (auto iter : Stance::names)
		{
			Stance::Id stance = iter.first;
			const std::string& stancename = iter.second;

			nl::node stancenode = src[stancename];

			if (!stancenode)
				continue;

			for (uint8_t frame = 0; nl::node framenode = stancenode[frame]; ++frame)
			{
				for (nl::node partnode : framenode)
				{
					std::string part = partnode.name();

					if (!partnode || partnode.data_type() != nl::node::type::bitmap)
						continue;

					Clothing::Layer z = chlayer;
					std::string zs = partnode["z"];

					if (part == "mailArm")
					{
						z = Clothing::Layer::MAILARM;
					}
					else
					{
						auto sublayer_iter = sublayernames.find(zs);

						if (sublayer_iter != sublayernames.end())
							z = sublayer_iter->second;
					}

					std::string parent;
					Point<int16_t> parentpos;

					for (auto mapnode : partnode["map"])
					{
						if (mapnode.data_type() == nl::node::type::vector)
						{
							parent = mapnode.name();
							parentpos = mapnode;
						}
					}

					nl::node mapnode = partnode["map"];
					Point<int16_t> shift;

					switch (eqslot)
					{
						case EquipSlot::Id::FACE:
							shift -= parentpos;
							break;
						case EquipSlot::Id::SHOES:
						case EquipSlot::Id::GLOVES:
						case EquipSlot::Id::TOP:
						case EquipSlot::Id::BOTTOM:
						case EquipSlot::Id::CAPE:
							shift = drawinfo.get_body_position(stance, frame) - parentpos;
							break;
						case EquipSlot::Id::HAT:
						case EquipSlot::Id::EARACC:
						case EquipSlot::Id::EYEACC:
							shift = drawinfo.getfacepos(stance, frame) - parentpos;
							break;
						case EquipSlot::Id::SHIELD:
						case EquipSlot::Id::WEAPON:
							if (parent == "handMove")
								shift += drawinfo.get_hand_position(stance, frame);
							else if (parent == "hand")
								shift += drawinfo.get_arm_position(stance, frame);
							else if (parent == "navel")
								shift += drawinfo.get_body_position(stance, frame);

							shift -= parentpos;
							break;
					}

					stances[stance][z].emplace(frame, partnode)->second.shift(shift);
				}
			}
		}

		static const std::unordered_set<int32_t> transparents =
			{
				1002186
			};

		transparent = transparents.count(itemid) > 0;
	}

	void Clothing::draw(Stance::Id stance, Layer layer, uint8_t frame, const DrawArgument& args) const
	{
		auto range = stances[stance][layer].equal_range(frame);

		for (auto iter = range.first; iter != range.second; ++iter)
			iter->second.draw(args);
	}

	bool Clothing::contains_layer(Stance::Id stance, Layer layer) const
	{
		return !stances[stance][layer].empty();
	}

	bool Clothing::is_transparent() const
	{
		return transparent;
	}

	bool Clothing::is_twohanded() const
	{
		return twohanded;
	}

	int32_t Clothing::get_id() const
	{
		return itemid;
	}

	Stance::Id Clothing::get_stand() const
	{
		return stand;
	}

	Stance::Id Clothing::get_walk() const
	{
		return walk;
	}

	EquipSlot::Id Clothing::get_eqslot() const
	{
		return eqslot;
	}

	const std::string& Clothing::get_vslot() const
	{
		return vslot;
	}

	const std::unordered_map<std::string, Clothing::Layer> Clothing::sublayernames =
		{
			// WEAPON
			{"weaponOverHand",       Clothing::Layer::WEAPON_OVER_HAND},
			{"weaponOverGlove",      Clothing::Layer::WEAPON_OVER_GLOVE},
			{"weaponOverBody",       Clothing::Layer::WEAPON_OVER_BODY},
			{"weaponBelowArm",       Clothing::Layer::WEAPON_BELOW_ARM},
			{"weaponBelowBody",      Clothing::Layer::WEAPON_BELOW_BODY},
			{"backWeaponOverShield", Clothing::Layer::BACKWEAPON},
			// SHIELD
			{"shieldOverHair",       Clothing::Layer::SHIELD_OVER_HAIR},
			{"shieldBelowBody",      Clothing::Layer::SHIELD_BELOW_BODY},
			{"backShield",           Clothing::Layer::BACKSHIELD},
			// GLOVE
			{"gloveWrist",           Clothing::Layer::WRIST},
			{"gloveOverHair",        Clothing::Layer::GLOVE_OVER_HAIR},
			{"gloveOverBody",        Clothing::Layer::GLOVE_OVER_BODY},
			{"gloveWristOverHair",   Clothing::Layer::WRIST_OVER_HAIR},
			{"gloveWristOverBody",   Clothing::Layer::WRIST_OVER_BODY},
			// CAP
			{"capOverHair",          Clothing::Layer::CAP_OVER_HAIR},
			{"capBelowBody",         Clothing::Layer::CAP_BELOW_BODY},
		};
}