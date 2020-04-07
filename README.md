# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Supported versions
- The client is currently compatible with version 83 servers.
- The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).
- There is also a Switch version available here: [HeavenClientNX](https://github.com/lain3d/HeavenClientNX).

# Configuration
The build can be configured by editing the **MapleStory.h** file. The following options are available:
- **USE_ASIO**: Use Asio for networking (additional dependency)
- **USE_CRYPTO**: Use cryptography when communicating for the server.

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Building
1. Run ```./build-deps.sh```. We try to build each dependency from source -- if any dependencies fail to build, you could try and find the corresponding package for your linux distro if it exists.
2. ```mkdir build```
3. ```cd build```
4. ```cmake ..```
5. ```make -j$CORES``` where $CORES is your number of CPU cores

# Required Files
*Always check **NxFiles.h** for an updated list of required nx files*
- [MapPretty.nx](https://drive.google.com/file/d/1d8HJkWY6ght5OAoJGtsAjNiG2BL1wcle/view?usp=sharing) (v167 GMS Map.wz)
- MapLatest.nx (Lastest GMS Map.wz)
- Map001.nx (Latest GMS Map001.wz)
- UI.nx (Latest GMS UI.wz)
- Everything else is from v83 GMS wz files

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/ryantpayton/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLAD](https://github.com/Dav1dde/glad), [FreeType](http://www.freetype.org/)

- Audio:
[OpenAL-soft](https://github.com/kcat/openal-soft)
[Alure](https://github.com/kcat/alure)

- Networking:
[Asio](http://think-async.com/) (optional)

# In-Game Issues
If you experience any kind of in-game glitches, UI rendering issues, or anything else that seems out of the ordinary that other developers are not experiences; Follow these steps in order to hopefully resolve aforementioned issues.
1. Clean Solution
2. Close Visual Studio
3. Delete the following files/folders: **.vs**, **x64**, **debug.log**, **MapleStory.aps**, **Settings**
4. Open Solution
5. Rebuild Solution

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.
