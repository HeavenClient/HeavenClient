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
#include "Footholdtree.h"

#include "../Console.h"

namespace ms
{
	Footholdtree::Footholdtree(nl::node src)
	{
		int16_t leftw = 30000;
		int16_t rightw = -30000;
		int16_t botb = -30000;
		int16_t topb = 30000;

		for (auto basef : src)
		{
			uint8_t layer;

			try
			{
				layer = static_cast<uint8_t>(std::stoi(basef.name()));
			}
			catch (const std::exception &ex)
			{
				Console::get().print(__func__, ex);
				continue;
			}

			for (auto midf : basef)
			{
				for (auto lastf : midf)
				{
					uint16_t id;

					try
					{
						id = static_cast<uint16_t>(std::stoi(lastf.name()));
					}
					catch (const std::exception &ex)
					{
						Console::get().print(__func__, ex);
						continue;
					}

					const Foothold &foothold = footholds.emplace(
							std::piecewise_construct,
							std::forward_as_tuple(id),
							std::forward_as_tuple(lastf, id, layer)
					).first->second;

					if (foothold.l() < leftw)
						leftw = foothold.l();

					if (foothold.r() > rightw)
						rightw = foothold.r();

					if (foothold.b() > botb)
						botb = foothold.b();

					if (foothold.t() < topb)
						topb = foothold.t();

					if (foothold.is_wall())
						continue;

					int16_t start = foothold.l();
					int16_t end = foothold.r();

					for (int16_t i = start; i <= end; i++)
						footholdsbyx.emplace(i, id);
				}
			}
		}

		walls = {leftw + 25, rightw - 25};
		borders = {topb - 300, botb + 100};
	}

	Footholdtree::Footholdtree()
	{}

	void Footholdtree::limit_movement(PhysicsObject &phobj) const
	{
		if (phobj.hmobile())
		{
			double crnt_x = phobj.crnt_x();
			double next_x = phobj.next_x();

			bool left = phobj.hspeed < 0.0f;
			double wall = get_wall(phobj.fhid, left, phobj.next_y());
			bool collision = left ? crnt_x >= wall && next_x <= wall : crnt_x <= wall && next_x >= wall;

			if (!collision && phobj.is_flag_set(PhysicsObject::Flag::TURNATEDGES))
			{
				wall = get_edge(phobj.fhid, left);
				collision = left ? crnt_x >= wall && next_x <= wall : crnt_x <= wall && next_x >= wall;
			}

			if (collision)
			{
				phobj.limitx(wall);
				phobj.clear_flag(PhysicsObject::Flag::TURNATEDGES);
			}
		}

		if (phobj.vmobile())
		{
			double crnt_y = phobj.crnt_y();
			double next_y = phobj.next_y();

			auto ground = Range<double>(
					get_fh(phobj.fhid).ground_below(phobj.crnt_x()),
					get_fh(phobj.fhid).ground_below(phobj.next_x())
			);

			bool collision = crnt_y <= ground.first() && next_y >= ground.second();

			if (collision)
			{
				phobj.limity(ground.second());

				limit_movement(phobj);
			} else
			{
				if (next_y < borders.first())
					phobj.limity(borders.first());
				else if (next_y > borders.second())
					phobj.limity(borders.second());
			}
		}
	}

