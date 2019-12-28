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
#include "Stage.h"

#include "../Audio/Audio.h"
#include "../Character/SkillId.h"
#include "../IO/Messages.h"
#include "../IO/UI.h"
#include "../Util/Misc.h"

#include "../IO/UITypes/UIStatusbar.h"
#include "../Net/Packets/AttackAndSkillPackets.h"
#include "../Net/Packets/GameplayPackets.h"

#include <nlnx/nx.hpp>

namespace ms
{
	Stage::Stage() : combat(player, chars, mobs, reactors)
	{
		state = State::INACTIVE;
	}

	void Stage::init()
	{
		drops.init();
	}

	void Stage::load(int32_t mapid, int8_t portalid)
	{
		switch (state)
		{
		case State::INACTIVE:
			load_map(mapid);
			respawn(portalid);
			break;
		case State::TRANSITION:
			respawn(portalid);
			break;
		}

		state = State::ACTIVE;
	}

	void Stage::loadplayer(const CharEntry& entry)
	{
		player = entry;
		playable = player;
	}

	void Stage::clear()
	{
		state = State::INACTIVE;

		chars.clear();
		npcs.clear();
		mobs.clear();
		drops.clear();
		reactors.clear();

		PlayerMapTransferPacket().dispatch();
	}

	void Stage::load_map(int32_t mapid)
	{
		Stage::mapid = mapid;
		std::string strid = string_format::extend_id(mapid, 9);
		std::string prefix = std::to_string(mapid / 100000000);
		nl::node src = nl::nx::map["Map"]["Map" + prefix][strid + ".img"];

		tilesobjs = MapTilesObjs(src);
		backgrounds = MapBackgrounds(src["back"]);
		physics = Physics(src["foothold"]);
		mapinfo = MapInfo(src, physics.get_fht().get_walls(), physics.get_fht().get_borders());
		portals = MapPortals(src["portal"], mapid);
	}

	void Stage::respawn(int8_t portalid)
	{
		Music(mapinfo.get_bgm()).play();

		Point<int16_t> spawnpoint = portals.get_portal_by_id(portalid);
		Point<int16_t> startpos = physics.get_y_below(spawnpoint);
		player.respawn(startpos, mapinfo.is_underwater());
		camera.set_position(startpos);
		camera.set_view(mapinfo.get_walls(), mapinfo.get_borders());
	}

	void Stage::draw(float alpha) const
	{
		if (state != State::ACTIVE)
			return;

		Point<int16_t> viewpos = camera.position(alpha);
		Point<double> viewrpos = camera.realposition(alpha);
		double viewx = viewrpos.x();
		double viewy = viewrpos.y();

		backgrounds.drawbackgrounds(viewx, viewy, alpha);

		for (auto id : Layer::IDs)
		{
			tilesobjs.draw(id, viewpos, alpha);
			reactors.draw(id, viewx, viewy, alpha);
			npcs.draw(id, viewx, viewy, alpha);
			mobs.draw(id, viewx, viewy, alpha);
			chars.draw(id, viewx, viewy, alpha);
			player.draw(id, viewx, viewy, alpha);
			drops.draw(id, viewx, viewy, alpha);
		}

		combat.draw(viewx, viewy, alpha);
		portals.draw(viewpos, alpha);
		backgrounds.drawforegrounds(viewx, viewy, alpha);
		effect.draw();
	}

	void Stage::update()
	{
		if (state != State::ACTIVE)
			return;

		combat.update();
		backgrounds.update();
		effect.update();
		tilesobjs.update();

		reactors.update(physics);
		npcs.update(physics);
		mobs.update(physics);
		chars.update(physics);
		drops.update(physics);
		player.update(physics);

		portals.update(player.get_position());
		camera.update(player.get_position());

		if (player.is_invincible())
			return;

		if (int32_t oid_id = mobs.find_colliding(player.get_phobj()))
		{
			if (MobAttack attack = mobs.create_attack(oid_id))
			{
				MobAttackResult result = player.damage(attack);
				TakeDamagePacket(result, TakeDamagePacket::From::TOUCH).dispatch();
			}
		}
	}

