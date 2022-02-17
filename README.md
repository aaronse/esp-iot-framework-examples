# esp-iot-framework-examples
The [esp8266-iot-framework](https://github.com/maakbaas/esp8266-iot-framework) includes usage documentation and [/examples](https://github.com/maakbaas/esp8266-iot-framework/tree/master/examples).

This repo has additional board specific examples/experiment projects showing how https://github.com/maakbaas/esp8266-iot-framework can be used to help build firmware for ESP8266 and ESP32 MCUs.

https://github.com/maakbaas/esp8266-iot-framework/tree/master/examples

https://github.com/maakbaas/esp8266-iot-framework

## Setup
esp8266-iot-framework compiled library, or source code, is needed to build the examples.  Am using PlatformIO within VS Code on Windows 10.

Ensure you've read and understand the setup documentation within https://github.com/maakbaas/esp8266-iot-framework

Typically <project folder>/lib/ folder will contain project specific (private) libraries, like /lib/esp8266-iot-framework/...

### Optmization for Windows Devs
However, Windows Devs can optionally use [NTFS links](https://en.wikipedia.org/wiki/NTFS_links#Command-line_tools_and_APIs) to reference an existing copy of the library sources already on their drive.  Rather than copy snapshots of library code, instead create NTFS link to reference library sources via mklink, e.g. :

```
cd C:\git\esp-iot-framework-examples\helloWorld
mkdir lib
cd lib
mklink /j esp8266-iot-framework C:\git\esp8266-iot-framework\
```

**NOTE:**  Adjust drive/path names appropriately for your setup.  Personally, I'm experimenting with editing forked versions of the framework library, so I actually use... mklink /j esp8266-iot-framework C:\git\esp8266-iot-framework-**aaronse**\

# Coding for ESP32, or both ESP32 and ESP8266

- ESP32 build uses **"LITTLEFS.h"** not "LittleFS.h" used by ESP8366 build.  So, ensure code references the correct header, #ifdef if need to build for both MCUs.  Otherwise, you'll encounter a build failure like... _"error: 'LittleFS' was not declared in this scope"_.  See helloWorld.cpp for example usage.


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
