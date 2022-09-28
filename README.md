# esp-iot-framework-examples
The [esp8266-iot-framework](https://github.com/maakbaas/esp8266-iot-framework) includes usage [documentation](https://github.com/maakbaas/esp8266-iot-framework/tree/master/docs) and [/examples](https://github.com/maakbaas/esp8266-iot-framework/tree/master/examples).  Please ensure you're familiar with those core examples.

This repo has additional board specific examples/experiment projects showing how https://github.com/maakbaas/esp8266-iot-framework can be used to help build firmware for various ESP8266 and ESP32 based boards.


## Setup
esp8266-iot-framework compiled library, or source code, is needed to build the examples in this repo.  Am using PlatformIO within VS Code on Windows 10.

Before going further, ensure you've read and understand the setup documentation within https://github.com/maakbaas/esp8266-iot-framework

Typically <project folder>/lib/ folder will contain project specific (private) libraries, like /lib/esp8266-iot-framework/...

- cd to your usual git/dev enlistment folder, e.g. c:\git, or some temporary scratch folder, e.g. c:\scratch 
- Clone this repo containing example projects, then clone my __ESP32__ branch of maakbaas's framework, until if/when ESP32 pull request is approved and merge into main branch.

```
git clone https://github.com/aaronse/esp-iot-framework-examples
cd esp-iot-framework-examples\helloWorld\lib
git clone --single-branch --branch esp32 https://github.com/aaronse/esp8266-iot-framework
```
- Ensure esp8266-iot-framework dependencies are installed, and that the framework UI will build and webpack ok.
```
cd esp8266-iot-framework
npm install
echo verify freshly cloned framework UI bundle builds successfully
npm run build
```

- Open helloworld app in VS Code.
```
cd ..\..
code .
```
- Use PlatformIO Build task within VS Code... Ctrl + Alt + B
  - Build should succeed, but with some warnings caused by code within referenced libraries).  Some build warning examples I observed:
```
    Transition by adding must_exist=False to SConscript calls.
    Missing SConscript 'C:\lib\esp8266-iot-framework\scripts\preBuild.py'
    File "C:\Users\aaron\.platformio\penv\lib\site-packages\platformio\builder\main.py", line 189, in <module>

    C:\Users\aaron\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-spi.c: In function 'spiTransferBytesNL':
    C:\Users\aaron\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-spi.c:922:39: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types] ...
    C:\Users\aaron\.platformio\packages\framework-arduinoespressif32\cores\esp32\esp32-hal-spi.c:923:40: warning: initialization from incompatible pointer type [-Wincompatible-pointer-types]   ...
```
- Use PlatformIO Upload task within VS Code.
- Use PlatformIO Monitor task within VS Code.
    -  [2022-02-28] Currently, for fresh device without SSID name/password configured, startup will block for 60secs during startup until connect times out.  Look at monitor output traces to observe this.  Eventually captive portal will start.  imo, cause is bug in the existing WifiManager.cpp startup logic.  Expecting startup time to be improved in the esp8266-iot-framework orthogonal to the ESP32 branch edits.
    
### Optmization for Windows Devs
2022-09-27, following no longer works, framework lib fails during compile/linking step.
~~Rather than copy snapshots of library code, Windows Devs can optionally use [NTFS links](https://en.wikipedia.org/wiki/NTFS_links#Command-line_tools_and_APIs) via mklink.exe Win OS tool to reference (via Symbolic links) an existing copy of the library sources already on their drive.  e.g. :~~

```
cd C:\git\esp-iot-framework-examples\helloWorld
mkdir lib
cd lib
mklink /j esp8266-iot-framework C:\git\esp8266-iot-framework\
```

**NOTE:**  Adjust drive/path names appropriately for your setup.  Personally, I'm experimenting with editing forked versions of the framework library, so I actually use... mklink /j esp8266-iot-framework C:\git\esp8266-iot-framework-**aaronse**\

# Coding for ESP32, or both ESP32 and ESP8266

- ESP32 build uses **"LITTLEFS.h"** not "LittleFS.h" used by ESP8266 build.  So, ensure code references the correct header, #ifdef if need to build for both MCUs.  Otherwise, you'll encounter a build failure like... _"error: 'LittleFS' was not declared in this scope"_.  See helloWorld.cpp for example usage.


# FAQ / Troubleshooting
### Firmware crashes on startup 
**problem:**
```
assertion "Invalid mbox" failed: file "/home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/lwip/lwip/src/api/tcpip.c", line 416, function: tcpip_api_call
abort() was called at PC 0x4010835f on core 1

ELF file SHA256: 0000000000000000

Backtrace: 0x400887bc:0x3ffb1e20 0x40088a39:0x3ffb1e40 0x4010835f:0x3ffb1e60 0x4013939b:0x3ffb1e90 0x40174a65:0x3ffb1ec0 0x400dc5e6:0x3ffb1f10 0x400d35fa:0x3ffb1f30 0x400d4aef:0x3ffb1f80 0x400ec756:0x3ffb1fb0 0x40089a4a:0x3ffb1fd0

Rebooting...
```
**cause:**  TCP must be initialized (via WiFiManager.begin()) before other webserver/network related code is called.  
```
    LittleFS.begin();
    GUI.begin();
    updater.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);
    timeSync.begin();
```
**fix:** Ensure WifiManager.begin is called before other network code (e.g. GUI.begin ).
```
    LittleFS.begin();
    updater.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);
    timeSync.begin();
    GUI.begin();
```
