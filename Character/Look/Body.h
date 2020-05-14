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
	class Body
	{
	public:
		enum Layer
		{
			NONE,
			BODY,
			ARM,
			ARM_BELOW_HEAD,
			ARM_BELOW_HEAD_OVER_MAIL,
			ARM_OVER_HAIR,
			ARM_OVER_HAIR_BELOW_WEAPON,
			HAND_BELOW_WEAPON,
			HAND_OVER_HAIR,
			HAND_OVER_WEAPON,
			HEAD,
			NUM_LAYERS
		};

		Body(int32_t skin, const BodyDrawInfo& drawinfo);

		void draw(Stance::Id stance, Layer layer, uint8_t frame, const DrawArgument& args) const;

		const std::string& get_name() const;

		static Layer layer_by_name(const std::string& name);

	private:
		std::unordered_map<uint8_t, Texture> stances[Stance::Id::LENGTH][Layer::NUM_LAYERS];
		std::string name;

		static const std::unordered_map<std::string, Layer> layers_by_name;
	};
}