/* ****************************************************************************
 * Modified fire effect from Dave's Garage
 * https://github.com/davepl/DavesGarageLEDSeries/blob/master/LED%20Episode%2010/include/fire.h
 * ***************************************************************************/
#include <Arduino.h>
#include <FastLED.h>
#include "ledgfx.h"

DEFINE_GRADIENT_PALETTE(heatmap_gp){
    0, 0, 0, 0,          // black
    32, 0, 0, 0,         // black
    48, 255, 0, 0,       // red
    64, 200, 60, 0,      // orange?
    200, 255, 100, 0,    // bright yellow
    255, 255, 100, 255}; // full white

class Fire
{
protected:
    float *heatMapLeft;
    float *heatMapRight;
    double LastDraw; // Last time we drew the flame

    // const float IGNITION_KNOB = 50.0f;   // Preference-based constant factor for ignition rate
    const float SPREADRATE_KNOB = 12.0f; // Preference-based constant for flame spread rate
    const float SPARKRATE_KNOB = 5.0f;   // Preference-based constant for spark ignition rate
    const int Size = NUM_LEDS / 2 + 4;

public:
    float Cooling = 0.2f;
    int SparkHeight = 3;
    float SparkProbability = 1.0 * SPARKRATE_KNOB / SparkHeight;
    float SpreadRate = 1.0 * SPREADRATE_KNOB;
    CRGBPalette16 HeatMap_gp = heatmap_gp;

    Fire()
    {
        heatMapLeft = new float[Size]{0};
        heatMapRight = new float[Size]{0};
    };

    virtual ~Fire()
    {
        delete[] heatMapLeft;
        delete[] heatMapRight;
    };

    void runFire(CRGB ledArr[])
    {
        FastLED.clear();

        float elapsedSeconds = UnixTime() - LastDraw;
        float cooldown = 1.0f * RandomFloat() * Cooling * elapsedSeconds;
        LastDraw = UnixTime();

        for (int i = 0; i < (Size); i++)
        {
            heatMapLeft[i] = max(0.0f, heatMapLeft[i] - cooldown); // Cool cell by cooldown amount, but don't go below zero

            int neighborIndex = (i == 0) ? (Size)-1 : i - 1; // Index of cell to our left, wrapping around to front
            float spreadAmount = min(1.0f, heatMapLeft[neighborIndex]) * SpreadRate * elapsedSeconds;
            spreadAmount = min(heatMapLeft[neighborIndex], spreadAmount);
            heatMapLeft[i] += spreadAmount; // Exchange 'spreadAmount' of heat between cells
            heatMapLeft[neighborIndex] -= spreadAmount;

            // Check to see if this cell ignites a new spark
            if (i <= SparkHeight && RandomFloat() < SparkProbability * elapsedSeconds)
            {
                // heatMapLeft[i] = heatMapLeft[i] + RandomFloat() * 30 * elapsedSeconds;
                heatMapLeft[i] = 0.75f; // min(1.0f, (heatMapLeft[i] + RandomFloat() * 30 * elapsedSeconds));
                // printf("Spark at %d: %f", i, heatMapLeft[i]);
            }
        }

        for (int i = 0; i < (Size); i++)
        {
            heatMapRight[i] = max(0.0f, heatMapRight[i] - cooldown); // Cool cell by cooldown amount, but don't go below zero

            int neighborIndex = (i == 0) ? (Size)-1 : i - 1; // Index of cell to our left, wrapping around to front
            float spreadAmount = min(1.0f, heatMapRight[neighborIndex]) * SpreadRate * elapsedSeconds;
            spreadAmount = min(heatMapRight[neighborIndex], spreadAmount);
            heatMapRight[i] += spreadAmount; // Exchange 'spreadAmount' of heat between cells
            heatMapRight[neighborIndex] -= spreadAmount;

            // Check to see if this cell ignites a new spark
            if (i <= SparkHeight && RandomFloat() < SparkProbability * elapsedSeconds)
            {
                // heatMapRight[i] = heatMapRight[i] + RandomFloat() * 30 * elapsedSeconds;
                heatMapRight[i] = 0.75; // min(1.0f, (heatMapRight[i] + RandomFloat() * 30 * elapsedSeconds));
                // printf("Spark at %d: %f", i, heatMapRight[i]);
            }
        }

        for (int i = 0; i < NUM_LEDS / 2; i++)
        {
            ledArr[i] = ColorFromPalette(HeatMap_gp, 240 * min(1.0f, heatMapLeft[i + 4]));
            ledArr[NUM_LEDS - 1 - i] = ColorFromPalette(HeatMap_gp, 240 * min(1.0f, heatMapRight[i + 4]));
        }
    };
};