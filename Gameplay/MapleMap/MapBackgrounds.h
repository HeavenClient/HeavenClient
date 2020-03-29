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

#include "../Console.h"
#include "../Constants.h"

#include "../Physics/PhysicsObject.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Animation.h"

#include <vector>

namespace ms
{
	class Background
	{
	public:
		Background(nl::node src);

		void draw(double viewx, double viewy, float alpha) const;

		void update();

	private:
		enum Type
		{
			NORMAL,
			HTILED,
			VTILED,
			TILED,
			HMOVEA,
			VMOVEA,
			HMOVEB,
			VMOVEB
		};

		static Type typebyid(int32_t id)
		{
			if (id >= Type::NORMAL && id <= Type::VMOVEB)
				return static_cast<Type>(id);

			Console::get().print("Unhandled background type: " + std::to_string(id));

			return Type::NORMAL;
		}

		void settype(Type type);

		int16_t VWIDTH;
		int16_t VHEIGHT;
		int16_t WOFFSET;
		int16_t HOFFSET;

		Animation animation;
		bool animated;
		int16_t cx;
		int16_t cy;
		double rx;
		double ry;
		int16_t htile;
		int16_t vtile;
		float opacity;
		bool flipped;

		MovingObject moveobj;
	};

	class MapBackgrounds
	{
	public:
		MapBackgrounds(nl::node src);

		MapBackgrounds();

		void drawbackgrounds(double viewx, double viewy, float alpha) const;

		void drawforegrounds(double viewx, double viewy, float alpha) const;

		void update();

	private:
		std::vector<Background> backgrounds;
		std::vector<Background> foregrounds;
		bool black;
	};
}