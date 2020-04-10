# HeavenClient

HeavenClient is a custom, made-from-scratch game client for Maplestory v83.

## Supported versions

- The client is currently compatible with version 83 servers.
- The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).
- There is also a Switch version available here: [HeavenClientNX](https://github.com/lain3d/HeavenClientNX).

## Configuration

The build can be configured by editing the **MapleStory.h** file. The following options are available:

- **USE_ASIO**: Use Asio for networking (additional dependency)
- **USE_CRYPTO**: Use cryptography when communicating for the server

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

## Building

*After cloning you need to check out the linux branch! To do so run ```git checkout linux``` in your HeavenClient directory.*

1. Run ```./build-deps.sh```. We try to build each dependency from source -- if any dependencies fail to build, you could try and find the corresponding package for your linux distro if it exists.
2. ```mkdir build```
3. ```cd build```
4. ```cmake ..```
5. ```make -j$CORES``` where $CORES is your number of CPU cores

## Required Files

*Always check **NxFiles.h** for an updated list of required nx files*

- [MapPretty.nx](https://drive.google.com/file/d/1d8HJkWY6ght5OAoJGtsAjNiG2BL1wcle/view?usp=sharing) (v167 GMS Map.wz)
- MapLatest.nx (Lastest GMS Map.wz)
- Map001.nx (Latest GMS Map001.wz)
- UI.nx (Latest GMS UI.wz)
- Everything else is from v83 GMS wz files

## Dependencies

- Nx library:
[NoLifeNX](https://github.com/ryantpayton/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLAD](https://github.com/Dav1dde/glad), [FreeType](http://www.freetype.org/)

- Audio:
[OpenAL-soft](https://github.com/kcat/openal-soft), [Alure](https://github.com/kcat/alure)

- Networking:
[Asio](http://think-async.com/)

---

## In-Game Issues

If you experience any kind of in-game glitches, UI rendering issues, or anything else that seems out of the ordinary that other developers are not experiences; Follow these steps in order to hopefully resolve aforementioned issues.

1. Clean Solution
2. Close Visual Studio
3. Delete the following files/folders: **.vs**, **x64**, **debug.log**, **MapleStory.aps**, **Settings**
4. Open Solution
5. Rebuild Solution

Note: These steps are not applicable to linux

---

## Vagrant setup

A [Vagrantfile](./Vagrantfile) has been included in the repo to simulate a complete HeaventClient setup on a linux ubuntu/bionic virtual machine (using virtualbox provider).

One can refer to the shell scripts written within the Vagrantfile to get an insight of the complete environmental setup and dependencies required to build and run the HeavenClient binary.

### Building/Running client via Vagrant

All ssh commands must be run on a shell within context of the HeavenClient directory (this project)

1. ```vagrant up```
2. Login to the desktop environment via the virtualbox window with default vagrant credentials (vagrant:vagrant) - once the bootstrap script has completed execution
   1. This is required to start up desktop environment (xfce)
3. ```vagrant ssh -- -R 8484:localhost:8484 -R 7575:localhost:7575 -R 7576:localhost:7576 -R 7577:localhost:7577```
   1. We SSH into the VM with reverse port-forwarding; this allows us to run/dev our [server](https://github.com/ronancpl/HeavenMS) on the host machine while allowing the client to connect to it and run on the VM
4. ```cd /home/vagrant/Desktop/HeavenClient/build/```
5. ```./HeavenClient```

Note: To run the HeavenClient from the build directory; all the [relevant **.nx** files](./Util/NxFiles.h) must be available within the linux build directory

Tip: Since the binary is built on the VM with a mounted/shared volume; the binary is also available/usable by a linux host

---

## Donations

If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.
