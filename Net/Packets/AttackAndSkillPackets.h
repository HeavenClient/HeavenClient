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
#include <chrono>
#include "../../Character/Buff.h"
#include "../OutPacket.h"
#include "../../Gameplay/Stage.h"
#include "../../Gameplay/MapleMap/Mob.h"
namespace ms
{
	// Notifies the server of an attack
	// The opcode is determined by the attack type
	// Attack::CLOSE = CLOSE_ATTACK(44)
	// Attack::RANGED = RANGED_ATTACK(45)
	// Attack::MAGIC = MAGIC_ATTACK(46)
	class AttackPacket : public OutPacket
	{
	public:
		AttackPacket(AttackResult& attack) : OutPacket(opcodefor(attack.type))
		{
			Player& cd = Stage::get().get_player();
			CharStats& cs = cd.get_stats();
			EnumMap<MapleStat::Id, uint16_t> bs = cs.get_basestats();
			//EnumMap<Buffstat::Id, Buff> ss = cd.get_buffs();
			int32_t wt = cd.get_weapontype();

			uint32_t randNumber;

			using namespace std::chrono;
			uint64_t tCur = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

			attack.time = tCur;

			int32_t action;
			int32_t actiontype;
						
			//TryDoingShootAttack(attack, cd, randNumber, tCur);

			skip(1);

			write_byte((attack.mobcount << 4) | attack.hitcount);
			write_int(attack.skill);

			if (attack.charge > 0)
				write_int(attack.charge);

			skip(8); // send something stupid!

			write_byte(attack.display);
			write_byte(attack.toleft);
			write_byte(attack.stance);
			write_byte(attack.action_type);
			write_byte(attack.speed);

			if (attack.type == Attack::Type::RANGED)
			{
				skip(1);
				write_byte(attack.toleft);
				skip(7);
				// TODO: skip(4); If hurricane, piercing arrow or rapidfire.
			}
			else
			{
				write_int(attack.time);
			}

			for (auto& damagetomob : attack.damagelines)
			{
				write_int(damagetomob.first);

				skip(14);

				for (auto& singledamage : damagetomob.second)
				{
					write_int(singledamage.first);
					// TODO: Add critical here
				}

				if (attack.skill != 5221004
					|| attack.skill != 21110007
					|| attack.skill != 21110008
					|| attack.skill != 21120009
					|| attack.skill != 21120010)
					skip(4);
			}
			if (attack.skill == 14111006)
				skip(4);
				write_short(0);
				write_short(0);
		}

	private:
		static OutPacket::Opcode opcodefor(Attack::Type type)
		{
			switch (type)
			{
			case Attack::Type::CLOSE:
				return OutPacket::Opcode::CLOSE_RANGE_ATTACK;
			case Attack::Type::RANGED:
				return OutPacket::Opcode::RANGED_ATTACK;
			default:
				return OutPacket::Opcode::MAGIC_ATTACK;
			}
		}

		int32_t get_cool_time(int32_t nSkillID) 
		{
			bool v2;
			if (nSkillID > 5211005)
			{
				if (nSkillID > 30001068)
				{
					if (nSkillID <= 33121005)
					{
						if (nSkillID == 33121005) {
							return 1000;
						}
						if (nSkillID == 32101001 || nSkillID == 32111011) {
							return 500;
						}
						if (nSkillID != 33101002) {
							return 0;
						}
						return 400;
					}
					if (nSkillID == 35001001) {
						return 1000;
					}
					v2 = nSkillID == 35101009;
				}
				else
				{
					if (nSkillID == 30001068) {
						return 1000;
					}
					if (nSkillID <= 15111004)
					{
						if (nSkillID == 15111004) {
							return 200;
						}
						if (nSkillID == 11101005) {
							return 1500;
						}
						if (nSkillID != 13101005)
						{
							if (nSkillID == 14111006) {
								return 1500;
							}
							return 0;
						}
						return 400;
					}
					if (nSkillID == 21001001) {
						return 1000;
					}
					v2 = nSkillID == 22141001;
				}
				if (v2) {
					return 1000;
				}
				return 0;
			}
			if (nSkillID >= 5211004) {
				return 450;
			}
			if (nSkillID > 4221007)
			{
				if (nSkillID <= 5121007)
				{
					if (nSkillID != 5121007)
					{
						if (nSkillID != 4321002)
						{
							if (nSkillID == 4341003)
								return 2000;
							if (nSkillID != 5121005)
								return 0;
							return 500;
						}
						return 1000;
					}
					return 200;
				}
				if (nSkillID == 5201002)
					return 1000;
				v2 = nSkillID - 5201002 == 4;
				if (v2) {
					return 1000;
				}
				return 0;
			}
			if (nSkillID == 4221007) {
				return 200;
			}
			if (nSkillID <= 3120010)
			{
				if (nSkillID != 3120010)
				{
					if (nSkillID != 1121001 && nSkillID != 1321001)
					{
						if (nSkillID != 3101003) {
							return 0;
						}
						return 400;
					}
					return 500;
				}
				return 1500;
			}
			if (nSkillID == 3201003) {
				return 400;
			}
			if (nSkillID != 4001003)
			{
				if (nSkillID == 4121008) {
					return 500;
				}
				return 0;
			}
			return 1000;
		}

