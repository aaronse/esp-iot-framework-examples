#include <Arduino.h>

#ifdef ESP32
    #include "LITTLEFS.h"   // https://github.com/lorol/LITTLEFS/tree/master/examples/LITTLEFS_PlatformIO
    #define LittleFS LITTLEFS
#elif defined(ESP8266)
    #include "LittleFS.h"
#endif

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"

void setup() 
{
    Serial.begin(115200);

    LittleFS.begin();
    updater.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);
    timeSync.begin();
    GUI.begin();

    Serial.println("Hello world, setup");
}

void loop() 
{
    int i = 0;

    while(true)
    {
        //software interrupts
        WiFiManager.loop();
        updater.loop();
        configManager.loop();

        //your code here
        Serial.print("Hello world, loop ");
        Serial.println(i++);
        delay(1000);
    }
}
