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

#include "Attack.h"

#include "../../Character/Char.h"

namespace ms
{
	class SkillAction
	{
	public:
		virtual ~SkillAction()
		{}

		virtual void apply(Char& target, Attack::Type atype) const = 0;
	};

	class NoAction : public SkillAction
	{
	public:
		void apply(Char&, Attack::Type) const override
		{}
	};

	class RegularAction : public SkillAction
	{
	public:
		void apply(Char& target, Attack::Type atype) const override;
	};

	class SingleAction : public SkillAction
	{
	public:
		SingleAction(nl::node src);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		std::string action;
	};

	class TwoHandedAction : public SkillAction
	{
	public:
		TwoHandedAction(nl::node src);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		BoolPair<std::string> actions;
	};

	class ByLevelAction : public SkillAction
	{
	public:
		ByLevelAction(nl::node src, int32_t skillid);

		void apply(Char& target, Attack::Type atype) const override;

	private:
		std::map<int32_t, std::string> actions;
		int32_t skillid;
	};
}