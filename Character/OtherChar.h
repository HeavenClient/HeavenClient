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

#include "Char.h"

#include "Look/CharLook.h"

#include "../Gameplay/Movement.h"

#include <queue>
#include <vector>

namespace ms
{
	// Other client's players.
	class OtherChar : public Char
	{
	public:
		OtherChar(int32_t charid, const CharLook &look, uint8_t level, int16_t job, const std::string &name,
				  int8_t stance, Point<int16_t> position);

		// Update the character.
		int8_t update(const Physics &physics) override;

		// Add the movements which this character will go through next.
		void send_movement(const std::vector<Movement> &movements);

		// Update a skill level.
		void update_skill(int32_t skillid, uint8_t skilllevel);

		// Update the attack speed.
		void update_speed(uint8_t attackspeed);

		// Update the character look.
		void update_look(const LookEntry &look);

		// Return the character's attacking speed.
		int8_t get_integer_attackspeed() const override;

		// Return the character's level.
		uint16_t get_level() const override;

		// Return the character's level of a skill.
		int32_t get_skilllevel(int32_t skillid) const override;

	private:
		uint16_t level;
		int16_t job;
		std::queue<Movement> movements;
		Movement lastmove;
		uint16_t timer;

		std::unordered_map<int32_t, uint8_t> skilllevels;
		uint8_t attackspeed;
	};
}