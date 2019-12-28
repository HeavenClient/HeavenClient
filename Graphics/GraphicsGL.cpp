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
#include "GraphicsGL.h"

#include "../Configuration.h"

namespace ms
{
	GraphicsGL::GraphicsGL()
	{
		locked = false;

		VWIDTH = Constants::Constants::get().get_viewwidth();
		VHEIGHT = Constants::Constants::get().get_viewheight();
		SCREEN = Rectangle<int16_t>(0, VWIDTH, 0, VHEIGHT);
	}

	Error GraphicsGL::init()
	{
		if (glewInit())
			return Error::Code::GLEW;

		if (FT_Init_FreeType(&ftlibrary))
			return Error::Code::FREETYPE;

		GLint result = GL_FALSE;
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);

		const char* vs_source =
			"#version 120\n"
			"attribute vec4 coord;"
			"attribute vec4 color;"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform vec2 screensize;"
			"uniform int yoffset;"

			"void main(void)"
			"{"
			"	float x = -1.0 + coord.x * 2.0 / screensize.x;"
			"	float y = 1.0 - (coord.y + yoffset) * 2.0 / screensize.y;"
			"   gl_Position = vec4(x, y, 0.0, 1.0);"
			"	texpos = coord.zw;"
			"	colormod = color;"
			"}";

