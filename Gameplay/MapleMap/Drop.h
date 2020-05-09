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

#include "MapObject.h"

#include "../../Template/Rectangle.h"

namespace ms
{
	class Drop : public MapObject
	{
	public:
		virtual int8_t update(const Physics &physics) override;

		void expire(int8_t, const PhysicsObject *);

		Rectangle<int16_t> bounds() const;

	protected:
		Drop(int32_t oid, int32_t owner, Point<int16_t> start,
			 Point<int16_t> dest, int8_t type, int8_t mode, bool playerdrop);

		Linear<float> opacity;
		Linear<float> angle;

	private:
		enum State
		{
			DROPPED,
			FLOATING,
			PICKEDUP
		};

		int32_t owner;
		int8_t pickuptype;
		bool playerdrop;

		const PhysicsObject *looter;
		State state;

		Point<int16_t> dest;
		double basey;
		double moved;
	};
}