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

namespace ms
{
#pragma region Base
	void PlayerState::play_jumpsound() const
	{
		Sound(Sound::Name::JUMP).play();
	}

	bool PlayerState::haswalkinput(const Player& player) const
	{
		return player.is_key_down(KeyAction::Id::LEFT) || player.is_key_down(KeyAction::Id::RIGHT);
	}

	bool PlayerState::hasleftinput(const Player& player) const
	{
		return player.is_key_down(KeyAction::Id::LEFT) && !player.is_key_down(KeyAction::Id::RIGHT);
	}

	bool PlayerState::hasrightinput(const Player& player) const
	{
		return player.is_key_down(KeyAction::Id::RIGHT) && !player.is_key_down(KeyAction::Id::LEFT);
	}
#pragma endregion

#pragma region Null
	void PlayerNullState::update_state(Player& player) const
	{
		Char::State state;

		if (player.get_phobj().onground)
		{
			if (player.is_key_down(KeyAction::Id::LEFT))
			{
				state = Char::State::WALK;

				player.set_direction(false);
			} else if (player.is_key_down(KeyAction::Id::RIGHT))
			{
				state = Char::State::WALK;

				player.set_direction(true);
			} else if (player.is_key_down(KeyAction::Id::DOWN))
			{
				state = Char::State::PRONE;
			} else
			{
				state = Char::State::STAND;
			}
		} else
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
#pragma endregion

#pragma region Standing
	void PlayerStandState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerStandState::send_action(Player &player, KeyAction::Id ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down && ka == KeyAction::Id::JUMP)
		{
			play_jumpsound();

			player.get_phobj().vforce = -player.get_jumpforce();
		}
	}

	void PlayerStandState::update(Player &player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);

		if (player.is_attacking())
			return;

		if (hasrightinput(player))
		{
			player.set_direction(true);
			player.set_state(Char::State::WALK);
		}
		else if (hasleftinput(player))
		{
			player.set_direction(false);
			player.set_state(Char::State::WALK);
		}

		if (player.is_key_down(KeyAction::Id::DOWN) && !player.is_key_down(KeyAction::Id::UP) && !haswalkinput(player))
			player.set_state(Char::State::PRONE);
	}

	void PlayerStandState::update_state(Player &player) const
	{
		if (!player.get_phobj().onground)
			player.set_state(Char::State::FALL);
	}
#pragma endregion

#pragma region Walking
	void PlayerWalkState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerWalkState::send_action(Player &player, KeyAction::Id ka, bool down) const
	{
		if (player.is_attacking())
			return;

		if (down && ka == KeyAction::Id::JUMP)
		{
			play_jumpsound();

			player.get_phobj().vforce = -player.get_jumpforce();
		}

		if (down && ka == KeyAction::Id::JUMP && player.is_key_down(KeyAction::Id::DOWN) && player.get_phobj().enablejd)
		{
			play_jumpsound();

			player.get_phobj().y = player.get_phobj().groundbelow;
			player.set_state(Char::State::FALL);
		}
	}

	void PlayerWalkState::update(Player &player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);

		if (player.is_attacking())
			return;

		if (haswalkinput(player))
		{
			if (hasrightinput(player))
			{
				player.set_direction(true);
				player.get_phobj().hforce += player.get_walkforce();
			}
			else if (hasleftinput(player))
			{
				player.set_direction(false);
				player.get_phobj().hforce += -player.get_walkforce();
			}
		}
		else
		{
			if (player.is_key_down(KeyAction::Id::DOWN))
				player.set_state(Char::State::PRONE);
		}
	}

	void PlayerWalkState::update_state(Player &player) const
	{
		if (player.get_phobj().onground)
		{
			if (!haswalkinput(player) || player.get_phobj().hspeed == 0.0f)
				player.set_state(Char::State::STAND);
		} else
		{
			player.set_state(Char::State::FALL);
		}
	}
#pragma endregion

#pragma region Falling
	void PlayerFallState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::NORMAL;
	}

	void PlayerFallState::update(Player& player) const
	{
		if (player.is_attacking())
			return;

		auto& hspeed = player.get_phobj().hspeed;

		if (hasleftinput(player) && hspeed > 0.0)
			hspeed -= 0.025;
		else if (hasrightinput(player) && hspeed < 0.0)
			hspeed += 0.025;

		if (hasleftinput(player))
			player.set_direction(false);
		else if (hasrightinput(player))
			player.set_direction(true);
	}

	void PlayerFallState::update_state(Player &player) const
	{
		if (player.get_phobj().onground)
		{
			if (player.is_key_down(KeyAction::Id::DOWN) && !haswalkinput(player))
				player.set_state(Char::State::PRONE);
			else
				player.set_state(Char::State::STAND);
		}
		else if (player.is_underwater())
		{
			player.set_state(Char::State::SWIM);
		}
	}