		glShaderSource(vs, 1, &vs_source, NULL);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &result);

		if (!result)
			return Error::Code::VERTEX_SHADER;

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

		const char* fs_source =
			"#version 120\n"
			"varying vec2 texpos;"
			"varying vec4 colormod;"
			"uniform sampler2D texture;"
			"uniform vec2 atlassize;"
			"uniform int fontregion;"

			"void main(void)"
			"{"
			"	if (texpos.y == 0)"
			"	{"
			"		gl_FragColor = colormod;"
			"	}"
			"	else if (texpos.y <= fontregion)"
			"	{"
			"		gl_FragColor = vec4(1, 1, 1, texture2D(texture, texpos / atlassize).r) * colormod;"
			"	}"
			"	else"
			"	{"
			"		gl_FragColor = texture2D(texture, texpos / atlassize) * colormod;"
			"	}"
			"}";

		glShaderSource(fs, 1, &fs_source, NULL);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &result);

		if (!result)
			return Error::Code::FRAGMENT_SHADER;

		program = glCreateProgram();

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &result);

		if (!result)
			return Error::Code::SHADER_PROGRAM;

		attribute_coord = glGetAttribLocation(program, "coord");
		attribute_color = glGetAttribLocation(program, "color");
		uniform_texture = glGetUniformLocation(program, "texture");
		uniform_atlassize = glGetUniformLocation(program, "atlassize");
		uniform_screensize = glGetUniformLocation(program, "screensize");
		uniform_yoffset = glGetUniformLocation(program, "yoffset");
		uniform_fontregion = glGetUniformLocation(program, "fontregion");

		if (attribute_coord == -1 || attribute_color == -1 || uniform_texture == -1 || uniform_atlassize == -1 || uniform_yoffset == -1 || uniform_screensize == -1)
			return Error::Code::SHADER_VARS;

		glGenBuffers(1, &vbo);

		glGenTextures(1, &atlas);
		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLASW, ATLASH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		fontborder.set_y(1);

		const std::string FONT_NORMAL = Setting<FontPathNormal>().get().load();
		const std::string FONT_BOLD = Setting<FontPathBold>().get().load();

		if (FONT_NORMAL.empty() || FONT_BOLD.empty())
			Console::get().print("Warning: A font path is empty, check your settings file.");

		const char* FONT_NORMAL_STR = FONT_NORMAL.c_str();
		const char* FONT_BOLD_STR = FONT_BOLD.c_str();

		addfont(FONT_NORMAL_STR, Text::Font::A11M, 0, 11);
		addfont(FONT_BOLD_STR, Text::Font::A11B, 0, 11);
		addfont(FONT_NORMAL_STR, Text::Font::A12M, 0, 12);
		addfont(FONT_BOLD_STR, Text::Font::A12B, 0, 12);
		addfont(FONT_NORMAL_STR, Text::Font::A13M, 0, 13);
		addfont(FONT_BOLD_STR, Text::Font::A13B, 0, 13);
		addfont(FONT_BOLD_STR, Text::Font::A15B, 0, 15);
		addfont(FONT_NORMAL_STR, Text::Font::A18M, 0, 18);

		fontymax += fontborder.y();

		leftovers = QuadTree<size_t, Leftover>(
			[](const Leftover& first, const Leftover& second)
			{
				bool wcomp = first.width() >= second.width();
				bool hcomp = first.height() >= second.height();

				if (wcomp && hcomp)
					return QuadTree<size_t, Leftover>::Direction::RIGHT;
				else if (wcomp)
					return QuadTree<size_t, Leftover>::Direction::DOWN;
				else if (hcomp)
					return QuadTree<size_t, Leftover>::Direction::UP;
				else
					return QuadTree<size_t, Leftover>::Direction::LEFT;
			}
		);

		return Error::Code::NONE;
	}

	bool GraphicsGL::addfont(const char* name, Text::Font id, FT_UInt pixelw, FT_UInt pixelh)
	{
		FT_Face face;

		if (FT_New_Face(ftlibrary, name, 0, &face))
			return false;

		if (FT_Set_Pixel_Sizes(face, pixelw, pixelh))
			return false;

		FT_GlyphSlot g = face->glyph;

		GLshort width = 0;
		GLshort height = 0;

		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			width += w;

			if (h > height)
				height = h;
		}

		if (fontborder.x() + width > ATLASW)
		{
			fontborder.set_x(0);
			fontborder.set_y(fontymax);
			fontymax = 0;
		}

		GLshort x = fontborder.x();
		GLshort y = fontborder.y();

		fontborder.shift_x(width);

		if (height > fontymax)
			fontymax = height;

		fonts[id] = Font(width, height);

		GLshort ox = x;
		GLshort oy = y;

		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			GLshort ax = static_cast<GLshort>(g->advance.x >> 6);
			GLshort ay = static_cast<GLshort>(g->advance.y >> 6);
			GLshort l = static_cast<GLshort>(g->bitmap_left);
			GLshort t = static_cast<GLshort>(g->bitmap_top);
			GLshort w = static_cast<GLshort>(g->bitmap.width);
			GLshort h = static_cast<GLshort>(g->bitmap.rows);

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, w, h, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			Offset offset = Offset(ox, oy, w, h);
			fonts[id].chars[c] = { ax, ay, w, h, l, t, offset };

			ox += w;
		}

		return true;
	}

	void GraphicsGL::reinit()
	{
		int32_t new_width = Constants::Constants::get().get_viewwidth();
		int32_t new_height = Constants::Constants::get().get_viewheight();

		if (VWIDTH != new_width || VHEIGHT != new_height)
		{
			VWIDTH = new_width;
			VHEIGHT = new_height;
			SCREEN = Rectangle<int16_t>(0, VWIDTH, 0, VHEIGHT);
		}

		glUseProgram(program);

		glUniform1i(uniform_fontregion, fontymax);
		glUniform2f(uniform_atlassize, ATLASW, ATLASH);
		glUniform2f(uniform_screensize, VWIDTH, VHEIGHT);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_SHORT, GL_FALSE, sizeof(Quad::Vertex), 0);
		glVertexAttribPointer(attribute_color, 4, GL_FLOAT, GL_FALSE, sizeof(Quad::Vertex), (const void*)8);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindTexture(GL_TEXTURE_2D, atlas);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		clearinternal();
	}

	void GraphicsGL::clearinternal()
	{
		border = Point<GLshort>(0, fontymax);
		yrange = Range<GLshort>();

		offsets.clear();
		leftovers.clear();
		rlid = 1;
		wasted = 0;
	}

	void GraphicsGL::clear()
	{
		size_t used = ATLASW * border.y() + border.x() * yrange.second();
		double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);

		if (usedpercent > 80.0)
			clearinternal();
	}

	void GraphicsGL::addbitmap(const nl::bitmap& bmp)
	{
		getoffset(bmp);
	}

	const GraphicsGL::Offset& GraphicsGL::getoffset(const nl::bitmap& bmp)
	{
		size_t id = bmp.id();
		auto offiter = offsets.find(id);

		if (offiter != offsets.end())
			return offiter->second;

		GLshort x = 0;
		GLshort y = 0;
		GLshort w = bmp.width();
		GLshort h = bmp.height();

		if (w <= 0 || h <= 0)
			return nulloffset;

		auto value = Leftover(x, y, w, h);

		size_t lid = leftovers.findnode(
			value,
			[](const Leftover& val, const Leftover& leaf)
			{
				return val.width() <= leaf.width() && val.height() <= leaf.height();
			}
		);

		if (lid > 0)
		{
			const Leftover& leftover = leftovers[lid];

			x = leftover.l;
			y = leftover.t;

			GLshort wdelta = leftover.width() - w;
			GLshort hdelta = leftover.height() - h;

			leftovers.erase(lid);

			wasted -= w * h;

			if (wdelta >= MINLOSIZE && hdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + w, y + h, wdelta, hdelta));
				rlid++;

				if (w >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + h, w, hdelta));
					rlid++;
				}

				if (h >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x + w, y, wdelta, h));
					rlid++;
				}
			}
			else if (wdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x + w, y, wdelta, h + hdelta));
				rlid++;
			}
			else if (hdelta >= MINLOSIZE)
			{
				leftovers.add(rlid, Leftover(x, y + h, w + wdelta, hdelta));
				rlid++;
			}
		}
		else
		{
			if (border.x() + w > ATLASW)
			{
				border.set_x(0);
				border.shift_y(yrange.second());

				if (border.y() + h > ATLASH)
					clearinternal();
				else
					yrange = Range<GLshort>();
			}

			x = border.x();
			y = border.y();

			border.shift_x(w);

			if (h > yrange.second())
			{
				if (x >= MINLOSIZE && h - yrange.second() >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(0, yrange.first(), x, h - yrange.second()));
					rlid++;
				}

				wasted += x * (h - yrange.second());

				yrange = Range<int16_t>(y + h, h);
			}
			else if (h < yrange.first() - y)
			{
				if (w >= MINLOSIZE && yrange.first() - y - h >= MINLOSIZE)
				{
					leftovers.add(rlid, Leftover(x, y + h, w, yrange.first() - y - h));
					rlid++;
				}

				wasted += w * (yrange.first() - y - h);
			}
		}

		//size_t used = ATLASW * border.y() + border.x() * yrange.second();
		//double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
		//double wastedpercent = static_cast<double>(wasted) / used;
		//Console::get().print("Used: " + std::to_string(usedpercent) + ", wasted: " + std::to_string(wastedpercent));

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, bmp.data());

		return offsets.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(id),
			std::forward_as_tuple(x, y, w, h)
		).first->second;
	}

	void GraphicsGL::draw(const nl::bitmap& bmp, const Rectangle<int16_t>& rect, const Color& color, float angle)
	{
		if (locked)
			return;

		if (color.invisible())
			return;

		if (!rect.overlaps(SCREEN))
			return;

		quads.emplace_back(rect.l(), rect.r(), rect.t(), rect.b(), getoffset(bmp), color, angle);
	}

	Text::Layout GraphicsGL::createlayout(const std::string& text, Text::Font id, Text::Alignment alignment, int16_t maxwidth, bool formatted, int16_t line_adj)
	{
		size_t length = text.length();

		if (length == 0)
			return Text::Layout();

		LayoutBuilder builder(fonts[id], alignment, maxwidth, formatted, line_adj);

		const char* p_text = text.c_str();

		size_t first = 0;
		size_t offset = 0;

		while (offset < length)
		{
			size_t last = text.find_first_of(" \\#", offset + 1);

			if (last == std::string::npos)
				last = length;

			first = builder.add(p_text, first, offset, last);
			offset = last;
		}

		return builder.finish(first, offset);
	}

	GraphicsGL::LayoutBuilder::LayoutBuilder(const Font& f, Text::Alignment a, int16_t mw, bool fm, int16_t la) : font(f), alignment(a), maxwidth(mw), formatted(fm), line_adj(la)
	{
		fontid = Text::Font::NUM_FONTS;
		color = Color::Name::NUM_COLORS;
		ax = 0;
		ay = font.linespace();
		width = 0;
		endy = 0;

		if (maxwidth == 0)
			maxwidth = 800;
	}

	size_t GraphicsGL::LayoutBuilder::add(const char* text, size_t prev, size_t first, size_t last)
	{
		if (first == last)
			return prev;

		Text::Font last_font = fontid;
		Color::Name last_color = color;
		size_t skip = 0;
		bool linebreak = false;

		if (formatted)
		{
			switch (text[first])
			{
			case '\\':
				if (first + 1 < last)
				{
					switch (text[first + 1])
					{
					case 'n':
						linebreak = true;
						break;
					case 'r':
						linebreak = ax > 0;
						break;
					}

					skip++;
				}

				skip++;
				break;
			case '#':
				if (first + 1 < last)
				{
					switch (text[first + 1])
					{
					case 'k':
						color = Color::Name::DARKGREY;
						break;
					case 'b':
						color = Color::Name::BLUE;
						break;
					case 'r':
						color = Color::Name::RED;
						break;
					case 'c':
						color = Color::Name::ORANGE;
						break;
					}

					skip++;
				}

				skip++;
				break;
			}
		}

		int16_t wordwidth = 0;

		if (!linebreak)
		{
			for (size_t i = first; i < last; i++)
			{
				char c = text[i];
				wordwidth += font.chars[c].ax;

				if (wordwidth > maxwidth)
				{
					if (last - first == 1)
					{
						return last;
					}
					else
					{
						prev = add(text, prev, first, i);
						return add(text, prev, i, last);
					}
				}
			}
		}

		bool newword = skip > 0;
		bool newline = linebreak || ax + wordwidth > maxwidth;

		if (newword || newline)
			add_word(prev, first, last_font, last_color);

		if (newline)
		{
			add_line();

			endy = ay;
			ax = 0;
			ay += font.linespace();

			if (lines.size() > 0)
				ay -= line_adj;
		}

		for (size_t pos = first; pos < last; pos++)
		{
			char c = text[pos];
			const Font::Char& ch = font.chars[c];

			advances.push_back(ax);

			if (pos < first + skip || newline && c == ' ')
				continue;

			ax += ch.ax;

			if (width < ax)
				width = ax;
		}

		if (newword || newline)
			return first + skip;
		else
			return prev;
	}

	Text::Layout GraphicsGL::LayoutBuilder::finish(size_t first, size_t last)
	{
		add_word(first, last, fontid, color);
		add_line();

		advances.push_back(ax);

		return Text::Layout(lines, advances, width, ay, ax, endy);
	}

	void GraphicsGL::LayoutBuilder::add_word(size_t word_first, size_t word_last, Text::Font word_font, Color::Name word_color)
	{
		words.push_back({ word_first, word_last, word_font, word_color });
	}

	void GraphicsGL::LayoutBuilder::add_line()
	{
		int16_t line_x = 0;
		int16_t line_y = ay;

		switch (alignment)
		{
		case Text::Alignment::CENTER:
			line_x -= ax / 2;
			break;
		case Text::Alignment::RIGHT:
			line_x -= ax;
			break;
		}

		lines.push_back({ words, { line_x, line_y } });
		words.clear();
	}

	void GraphicsGL::drawtext(const DrawArgument& args, const std::string& text, const Text::Layout& layout, Text::Font id, Color::Name colorid, Text::Background background)
	{
		if (locked)
			return;

		const Color& color = args.get_color();

		if (text.empty() || color.invisible())
			return;

		const Font& font = fonts[id];

		GLshort x = args.getpos().x();
		GLshort y = args.getpos().y();
		GLshort w = layout.width();
		GLshort h = layout.height();

		switch (background)
		{
		case Text::Background::NAMETAG:
			// If ever changing code in here confirm placements with map 10000
			for (const Text::Layout::Line& line : layout)
			{
				GLshort left = x + line.position.x() - 1;
				GLshort right = left + w + 3;
				GLshort top = y + line.position.y() - font.linespace() + 6;
				GLshort bottom = top + h - 2;
				Color ntcolor = Color(0.0f, 0.0f, 0.0f, 0.6f);

				quads.emplace_back(left, right, top, bottom, nulloffset, ntcolor, 0.0f);
				quads.emplace_back(left - 1, left, top + 1, bottom - 1, nulloffset, ntcolor, 0.0f);
				quads.emplace_back(right, right + 1, top + 1, bottom - 1, nulloffset, ntcolor, 0.0f);
			}

			break;
		}

		for (const Text::Layout::Line& line : layout)
		{
			Point<int16_t> position = line.position;

			for (const Text::Layout::Word& word : line.words)
			{
				GLshort ax = position.x() + layout.advance(word.first);
				GLshort ay = position.y();

				const GLfloat* wordcolor;

				if (word.color < Color::Name::NUM_COLORS)
					wordcolor = Color::colors[word.color];
				else
					wordcolor = Color::colors[colorid];

				Color abscolor = color * Color(wordcolor[0], wordcolor[1], wordcolor[2], 1.0f);

				for (size_t pos = word.first; pos < word.last; ++pos)
				{
					const char c = text[pos];
					const Font::Char& ch = font.chars[c];

					GLshort chx = x + ax + ch.bl;
					GLshort chy = y + ay - ch.bt;
					GLshort chw = ch.bw;
					GLshort chh = ch.bh;

					if (ax == 0 && c == ' ')
						continue;

					ax += ch.ax;

					if (chw <= 0 || chh <= 0)
						continue;

					quads.emplace_back(chx, chx + chw, chy, chy + chh, ch.offset, abscolor, 0.0f);
				}
			}
		}
	}

	void GraphicsGL::drawrectangle(int16_t x, int16_t y, int16_t w, int16_t h, float r, float g, float b, float a)
	{
		if (locked)
			return;

		quads.emplace_back(x, x + w, y, y + h, nulloffset, Color(r, g, b, a), 0.0f);
	}

	void GraphicsGL::drawscreenfill(float r, float g, float b, float a)
	{
		drawrectangle(0, 0, VWIDTH, VHEIGHT, r, g, b, a);
	}

	void GraphicsGL::lock()
	{
		locked = true;
	}

	void GraphicsGL::unlock()
	{
		locked = false;
	}

	void GraphicsGL::flush(float opacity)
	{
		bool coverscene = opacity != 1.0f;

		if (coverscene)
		{
			float complement = 1.0f - opacity;
			Color color = Color(0.0f, 0.0f, 0.0f, complement);

			quads.emplace_back(SCREEN.l(), SCREEN.r(), SCREEN.t(), SCREEN.b(), nulloffset, color, 0.0f);
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		GLsizei csize = static_cast<GLsizei>(quads.size() * sizeof(Quad));
		GLsizei fsize = static_cast<GLsizei>(quads.size() * Quad::LENGTH);

		glEnableVertexAttribArray(attribute_coord);
		glEnableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, csize, quads.data(), GL_STREAM_DRAW);

		glDrawArrays(GL_QUADS, 0, fsize);

		glDisableVertexAttribArray(attribute_coord);
		glDisableVertexAttribArray(attribute_color);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (coverscene)
			quads.pop_back();
	}

	void GraphicsGL::clearscene()
	{
		if (!locked)
			quads.clear();
	}
}