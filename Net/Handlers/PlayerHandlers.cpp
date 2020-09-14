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
#include "PlayerHandlers.h"

#include "Helpers/LoginParser.h"

#include "../../Gameplay/Stage.h"
#include "../../IO/UI.h"

#include "../../IO/UITypes/UIBuffList.h"
#include "../../IO/UITypes/UICashShop.h"
#include "../../IO/UITypes/UISkillBook.h"
#include "../../IO/UITypes/UIStatsInfo.h"
#include "../../IO/Window.h"

namespace ms
{
	void ChangeChannelHandler::handle(InPacket& recv) const
	{
		LoginParser::parse_login(recv);

		auto cashshop = UI::get().get_element<UICashShop>();

		if (cashshop) {
			cashshop->exit_cashshop();
		}
		 else {
		 Player &player = Stage::get().get_player();
		 int32_t mapid = player.get_stats().get_mapid();
		 uint8_t portalid = player.get_stats().get_portal();

		 float fadestep = 0.025f;

		 Window::get().fadeout(fadestep, [mapid, portalid]() { GraphicsGL::get().clear(); Stage::get().load(mapid, portalid); UI::get().enable(); GraphicsGL::get().unlock(); });

		 GraphicsGL::get().lock();
		 Stage::get().clear_channel_objects();
		 }
	}

	void ChangeStatsHandler::handle(InPacket& recv) const
	{
		recv.read_bool(); // 'itemreaction'
		int32_t updatemask = recv.read_int();

		bool recalculate = false;

		for (auto iter : MapleStat::codes)
			if (updatemask & iter.second)
				recalculate |= handle_stat(iter.first, recv);

		if (recalculate)
			Stage::get().get_player().recalc_stats(false);

		UI::get().enable();
	}

	bool ChangeStatsHandler::handle_stat(MapleStat::Id stat, InPacket& recv) const
	{
		Player& player = Stage::get().get_player();

		bool recalculate = false;

		switch (stat)
		{
		case MapleStat::Id::SKIN:
			player.change_look(stat, recv.read_byte());
			break;
		case MapleStat::Id::FACE:
		case MapleStat::Id::HAIR:
			player.change_look(stat, recv.read_int());
			break;
		case MapleStat::Id::LEVEL:
			player.change_level(recv.read_short());
			break;
		case MapleStat::Id::JOB:
			player.change_job(recv.read_short());
			break;
		case MapleStat::Id::EXP:
			player.get_stats().set_exp(recv.read_int());
			break;
		case MapleStat::Id::MESO:
			player.get_inventory().set_meso(recv.read_int());
			break;
		case MapleStat::Id::HP:
			player.get_stats().set_hp(recv.read_int());
			break;
		case MapleStat::Id::MP:
			player.get_stats().set_mp(recv.read_int());
			break;
		case MapleStat::Id::MAXHP:
			player.get_stats().set_maxhp(recv.read_int());
			break;
		case MapleStat::Id::MAXMP:
			player.get_stats().set_maxmp(recv.read_int());
			break;
		default:
			player.get_stats().set_stat(stat, recv.read_short());
			recalculate = true;
			break;
		}

		bool update_statsinfo = need_statsinfo_update(stat);

		if (update_statsinfo && !recalculate)
			if (auto statsinfo = UI::get().get_element<UIStatsInfo>())
				statsinfo->update_stat(stat);

		bool update_skillbook = need_skillbook_update(stat);

		if (update_skillbook)
		{
			int16_t value = player.get_stats().get_stat(stat);

			if (auto skillbook = UI::get().get_element<UISkillBook>())
				skillbook->update_stat(stat, value);
		}

		return recalculate;
	}

	bool ChangeStatsHandler::need_statsinfo_update(MapleStat::Id stat) const
	{
		switch (stat)
		{
		case MapleStat::Id::JOB:
		case MapleStat::Id::STR:
		case MapleStat::Id::DEX:
		case MapleStat::Id::INT:
		case MapleStat::Id::LUK:
		case MapleStat::Id::HP:
		case MapleStat::Id::MAXHP:
		case MapleStat::Id::MP:
		case MapleStat::Id::MAXMP:
		case MapleStat::Id::AP:
			return true;
		default:
			return false;
		}
	}

	bool ChangeStatsHandler::need_skillbook_update(MapleStat::Id stat) const
	{
		switch (stat)
		{
		case MapleStat::Id::JOB:
		case MapleStat::Id::SP:
			return true;
		default:
			return false;
		}
	}

