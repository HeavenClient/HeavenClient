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

#include "Template/Point.h"
#include "Template/Singleton.h"
#include "Template/TypeMap.h"
#include "Util/Misc.h"

#include <cstdint>
#include <string>
#include <type_traits>
#include <functional>

namespace ms
{
	// Manages the 'Settings' file which contains configurations set by user behaviour.
	class Configuration : public Singleton<Configuration>
	{
	public:
		// Add the settings which will be used and load them.
		Configuration();
		// Save.
		~Configuration();

		// Load all settings. If something is missing, set the default value. Can be used for reloading.
		void load();
		// Save the current settings. 
		void save() const;
		// Get private member SHOW_FPS
		bool get_show_fps() const;
		// Get private member SHOW_PACKETS
		bool get_show_packets() const;
		// Get private member AUTO_LOGIN
		bool get_auto_login() const;
		// Get the world to login with
		uint8_t get_auto_world();
		// Get the channel to login with
		uint8_t get_auto_channel();
		// Get the account to login with
		std::string get_auto_acc();
		// Get the password to login with
		std::string get_auto_pass();
		// Get the pic to login with
		std::string get_auto_pic();
		// Get the character id to login with
		int32_t get_auto_cid();
		// Get private member TITLE
		std::string get_title() const;
		// Get private member VERSION
		std::string get_version() const;
		// Get private member JOINLINK
		std::string get_joinlink() const;
		// Get private member WEBSITE
		std::string get_website() const;
		// Get private member FINDID
		std::string get_findid() const;
		// Get private member FINDPASS
		std::string get_findpass() const;
		// Get private member RESETPIC
		std::string get_resetpic() const;
		// Set private member MACS
		void set_macs(char* macs);
		// Set private member HWID
		void set_hwid(char* hwid, char* volumeSerialNumber);
		// Set private member MAXWIDTH
		void set_max_width(int16_t max_width);
		// Set private member MAXHEIGHT
		void set_max_height(int16_t max_height);
		// Get private member MACS
		std::string get_macs();
		// Get private member HWID
		std::string get_hwid();
		// Get the Hard Drive Volume Serial Number
		std::string get_vol_serial_num();
		// Get the max width allowed
		int16_t get_max_width();
		// Get the max height allowed
		int16_t get_max_height();
		// Get the shop's "Right-click to sell item" boolean
		bool get_rightclicksell();
		// Set the shop's "Right-click to sell item" boolean
		void set_rightclicksell(bool value);
		// Whether to show the weekly maple star in Maple Chat
		bool get_show_weekly();
		// Set whether to show the weekly maple star in Maple Chat
		void set_show_weekly(bool value);
		// Whether to show the start screen
		bool get_start_shown();
		// Set whether to show the start screen
		void set_start_shown(bool value);
		// Get the character's selected world
		uint8_t get_worldid();
		// Set the character's selected world
		void set_worldid(uint8_t id);
		// Get the character's selected channel
		uint8_t get_channelid();
		// Set the character's selected channel
		void set_channelid(uint8_t id);

		// Base class for an entry in the settings file.
		class Entry
		{
		protected:
			Entry(const char* n, const char* v) : name(n), value(v) {}

			std::string name;
			std::string value;

		private:
			friend class Configuration;

			std::string to_string() const
			{
				return name + " = " + value;
			}
		};

		// Setting which converts to a bool.
		class BoolEntry : public Entry
		{
		public:
			void save(bool b);
			bool load() const;

		protected:
			using Entry::Entry;
		};

		// Setting which uses the raw string.
		class StringEntry : public Entry
		{
		public:
			void save(std::string str);
			std::string load() const;

		protected:
			using Entry::Entry;
		};

		// Setting which converts to a Point<int16_t>.
		class PointEntry : public Entry
		{
		public:
			void save(Point<int16_t> p);
			Point<int16_t> load() const;

		protected:
			using Entry::Entry;
		};

		// Setting which converts to an integer type.
		template <class T>
		class IntegerEntry : public Entry
		{
		public:
			void save(T num)
			{
				value = std::to_string(num);
			}

			T load() const
			{
				return string_conversion::or_zero<T>(value);
			}

		protected:
			using Entry::Entry;
		};

		// Setting which converts to a byte.
		class ByteEntry : public IntegerEntry<uint8_t>
		{
		protected:
			using IntegerEntry::IntegerEntry;
		};

