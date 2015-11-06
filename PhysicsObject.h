/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cstdint>

namespace Gameplay
{
	using::std::int8_t;
	using::std::uint16_t;
	// Determines which physics engine to use for an object. Currently only 'Normal' is implemented.
	enum PhysicsTerrain
	{
		PHT_NORMAL,
		PHT_ICE,
		PHT_SWIMMING,
		PHT_FLYING
	};

	// Struct that contains all properties neccessary for physics calculations.
	struct PhysicsObject
	{
		PhysicsTerrain terrain = PHT_NORMAL;
		uint16_t fhid = 0;
		float fhslope = 0.0f;
		int8_t fhlayer = 0;
		bool canfall = true;
		bool onground = true;
		float mass = 1.0f;
		float fx = 0.0f;
		float fy = 0.0f;
		float hforce = 0.0f;
		float vforce = 0.0f;
		float hacc = 0.0f;
		float vacc = 0.0f;
		float hspeed = 0.0f;
		float vspeed = 0.0f;
	};
}
