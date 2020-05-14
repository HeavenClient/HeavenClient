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

#include "../../Graphics/Animation.h"
#include "../../Graphics/Text.h"
#include "../../Template/EnumMap.h"

#include "../../Gameplay/Physics/Physics.h"

namespace ms
{
	class PetLook
	{
	public:
		enum Stance : uint8_t
		{
			MOVE,
			STAND,
			JUMP,
			ALERT,
			PRONE,
			FLY,
			HANG,
			WARP,
			LENGTH
		};

		static Stance stancebyvalue(uint8_t value)
		{
			uint8_t valueh = value / 2;

			return valueh >= LENGTH ? STAND : static_cast<Stance>(valueh);
		}

		PetLook(int32_t iid, std::string name, int32_t uniqueid, Point<int16_t> pos, uint8_t stance, int32_t fhid);

		PetLook();

		void draw(double viewx, double viewy, float alpha) const;

		void update(const Physics& physics, Point<int16_t> charpos);

		void set_position(int16_t xpos, int16_t ypos);

		void set_stance(Stance stance);

		void set_stance(uint8_t stancebyte);

		int32_t get_itemid() const;

		Stance get_stance() const;

	private:
		int32_t itemid;
		std::string name;
		int32_t uniqueid;
		Stance stance;
		bool flip;

		EnumMap<Stance, Animation> animations;
		PhysicsObject phobj;
		Text namelabel;
	};
}