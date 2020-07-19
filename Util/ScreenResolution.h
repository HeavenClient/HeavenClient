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

#include "../Configuration.h"

#ifdef _WIN32
#include <windef.h>
#include <WinUser.h>
#elif defined(__linux__)

#include <X11/Xlib.h>

#else defined(__APPLE__)
#include <CoreGraphics/CGDisplayConfiguration.h>
#endif

namespace ms
{
	class ScreenResolution
	{
	public:
		ScreenResolution()
		{
#ifdef _WIN32
			RECT desktop;

			// Get a handle to the desktop window
			const HWND hDesktop = GetDesktopWindow();

			//Get the size of screen to the variable desktop
			GetWindowRect(hDesktop, &desktop);

			// The top left corner will have coordinates (0, 0) and the bottom right corner will have coordinates (horizontal, vertical)
			Configuration::get().set_max_width(desktop.right);
			Configuration::get().set_max_height(desktop.bottom);
#elif defined(__linux__)
			Display* disp = XOpenDisplay(NULL);
			Screen* scrn = DefaultScreenOfDisplay(disp);
			int height = scrn->height;
			int width = scrn->width;

			Configuration::get().set_max_width(width);
			Configuration::get().set_max_height(height);
#elif defined(__APPLE__)
			auto mainDisplayId = CGMainDisplayID();
			int width = CGDisplayPixelsWide(mainDisplayId);
			int height = CGDisplayPixelsHigh(mainDisplayId);

			Configuration::get().set_max_width(width);
			Configuration::get().set_max_height(height);
#endif
		}
	};
}