#include <Arduino.h>
#include <FastLED.h>
#include <OneButton.h>

// Digital Pins
#define DIGITAL_SENSOR_PIN 6 // Sensor digital pin
#define MIRROR_PIN 8         // Switch pin
#define LED_PIN 7            // LED data pin
#define LED_BUILTIN 17       // Builtin LED

#define NUM_LEDS 56
#define MAX_DRAW_MA 1000 // Max power draw in mA

#define NUM_PATTERNS 5

#include "include/Fire.h"
#include "include/Orange-Pulse.h"

CRGB leds[NUM_LEDS];
Fire fire;

byte gPatternIndex = 0;

void setup()
{
    // Power indicator
    pinMode(LED_BUILTIN, OUTPUT);

    // Set up FastLED
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(128);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_DRAW_MA);
    set_max_power_indicator_LED(LED_BUILTIN);


    delay(200); // Give the monitor time to start up
    gPatternIndex = analogRead(A0) % 2;
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
    // default:
    //     fill_solid(leds, NUM_LEDS, CHSV(beatsin8(30,0,255), 255, 0));
    //     break;
    }
    FastLED.show();
}