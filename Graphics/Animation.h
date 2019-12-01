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

#include "Texture.h"

#include "../Template/Interpolated.h"
#include "../Template/Rectangle.h"

#include <vector>

namespace ms
{
	// A single frame within an animation.
	class Frame
	{
	public:
		Frame(nl::node src);
		Frame();

		void draw(const DrawArgument& args) const;

		uint8_t start_opacity() const;
		uint16_t start_scale() const;
		uint16_t get_delay() const;
		Point<int16_t> get_origin() const;
		Point<int16_t> get_dimensions() const;
		Point<int16_t> get_head() const;
		Rectangle<int16_t> get_bounds() const;
		float opcstep(uint16_t timestep) const;
		float scalestep(uint16_t timestep) const;

	private:
		Texture texture;
		uint16_t delay;
		std::pair<uint8_t, uint8_t> opacities;
		std::pair<int16_t, int16_t> scales;
		Rectangle<int16_t> bounds;
		Point<int16_t> head;
	};

	// Class which consists of multiple textures to make an Animation.
	class Animation
	{
	public:
		Animation(nl::node source);
		Animation();

		bool update();
		bool update(uint16_t timestep);
		void reset();

		void draw(const DrawArgument& arguments, float alpha) const;

		uint16_t get_delay(int16_t frame) const;
		uint16_t getdelayuntil(int16_t frame) const;
		Point<int16_t> get_origin() const;
		Point<int16_t> get_dimensions() const;
		Point<int16_t> get_head() const;
		Rectangle<int16_t> get_bounds() const;

	private:
		const Frame& get_frame() const;

		std::vector<Frame> frames;
		bool animated;
		bool zigzag;

		Nominal<int16_t> frame;
		Linear<float> opacity;
		Linear<float> xyscale;

		uint16_t delay;
		int16_t framestep;
		float opcstep;
	};
}