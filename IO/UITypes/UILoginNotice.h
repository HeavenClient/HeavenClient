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

#include "../UIElement.h"

namespace ms
{
	class UIKeyConfirm : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGINNOTICE_CONFIRM;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UIKeyConfirm(bool alternate, std::function<void()> oh, bool login);

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		void confirm();

		enum Buttons
		{
			OK
		};

		std::function<void()> okhandler;
		bool login;
	};

	class UIKeySelect : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGINNOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UIKeySelect(std::function<void(bool)> okhandler, bool login);

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons
		{
			CLOSE,
			TYPEA,
			TYPEB
		};

		std::function<void(bool)> okhandler;
		bool login;
	};

	class UIClassConfirm : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGINNOTICE;
		static constexpr bool FOCUSED = false;
		static constexpr bool TOGGLED = false;

		UIClassConfirm(uint16_t selected_class, bool unavailable, std::function<void()> okhandler);

		Cursor::State send_cursor(bool clicked, Point<int16_t> cursorpos) override;

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons : uint16_t
		{
			OK,
			CANCEL
		};

		std::function<void()> okhandler;
	};

	class UIQuitConfirm : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGINNOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		UIQuitConfirm();

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons
		{
			BT_OK,
			BT_CANCEL
		};
	};

	class UILoginNotice : public UIElement
	{
	public:
		static constexpr Type TYPE = UIElement::Type::LOGINNOTICE;
		static constexpr bool FOCUSED = true;
		static constexpr bool TOGGLED = false;

		enum Message : uint16_t
		{
			VULGAR_NAME,
			DELETE_CHAR_ENTER_BIRTHDAY,
			WRONG_EMAIL,
			WRONG_PASSWORD,
			INCORRECT_EMAIL,
			NAME_IN_USE,
			NAME_OK,
			RETURN_TO_FIRST_PAGE,
			NAME_IN_USE2,
			FULL_CHARACTER_SLOTS,
			ILLEGAL_NAME,
			BIRTHDAY_INCORRECT,
			PRESS_CHECK_BUTTON,
			DELETE_CONFIRMATION,
			MATURE_CHANNEL,
			TROUBLE_LOGGING_IN,
			BLOCKED_ID,
			ALREADY_LOGGED_IN,
			UNKNOWN_ERROR,
			TOO_MANY_REQUESTS,
			NOT_REGISTERED,
			UNABLE_TO_LOGIN_WITH_IP,
			UNABLE_TO_LOGIN,
			UNABLE_TO_CONNECT,
			AN_ERROR_OCCURED,
			AN_ERROR_OCCURED_DETAILED,
			CANNOT_ACCESS_ACCOUNT,
			WRONG_GATEWAY,
			INCORRECT_LOGINID,
			INCORRECT_FORM,
			VERIFICATION_NOTICE_7,
			VERIFICATION_NOTICE_30,
			KOREAN,
			VERIFY_EMAIL,
			CANNOT_DELETE_GUILD_LEADER,
			SUSPICIOUS_PROGRAMS,
			POPULATION_TOO_HIGH,
			SELECT_A_CHANNEL,
			GAME_GUARD_UPDATED,
			CANNOT_DELETE_ENGAGED,
			PLEASE_SIGN_UP,
			PASSWORD_IS_INCORRECT,
			SECOND_PASSWORD_INCORRECT,
			TEMPORARY_IP_BAN,
			DISABLE_SAFETY_MEASURE,
			SECOND_PASSWORD_NOT_DIFFERENT,
			CANNOT_DELETE_ENGAGED2,
			KOREAN2,
			KOREAN3,
			KOREAN4,
			KOREAN5,
			KOREAN6,
			KOREAN7,
			CONFIRM_EXIT,
			CANNOT_DELETE_FAMILY_LEADER,
			CASH_ITEMS_CONFIRM_DELETION,
			KOREAN8,
			FAMILY_CONFIRM_DELETION,
			FAMILY_AND_CASH_ITEMS_CONFIRM_DELETION,
			KOREAN9,
			KOREAN10,
			IDENTITY_VERIFICATION_REQ_ERROR,
			KOREAN11,
			LOGIN_FAIL_SERVER_OVERBURDEN,
			KOREAN12,
			KOREAN13,
			KOREAN14,
			KOREAN15,
			IDENTITY_VERIFICATION_REQ,
			PART_TIME_JOB_ACTIVE,
			DEL_CHAR_FAIL_HIRED_MERCH_ACTIVE,
			SET_SEC_PASS,
			SEC_PASS_AUTH_FAIL,
			EMAIL_ID_FAIL_USE_MAPLE_ID,
			LOGIN_USING_EMAIL_ID,
			MAPLE_ID_REQ,
			MAPLE_ID_SUCCESS,
			MAPLE_ID_FAIL,
			LOGIN_USING_MAPLE_ID_OR_EMAIL_ID,
			DEL_CHAR_FAIL_ACTIVE_ITEM_GUARD,
			MAPLE_ID_SUCCESS_BOLD,
			CHARACTER_RANGE_FAILED,
			MAPLE_ID_ALREADY_EXISTS,
			MAPLE_ID_ONLY_LETTERS_AND_NUMS,
			MAPLE_ID_EMAIL_ID_NOT_AUTH,
			MAPLE_ID_ALREADY_CREATED_TODAY,
			MAPLE_ID_MAX_LIMIT_REACHED_BLOCK,
			MAPLE_ID_MAX_LIMIT_REACHED_CREATE,
			KOREAN16,
			UNDER_AGE,
			UNDER_AGE2,
			PIC_SAME_AS_PASSWORD,
			PIC_CONTAINS_PIN,
			INCORRECT_PIC,
			CHAR_DEL_FAIL_NO_PIC,
			PIC_REQ,
			OTP_SERVICE_IN_USE,
			CHAR_DEL_FAIL_OTP_SERVICE,
			SEC_PASS_CHANGE_FAIL_OTP_SERVICE,
			CLIENT_ALREADY_RUNNING,
			CHAR_TRANS_SUCCESS = 105,
			CHAR_DEL_FAIL_MAX_LIMIT_REACHED,
			OVERSEAS_LOGIN_BLOCKED,
			DEL_CHAR_FAIL_ITEMS_IN_AUC_HOUSE,
			ENTER_STAR_PLANET_CONF_SEL_CHAR,
			ENTER_STAR_PLANET_CONF_STAR_PLANET_CHAR,
			CREATE_SHINING_STAR_CHAR_CONF,
			CLASS_UNAVAILABLE,
			START_PLANET_FAIL_LV_33_REQ,
			PIC_ACTIVATED = 902,
			PIC_DEACTIVATED,
			CHANNEL_SEL_REQ,
			GAMEGUARD_UPDATE_REQ,
			IP_BLOCK_GMS,
			PIC_USED_TOO_FREQ,
			PIC_CHANGES_REQ_FROM_WEB,
			PIC_UNSECURE,
			PIC_STALE,
			PIC_REPITIVE,
			NEW_PIC_REQ,
			JAPANESE = 10000,
			JAPANESE2
		};

		UILoginNotice(uint16_t message, std::function<void()> okhandler, std::function<void()> cancelhandler);

		UILoginNotice(uint16_t message, std::function<void()> okhandler);

		UILoginNotice(uint16_t message);

		void send_key(int32_t keycode, bool pressed, bool escape) override;

		UIElement::Type get_type() const override;

	protected:
		Button::State button_pressed(uint16_t buttonid) override;

	private:
		enum Buttons : uint16_t
		{
			YES,
			NO
		};

		bool saveid;
		bool multiple;
		std::function<void()> okhandler;
		std::function<void()> cancelhandler;
	};
}