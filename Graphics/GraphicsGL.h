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

#include "Text.h"

#include "../Constants.h"
#include "../Error.h"

#include "../Util/QuadTree.h"

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <nlnx/bitmap.hpp>

namespace ms
{
	// Graphics engine which uses OpenGL
	class GraphicsGL : public Singleton<GraphicsGL>
	{
	public:
		GraphicsGL();

		// Initialize all resources
		Error init();

		// Re-initialise after changing screen modes.
		void reinit();

		// Clear all bitmaps if most of the space is used up
		void clear();

		// Add a bitmap to the available resources.
		void addbitmap(const nl::bitmap &bmp);

		// Draw the bitmap with the given parameters.
		void draw(const nl::bitmap &bmp, const Rectangle<int16_t> &rect, const Color &color, float angle);

		// Create a layout for the text with the parameters specified.
		Text::Layout createlayout(const std::string &text, Text::Font font, Text::Alignment alignment, int16_t maxwidth,
								  bool formatted, int16_t line_adj);

		// Draw a text with the given parameters.
		void drawtext(const DrawArgument &args, const Range<int16_t> &vertical, const std::string &text,
					  const Text::Layout &layout, Text::Font font, Color::Name color, Text::Background back);

		// Draw a rectangle filled with the specified color.
		void drawrectangle(int16_t x, int16_t y, int16_t width, int16_t height, float red, float green, float blue,
						   float alpha);

		// Fill the screen with the specified color.
		void drawscreenfill(float red, float green, float blue, float alpha);

		// Lock the current scene
		void lock();

		// Unlock the scene.
		void unlock();

		// Draw the buffer contents with the specified scene opacity
		void flush(float opacity);

		// Clear the buffer contents.
		void clearscene();

	private:
		void clearinternal();

		bool addfont(const char *name, Text::Font id, FT_UInt width, FT_UInt height);

		struct Offset
		{
			GLshort left;
			GLshort right;
			GLshort top;
			GLshort bottom;

			Offset(GLshort x, GLshort y, GLshort width, GLshort height)
			{
				left = x;
				right = x + width;
				top = y;
				bottom = y + height;
			}

			Offset()
			{
				left = 0;
				right = 0;
				top = 0;
				bottom = 0;
			}
		};

		// Add a bitmap to the available resources.
		const Offset &getoffset(const nl::bitmap &bmp);

		struct Leftover
		{
			GLshort left;
			GLshort right;
			GLshort top;
			GLshort bottom;

			Leftover(GLshort x, GLshort y, GLshort width, GLshort height)
			{
				left = x;
				right = x + width;
				top = y;
				bottom = y + height;
			}

			Leftover()
			{
				left = 0;
				right = 0;
				top = 0;
				bottom = 0;
			}

			GLshort width() const
			{
				return right - left;
			}

			GLshort height() const
			{
				return bottom - top;
			}
		};

		struct Quad
		{
			struct Vertex
			{
				// Local Space Position
				GLshort localcoord_x;
				GLshort localcoord_y;

				// Texture Coordinates
				GLshort texcoord_x;
				GLshort texcoord_y;

				Color color;
			};

			static const size_t LENGTH = 4;
			Vertex vertices[LENGTH];

			Quad(GLshort left, GLshort right, GLshort top, GLshort bottom, const Offset &offset, const Color &color,
				 GLfloat rotation)
			{
				vertices[0] = {left, top, offset.left, offset.top, color};
				vertices[1] = {left, bottom, offset.left, offset.bottom, color};
				vertices[2] = {right, bottom, offset.right, offset.bottom, color};
				vertices[3] = {right, top, offset.right, offset.top, color};

				if (rotation != 0.0f)
				{
					GLfloat cos = std::cos(rotation);
					GLfloat sin = std::sin(rotation);
					GLshort center_x = (left + right) / 2;
					GLshort center_y = (top + bottom) / 2;

					for (size_t i = 0; i < LENGTH; i++)
					{
						GLshort vertice_x = vertices[i].localcoord_x - center_x;
						GLshort vertice_y = vertices[i].localcoord_y - center_y;
						GLfloat rounded_x = std::roundf(vertice_x * cos - vertice_y * sin);
						GLfloat rounded_y = std::roundf(vertice_x * sin + vertice_y * cos);
						vertices[i].localcoord_x = static_cast<GLshort>(rounded_x + center_x);
						vertices[i].localcoord_y = static_cast<GLshort>(rounded_y + center_y);
					}
				}
			}
		};

		struct Font
		{
			struct Char
			{
				GLshort ax;
				GLshort ay;
				GLshort bw;
				GLshort bh;
				GLshort bl;
				GLshort bt;
				Offset offset;
			};

			GLshort width;
			GLshort height;
			Char chars[128];

			Font(GLshort w, GLshort h)
			{
				width = w;
				height = h;
			}

			Font()
			{
				width = 0;
				height = 0;
			}

			int16_t linespace() const
			{
				return static_cast<int16_t>(height * 1.35 + 1);
			}
		};

		class LayoutBuilder
		{
		public:
			LayoutBuilder(const Font &font, Text::Alignment alignment, int16_t maxwidth, bool formatted,
						  int16_t line_adj);

			size_t add(const char *text, size_t prev, size_t first, size_t last);

			Text::Layout finish(size_t first, size_t last);

		private:
			void add_word(size_t first, size_t last, Text::Font font, Color::Name color);

			void add_line();

			const Font &font;

			Text::Alignment alignment;
			Text::Font fontid;
			Color::Name color;
			int16_t maxwidth;
			bool formatted;

			int16_t ax;
			int16_t ay;

			std::vector<Text::Layout::Line> lines;
			std::vector<Text::Layout::Word> words;
			std::vector<int16_t> advances;
			int16_t width;
			int16_t endy;
			int16_t line_adj;
		};

		int16_t VWIDTH;
		int16_t VHEIGHT;
		Rectangle<int16_t> SCREEN;

		static const GLshort ATLASW = 8192;
		static const GLshort ATLASH = 8192;
		static const GLshort MINLOSIZE = 32;

		bool locked;

		std::vector<Quad> quads;
		GLuint VBO;
		GLuint atlas;

		GLint shaderProgram;
		GLint attribute_coord;
		GLint attribute_color;
		GLint uniform_texture;
		GLint uniform_atlassize;
		GLint uniform_screensize;
		GLint uniform_yoffset;
		GLint uniform_fontregion;

		std::unordered_map<size_t, Offset> offsets;
		Offset nulloffset;

		QuadTree<size_t, Leftover> leftovers;
		size_t rlid;
		size_t wasted;
		Point<GLshort> border;
		Range<GLshort> yrange;

		FT_Library ftlibrary;
		Font fonts[Text::Font::NUM_FONTS];
		Point<GLshort> fontborder;
		GLshort fontymax;
	};
}