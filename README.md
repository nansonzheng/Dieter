# Dieter
Dieter is a fork of HeavenClient, a custom, made-from-scratch game client. Dieter is to be used as the client for [DietStory](https://github.com/BenjixD/MapleSolaxiaV2).

# Supported versions
The client is currently compatible with **version 83** servers.
The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).
For the **UI.wz** and **Map001.wz**, the latest version (**v208.3** as of writing this README) should be used. Older versions, starting from at least v154, may cause the client to not function properly.

# Configuration
<del>The build can be configured by editing the **Journey.h** file. The following options are available:
- **JOURNEY_USE_CRYPTO**: Use cryptography when communicating for the server.
- **JOURNEY_USE_XXHASH** : Use xxhash for file check (additional dependency)
- **JOURNEY_USE_ASIO**: Use Asio for networking (additional dependency)

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.</del>

**Journey.h** does not exist in the current repository. For the time being, settings such as server connection can be configured in **Configuration.h**, and security settings listed about can be found in **MapleStory.h**.

# Building
1. Open **MapleStory.sln** in Visual Studio 2017 CE
2. Make sure to use **Windows SDK Version: 8.1** and **Platform Toolset: v140** (If you don't have these, download them)
   * [Windows 8.1 SDK](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)
3. Press **Build** > **Build Solution** or **Ctrl + Shift + B**
4. Acquire the needed **.nx** files by converting **.wz** files and put them in the root folder
	- Use [NoLifeStory.zip](https://drive.google.com/open?id=1Mk3Kq1lY4NTMqylN5sn0-DQNAcoZZRYH) to convert the **.wz** files to **.nx** files. 
		- Extract the zip to anywhere
		- Put the **.wz** files in the **files** folder and run start.bat
		- The **.nx** files will be in the same directory as the **.wz** files
	- For a list of files required, navigate to **NxFiles.h**
4. After a successful build, you can now run the program by pressing **Debug** > **Start Debugging** or **F5**

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLEW](http://glew.sourceforge.net/), [FreeType](http://www.freetype.org/)

- Audio:
[Bass](http://www.un4seen.com/)

- Utility:
[xxHash](https://github.com/Cyan4973/xxHash) (optional)

- Networking:
[Asio](http://think-async.com/) (optional)

# Binaries (08.19.2019)
The latest build ([ddd67c8](https://github.com/ryantpayton/HeavenClient/commit/ddd67c8693ef2ad352e8c2dc1e4628e625c49953)) can be found here: [HeavenClient.zip](https://drive.google.com/open?id=186fBZWcuK5uSB3CN6jeTJ_AdU6tK82Ey)

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.