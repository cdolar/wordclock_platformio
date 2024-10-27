#include "Wordclock.h"
#include "RealTimeClock.h"
#include "Configuration.h"
#include "Webserver.h"

#include <NeoPixelBus.h>

typedef LedMatrixConfig_DE_11x10_reversed       led_matrix_config_t;
typedef MatrixEmulator<led_matrix_config_t>     matrixEmu_t;
typedef Wordclock<led_matrix_config_t>          wordclock_t;

wordclock_t mywordclock;
matrixEmu_t matrixEmu;

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> matrix(wordclock_t::SIZE);

using namespace wordclock;

void showCurrentTime()
{
    matrix.ClearTo(RgbColor(0));
    matrixEmu.clear();
    DateTime now = rtc.now();
    uint8_t colorSaturation = config.maxBrightness;
    if (config.brightnessMode == timeBrightness) 
    {
        if ((now.hour() < config.brightnessStartHour) || (now.hour() >= config.brightnessEndHour))
        {
            colorSaturation = config.minBrightness;
        }
    }
    RgbColor ledColor(colorSaturation);
    mywordclock.setTime(now.hour(), now.minute(), matrixEmu, ledColor);
    mywordclock.setTime(now.hour(), now.minute(), matrix, ledColor);
    matrix.Show();
    matrixEmu.print();
}

void 

void setup()
{
    Serial.begin(115200);
    delay(3000); // Wait on terminal

    Serial.println("Restoring configuration from flash");
    configurationSetup();
    Serial.println("The Configuration looks like this:");
    config.toSerial();

    Serial.println("Setting up RealTimeClock");
    if (rtc.init())
    {
        Serial.println("...successful");
    }
    else
    {
        Serial.println("...not successful");
    }

    Serial.println("Setting up Webserver");
    webserver.init();

    // initializing led matrix and run a short test
    uint8_t colorSaturation = config.maxBrightness;
    RgbColor red(colorSaturation, 0, 0);
    RgbColor green(0, colorSaturation, 0);
    RgbColor blue(0, 0, colorSaturation);
    RgbColor white(colorSaturation);
    RgbColor black(0);

    matrix.Begin();
    while(!matrix.CanShow()){}
    matrix.Show();
    matrix.SetPixelColor(0,red);
    matrix.SetPixelColor(1,green);
    matrix.SetPixelColor(2,blue);
    matrix.SetPixelColor(3,white);
    while(!matrix.CanShow()){}
    matrix.Show();
    for (int i=0; i<wordclock_t::SIZE; ++i)
    {
        delay(300);
        matrix.RotateRight(1);
        while(!matrix.CanShow()){}
        matrix.Show();
    }

    showCurrentTime();
}


void loop()
{
    webserver.loop();

    if (rtc.hasNewTime())
    {
        showCurrentTime();
    }
}