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

#include "../../Graphics/Texture.h"

namespace ms
{
	class Expression
	{
	public:
		enum Id
		{
			DEFAULT,
			BLINK,
			HIT,
			SMILE,
			TROUBLED,
			CRY,
			ANGRY,
			BEWILDERED,
			STUNNED,
			BLAZE,
			BOWING,
			CHEERS,
			CHU,
			DAM,
			DESPAIR,
			GLITTER,
			HOT,
			HUM,
			LOVE,
			OOPS,
			PAIN,
			SHINE,
			VOMIT,
			WINK,
			LENGTH
		};

		static Id byaction(size_t action);

		static const EnumMap<Id, std::string> names;
	};

	class Face
	{
	public:
		Face(int32_t faceid);

		void draw(Expression::Id expression, uint8_t frame, const DrawArgument &args) const;

		uint8_t nextframe(Expression::Id expression, uint8_t frame) const;

		int16_t get_delay(Expression::Id expression, uint8_t frame) const;

		const std::string &get_name() const;

	private:
		struct Frame
		{
			Texture texture;
			uint16_t delay;

			Frame(nl::node src)
			{
				texture = src["face"];

				Point<int16_t> shift = src["face"]["map"]["brow"];
				texture.shift(-shift);

				delay = src["delay"];

				if (delay == 0)
					delay = 2500;
			}
		};

		std::unordered_map<uint8_t, Frame> expressions[Expression::Id::LENGTH];
		std::string name;
	};
}