		// Setting which converts to a short.
		class ShortEntry : public IntegerEntry<uint16_t>
		{
		protected:
			using IntegerEntry::IntegerEntry;
		};

		// Setting which converts to an int.
		class IntEntry : public IntegerEntry<uint32_t>
		{
		protected:
			using IntegerEntry::IntegerEntry;
		};

		// Setting which converts to a long.
		class LongEntry : public IntegerEntry<uint64_t>
		{
		protected:
			using IntegerEntry::IntegerEntry;
		};

	private:
		template <typename T>
		friend struct Setting;

		const char* FILENAME = "Settings";
		const char* TITLE = "MapleStory";
		const char* VERSION = "210.1";
		const char* JOINLINK = "https://www.nexon.com/account/en/create";
		const char* WEBSITE = "http://maplestory.nexon.net/";
		const char* FINDID = "https://www.nexon.com/account/en/login";
		const char* FINDPASS = "https://www.nexon.com/account/en/reset-password";
		const char* RESETPIC = "https://www.nexon.com/account/en/login";
		const bool SHOW_FPS = false;
		const bool SHOW_PACKETS = false;
		const bool AUTO_LOGIN = false;
		const uint8_t auto_world = 0;
		const uint8_t auto_channel = 0;
		const std::string auto_acc = "";
		const std::string auto_pass = "";
		const std::string auto_pic = "";
		const int32_t auto_cid = 0;
		bool rightclicksell = false;
		bool show_weekly = true;
		bool start_shown = false;
		std::string MACS;
		std::string HWID;
		int16_t MAXWIDTH;
		int16_t MAXHEIGHT;
		std::string VolumeSerialNumber;
		uint8_t worldid;
		uint8_t channelid;
		TypeMap<Entry> settings;
	};

	// IP Address which the client will connect to.
	struct ServerIP : public Configuration::StringEntry
	{
		ServerIP() : StringEntry("ServerIP", "127.0.0.1") {}
	};

	// Port which the client will connect to.
	struct ServerPort : public Configuration::StringEntry
	{
		ServerPort() : StringEntry("ServerPort", "8484") {}
	};

	// Whether to start in fullscreen mode.
	struct Fullscreen : public Configuration::BoolEntry
	{
		Fullscreen() : BoolEntry("Fullscreen", "false") {}
	};

	// The width of the screen
	struct Width : public Configuration::ShortEntry
	{
		Width() : ShortEntry("Width", "800") {}
	};

	// The height of the screen
	struct Height : public Configuration::ShortEntry
	{
		Height() : ShortEntry("Height", "600") {}
	};

	// Whether to use VSync.
	struct VSync : public Configuration::BoolEntry
	{
		VSync() : BoolEntry("VSync", "true") {}
	};

	// The normal font which will be used.
	struct FontPathNormal : public Configuration::StringEntry
	{
		FontPathNormal() : StringEntry("FontPathNormal", "fonts/Roboto/Roboto-Regular.ttf") {}
	};

	// The bold font which will be used.
	struct FontPathBold : public Configuration::StringEntry
	{
		FontPathBold() : StringEntry("FontPathBold", "fonts/Roboto/Roboto-Bold.ttf") {}
	};

	// Music Volume, a number from 0 to 100.
	struct BGMVolume : public Configuration::ByteEntry
	{
		BGMVolume() : ByteEntry("BGMVolume", "50") {}
	};

	// Sound Volume, a number from 0 to 100.
	struct SFXVolume : public Configuration::ByteEntry
	{
		SFXVolume() : ByteEntry("SFXVolume", "50") {}
	};

	// Whether to save the last used account name.
	struct SaveLogin : public Configuration::BoolEntry
	{
		SaveLogin() : BoolEntry("SaveLogin", "false") {}
	};

	// The last used account name.
	struct DefaultAccount : public Configuration::StringEntry
	{
		DefaultAccount() : StringEntry("Account", "") {}
	};

	// The last used world.
	struct DefaultWorld : public Configuration::ByteEntry
	{
		DefaultWorld() : ByteEntry("World", "0") {}
	};

	// The last used channel.
	struct DefaultChannel : public Configuration::ByteEntry
	{
		DefaultChannel() : ByteEntry("Channel", "0") {}
	};

	// The last used region.
	struct DefaultRegion : public Configuration::ByteEntry
	{
		DefaultRegion() : ByteEntry("Region", "5") {}
	};

	// The last used character.
	struct DefaultCharacter : public Configuration::ByteEntry
	{
		DefaultCharacter() : ByteEntry("Character", "0") {}
	};