		void TryDoingMeleeAttack(AttackResult& attack, Player& cd, uint32_t randNumber, uint64_t tCur) 
		{
			randNumber = cd.get_calcdamage().get_RndGenForCharacter().random();
			if (cd.get_stats().get_hp() <= 0) 
				return;
			if (!is_throw_bomb_skill(attack.skill))
				return;
			if (attack.skill == 1311006
					|| attack.skill == 4221001
					|| attack.skill == 1121006
					|| attack.skill == 1221007
					|| attack.skill == 1321003
					|| attack.skill == 4321001
					|| attack.skill == 5121004
					|| attack.skill == 5111006
					|| attack.skill == 15111003
					|| attack.skill == 5101002
					|| attack.skill == 5221003
					|| attack.skill == 5121001
					|| attack.skill == 5121005
					|| attack.skill == 5121007
					|| attack.skill == 15111004
					|| attack.skill == 1009
					|| attack.skill == 10001009
					|| attack.skill == 20001009
					|| attack.skill == 20011009
					|| attack.skill == 30001009
					|| attack.skill == 10001020
					|| attack.skill == 20001020
					|| attack.skill == 20011020
					|| attack.skill == 30001020
					|| attack.skill == 21000002
					|| attack.skill == 21100001
					|| attack.skill == 21110007
					|| attack.skill == 21110008
					|| attack.skill == 21120009
					|| attack.skill == 21120010
					|| attack.skill == 21100002
					|| attack.skill == 20000014
					|| attack.skill == 20000015
					|| attack.skill == 4311003
					|| attack.skill == 4321000
					|| attack.skill == 4331000
					|| attack.skill == 4341004
					|| attack.skill == 32001007
					|| attack.skill == 32001008
					|| attack.skill == 32001009
					|| attack.skill == 32001010
					|| attack.skill == 32001011
					|| attack.skill == 35121004
					|| attack.skill == 33111002
					|| attack.skill == 35001003
					|| attack.skill == 1121001
					|| attack.skill == 1321001
					|| attack.skill == 3120010)
			{
				return;
			}
			if (attack.skill == 4211002
					|| attack.skill == 4221001
					|| attack.skill == 1121006
					|| attack.skill == 1221007
					|| attack.skill == 1321003
					|| attack.skill == 4321001
					|| attack.skill == 4121008
					|| attack.skill == 5101002
					|| attack.skill == 5101004
					|| attack.skill == 15101003
					|| attack.skill == 5121005
					|| attack.skill == 21100002
					|| attack.skill == 21110003
					|| attack.skill == 21110006
					|| attack.skill == 4311003
					|| attack.skill == 4331000
					|| attack.skill == 4331005
					|| attack.skill == 4331004
					|| attack.skill == 4341002
					|| attack.skill == 33111002
					|| attack.skill == 35001003
					|| attack.skill == 1121001
					|| attack.skill == 1321001
					|| attack.skill == 3120010)
			{
				return;
			} 
			if ((tCur < (tCur + get_cool_time(attack.skill)))
				&& (attack.skill == 1121006
					|| attack.skill == 1221007
					|| attack.skill == 1321003
					|| attack.skill == 5101002
					|| attack.skill == 5101004
					|| attack.skill == 15101003
					|| attack.skill == 4331005
					|| attack.skill == 4311003
					|| attack.skill == 4331000
					|| attack.skill == 4341002
					|| attack.skill == 35001003
					|| attack.skill == 33111002))
			{
				return;
			}
			if (attack.skill == 4221007)
			{
				return;
			}  
			if ((attack.skill == 4211002
				|| attack.serialskill == 4221001
				|| attack.skill == 5111006
				|| attack.skill == 15111003)
				//&& !CUserLocal::IsTeleportSkillAvailable(v506, pSkill, nSLV, &ptAfterTeleport)
				)
			{
				return;
			}
		}

