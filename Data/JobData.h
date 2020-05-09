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

#include "../Graphics/Texture.h"
#include "../Template/Cache.h"

#include <vector>

namespace ms
{
	// Contains information about a job
	class JobData : public Cache<JobData>
	{
	public:
		// Return the ids of the skills for this job.
		const std::vector<std::int32_t> &get_skills() const;

		// Return the name of the skill book.
		const std::string &get_name() const;

		// Return the book cover icon.
		const Texture &get_icon() const;

	private:
		friend Cache<JobData>;

		JobData(int32_t id);

		Texture icon;
		std::vector<std::int32_t> skills;
		std::string name;
	};
}