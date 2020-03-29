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
#include "Questlog.h"

namespace ms
{
	void Questlog::add_started(int16_t qid, const std::string &qdata)
	{
		started[qid] = qdata;
	}

	void Questlog::add_in_progress(int16_t qid, int16_t qidl, const std::string &qdata)
	{
		in_progress[qid] = make_pair(qidl, qdata);
	}

	void Questlog::add_completed(int16_t qid, int64_t time)
	{
		completed[qid] = time;
	}

	bool Questlog::is_started(int16_t qid)
	{
		return started.count(qid) > 0;
	}

	int16_t Questlog::get_last_started()
	{
		auto qend = started.end();
		qend--;

		return qend->first;
	}
}