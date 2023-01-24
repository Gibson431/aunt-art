#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>

// Digital Pins
#define DIGITAL_SENSOR_PIN 6 // Sensor digital pin
#define MIRROR_PIN 8         // Switch pin
#define LED_PIN 7            // LED data pin
#define LED_BUILTIN 17       // Builtin LED

#define NUM_LEDS 56
#define MAX_DRAW_MA 1000 // Max power draw in mA

#define NUM_PATTERNS 2

#include "include/Fire.h"
#include "include/Orange-Pulse.h"

CRGB leds[NUM_LEDS];
Fire fire;

byte gPatternIndex = 0;

void setup()
{
    int addr = 0;
    byte lastPattern = EEPROM.read(addr);
    gPatternIndex = (lastPattern + 1) % NUM_PATTERNS;
    EEPROM.write(addr, gPatternIndex);
    
    // Power indicator
    pinMode(LED_BUILTIN, OUTPUT);

    // Set up FastLED
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(128);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_DRAW_MA);
    set_max_power_indicator_LED(LED_BUILTIN);

    delay(200); // Give the monitor time to start up
    FastLED.clear(true);
}

void loop()
{
    switch (gPatternIndex)
    {
    case 0:
        fire.runFire(leds);
        break;
    default:
        runOrangePulse(leds);
        break;
    }
    FastLED.show();
}
