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

#include "Player.h"

namespace ms
{
	// Base class for player states
	class PlayerState
	{
	public:
		virtual ~PlayerState()
		{}

		// Actions taken when transitioning into the state
		virtual void initialize(Player& player) const = 0;

		// How to handle inputs while in the state
		virtual void send_action(Player& player, KeyAction::Id action, bool pressed) const = 0;

		// Actions taken in the player's update method, before physics are applied.
		virtual void update(Player& player) const = 0;

		// Transition into a new state after physics have been applied
		virtual void update_state(Player& player) const = 0;

	protected:
		// Play the jumping sound
		void play_jumpsound() const;

		// Check if the left or right key is pressed
		bool haswalkinput(const Player& player) const;

		// Check if only the left key is pressed and not the right key
		bool hasleftinput(const Player& player) const;

		// Check if only the right key is pressed and not the left key
		bool hasrightinput(const Player& player) const;
	};

	// The initial state, determines which state the player should be in.
	class PlayerNullState : public PlayerState
	{
	public:
		void initialize(Player&) const override
		{}

		void send_action(Player&, KeyAction::Id, bool) const override
		{}

		void update(Player&) const override
		{}

		void update_state(Player& player) const override;
	};

	// The standing state
	class PlayerStandState : public PlayerState
	{
	public:
		void initialize(Player& player) const override;

		void send_action(Player& player, KeyAction::Id ka, bool down) const override;

		void update(Player&) const override;

		void update_state(Player& player) const override;
	};

	// The walking state
	class PlayerWalkState : public PlayerState
	{
		void initialize(Player& player) const override;

		void send_action(Player& player, KeyAction::Id ka, bool down) const override;

		void update(Player& player) const override;

		void update_state(Player& player) const override;
	};

	// The falling state
	class PlayerFallState : public PlayerState
	{
	public:
		void initialize(Player& player) const override;

		void send_action(Player& player, KeyAction::Id ka, bool down) const override
		{}

		void update(Player& player) const override;

		void update_state(Player& player) const override;
	};

	// The prone state (Lying down)
	class PlayerProneState : public PlayerState
	{
	public:
		void initialize(Player&) const override
		{}

		void send_action(Player& player, KeyAction::Id ka, bool down) const override;

		void update(Player&) const override;

		void update_state(Player&) const override
		{}
	};

	// The sitting state
	class PlayerSitState : public PlayerState
	{
	public:
		void initialize(Player&) const override
		{}

		void send_action(Player& player, KeyAction::Id ka, bool down) const override;

		void update(Player&) const override
		{}

		void update_state(Player&) const override
		{}
	};

	// The flying or swimming state
	class PlayerFlyState : public PlayerState
	{
		void initialize(Player& player) const override;

		void send_action(Player& player, KeyAction::Id ka, bool down) const override;

		void update(Player& player) const override;

		void update_state(Player& player) const override;
	};

	// The climbing state
	class PlayerClimbState : public PlayerState
	{
	public:
		void initialize(Player& player) const override;

		void send_action(Player&, KeyAction::Id, bool) const override
		{}

		void update(Player& player) const override;

		void update_state(Player& player) const override;

	private:
		void cancel_ladder(Player& player) const;
	};
}