	void Stage::show_character_effect(int32_t cid, CharEffect::Id effect)
	{
		if (auto character = get_character(cid))
			character->show_effect_id(effect);
	}

	void Stage::check_portals()
	{
		if (player.is_attacking())
			return;

		Point<int16_t> playerpos = player.get_position();
		Portal::WarpInfo warpinfo = portals.find_warp_at(playerpos);

		if (warpinfo.intramap)
		{
			Point<int16_t> spawnpoint = portals.get_portal_by_name(warpinfo.toname);
			Point<int16_t> startpos = physics.get_y_below(spawnpoint);

			player.respawn(startpos, mapinfo.is_underwater());
		}
		else if (warpinfo.valid)
		{
			ChangeMapPacket(false, -1, warpinfo.name, false).dispatch();

			CharStats& stats = Stage::get().get_player().get_stats();

			stats.set_mapid(warpinfo.mapid);

			Sound(Sound::Name::PORTAL).play();
		}
	}

	void Stage::check_seats()
	{
		if (player.is_sitting() || player.is_attacking())
			return;

		Optional<const Seat> seat = mapinfo.findseat(player.get_position());
		player.set_seat(seat);
	}

	void Stage::check_ladders(bool up)
	{
		if (player.is_climbing() || player.is_attacking())
			return;

		Optional<const Ladder> ladder = mapinfo.findladder(player.get_position(), up);
		player.set_ladder(ladder);
	}

	void Stage::check_drops()
	{
		Point<int16_t> playerpos = player.get_position();
		MapDrops::Loot loot = drops.find_loot_at(playerpos);

		if (loot.first)
			PickupItemPacket(loot.first, loot.second).dispatch();
	}

	void Stage::send_key(KeyType::Id type, int32_t action, bool down)
	{
		if (state != State::ACTIVE || !playable)
			return;

		switch (type)
		{
		case KeyType::Id::ACTION:
			if (down)
			{
				switch (action)
				{
				case KeyAction::Id::UP:
					check_ladders(true);
					check_portals();
					break;
				case KeyAction::Id::DOWN:
					check_ladders(false);
					break;
				case KeyAction::Id::SIT:
					check_seats();
					break;
				case KeyAction::Id::ATTACK:
					combat.use_move(0);
					break;
				case KeyAction::Id::PICKUP:
					check_drops();
					break;
				}
			}

			playable->send_action(KeyAction::actionbyid(action), down);
			break;
		case KeyType::Id::SKILL:
			combat.use_move(action);
			break;
		case KeyType::Id::ITEM:
			player.use_item(action);
			break;
		case KeyType::Id::FACE:
			player.set_expression(action);
			break;
		}
	}

	Cursor::State Stage::send_cursor(bool pressed, Point<int16_t> position)
	{
		auto statusbar = UI::get().get_element<UIStatusbar>();

		if (statusbar && statusbar->is_menu_active())
		{
			if (pressed)
			{
				statusbar->remove_menus();

				return npcs.send_cursor(pressed, position, camera.position());
			}

			return statusbar->send_cursor(pressed, position);
		}

		return npcs.send_cursor(pressed, position, camera.position());
	}

	bool Stage::is_player(int32_t cid) const
	{
		return cid == player.get_oid();
	}

	MapNpcs& Stage::get_npcs()
	{
		return npcs;
	}

	MapChars& Stage::get_chars()
	{
		return chars;
	}

	MapMobs& Stage::get_mobs()
	{
		return mobs;
	}

	MapReactors& Stage::get_reactors()
	{
		return reactors;
	}

	MapDrops& Stage::get_drops()
	{
		return drops;
	}

	Player& Stage::get_player()
	{
		return player;
	}

	Combat& Stage::get_combat()
	{
		return combat;
	}

	Optional<Char> Stage::get_character(int32_t cid)
	{
		if (is_player(cid))
			return player;
		else
			return chars.get_char(cid);
	}

	int Stage::get_mapid()
	{
		return mapid;
	}

	void Stage::add_effect(std::string path)
	{
		effect = MapEffect(path);
	}
}