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
#include "PlayerStates.h"

#include "../Audio/Audio.h"

namespace ms
{
	// Base class
	void PlayerState::play_jumpsound() const
	{
		Sound(Sound::JUMP).play();
	}

	// Null state
	void PlayerNullState::update_state(Player& player) const
	{
		Char::State state;

		if (player.get_phobj().onground)
		{
			if (player.is_key_down(KeyAction::Id::LEFT))
			{
				state = Char::State::WALK;
				player.set_direction(false);
			}
			else if (player.is_key_down(KeyAction::Id::RIGHT))
			{
				state = Char::State::WALK;
				player.set_direction(true);
			}
			else if (player.is_key_down(KeyAction::Id::DOWN))
			{
				state = Char::State::PRONE;
			}
			else
			{
				state = Char::State::STAND;
			}
		}
		else
		{
			Optional<const Ladder> ladder = player.get_ladder();

			if (ladder)
				state = ladder->is_ladder() ? Char::State::LADDER : Char::State::ROPE;
			else
				state = Char::State::FALL;
		}

		player.get_phobj().clear_flags();

		player.set_state(state);
	}

	// Standing
	void PlayerStandState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerStandState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::LEFT:
				player.set_direction(false);
				player.set_state(Char::State::WALK);
				break;
			case KeyAction::Id::RIGHT:
				player.set_direction(true);
				player.set_state(Char::State::WALK);
				break;
			case KeyAction::Id::JUMP:
				play_jumpsound();
				player.get_phobj().vforce = -player.get_jumpforce();
				break;
			case KeyAction::Id::DOWN:
				player.set_state(Char::State::PRONE);
				break;
			}
		}
	}

	void PlayerStandState::update(Player& player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);
	}

	void PlayerStandState::update_state(Player& player) const
	{
		if (!player.get_phobj().onground)
			player.set_state(Char::State::FALL);
	}

	// Walking
	void PlayerWalkState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerWalkState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::LEFT:
				player.set_direction(false);
				break;
			case KeyAction::Id::RIGHT:
				player.set_direction(true);
				break;
			case KeyAction::Id::JUMP:
				play_jumpsound();
				player.get_phobj().vforce = -player.get_jumpforce();
				break;
			case KeyAction::Id::DOWN:
				player.set_state(Char::State::PRONE);
				break;
			}
		}
	}

	bool PlayerWalkState::haswalkinput(const Player& player) const
	{
		return player.is_key_down(KeyAction::Id::LEFT) || player.is_key_down(KeyAction::Id::RIGHT);
	}

	void PlayerWalkState::update(Player& player) const
	{
		if (!player.is_attacking() && haswalkinput(player))
			player.get_phobj().hforce += player.getflip() ? player.get_walkforce() : -player.get_walkforce();

		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);
	}

	void PlayerWalkState::update_state(Player& player) const
	{
		if (player.get_phobj().onground)
		{
			if (!haswalkinput(player) || player.get_phobj().hspeed == 0.0f)
				player.set_state(Char::State::STAND);
		}
		else
		{
			player.set_state(Char::State::FALL);
		}
	}

	// Falling
	void PlayerFallState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerFallState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::LEFT:
				player.set_direction(false);
				break;
			case KeyAction::Id::RIGHT:
				player.set_direction(true);
				break;
			}
		}
	}

	void PlayerFallState::update(Player& player) const
	{
		auto& hspeed = player.get_phobj().hspeed;

		if (player.is_key_down(KeyAction::Id::LEFT) && hspeed > 0.0)
		{
			hspeed -= 0.025;

			if (hspeed < 0.0)
				hspeed = 0.0;
		}
		else if (player.is_key_down(KeyAction::Id::RIGHT) && hspeed < 0.0)
		{
			hspeed += 0.025;

			if (hspeed > 0.0)
				hspeed = 0.0;
		}
	}

	void PlayerFallState::update_state(Player& player) const
	{
		if (player.get_phobj().onground)
		{
			if (player.is_key_down(KeyAction::Id::LEFT))
			{
				player.set_direction(false);
				player.set_state(Char::State::WALK);
			}
			else if (player.is_key_down(KeyAction::Id::RIGHT))
			{
				player.set_direction(true);
				player.set_state(Char::State::WALK);
			}
			else if (player.is_key_down(KeyAction::Id::DOWN))
			{
				player.set_state(Char::State::PRONE);
			}
			else
			{
				player.set_state(Char::State::STAND);
			}
		}
		else if (player.is_underwater())
		{
			player.set_state(Char::State::SWIM);
		}
	}

	// Prone
	void PlayerProneState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::JUMP:
				if (player.get_phobj().enablejd && player.is_key_down(KeyAction::Id::DOWN))
				{
					play_jumpsound();
					player.get_phobj().y = player.get_phobj().groundbelow;
					player.set_state(Char::State::FALL);
				}

				break;
			}
		}
		else
		{
			switch (ka)
			{
			case KeyAction::Id::DOWN:
				player.set_state(Char::State::STAND);
				break;
			}
		}
	}

	void PlayerProneState::update(Player& player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);
	}

	// Sitting
	void PlayerSitState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::LEFT:
				player.set_direction(false);
				player.set_state(Char::State::WALK);
				break;
			case KeyAction::Id::RIGHT:
				player.set_direction(true);
				player.set_state(Char::State::WALK);
				break;
			case KeyAction::Id::JUMP:
				play_jumpsound();
				player.set_state(Char::State::STAND);
				break;
			case KeyAction::Id::UP:
				player.set_state(Char::State::STAND);
				break;
			}
		}
	}

	// Flying
	void PlayerFlyState::initialize(Player& player) const
	{
		player.get_phobj().type = player.is_underwater() ? PhysicsObject::Type::SWIMMING : PhysicsObject::Type::FLYING;
	}

	void PlayerFlyState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::LEFT:
				player.set_direction(false);
				break;
			case KeyAction::Id::RIGHT:
				player.set_direction(true);
				break;
			}
		}
	}

	void PlayerFlyState::update(Player& player) const
	{
		if (player.is_attacking())
			return;

		if (player.is_key_down(KeyAction::Id::LEFT))
			player.get_phobj().hforce = -player.get_flyforce();
		else if (player.is_key_down(KeyAction::Id::RIGHT))
			player.get_phobj().hforce = player.get_flyforce();

		if (player.is_key_down(KeyAction::Id::UP))
			player.get_phobj().vforce = -player.get_flyforce();
		else if (player.is_key_down(KeyAction::Id::DOWN))
			player.get_phobj().vforce = player.get_flyforce();
	}

	void PlayerFlyState::update_state(Player& player) const
	{
		if (player.get_phobj().onground && player.is_underwater())
		{
			Char::State state;

			if (player.is_key_down(KeyAction::Id::LEFT))
			{
				state = Char::State::WALK;
				player.set_direction(false);
			}
			else if (player.is_key_down(KeyAction::Id::RIGHT))
			{
				state = Char::State::WALK;
				player.set_direction(true);
			}
			else if (player.is_key_down(KeyAction::Id::DOWN))
			{
				state = Char::State::PRONE;
			}
			else
			{
				state = Char::State::STAND;
			}

			player.set_state(state);
		}
	}

	// Climbing
	void PlayerClimbState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::FIXATED;
	}

	void PlayerClimbState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down)
		{
			switch (ka)
			{
			case KeyAction::Id::JUMP:
				if (player.is_key_down(KeyAction::Id::LEFT))
				{
					play_jumpsound();
					player.set_direction(false);
					player.get_phobj().hspeed = -player.get_walkforce() * 8.0;
					player.get_phobj().vspeed = -player.get_jumpforce() / 1.5;
					cancel_ladder(player);
				}
				else if (player.is_key_down(KeyAction::Id::RIGHT))
				{
					play_jumpsound();
					player.set_direction(true);
					player.get_phobj().hspeed = player.get_walkforce() * 8.0;
					player.get_phobj().vspeed = -player.get_jumpforce() / 1.5;
					cancel_ladder(player);
				}

				break;
			}
		}
	}

	void PlayerClimbState::update(Player& player) const
	{
		if (player.is_key_down(KeyAction::Id::UP))
			player.get_phobj().vspeed = -player.get_climbforce();
		else if (player.is_key_down(KeyAction::Id::DOWN))
			player.get_phobj().vspeed = player.get_climbforce();
		else
			player.get_phobj().vspeed = 0.0;
	}

	void PlayerClimbState::update_state(Player& player) const
	{
		int16_t y = player.get_phobj().get_y();
		bool downwards = player.is_key_down(KeyAction::Id::DOWN);
		auto ladder = player.get_ladder();

		if (ladder && ladder->felloff(y, downwards))
			cancel_ladder(player);
	}

	void PlayerClimbState::cancel_ladder(Player& player) const
	{
		player.set_state(Char::State::FALL);
		player.set_ladder(nullptr);
	}
}