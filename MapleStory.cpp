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
#include "Gameplay/Stage.h"
#include "IO/UI.h"
#include "IO/Window.h"
#include "Net/Session.h"
#include "Util/HardwareInfo.h"
#include "Util/ScreenResolution.h"

#ifdef USE_NX

#include "Util/NxFiles.h"

#else
#include "Util/WzFiles.h"
#endif

namespace ms
{
	Error init()
	{
		if (Error error = Session::get().init())
			return error;

#ifdef USE_NX
		if (Error error = NxFiles::init())
			return error;
#else
		if (Error error = WzFiles::init())
			return error;

#endif

		if (Error error = Window::get().init())
			return error;

		if (Error error = Music::init())
			return error;
		if (Error error = Sound::init())
			return error;

		Char::init();
		DamageNumber::init();
		MapPortals::init();
		Stage::get().init();
		UI::get().init();

		return Error::NONE;
	}

	void update()
	{
		Window::get().check_events();
		Window::get().update();
		Stage::get().update();
		UI::get().update();
		Session::get().read();
#if defined(__linux__) || defined(__APPLE__)
		Music::update_context();
#endif
	}

	void draw(float alpha)
	{
		Window::get().begin();
		Stage::get().draw(alpha);
		UI::get().draw(alpha);
		Window::get().end();
	}

	bool running()
	{
		return Session::get().is_connected()
			   && UI::get().not_quitted()
			   && Window::get().not_closed();
	}

	void loop()
	{
		Timer::get().start();

		int64_t timestep = Constants::TIMESTEP * 1000;
		int64_t accumulator = timestep;

		int64_t period = 0;
		int32_t samples = 0;

		bool show_fps = Configuration::get().get_show_fps();

		while (running())
		{
			int64_t elapsed = Timer::get().stop();

			// Update game with constant timestep as many times as possible.
			for (accumulator += elapsed; accumulator >= timestep; accumulator -= timestep)
				update();

			// Draw the game. Interpolate to account for remaining time.
			float alpha = static_cast<float>(accumulator) / timestep;
			draw(alpha);

			if (show_fps)
			{
				if (samples < 100)
				{
					period += elapsed;
					samples++;
				}
				else if (period)
				{
					int64_t fps = (samples * 1000000) / period;

					std::cout << "FPS: " << fps << std::endl;

					period = 0;
					samples = 0;
				}
			}
		}

		Sound::close();
	}

	void start()
	{
		// Initialize and check for errors
		if (Error error = init())
		{
			const char* message = error.get_message();
			const char* args = error.get_args();
			bool can_retry = error.can_retry();

			std::cout << "Error: " << message << std::endl;

			if (args && args[0])
				std::cout << "Message: " << args << std::endl;

			if (can_retry)
				std::cout << "Enter 'retry' to try again." << std::endl;

			std::string command;
			std::cin >> command;

			if (can_retry && command == "retry")
				start();
		}
		else
		{
			loop();
		}
	}
}

int main()
{
	ms::HardwareInfo();
	ms::ScreenResolution();
	ms::start();

	return 0;
}