		bool is_throw_bomb_skill(int32_t nSkillID) 
		{
			int v1; // ecx
			bool result; // eax

			if (is_prepare_bomb_skill(nSkillID) || (result = is_not_prepare_bomb_skill(v1)) != 0)
				result = 1;
			return result;
		}

		bool is_prepare_bomb_skill(int32_t nSkillID) 
		{
			return nSkillID == 4341003 || nSkillID == 5201002 || nSkillID == 14111006;
		}

		bool is_not_prepare_bomb_skill(int32_t nSkillID) 
		{
			return nSkillID == 4321002;
		}

		void TryDoingShootAttack(AttackResult& attack, Player& cd, uint32_t randNumber, uint64_t tCur) 
		{
			if (attack.display) { // bMortalBlow
				randNumber = cd.get_calcdamage().get_RndForMortalBlow().random();
			}
			else {
				randNumber = cd.get_calcdamage().get_RndGenForCharacter().random();
			}
			if (cd.get_stats().get_hp() <= 0) {
				return;
			}
			if (attack.skill == 3121003 || attack.skill == 3221003 || attack.skill == 5201006) {
				return;

			}
			// TODO: handle if !foothold, !IsSwimming, !is_able_to_jumpshoot, On Ladder or Rope
			if (attack.skill == 35001001 && attack.skill == 35101009 && (tCur < (tCur + get_cool_time(attack.skill)))) {
				return;
			}
			if (attack.skill == 4121003 || attack.skill == 4221003) {
				attack.action = 96;
			}
			else if (attack.skill == 5121002) {
				attack.action = 105;
			}
			else if (attack.skill == 5201006) {
				attack.action = 115;
			}
			else if (attack.skill == 5201006) {
				attack.action = 115;
			}
			// 1 more
			else {
				attack.action = -1;
			}
			if (attack.action < 0) {
				return;
			}
			bool bSoulArrow = cd.has_buff(Buffstat::SOULARROW);
			bool bShadowPartner = cd.has_buff(Buffstat::SHADOWPARTNER);
			if (attack.skill == 14101006)
				bShadowPartner = false;
			bool bSpark = cd.has_buff(Buffstat::SPARK);
			if (!bSoulArrow && !is_shoot_skill_not_consuming_bullet(attack.skill)) {
				if (cd.get_inventory().get_bulletcount() <= 0) {
					return;
				}
			}
			if (attack.mobcount <= 0) {
				attack.mobcount = 1;
			}
			int32_t degree = cd.get_weaponspeed();
			int32_t weaponbooster = cd.get_integer_attackspeed() - degree;
			attack.speed = get_attack_speed_degree(degree, attack.skill, weaponbooster, 0, 0, 0);
		}

		bool is_shoot_skill_not_consuming_bullet(int32_t nSkillID) 
		{
			int16_t v1; // ecx

			if (is_shoot_skill_not_using_shooting_weapon(nSkillID))
				return true;
			if (v1 > 35001001)
			{
				if (v1 > 35111015)
				{
					if (v1 == 35121005 || v1 > 35121011 && v1 <= 35121013) {
						return true;
					}
					return false;
				}
				if (v1 != 35111015)
				{
					if (v1 <= 35101010)
					{
						if (v1 >= 35101009 || v1 == 35001004)
							return true;
						return false;
					}
					if (v1 != 35111004)
						return false;
				}
				return true;
			}
			if (v1 == 35001001) {
				return true;
			}
			if (v1 <= 13101005)
			{
				if (v1 == 13101005 || v1 == 3101003 || v1 == 3201003 || v1 == 4111004) {
					return true;
				}
				return false;
			}
			if (v1 == 14101006 || v1 == 33101002) {
				return true;
			}
			return false;
		}