	// Whether to show the chatbar.
	struct Chatopen : public Configuration::BoolEntry
	{
		Chatopen() : BoolEntry("Chatopen", "false") {}
	};

	// The default position of the character stats inventory.
	struct PosSTATS : public Configuration::PointEntry
	{
		PosSTATS() : PointEntry("PosSTATS", "(72,72)") {}
	};

	// The default position of the equipment inventory.
	struct PosEQINV : public Configuration::PointEntry
	{
		PosEQINV() : PointEntry("PosEQINV", "(250,160)") {}
	};

	// The default position of the item inventory.
	struct PosINV : public Configuration::PointEntry
	{
		PosINV() : PointEntry("PosINV", "(300,160)") {}
	};

	// The default position of the skill inventory.
	struct PosSKILL : public Configuration::PointEntry
	{
		PosSKILL() : PointEntry("PosSKILL", "(96,96)") {}
	};

	// The default position of the quest log.
	struct PosQUEST : public Configuration::PointEntry
	{
		PosQUEST() : PointEntry("PosQUEST", "(300,160)") {}
	};

	// The default position of the world map.
	struct PosMAP : public Configuration::PointEntry
	{
		PosMAP() : PointEntry("PosMAP", "(100,35)") {}
	};

	// The default position of the userlist features.
	struct PosUSERLIST : public Configuration::PointEntry
	{
		PosUSERLIST() : PointEntry("PosUSERLIST", "(104, 104)") {}
	};

	// The default position of the chatbar.
	struct PosCHAT : public Configuration::PointEntry
	{
		PosCHAT() : PointEntry("PosCHAT", "(0, 572)") {}
	};

	// The default position of the mini map.
	struct PosMINIMAP : public Configuration::PointEntry
	{
		PosMINIMAP() : PointEntry("PosMINIMAP", "(0, 0)") {}
	};

	// The default position of shops.
	struct PosSHOP : public Configuration::PointEntry
	{
		PosSHOP() : PointEntry("PosSHOP", "(146, 48)") {}
	};

	// The default position of the notice windows.
	struct PosNOTICE : public Configuration::PointEntry
	{
		PosNOTICE() : PointEntry("PosNOTICE", "(400, 285)") {}
	};

	// The default position of the maple chat.
	struct PosMAPLECHAT : public Configuration::PointEntry
	{
		PosMAPLECHAT() : PointEntry("PosMAPLECHAT", "(50, 46)") {}
	};

	// The default position of the channel change.
	struct PosCHANNEL : public Configuration::PointEntry
	{
		PosCHANNEL() : PointEntry("PosCHANNEL", "(215, 100)") {}
	};

	// The default position of the joypad.
	struct PosJOYPAD : public Configuration::PointEntry
	{
		PosJOYPAD() : PointEntry("PosJOYPAD", "(312, 134)") {}
	};

	// The default position of the event list.
	struct PosEVENT : public Configuration::PointEntry
	{
		PosEVENT() : PointEntry("PosEVENT", "(99, 100)") {}
	};

	// The default position of the key bindings.
	struct PosKEYCONFIG : public Configuration::PointEntry
	{
		PosKEYCONFIG() : PointEntry("PosKEYCONFIG", "(65, 50)") {}
	};

	// The default position of the option menu.
	struct PosOPTIONMENU : public Configuration::PointEntry
	{
		PosOPTIONMENU() : PointEntry("PosUSERLIST", "(170, -1)") {}
	};

	struct MiniMapType : public Configuration::ByteEntry
	{
		MiniMapType() : ByteEntry("MiniMapType", "0") {}
	};

	struct MiniMapSimpleMode : public Configuration::BoolEntry
	{
		MiniMapSimpleMode() : BoolEntry("MiniMapSimpleMode", "false") {}
	};

	struct MiniMapDefaultHelpers : public Configuration::BoolEntry
	{
		MiniMapDefaultHelpers() : BoolEntry("MiniMapDefaultHelpers", "false") {}
	};

	template <typename T>
	// Can be used to access settings.
	struct Setting
	{
		// Access a setting.
		static T& get()
		{
			static_assert(std::is_base_of<Configuration::Entry, T>::value, "template parameter T for Setting must inherit from Configuration::Entry.");

			auto* entry = Configuration::get().settings.get<T>();

			if (entry)
			{
				return *entry;
			}
			else
			{
				static T defaultentry;
				return defaultentry;
			}
		}
	};
}