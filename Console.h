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

#include "MapleStory.h"

#include "Template/Singleton.h"

#include <unordered_set>
#include <string>
#include <iostream>

namespace ms
{
#ifndef PRINT_WARNINGS
	class Console : public Singleton<Console>
	{
	public:
		void print(const char*, const std::string&) {}
		void print(const char*, const std::exception&) {}
		void print(const std::string&) {}
	};
#else

	class Console : public Singleton<Console>
	{
	public:
		void print(const char *func, const std::string &msg)
		{
			static const std::string delim = ", ";
			print(func + delim + msg);
		}

		void print(const char *func, const std::exception &ex)
		{
			print(func, {ex.what()});
		}

		void print(const std::string &str)
		{
			if (!printed.count(str))
			{
				std::cout << str << std::endl;
				printed.insert(str);
			}
		}

	private:
		std::unordered_set<std::string> printed;
	};

#endif
}