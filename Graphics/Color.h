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

#if defined(__linux__) || defined(__APPLE__)

#include <glad/glad.h>

#else
#define GLEW_STATIC
#include <glew.h>
#endif

#include <array>
#include <algorithm>

namespace ms
{
	// Simple color class which stores RGBA components
	class Color
	{
	public:
		static constexpr size_t LENGTH = 4;
		using underlying_t = std::array<float, LENGTH>;

		// Codes of predefined colors
		enum Code : uint32_t
		{
			CNONE = 0x00000000,
			CWHITE = 0xFFFFFFFF,
			CBLACK = 0x000000FF,
			CRED = 0xFF0000FF,
			CGREEN = 0x00FF00FF,
			CBLUE = 0x0000FFFF,
			CYELLOW = 0xFFFF00FF,
			CTURQUOISE = 0x00FFFFFF,
			CPURPLE = 0xFF00FFFF
		};

		// Name of predefined colors
		enum Name : uint32_t
		{
			BLACK,
			WHITE,
			YELLOW,
			BLUE,
			RED,
			DARKRED,
			BROWN,
			JAMBALAYA,
			LIGHTGREY,
			DARKGREY,
			ORANGE,
			MEDIUMBLUE,
			VIOLET,
			TOBACCOBROWN,
			EAGLE,
			LEMONGRASS,
			TUNA,
			GALLERY,
			DUSTYGRAY,
			EMPEROR,
			MINESHAFT,
			HALFANDHALF,
			ENDEAVOUR,
			BROWNDERBY,
			PORCELAIN,
			IRISHCOFFEE,
			BOULDER,
			GREEN,
			LIGHTGREEN,
			JAPANESELAUREL,
			GRAYOLIVE,
			ELECTRICLIME,
			SUPERNOVA,
			CHARTREUSE,
			MALIBU,
			SILVERCHALICE,
			GRAY,
			TORCHRED,
			CREAM,
			SMALT,
			PRUSSIANBLUE,
			NUM_COLORS
		};

		// Predefined colors by name
		static constexpr GLfloat colors[Name::NUM_COLORS][3] =
			{
				{0.00f, 0.00f, 0.00f}, // Black
				{1.00f, 1.00f, 1.00f}, // White
				{1.00f, 1.00f, 0.00f}, // Yellow
				{0.00f, 0.00f, 1.00f}, // Blue
				{1.00f, 0.00f, 0.00f}, // Red
				{0.80f, 0.30f, 0.30f}, // DarkRed
				{0.50f, 0.25f, 0.00f}, // Brown
				{0.34f, 0.20f, 0.07f}, // Jambalaya
				{0.50f, 0.50f, 0.50f}, // Lightgrey
				{0.25f, 0.25f, 0.25f}, // Darkgrey
				{1.00f, 0.50f, 0.00f}, // Orange
				{0.00f, 0.75f, 1.00f}, // Mediumblue
				{0.50f, 0.00f, 0.50f}, // Violet
				{0.47f, 0.40f, 0.27f}, // Tobacco Brown
				{0.74f, 0.74f, 0.67f}, // Eagle
				{0.60f, 0.60f, 0.54f}, // Lemon Grass
				{0.20f, 0.20f, 0.27f}, // Tuna
				{0.94f, 0.94f, 0.94f}, // Gallery
				{0.60f, 0.60f, 0.60f}, // Dusty Gray
				{0.34f, 0.34f, 0.34f}, // Emperor
				{0.20f, 0.20f, 0.20f}, // Mine Shaft
				{1.00f, 1.00f, 0.87f}, // Half and Half
				{0.00f, 0.40f, 0.67f}, // Endeavour
				{0.30f, 0.20f, 0.10f}, // Brown Derby
				{0.94f, 0.95f, 0.95f}, // Porcelain
				{0.34f, 0.27f, 0.14f}, // Irish Coffee
				{0.47f, 0.47f, 0.47f}, // Boulder
				{0.00f, 0.75f, 0.00f}, // Green (Mob HP Bar)
				{0.00f, 1.00f, 0.00f}, // Light Green (Mob HP Bar)
				{0.00f, 0.50f, 0.00f}, // Japanese Laurel (Mob HP Bar)
				{0.67f, 0.67f, 0.60f}, // Gray Olive
				{0.80f, 1.00f, 0.00f}, // Electric Lime
				{1.00f, 0.80f, 0.00f}, // Supernova
				{0.47f, 1.00f, 0.00f}, // Chartreuse
				{0.47f, 0.80f, 1.00f}, // Malibu
				{0.67f, 0.67f, 0.67f}, // Silver Chalice
				{0.54f, 0.54f, 0.54f}, // Gray
				{0.94f, 0.00f, 0.20f}, // Torch Red
				{1.00f, 1.00f, 0.80f}, // Cream
				{0.00f, 0.23f, 0.56f}, // Smalt
				{0.01f, 0.19f, 0.28f}  // Prussian Blue
			};

		// Create a color by an array of real numbers [0.0f, 1.0f]
		constexpr Color(underlying_t comps) : rgba(comps)
		{}

		// Create a color by real numbers [0.0f, 1.0f]
		constexpr Color(float red, float green, float blue, float alpha) : Color(underlying_t{red, green, blue, alpha})
		{}

		// Create a color by an array of natural numbers [0, 255]
		constexpr Color(const std::array<uint8_t, Color::LENGTH> comps) : Color(comps[0], comps[1], comps[2], comps[3])
		{}

		// Create a color by natural numbers [0, 255]
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : Color(
			static_cast<float>(red) / 255,
			static_cast<float>(green) / 255,
			static_cast<float>(blue) / 255,
			static_cast<float>(alpha) / 255
		)
		{}

		// Create a color by code
		constexpr Color(uint32_t code) : Color(
			static_cast<uint8_t>(code >> 24),
			static_cast<uint8_t>(code >> 16),
			static_cast<uint8_t>(code >> 8),
			static_cast<uint8_t>(code)
		)
		{}

		constexpr Color() : Color(Code::CNONE)
		{}

		// Check whether the color is completely invisible
		constexpr bool invisible() const
		{
			return rgba[3] <= 0.0f;
		}

		// Return the red component
		constexpr float r() const
		{
			return rgba[0];
		}

		// Return the green component
		constexpr float g() const
		{
			return rgba[1];
		}

		// Return the blue component
		constexpr float b() const
		{
			return rgba[2];
		}

		// Return the alpha (opacity) component
		constexpr float a() const
		{
			return rgba[3];
		}

		// Return all components.
		const float* data() const;

		// Return a begin iterator
		underlying_t::const_iterator begin() const;

		// Return an end iterator
		underlying_t::const_iterator end() const;

		// Blend the second color into the first.
		Color blend(const Color& other, float alpha) const;

		// Combine two colors.
		constexpr Color operator*(const Color& o) const
		{
			return Color(
				r() * o.r(),
				g() * o.g(),
				b() * o.b(),
				a() * o.a()
			);
		}

		// Combine two colors.
		constexpr Color operator/(const Color& o) const
		{
			return Color(
				r() / o.r(),
				g() / o.g(),
				b() / o.b(),
				a() / o.a()
			);
		}

	private:
		underlying_t rgba;
	};
}