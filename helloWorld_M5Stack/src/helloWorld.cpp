// HelloWorld for M5Stack board 
// - Based on HelloWorld examples within esp8266-iot-framework and M5Stack.  POC to verify ESP32
//   build of esp8266-iot-framework on M5Stack board.

// RESOURCES:
// - Docs/Specs/Pinout @ https://docs.m5stack.com/en/core/gray
// - Buy @ https://shop.m5stack.com/products/face

// NOTE: Alternative (easier) Dev options:
// - UI Flow, see https://flow.m5stack.com/
// - Micropython, see https://micropython.org/


#include <M5Stack.h>
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

    // M5Stack initialization
    M5.begin();
    M5.Power.begin();   //Init Power module.
                        /* Power chip connected to gpio21, gpio22, I2C device
                        Set battery charging voltage and current
                        If used battery, please call this function in your project */

    Serial.println("Setup begin...");
    M5.Lcd.println("Setup begin...");  // Print text on the screen (string)

    // ESP8266-iot-framework intialization.  
    // Note:  If Wifi not already configured, i.e. first boot, then WifiManager will block setup() 
    // until if/when User visits captive portal, or 60sec timeout expires.
    LittleFS.begin();
    updater.begin();
    configManager.begin();
    M5.Lcd.println("WiFiManager.begin(), calling...");
    WiFiManager.begin(configManager.data.projectName);
    M5.Lcd.println("WiFiManager.begin(), done");
    timeSync.begin();
    GUI.begin();

    Serial.println("Setup done");
    M5.Lcd.println("Setup done");  // Print text on the screen (string)
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
        Serial.print(i);
        Serial.print(" Loop: ");
        Serial.println(configManager.data.message);

        M5.Lcd.print(i);
        M5.Lcd.print(" Loop: ");
        M5.Lcd.println(configManager.data.message);

        delay(1000);
        i++;
    }
}
