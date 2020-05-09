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

#include "../Error.h"

#include "../Template/Singleton.h"

#ifdef _WIN32
#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>
#include <GL/gl.h>
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#endif

#include <functional>

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else

#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include<iostream>

namespace ms
{
	class Window : public Singleton<Window>
	{
	public:
		Window();

		~Window();

		Error init();

		Error initwindow();

		bool not_closed() const;

		void update();

		void begin() const;

		void end() const;

		void fadeout(float step, std::function<void()> fadeprocedure);

		void check_events();

		void setclipboard(const std::string &text) const;

		std::string getclipboard() const;

		void toggle_fullscreen();

	private:
		void updateopc();

		GLFWwindow *glwnd;
		GLFWwindow *context;
		bool fullscreen;
		float opacity;
		float opcstep;
		std::function<void()> fadeprocedure;
		int16_t width;
		int16_t height;

		std::string GetCurrentWorkingDir(void);
	};
}