#include <Arduino.h>

#ifdef ESP32
    #include "LITTLEFS.h"   // https://github.com/lorol/LITTLEFS/tree/master/examples/LITTLEFS_PlatformIO
    #define LittleFS LITTLEFS
#elif defined(ESP8266)
    #include "LittleFS.h"
    #include <TZ.h>
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

    // Configure device time using NTP server, with Amsterdam timezone
#ifdef ESP32
    // ESP32 sntp APIs expect posix based time zone expressions, see https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
    timeSync.begin(PSTR("CET-1CEST,M3.5.0,M10.5.0/3"), "pool.ntp.org");
#elif defined(ESP8266)
    timeSync.begin(TZ_Europe_Amsterdam);
#endif

#ifdef ESP32
    // Two example approaches for looking up current time... 

    // Example 1, fill tm struct using getLocalTime.  Accessing time parts explicitly.
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "Current time in Amsterdam: %A, %B %d %Y %H:%M:%S");

    // Example 2, use asctime to serialize local time to string
    time_t now = time(nullptr);
    Serial.print(PSTR("Current time in Amsterdam: "));
    Serial.print(asctime(localtime(&now)));
    

#elif defined(ESP8266)
    //Wait for connection
    timeSync.waitForSyncResult(10000);

    if (timeSync.isSynced())
    {
        time_t now = time(nullptr);
        Serial.print(PSTR("Current time in Amsterdam: "));
        Serial.print(asctime(localtime(&now)));
    }
    else 
    {
        Serial.println("Timeout while receiving the time");
    }

#endif
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
