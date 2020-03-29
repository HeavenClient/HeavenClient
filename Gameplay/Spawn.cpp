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
#include "Spawn.h"

#include "MapleMap/Npc.h"
#include "MapleMap/Mob.h"
#include "MapleMap/Reactor.h"
#include "MapleMap/ItemDrop.h"
#include "MapleMap/MesoDrop.h"

#include "../Character/OtherChar.h"

namespace ms
{
	NpcSpawn::NpcSpawn(int32_t o, int32_t i, Point<int16_t> p, bool fl, uint16_t f) : oid(o), id(i), position(p),
																					  flip(fl), fh(f)
	{}

	int32_t NpcSpawn::get_oid() const
	{
		return oid;
	}

	std::unique_ptr<MapObject> NpcSpawn::instantiate(const Physics &physics) const
	{
		auto spawnposition = physics.get_y_below(position);
		return std::make_unique<Npc>(id, oid, flip, fh, false, spawnposition);
	}

	MobSpawn::MobSpawn(int32_t o, int32_t i, int8_t m, int8_t st, uint16_t f, bool ns, int8_t t, Point<int16_t> p)
			: oid(o), id(i), mode(m), stance(st), fh(f), newspawn(ns), team(t), position(p)
	{}

	int8_t MobSpawn::get_mode() const
	{
		return mode;
	}

	int32_t MobSpawn::get_oid() const
	{
		return oid;
	}

	std::unique_ptr<MapObject> MobSpawn::instantiate() const
	{
		return std::make_unique<Mob>(oid, id, mode, stance, fh, newspawn, team, position);
	}

	ReactorSpawn::ReactorSpawn(int32_t o, int32_t r, int8_t s, Point<int16_t> p) : oid(o), rid(r), state(s), position(p)
	{}

	int32_t ReactorSpawn::get_oid() const
	{
		return oid;
	}

	std::unique_ptr<MapObject> ReactorSpawn::instantiate(const Physics &physics) const
	{
		auto spawnposition = physics.get_y_below(position);
		return std::make_unique<Reactor>(oid, rid, state, spawnposition);
	}

	DropSpawn::DropSpawn(int32_t o, int32_t i, bool ms, int32_t ow, Point<int16_t> p, Point<int16_t> d, int8_t t,
						 int8_t m, bool pd) : oid(o), id(i), meso(ms), owner(ow), start(p), dest(d), droptype(t),
											  mode(m), playerdrop(pd)
	{}

	bool DropSpawn::is_meso() const
	{
		return meso;
	}

	int32_t DropSpawn::get_itemid() const
	{
		return id;
	}

	int32_t DropSpawn::get_oid() const
	{
		return oid;
	}

	std::unique_ptr<MapObject> DropSpawn::instantiate(const Animation &icon) const
	{
		return std::make_unique<MesoDrop>(oid, owner, start, dest, droptype, mode, playerdrop, icon);
	}

	std::unique_ptr<MapObject> DropSpawn::instantiate(const Texture &icon) const
	{
		return std::make_unique<ItemDrop>(oid, owner, start, dest, droptype, mode, id, playerdrop, icon);
	}

	CharSpawn::CharSpawn(int32_t c, const LookEntry &lk, uint8_t l, int16_t j, const std::string &nm, int8_t st,
						 Point<int16_t> p) : cid(c), look(lk), level(l), job(j), name(nm), stance(st), position(p)
	{}

	int32_t CharSpawn::get_cid() const
	{
		return cid;
	}

	std::unique_ptr<MapObject> CharSpawn::instantiate() const
	{
		return std::make_unique<OtherChar>(cid, look, level, job, name, stance, position);
	}
}