#pragma endregion

#pragma region Prone
	void PlayerProneState::send_action(Player& player, KeyAction::Id ka, bool down) const
	{
		if (down && ka == KeyAction::Id::JUMP)
		{
			if (player.get_phobj().enablejd && player.is_key_down(KeyAction::Id::DOWN))
			{
				play_jumpsound();

				player.get_phobj().y = player.get_phobj().groundbelow;
				player.set_state(Char::State::FALL);
			}
		}
	}

	void PlayerProneState::update(Player &player) const
	{
		if (player.get_phobj().enablejd == false)
			player.get_phobj().set_flag(PhysicsObject::Flag::CHECKBELOW);

		if (player.is_key_down(KeyAction::Id::UP) || !player.is_key_down(KeyAction::Id::DOWN))
			player.set_state(Char::State::STAND);

		if (player.is_key_down(KeyAction::Id::LEFT))
		{
			player.set_direction(false);
			player.set_state(Char::State::WALK);
		}

		if (player.is_key_down(KeyAction::Id::RIGHT))
		{
			player.set_direction(true);
			player.set_state(Char::State::WALK);
		}
	}
#pragma endregion

#pragma region Sitting
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
					player.set_state(Char::State::STAND);
					break;
			}
		}
	}
#pragma endregion

#pragma region Flying
	void PlayerFlyState::initialize(Player& player) const
	{
		player.get_phobj().type = player.is_underwater() ? PhysicsObject::Type::SWIMMING : PhysicsObject::Type::FLYING;
	}

	void PlayerFlyState::send_action(Player &player, KeyAction::Id ka, bool down) const
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

	void PlayerFlyState::update(Player &player) const
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

	void PlayerFlyState::update_state(Player &player) const
	{
		if (player.get_phobj().onground && player.is_underwater())
		{
			Char::State state;

			if (player.is_key_down(KeyAction::Id::LEFT))
			{
				state = Char::State::WALK;

				player.set_direction(false);
			} else if (player.is_key_down(KeyAction::Id::RIGHT))
			{
				state = Char::State::WALK;

				player.set_direction(true);
			} else if (player.is_key_down(KeyAction::Id::DOWN))
			{
				state = Char::State::PRONE;
			} else
			{
				state = Char::State::STAND;
			}

			player.set_state(state);
		}
	}
#pragma endregion

#pragma region Climbing
	void PlayerClimbState::initialize(Player& player) const
	{
		player.get_phobj().type = PhysicsObject::Type::FIXATED;
	}

	void PlayerClimbState::update(Player& player) const
	{
		if (player.is_key_down(KeyAction::Id::UP) && !player.is_key_down(KeyAction::Id::DOWN))
		{
			player.get_phobj().vspeed = -player.get_climbforce();
		}
		else if (player.is_key_down(KeyAction::Id::DOWN) && !player.is_key_down(KeyAction::Id::UP))
		{
			player.get_phobj().vspeed = player.get_climbforce();
		}
		else
		{
			player.get_phobj().vspeed = 0.0;
		}

		if (player.is_key_down(KeyAction::Id::JUMP) && haswalkinput(player))
		{
			play_jumpsound();

			const auto& walkforce = player.get_walkforce() * 8.0;

			player.set_direction(player.is_key_down(KeyAction::Id::RIGHT));

			player.get_phobj().hspeed = player.is_key_down(KeyAction::Id::LEFT) ? -walkforce : walkforce;
			player.get_phobj().vspeed = -player.get_jumpforce() / 1.5;

			cancel_ladder(player);
		}
	}

	void PlayerClimbState::update_state(Player &player) const
	{
		int16_t y = player.get_phobj().get_y();
		bool downwards = player.is_key_down(KeyAction::Id::DOWN);
		auto ladder = player.get_ladder();

		if (ladder && ladder->felloff(y, downwards))
			cancel_ladder(player);
	}

	void PlayerClimbState::cancel_ladder(Player &player) const
	{
		player.set_state(Char::State::FALL);
		player.set_ladder(nullptr);
		player.set_climb_cooldown();
	}
#pragma endregion
}