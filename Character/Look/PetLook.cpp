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
#include "PetLook.h"

#ifdef USE_NX

#include <nlnx/nx.hpp>

#endif

namespace ms
{
	PetLook::PetLook(int32_t iid, std::string nm, int32_t uqid, Point<int16_t> pos, uint8_t st, int32_t)
	{
		itemid = iid;
		name = nm;
		uniqueid = uqid;

		set_position(pos.x(), pos.y());
		set_stance(st);

		namelabel = Text(Text::Font::A13M, Text::Alignment::CENTER, Color::Name::WHITE, Text::Background::NAMETAG,
						 name);

		std::string strid = std::to_string(iid);
		nl::node src = nl::nx::item["Pet"][strid + ".img"];

		animations[Stance::MOVE] = src["move"];
		animations[Stance::STAND] = src["stand0"];
		animations[Stance::JUMP] = src["jump"];
		animations[Stance::ALERT] = src["alert"];
		animations[Stance::PRONE] = src["prone"];
		animations[Stance::FLY] = src["fly"];
		animations[Stance::HANG] = src["hang"];

		nl::node effsrc = nl::nx::effect["PetEff.img"][strid];

		animations[Stance::WARP] = effsrc["warp"];
	}

	PetLook::PetLook()
	{
		itemid = 0;
		name = "";
		uniqueid = 0;
		stance = Stance::STAND;
	}

	void PetLook::draw(double viewx, double viewy, float alpha) const
	{
		Point<int16_t> absp = phobj.get_absolute(viewx, viewy, alpha);

		animations[stance].draw(DrawArgument(absp, flip), alpha);
		namelabel.draw(absp);
	}

	void PetLook::update(const Physics& physics, Point<int16_t> charpos)
	{
		static const double PETWALKFORCE = 0.35;
		static const double PETFLYFORCE = 0.2;

		Point<int16_t> curpos = phobj.get_position();

		switch (stance)
		{
			case Stance::STAND:
			case Stance::MOVE:
				if (curpos.distance(charpos) > 150)
				{
					set_position(charpos.x(), charpos.y());
				}
				else
				{
					if (charpos.x() - curpos.x() > 50)
					{
						phobj.hforce = PETWALKFORCE;
						flip = true;

						set_stance(Stance::MOVE);
					}
					else if (charpos.x() - curpos.x() < -50)
					{
						phobj.hforce = -PETWALKFORCE;
						flip = false;

						set_stance(Stance::MOVE);
					}
					else
					{
						phobj.hforce = 0.0;

						set_stance(Stance::STAND);
					}
				}

				phobj.type = PhysicsObject::Type::NORMAL;
				phobj.clear_flag(PhysicsObject::Flag::NOGRAVITY);
				break;
			case Stance::HANG:
				set_position(charpos.x(), charpos.y());
				phobj.set_flag(PhysicsObject::Flag::NOGRAVITY);
				break;
			case Stance::FLY:
				if ((charpos - curpos).length() > 250)
				{
					set_position(charpos.x(), charpos.y());
				}
				else
				{
					if (charpos.x() - curpos.x() > 50)
					{
						phobj.hforce = PETFLYFORCE;
						flip = true;
					}
					else if (charpos.x() - curpos.x() < -50)
					{
						phobj.hforce = -PETFLYFORCE;
						flip = false;
					}
					else
					{
						phobj.hforce = 0.0f;
					}

					if (charpos.y() - curpos.y() > 50.0f)
						phobj.vforce = PETFLYFORCE;
					else if (charpos.y() - curpos.y() < -50.0f)
						phobj.vforce = -PETFLYFORCE;
					else
						phobj.vforce = 0.0f;
				}

				phobj.type = PhysicsObject::Type::FLYING;
				phobj.clear_flag(PhysicsObject::Flag::NOGRAVITY);
				break;
		}

		physics.move_object(phobj);

		animations[stance].update();
	}

	void PetLook::set_position(int16_t x, int16_t y)
	{
		phobj.set_x(x);
		phobj.set_y(y);
	}

	void PetLook::set_stance(Stance st)
	{
		if (stance != st)
		{
			stance = st;
			animations[stance].reset();
		}
	}

	void PetLook::set_stance(uint8_t stancebyte)
	{
		flip = stancebyte % 2 == 1;
		stance = stancebyvalue(stancebyte);
	}

	int32_t PetLook::get_itemid() const
	{
		return itemid;
	}

	PetLook::Stance PetLook::get_stance() const
	{
		return stance;
	}
}