		bool is_shoot_skill_not_using_shooting_weapon(int32_t nSkillID) 
		{
			bool v1; // zf

			if (nSkillID > 15111007)
			{
				if (nSkillID > 21120006)
				{
					v1 = nSkillID == 33101007;
				}
				else
				{
					if (nSkillID == 21120006 || nSkillID == 21100004) {
						return 1;
					}
					v1 = nSkillID == 21110004;
				}
			}
			else
			{
				if (nSkillID >= 15111006) {
					return true;
				}
				if (nSkillID > 5121002)
				{
					v1 = nSkillID == 11101004;
				}
				else
				{
					if (nSkillID == 5121002 || nSkillID == 4121003) {
						return true;
					}
					v1 = nSkillID == 4221003;
				}
			}
			if (!v1) {
				return false;
			}
			return true;
		}

		int32_t get_attack_speed_degree(int32_t nDegree, int32_t nSkillID, int32_t nWeaponBooster, int32_t nPartyBooster, int32_t nAuraBooster, int32_t nFrozen) 
		{
			int v6; // esi
			int v7; // esi
			int result; // eax

			if (nSkillID > 4331000)
			{
				if (nSkillID != 4331005 && (nSkillID <= 4341001 || nSkillID > 4341003))
					goto LABEL_12;
				goto LABEL_11;
			}
			if (nSkillID == 4331000)
			{
			LABEL_11:
				nWeaponBooster = 0;
				nPartyBooster = 0;
				nFrozen = 0;
			LABEL_12:
				v6 = nDegree;
				goto LABEL_13;
			}
			if (nSkillID != 4001334)
			{
				if (nSkillID != 4311003 && nSkillID != 4321001)
					goto LABEL_12;
				goto LABEL_11;
			}
			v6 = nDegree - 2;
		LABEL_13:
			v7 = nAuraBooster + nPartyBooster + nWeaponBooster + v6;
			if (nFrozen && v7 < 10)
				v7 += ((nFrozen * (10 - v7)) / 100.0);
			if (v7 <= 2)
				return 2;
			result = v7;
			if (v7 >= 10)
				result = 10;
			return result;
		}
	};

	// Tells the server that the player took damage
	// Opcode: TAKE_DAMAGE(48)
	class TakeDamagePacket : public OutPacket
	{
	public:
		enum From : int8_t
		{
			TOUCH = -1
		};

		TakeDamagePacket(int8_t from, uint8_t element, int32_t damage, int32_t mobid, int32_t oid, uint8_t direction) : OutPacket(OutPacket::Opcode::TAKE_DAMAGE)
		{
			Player& player = Stage::get().get_player();
			write_time();
			write_byte(from);
			write_byte(element);
			write_int(damage);
			write_int(mobid);
			write_int(oid);
			write_byte(direction);

			player.get_stats().add_hp(-damage);
		}

		// From mob attack result
		TakeDamagePacket(const MobAttackResult& result, From from) : TakeDamagePacket(from, 0, result.damage, result.mobid, result.oid, result.direction) {}
	};

	// Packet which notifies the server of a skill usage
	// Opcode: USE_SKILL(91)
	class UseSkillPacket : public OutPacket
	{
	public:
		UseSkillPacket(int32_t skillid, int32_t level) : OutPacket(OutPacket::Opcode::SPECIAL_MOVE)
		{
			write_time();
			write_int(skillid);
			write_byte(static_cast<uint8_t>(level));

			// MONSTER MAGNET
			if (skillid == 1121001 || skillid == 1221001 || skillid == 1321001) {
				int32_t num = 0;
				write_int(num);
				for (int i = 0; i < num; i++) {
					write_int(0); // mobOid
					write_byte(0); // success
					write_byte(0); // direction
				}
			}
			else if (skillid == 9101000) {
				skip(11);
			}
			else if (skillid % 10000000 == 1004) {
				skip(2); // TODO: No idea what this could be
			}

			// TODO: A point (4 bytes) could be added at the end
		}
	};
}