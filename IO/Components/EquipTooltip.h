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

#include "Charset.h"
#include "Tooltip.h"

#include "../../Character/MapleStat.h"
#include "../../Graphics/Text.h"
#include "../../Template/BoolPair.h"

#include "../../Character/Inventory/Equip.h"

namespace ms
{
	class EquipTooltip : public Tooltip
	{
	public:
		EquipTooltip();

		void set_equip(Tooltip::Parent parent, int16_t invpos);

		void draw(Point<int16_t> position) const override;

		void draw_preview(Point<int16_t> position) const;

	private:
		int16_t invpos;
		int16_t invpos_preview;
		int16_t height;
		int16_t height_preview;
		bool hasdesc;
		bool hasdesc_preview;
		bool hasslots;
		bool hasslots_preview;
		bool is_weapon;
		bool is_weapon_preview;
		EnumMap<MapleStat::Id, std::string> reqstatstrings;
		EnumMap<MapleStat::Id, std::string> reqstatstrings_preview;
		Texture itemicon;
		Texture itemicon_preview;

		Text name;
		Text name_preview;
		Text desc;
		Text desc_preview;
		Text potflag;
		Text potflag_preview;
		Text category;
		Text category_preview;
		Text wepspeed;
		Text wepspeed_preview;
		Text slots;
		Text slots_preview;
		Text hammers;
		Text hammers_preview;
		Text atkinc;
		Text atkinc_preview;
		EnumMap<EquipStat::Id, Text> statlabels;
		EnumMap<EquipStat::Id, Text> statlabels_preview;

		Texture top;
		Texture mid;
		Texture line;
		Texture bot;
		Texture base;

		EnumMap<Equip::Potential, Texture> potential;
		Equip::Potential prank;
		Equip::Potential prank_preview;

		Texture cover;
		Texture itemcover;
		BoolPair<Texture> type;

		std::vector<MapleStat::Id> requirements;
		EnumMap<MapleStat::Id, BoolPair<Texture>> reqstattextures;
		EnumMap<MapleStat::Id, bool> canequip;
		EnumMap<MapleStat::Id, bool> canequip_preview;
		EnumMap<MapleStat::Id, Point<int16_t>> reqstatpositions;
		BoolPair<Charset> reqset;
		BoolPair<Charset> lvset;
		BoolPair<Charset> atkincset;

		Texture jobsback;
		BoolPair<std::map<uint8_t, Texture>> jobs;
		std::vector<uint8_t> okjobs;
		std::vector<uint8_t> okjobs_preview;
	};
}