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

#include "../UIDragElement.h"

#include "../../Character/CharStats.h"
#include "../../Template/BoolPair.h"

namespace ms
{
	class UIStatsInfo : public UIDragElement<PosSTATS>
	{
	public:
		static constexpr Type TYPE = UIElement::Type::STATSINFO;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = true;

		UIStatsInfo(const CharStats &stats);

		void draw(float alpha) const override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		bool is_in_range(Point<int16_t> cursorpos) const override;

		UIElement::Type get_type() const override;

		void update_all_stats();
		void update_stat(MapleStat::Id stat);

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum StatLabel
		{
			// Normal
			NAME,
			JOB,
			GUILD,
			FAME,
			DAMAGE,
			HP,
			MP,
			AP,
			STR,
			DEX,
			INT,
			LUK,
			NUM_NORMAL,
			// Detailed
			DAMAGE_DETAILED,
			DAMAGE_BONUS,
			BOSS_DAMAGE,
			FINAL_DAMAGE,
			IGNORE_DEFENSE,
			CRITICAL_RATE,
			CRITICAL_DAMAGE,
			STATUS_RESISTANCE,
			KNOCKBACK_RESISTANCE,
			DEFENSE,
			SPEED,
			JUMP,
			HONOR,
			// Total
					NUM_LABELS
		};

		void update_ap();

		void update_simple(StatLabel label, MapleStat::Id stat);

		void update_basevstotal(StatLabel label, MapleStat::Id bstat, EquipStat::Id tstat);

		void update_buffed(StatLabel label, EquipStat::Id stat);

		void send_apup(MapleStat::Id stat) const;

		void set_detail(bool enabled);

		enum Buttons
		{
			BT_CLOSE,
			BT_HP,
			BT_MP,
			BT_STR,
			BT_DEX,
			BT_INT,
			BT_LUK,
			BT_AUTO,
			BT_HYPERSTATOPEN,
			BT_HYPERSTATCLOSE,
			BT_DETAILOPEN,
			BT_DETAILCLOSE,
			BT_ABILITY,
			BT_DETAIL_DETAILCLOSE
		};

		const CharStats &stats;

		enum Ability
		{
			RARE,
			EPIC,
			UNIQUE,
			LEGENDARY,
			NONE,
			NUM_ABILITIES
		};

		std::array<Texture, Ability::NUM_ABILITIES> abilities;
		BoolPair<Texture> inner_ability;

		std::vector<Texture> textures_detail;
		bool showdetail;

		bool hasap;

		Text statlabels[StatLabel::NUM_LABELS];
		Point<int16_t> statoffsets[StatLabel::NUM_LABELS];
	};
}