	void BuffHandler::handle(InPacket& recv) const
	{
		bool special = false;
		bool ismovementaffectingstat = false;
		uint64_t firstmask = recv.read_long();
		uint64_t secondmask = recv.read_long();

		if (secondmask == Buffstat::BATTLESHIP) {
			handle_buff(recv, Buffstat::BATTLESHIP);
			return;
		}

		for (const auto& iter : Buffstat::first_codes) {
			if (iter.first == Buffstat::MONSTER_RIDING
				|| Buffstat::DASH
				|| Buffstat::DASH2) {
				ismovementaffectingstat = true;
			}
			if (firstmask & iter.second) {
				if (iter.first == Buffstat::SPEED_INFUSION
					|| iter.first == Buffstat::DASH
					|| iter.first == Buffstat::DASH2) {
					recv.read_short();
					special = true;
				}
				handle_buff(recv, iter.first);
			}
		}

		for (const auto& iter : Buffstat::second_codes) {
			if (iter.first == Buffstat::SPEED
				|| Buffstat::JUMP
				|| Buffstat::SEAL
				|| Buffstat::STUN
				|| Buffstat::WEAKEN
				|| Buffstat::MORPH
				|| Buffstat::GHOST_MORPH
				|| Buffstat::CURSE) {
				ismovementaffectingstat = true;
			}
			if (secondmask & iter.second) {
				handle_buff(recv, iter.first);				
			}
		}

		if (special) {
			recv.skip(3);
		}
		else {
			uint8_t defenseatt = recv.read_byte();
			uint8_t defensestate = recv.read_byte();
			uint16_t delay = recv.read_short();
			if (ismovementaffectingstat)
				recv.skip(1);
		}

		Stage::get().get_player().recalc_stats(false);
	}

	void ApplyBuffHandler::handle_buff(InPacket& recv, Buffstat::Id bs) const
	{
		Player& player = Stage::get().get_player();
		int16_t value = recv.read_short();
		int32_t skillid = recv.read_int();
		if (bs == Buffstat::SPEED_INFUSION
			|| bs == Buffstat::DASH
			|| bs == Buffstat::DASH2)
			recv.skip(bs == Buffstat::SPEED_INFUSION ? 10 : 5);
		int32_t duration = recv.read_int();

		player.give_buff({ bs, value, skillid, duration });

		if (auto bufflist = UI::get().get_element<UIBuffList>()) {
			bufflist->add_buff(skillid, duration);
		}		
	}

	void CancelBuffHandler::handle_buff(InPacket&, Buffstat::Id bs) const
	{
		Stage::get().get_player().cancel_buff(bs);
	}

	void RecalculateStatsHandler::handle(InPacket&) const
	{
		Stage::get().get_player().recalc_stats(false);
	}

	void UpdateSkillHandler::handle(InPacket& recv) const
	{
		recv.skip(3);

		int32_t skillid = recv.read_int();
		int32_t level = recv.read_int();
		int32_t masterlevel = recv.read_int();
		int64_t expire = recv.read_long();

		int8_t stat = recv.read_byte(); // SetSecondaryStatChangedPoint

		Stage::get().get_player().change_skill(skillid, level, masterlevel, expire);

		if (auto skillbook = UI::get().get_element<UISkillBook>())
			skillbook->update_skills(skillid);

		UI::get().enable();
	}

	void GivePopularityResultHandler::handle(InPacket& recv) const
	{
		recv.skip(1);
		std::string charname = recv.read_string();
		recv.skip(1);
		uint16_t newfame = recv.read_byte();
		recv.skip(2);

		/// NotifyGivePopResult TODO
	}

	void SkillMacrosHandler::handle(InPacket& recv) const
	{
		uint8_t size = recv.read_byte();

		for (uint8_t i = 0; i < size; i++)
		{
			recv.read_string();	// name
			recv.read_byte();	// 'shout' byte
			recv.read_int();	// skill 1
			recv.read_int();	// skill 2
			recv.read_int();	// skill 3
		}
	}

	void AddCooldownHandler::handle(InPacket& recv) const
	{
		int32_t skill_id = recv.read_int();
		int16_t cooltime = recv.read_short();

		Stage::get().get_player().add_cooldown(skill_id, cooltime);
	}
	
	/// TODO
	void forcedStatSetHandler::handle(InPacket& recv) const
	{
		Player& player = Stage::get().get_player();
		uint32_t mask = recv.read_int();
		if ((mask & 1) != 0) {
			uint16_t STR = recv.read_short();
			player.get_stats().set_stat(MapleStat::STR, STR);
		}
		if ((mask & 2) != 0) {
			uint16_t DEX = recv.read_short();
			player.get_stats().set_stat(MapleStat::STR, DEX);
		}
		if ((mask & 4) != 0) {
			uint16_t INT = recv.read_short();
			player.get_stats().set_stat(MapleStat::STR, INT);
		}
		if ((mask & 8) != 0)  {
			uint16_t LUK = recv.read_short();
			player.get_stats().set_stat(MapleStat::STR, LUK);
		}
		if ((mask & 0x10) != 0)  {
			uint16_t PAD = recv.read_short();
		}
		if ((mask & 0x20) != 0)  {
			uint16_t PDD = recv.read_short();
		}
		if ((mask & 0x40) != 0)  {
			uint16_t MAD = recv.read_short();
		}
		if ((mask & 0x80) != 0)  {
			uint16_t MDD = recv.read_short();
		}
		if ((mask & 0x100) != 0) {
			uint16_t ACC = recv.read_short();
		}
		if ((mask & 0x200) != 0) {
			uint16_t EVA = recv.read_short();
		}
		if ((mask & 0x400) != 0) {
			uint8_t Speed = recv.read_byte();
		}
		if ((mask & 0x800) != 0) {
			uint8_t Jump = recv.read_byte();
		}
		if ((mask & 0x800) != 0) {
			uint8_t SpeedMax = recv.read_byte();
		}
	}

	void SetGenderHandler::handle(InPacket& recv) const
	{
		Player& player = Stage::get().get_player();
		player.get_stats().set_female(recv.read_byte());
	}
}