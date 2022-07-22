#include <Arduino.h>
#include <FastLED.h>

void runOrangePulse(CRGB ledArr[])
{
    fill_solid(ledArr, NUM_LEDS, CHSV(beatsin8(10, 16, 24), 255, beatsin8(10, 192, 255)));
}