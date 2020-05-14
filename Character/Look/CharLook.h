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

#include "Body.h"
#include "CharEquips.h"
#include "Face.h"
#include "Hair.h"

#include "../../Net/Login.h"
#include "../../Template/Interpolated.h"
#include "../../Util/Randomizer.h"
#include "../../Util/TimedBool.h"

namespace ms
{
	class CharLook
	{
	public:
		CharLook(const LookEntry& entry);

		CharLook();

		void reset();

		void draw(const DrawArgument& args, float alpha) const;

		void draw(Point<int16_t> pos, bool flip, Stance::Id stance, Expression::Id expression) const;

		bool update(uint16_t timestep);

		void set_hair(int32_t hairid);

		void set_body(int32_t skinid);

		void set_face(int32_t faceid);

		void add_equip(int32_t equipid);

		void remove_equip(EquipSlot::Id slot);

		void attack(bool degenerate);

		void attack(Stance::Id stance);

		void set_stance(Stance::Id stance);

		void set_expression(Expression::Id expression);

		void set_action(const std::string& action);

		void set_direction(bool mirrored);

		void set_alerted(int64_t millis);

		bool get_alerted() const;

		bool is_twohanded(Stance::Id stance) const;

		uint16_t get_attackdelay(size_t no, uint8_t first_frame) const;

		uint8_t get_frame() const;

		Stance::Id get_stance() const;

		const Body* get_body() const;

		const Hair* get_hair() const;

		const Face* get_face() const;

		const CharEquips& get_equips() const;

		// Initialize drawinfo
		static void init();

	private:
		void updatetwohanded();

		void draw(const DrawArgument& args, Stance::Id interstance, Expression::Id interexp, uint8_t interframe,
				  uint8_t interfcframe) const;

		uint16_t get_delay(Stance::Id stance, uint8_t frame) const;

		uint8_t getnextframe(Stance::Id stance, uint8_t frame) const;

		Stance::Id getattackstance(uint8_t attack, bool degenerate) const;

		Nominal<Stance::Id> stance;
		Nominal<uint8_t> stframe;
		uint16_t stelapsed;

		Nominal<Expression::Id> expression;
		Nominal<uint8_t> expframe;
		uint16_t expelapsed;
		TimedBool expcooldown;

		bool flip;

		const BodyAction* action;
		std::string actionstr;
		uint8_t actframe;

		const Body* body;
		const Hair* hair;
		const Face* face;
		CharEquips equips;

		Randomizer randomizer;
		TimedBool alerted;

		static BodyDrawInfo drawinfo;
		static std::unordered_map<int32_t, Hair> hairstyles;
		static std::unordered_map<int32_t, Face> facetypes;
		static std::unordered_map<int32_t, Body> bodytypes;
	};
}