	void Footholdtree::update_fh(PhysicsObject &phobj) const
	{
		if (phobj.type == PhysicsObject::Type::FIXATED && phobj.fhid > 0)
			return;

		const Foothold &curfh = get_fh(phobj.fhid);
		bool checkslope = false;

		double x = phobj.crnt_x();
		double y = phobj.crnt_y();

		if (phobj.onground)
		{
			if (std::floor(x) > curfh.r())
				phobj.fhid = curfh.next();
			else if (std::ceil(x) < curfh.l())
				phobj.fhid = curfh.prev();

			if (phobj.fhid == 0)
				phobj.fhid = get_fhid_below(x, y);
			else
				checkslope = true;
		} else
		{
			phobj.fhid = get_fhid_below(x, y);
		}

		const Foothold &nextfh = get_fh(phobj.fhid);
		phobj.fhslope = nextfh.slope();

		double ground = nextfh.ground_below(x);

		if (phobj.vspeed == 0.0 && checkslope)
		{
			double vdelta = abs(phobj.fhslope);

			if (phobj.fhslope < 0.0)
				vdelta *= (ground - y);
			else if (phobj.fhslope > 0.0)
				vdelta *= (y - ground);

			if (curfh.slope() != 0.0 || nextfh.slope() != 0.0)
			{
				if (phobj.hspeed > 0.0 && vdelta <= phobj.hspeed)
					phobj.y = ground;
				else if (phobj.hspeed < 0.0 && vdelta >= phobj.hspeed)
					phobj.y = ground;
			}
		}

		phobj.onground = phobj.y == ground;

		if (phobj.enablejd || phobj.is_flag_set(PhysicsObject::Flag::CHECKBELOW))
		{
			uint16_t belowid = get_fhid_below(x, nextfh.ground_below(x) + 1.0);

			if (belowid > 0)
			{
				double nextground = get_fh(belowid).ground_below(x);
				phobj.enablejd = (nextground - ground) < 600.0;
				phobj.groundbelow = ground + 1.0;
			} else
			{
				phobj.enablejd = false;
			}

			phobj.clear_flag(PhysicsObject::Flag::CHECKBELOW);
		}

		if (phobj.fhlayer == 0 || phobj.onground)
			phobj.fhlayer = nextfh.layer();

		if (phobj.fhid == 0)
		{
			phobj.fhid = curfh.id();
			phobj.limitx(curfh.x1());
		}
	}

	const Foothold &Footholdtree::get_fh(uint16_t fhid) const
	{
		auto iter = footholds.find(fhid);

		if (iter == footholds.end())
			return nullfh;

		return iter->second;
	}

	double Footholdtree::get_wall(uint16_t curid, bool left, double fy) const
	{
		auto shorty = static_cast<int16_t>(fy);
		Range<int16_t> vertical(shorty - 50, shorty - 1);
		const Foothold &cur = get_fh(curid);

		if (left)
		{
			const Foothold &prev = get_fh(cur.prev());

			if (prev.is_blocking(vertical))
				return cur.l();

			const Foothold &prev_prev = get_fh(prev.prev());

			if (prev_prev.is_blocking(vertical))
				return prev.l();

			return walls.first();
		} else
		{
			const Foothold &next = get_fh(cur.next());

			if (next.is_blocking(vertical))
				return cur.r();

			const Foothold &next_next = get_fh(next.next());

			if (next_next.is_blocking(vertical))
				return next.r();

			return walls.second();
		}
	}

	double Footholdtree::get_edge(uint16_t curid, bool left) const
	{
		const Foothold &fh = get_fh(curid);

		if (left)
		{
			uint16_t previd = fh.prev();

			if (!previd)
				return fh.l();

			const Foothold &prev = get_fh(previd);
			uint16_t prev_previd = prev.prev();

			if (!prev_previd)
				return prev.l();

			return walls.first();
		} else
		{
			uint16_t nextid = fh.next();

			if (!nextid)
				return fh.r();

			const Foothold &next = get_fh(nextid);
			uint16_t next_nextid = next.next();

			if (!next_nextid)
				return next.r();

			return walls.second();
		}
	}

	uint16_t Footholdtree::get_fhid_below(double fx, double fy) const
	{
		uint16_t ret = 0;
		double comp = borders.second();

		int16_t x = static_cast<int16_t>(fx);
		auto range = footholdsbyx.equal_range(x);

		for (auto iter = range.first; iter != range.second; ++iter)
		{
			const Foothold &fh = footholds.at(iter->second);
			double ycomp = fh.ground_below(fx);

			if (comp >= ycomp && ycomp >= fy)
			{
				comp = ycomp;
				ret = fh.id();
			}
		}

		return ret;
	}

	int16_t Footholdtree::get_y_below(Point<int16_t> position) const
	{
		if (uint16_t fhid = get_fhid_below(position.x(), position.y()))
		{
			const Foothold &fh = get_fh(fhid);

			return static_cast<int16_t>(fh.ground_below(position.x()));
		} else
		{
			return borders.second();
		}
	}

	Range<int16_t> Footholdtree::get_walls() const
	{
		return walls;
	}

	Range<int16_t> Footholdtree::get_borders() const
	{
		return borders;
	}
}