//////////////////////////////////////////////////////////////////////////////
// NoLifeNx - Part of the NoLifeStory project                               //
// Copyright © 2013 Peter Atashian                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "nxfwd.hpp"

namespace nl
{
	namespace nx
	{
		// Pre-defined nodes to access standard MapleStory style data
		// Make sure you called load_all first
		extern node base, character, effect, etc, item, map, mapPretty, mapLatest, map001, mob, morph, npc, quest, reactor, skill, sound, string, tamingmob, ui;

		// Loads the pre-defined nodes from a standard setup of nx files for MapleStory
		// Only call this function once
		void load_all();
	}
}