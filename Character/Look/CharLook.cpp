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
#include "CharLook.h"

#include "../../Data/WeaponData.h"

namespace ms
{
	CharLook::CharLook(const LookEntry &entry)
	{
		reset();

		set_body(entry.skin);
		set_hair(entry.hairid);
		set_face(entry.faceid);

		for (auto &equip : entry.equips)
			add_equip(equip.second);
	}

	CharLook::CharLook()
	{
		reset();

		body = nullptr;
		hair = nullptr;
		face = nullptr;
	}

	void CharLook::reset()
	{
		flip = true;

		action = nullptr;
		actionstr = "";
		actframe = 0;

		set_stance(Stance::Id::STAND1);
		stframe.set(0);
		stelapsed = 0;

		set_expression(Expression::Id::DEFAULT);
		expframe.set(0);
		expelapsed = 0;
	}

	void
	CharLook::draw(const DrawArgument &args, Stance::Id interstance, Expression::Id interexpression, uint8_t interframe,
				   uint8_t interexpframe) const
	{
		Point<int16_t> faceshift = drawinfo.getfacepos(interstance, interframe);
		DrawArgument faceargs = args + DrawArgument{faceshift, false, Point<int16_t>{}};

		if (Stance::is_climbing(interstance))
		{
			body->draw(interstance, Body::Layer::BODY, interframe, args);
			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::GLOVE, interframe, args);
			equips.draw(EquipSlot::Id::SHOES, interstance, Clothing::Layer::SHOES, interframe, args);
			equips.draw(EquipSlot::Id::BOTTOM, interstance, Clothing::Layer::PANTS, interframe, args);
			equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::TOP, interframe, args);
			equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::MAIL, interframe, args);
			equips.draw(EquipSlot::Id::CAPE, interstance, Clothing::Layer::CAPE, interframe, args);
			body->draw(interstance, Body::Layer::HEAD, interframe, args);
			equips.draw(EquipSlot::Id::EARACC, interstance, Clothing::Layer::EARRINGS, interframe, args);

			switch (equips.getcaptype())
			{
				case CharEquips::CapType::NONE:
					hair->draw(interstance, Hair::Layer::BACK, interframe, args);
					break;
				case CharEquips::CapType::HEADBAND:
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					hair->draw(interstance, Hair::Layer::BACK, interframe, args);
					break;
				case CharEquips::CapType::HALFCOVER:
					hair->draw(interstance, Hair::Layer::BELOWCAP, interframe, args);
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					break;
				case CharEquips::CapType::FULLCOVER:
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					break;
			}

			equips.draw(EquipSlot::Id::SHIELD, interstance, Clothing::Layer::BACKSHIELD, interframe, args);
			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::BACKWEAPON, interframe, args);
		}
		else
		{
			hair->draw(interstance, Hair::Layer::BELOWBODY, interframe, args);
			equips.draw(EquipSlot::Id::CAPE, interstance, Clothing::Layer::CAPE, interframe, args);
			equips.draw(EquipSlot::Id::SHIELD, interstance, Clothing::Layer::SHIELD_BELOW_BODY, interframe, args);
			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON_BELOW_BODY, interframe, args);
			equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP_BELOW_BODY, interframe, args);
			body->draw(interstance, Body::Layer::BODY, interframe, args);
			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::WRIST_OVER_BODY, interframe, args);
			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::GLOVE_OVER_BODY, interframe, args);
			equips.draw(EquipSlot::Id::SHOES, interstance, Clothing::Layer::SHOES, interframe, args);
			body->draw(interstance, Body::Layer::ARM_BELOW_HEAD, interframe, args);

			if (equips.has_overall())
			{
				equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::MAIL, interframe, args);
			}
			else
			{
				equips.draw(EquipSlot::Id::BOTTOM, interstance, Clothing::Layer::PANTS, interframe, args);
				equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::TOP, interframe, args);
			}

			body->draw(interstance, Body::Layer::ARM_BELOW_HEAD_OVER_MAIL, interframe, args);
			equips.draw(EquipSlot::Id::SHIELD, interstance, Clothing::Layer::SHIELD_OVER_HAIR, interframe, args);
			equips.draw(EquipSlot::Id::EARACC, interstance, Clothing::Layer::EARRINGS, interframe, args);
			body->draw(interstance, Body::Layer::HEAD, interframe, args);
			hair->draw(interstance, Hair::Layer::SHADE, interframe, args);
			hair->draw(interstance, Hair::Layer::DEFAULT, interframe, args);
			face->draw(interexpression, interexpframe, faceargs);
			equips.draw(EquipSlot::Id::FACE, interstance, Clothing::Layer::FACEACC, 0, faceargs);
			equips.draw(EquipSlot::Id::EYEACC, interstance, Clothing::Layer::EYEACC, interframe, args);
			equips.draw(EquipSlot::Id::SHIELD, interstance, Clothing::Layer::SHIELD, interframe, args);

			switch (equips.getcaptype())
			{
				case CharEquips::CapType::NONE:
					hair->draw(interstance, Hair::Layer::OVERHEAD, interframe, args);
					break;
				case CharEquips::CapType::HEADBAND:
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					hair->draw(interstance, Hair::Layer::DEFAULT, interframe, args);
					hair->draw(interstance, Hair::Layer::OVERHEAD, interframe, args);
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP_OVER_HAIR, interframe, args);
					break;
				case CharEquips::CapType::HALFCOVER:
					hair->draw(interstance, Hair::Layer::DEFAULT, interframe, args);
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					break;
				case CharEquips::CapType::FULLCOVER:
					equips.draw(EquipSlot::Id::HAT, interstance, Clothing::Layer::CAP, interframe, args);
					break;
			}

			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON_BELOW_ARM, interframe, args);
			bool twohanded = is_twohanded(interstance);

			if (twohanded)
			{
				equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::MAILARM, interframe, args);
				body->draw(interstance, Body::Layer::ARM, interframe, args);
				equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON, interframe, args);
			}
			else
			{
				equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON, interframe, args);
				body->draw(interstance, Body::Layer::ARM, interframe, args);
				equips.draw(EquipSlot::Id::TOP, interstance, Clothing::Layer::MAILARM, interframe, args);
			}

			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::WRIST, interframe, args);
			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::GLOVE, interframe, args);
			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON_OVER_GLOVE, interframe, args);

			body->draw(interstance, Body::Layer::HAND_BELOW_WEAPON, interframe, args);

			body->draw(interstance, Body::Layer::ARM_OVER_HAIR, interframe, args);
			body->draw(interstance, Body::Layer::ARM_OVER_HAIR_BELOW_WEAPON, interframe, args);
			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON_OVER_HAND, interframe, args);
			equips.draw(EquipSlot::Id::WEAPON, interstance, Clothing::Layer::WEAPON_OVER_BODY, interframe, args);
			body->draw(interstance, Body::Layer::HAND_OVER_HAIR, interframe, args);
			body->draw(interstance, Body::Layer::HAND_OVER_WEAPON, interframe, args);

			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::WRIST_OVER_HAIR, interframe, args);
			equips.draw(EquipSlot::Id::GLOVES, interstance, Clothing::Layer::GLOVE_OVER_HAIR, interframe, args);
		}
	}

	void CharLook::draw(const DrawArgument &args, float alpha) const
	{
		if (!body || !hair || !face)
			return;

		Point<int16_t> acmove;

		if (action)
			acmove = action->get_move();

		DrawArgument relargs = {acmove, flip};

		Stance::Id interstance = stance.get(alpha);
		Expression::Id interexpression = expression.get(alpha);
		uint8_t interframe = stframe.get(alpha);
		uint8_t interexpframe = expframe.get(alpha);

		switch (interstance)
		{
			case Stance::Id::STAND1:
			case Stance::Id::STAND2:
				if (alerted)
					interstance = Stance::Id::ALERT;

				break;
		}

		draw(relargs + args, interstance, interexpression, interframe, interexpframe);
	}

	void CharLook::draw(Point<int16_t> position, bool flipped, Stance::Id interstance,
						Expression::Id interexpression) const
	{
		interstance = equips.adjust_stance(interstance);
		draw({position, flipped}, interstance, interexpression, 0, 0);
	}

	bool CharLook::update(uint16_t timestep)
	{
		if (timestep == 0)
		{
			stance.normalize();
			stframe.normalize();
			expression.normalize();
			expframe.normalize();
			return false;
		}

		alerted.update();
		expcooldown.update();

		bool aniend = false;

		if (action == nullptr)
		{
			uint16_t delay = get_delay(stance.get(), stframe.get());
			uint16_t delta = delay - stelapsed;

			if (timestep >= delta)
			{
				stelapsed = timestep - delta;

				uint8_t nextframe = getnextframe(stance.get(), stframe.get());
				float threshold = static_cast<float>(delta) / timestep;
				stframe.next(nextframe, threshold);

				if (stframe == 0)
					aniend = true;
			} else
			{
				stance.normalize();
				stframe.normalize();

				stelapsed += timestep;
			}
		} else
		{
			uint16_t delay = action->get_delay();
			uint16_t delta = delay - stelapsed;

			if (timestep >= delta)
			{
				stelapsed = timestep - delta;
				actframe = drawinfo.next_actionframe(actionstr, actframe);

				if (actframe > 0)
				{
					action = drawinfo.get_action(actionstr, actframe);

					float threshold = static_cast<float>(delta) / timestep;
					stance.next(action->get_stance(), threshold);
					stframe.next(action->get_frame(), threshold);
				} else
				{
					aniend = true;
					action = nullptr;
					actionstr = "";
					set_stance(Stance::Id::STAND1);
				}
			} else
			{
				stance.normalize();
				stframe.normalize();

				stelapsed += timestep;
			}
		}

		uint16_t expdelay = face->get_delay(expression.get(), expframe.get());
		uint16_t expdelta = expdelay - expelapsed;

		if (timestep >= expdelta)
		{
			expelapsed = timestep - expdelta;

			uint8_t nextexpframe = face->nextframe(expression.get(), expframe.get());
			float fcthreshold = static_cast<float>(expdelta) / timestep;
			expframe.next(nextexpframe, fcthreshold);

			if (expframe == 0)
			{
				if (expression == Expression::Id::DEFAULT)
					expression.next(Expression::Id::BLINK, fcthreshold);
				else
					expression.next(Expression::Id::DEFAULT, fcthreshold);
			}
		} else
		{
			expression.normalize();
			expframe.normalize();

			expelapsed += timestep;
		}

		return aniend;
	}

	void CharLook::set_body(int32_t skin_id)
	{
		auto iter = bodytypes.find(skin_id);

		if (iter == bodytypes.end())
		{
			iter = bodytypes.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(skin_id),
					std::forward_as_tuple(skin_id, drawinfo)
			).first;
		}

		body = &iter->second;
	}

	void CharLook::set_hair(int32_t hair_id)
	{
		auto iter = hairstyles.find(hair_id);

		if (iter == hairstyles.end())
		{
			iter = hairstyles.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(hair_id),
					std::forward_as_tuple(hair_id, drawinfo)
			).first;
		}

		hair = &iter->second;
	}

	void CharLook::set_face(int32_t face_id)
	{
		auto iter = facetypes.find(face_id);

		if (iter == facetypes.end())
			iter = facetypes.emplace(face_id, face_id).first;

		face = &iter->second;
	}

	void CharLook::updatetwohanded()
	{
		Stance::Id basestance = Stance::baseof(stance.get());
		set_stance(basestance);
	}

	void CharLook::add_equip(int32_t itemid)
	{
		equips.add_equip(itemid, drawinfo);
		updatetwohanded();
	}

	void CharLook::remove_equip(EquipSlot::Id slot)
	{
		equips.remove_equip(slot);

		if (slot == EquipSlot::Id::WEAPON)
			updatetwohanded();
	}

	void CharLook::attack(bool degenerate)
	{
		int32_t weapon_id = equips.get_weapon();

		if (weapon_id <= 0)
			return;

		const WeaponData &weapon = WeaponData::get(weapon_id);

		uint8_t attacktype = weapon.get_attack();

		if (attacktype == 9 && !degenerate)
		{
			stance.set(Stance::Id::SHOT);
			set_action("handgun");
		} else
		{
			stance.set(getattackstance(attacktype, degenerate));
			stframe.set(0);
			stelapsed = 0;
		}

		weapon.get_usesound(degenerate).play();
	}

	void CharLook::attack(Stance::Id newstance)
	{
		if (action || newstance == Stance::Id::NONE)
			return;

		switch (newstance)
		{
			case Stance::Id::SHOT:
				set_action("handgun");
				break;
			default:
				set_stance(newstance);
				break;
		}
	}

	void CharLook::set_stance(Stance::Id newstance)
	{
		if (action || newstance == Stance::Id::NONE)
			return;

		Stance::Id adjstance = equips.adjust_stance(newstance);

		if (stance != adjstance)
		{
			stance.set(adjstance);
			stframe.set(0);
			stelapsed = 0;
		}
	}

	Stance::Id CharLook::getattackstance(uint8_t attack, bool degenerate) const
	{
		if (stance == Stance::Id::PRONE)
			return Stance::Id::PRONESTAB;

		enum Attack
		{
			NONE = 0,
			S1A1M1D = 1,
			SPEAR = 2,
			BOW = 3,
			CROSSBOW = 4,
			S2A2M2 = 5,
			WAND = 6,
			CLAW = 7,
			GUN = 9,
			NUM_ATTACKS
		};

		static const std::array<std::vector<Stance::Id>, Attack::NUM_ATTACKS> degen_stances = {
				{
						{Stance::Id::NONE},
						{Stance::Id::NONE},
						{Stance::Id::NONE},
						{Stance::Id::SWINGT1, Stance::Id::SWINGT3},
						{Stance::Id::SWINGT1, Stance::Id::STABT1},
						{Stance::Id::NONE},
						{Stance::Id::NONE},
						{Stance::Id::SWINGT1, Stance::Id::STABT1},
						{Stance::Id::NONE},
						{Stance::Id::SWINGP1, Stance::Id::STABT2}
				}
		};

		static const std::array<std::vector<Stance::Id>, NUM_ATTACKS> attack_stances = {
				{
						{Stance::Id::NONE},
						{Stance::Id::STABO1, Stance::Id::STABO2, Stance::Id::SWINGO1, Stance::Id::SWINGO2, Stance::Id::SWINGO3},
						{Stance::Id::STABT1, Stance::Id::SWINGP1},
						{Stance::Id::SHOOT1},
						{Stance::Id::SHOOT2},
						{Stance::Id::STABO1, Stance::Id::STABO2, Stance::Id::SWINGT1, Stance::Id::SWINGT2, Stance::Id::SWINGT3},
						{Stance::Id::SWINGO1, Stance::Id::SWINGO2},
						{Stance::Id::SWINGO1, Stance::Id::SWINGO2},
						{Stance::Id::NONE},
						{Stance::Id::SHOT}
				}
		};

		if (attack <= Attack::NONE || attack >= Attack::NUM_ATTACKS)
			return Stance::Id::STAND1;

		const auto &stances = degenerate ? degen_stances[attack] : attack_stances[attack];

		if (stances.empty())
			return Stance::Id::STAND1;

		size_t index = randomizer.next_int(stances.size());

		return stances[index];
	}

	uint16_t CharLook::get_delay(Stance::Id st, uint8_t fr) const
	{
		return drawinfo.get_delay(st, fr);
	}

	uint8_t CharLook::getnextframe(Stance::Id st, uint8_t fr) const
	{
		return drawinfo.nextframe(st, fr);
	}

	void CharLook::set_expression(Expression::Id newexpression)
	{
		if (expression != newexpression && !expcooldown)
		{
			expression.set(newexpression);
			expframe.set(0);

			expelapsed = 0;
			expcooldown.set_for(5000);
		}
	}

	void CharLook::set_action(const std::string &acstr)
	{
		if (acstr == actionstr || acstr == "")
			return;

		if (Stance::Id ac_stance = Stance::by_string(acstr))
		{
			set_stance(ac_stance);
		} else
		{
			action = drawinfo.get_action(acstr, 0);

			if (action)
			{
				actframe = 0;
				stelapsed = 0;
				actionstr = acstr;

				stance.set(action->get_stance());
				stframe.set(action->get_frame());
			}
		}
	}

	void CharLook::set_direction(bool f)
	{
		flip = f;
	}

	void CharLook::set_alerted(int64_t millis)
	{
		alerted.set_for(millis);
	}

	bool CharLook::get_alerted() const
	{
		return (bool) alerted;
	}

	bool CharLook::is_twohanded(Stance::Id st) const
	{
		switch (st)
		{
			case Stance::Id::STAND1:
			case Stance::Id::WALK1:
				return false;
			case Stance::Id::STAND2:
			case Stance::Id::WALK2:
				return true;
			default:
				return equips.is_twohanded();
		}
	}

	uint16_t CharLook::get_attackdelay(size_t no, uint8_t first_frame) const
	{
		if (action)
		{
			return drawinfo.get_attackdelay(actionstr, no);
		} else
		{
			uint16_t delay = 0;

			for (uint8_t frame = 0; frame < first_frame; frame++)
				delay += get_delay(stance.get(), frame);

			return delay;
		}
	}

	uint8_t CharLook::get_frame() const
	{
		return stframe.get();
	}

	Stance::Id CharLook::get_stance() const
	{
		return stance.get();
	}

	const Body *CharLook::get_body() const
	{
		return body;
	}

	const Hair *CharLook::get_hair() const
	{
		return hair;
	}

	const Face *CharLook::get_face() const
	{
		return face;
	}

	const CharEquips &CharLook::get_equips() const
	{
		return equips;
	}

	void CharLook::init()
	{
		drawinfo.init();
	}

	BodyDrawInfo CharLook::drawinfo;
	std::unordered_map<int32_t, Hair> CharLook::hairstyles;
	std::unordered_map<int32_t, Face> CharLook::facetypes;
	std::unordered_map<int32_t, Body> CharLook::bodytypes;
}