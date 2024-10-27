// File inspired by code from bitlair: https://github.com/bitlair/wordclock
#include "Arduino.h"
#include <EEPROM.h>

namespace wordclock
{

  //! @brief The Brightness Mode
  enum BrightnessMode : uint8_t 
  {
    fixedBrightness   = 1,    //!< Fixed brightness
    timeBrightness    = 2     //!< Adjusting brightness to the hour of the day
  };

  //! @brief The Configuration data
  struct Configuration_v1
  {
    static const int MAX_HOSTNAME_LENGTH = 64;  //!< The maximum length of the host name
    
    uint8_t version;
    uint8_t structSize; 
    char    hostname[64];
    char    password[64];
    uint8_t brightnessMode;
    uint8_t maxBrightness;
    uint8_t minBrightness;
    uint8_t brightnessStartHour;
    uint8_t brightnessEndHour;
    uint8_t checksum;

    Configuration_v1()
    : version(1)
    , structSize(sizeof(Configuration_v1))
    , brightnessMode(timeBrightness)
    , maxBrightness(255)
    , minBrightness(65)
    , brightnessStartHour(8)
    , brightnessEndHour(22)
    {
        char default_name[] = "wordclock";
        char default_passwd[] = "MywWordclock!";
        memcpy(hostname, default_name, sizeof(default_name));
        memcpy(password, default_passwd, sizeof(default_passwd));
    }

    void toSerial()
    {
      Serial.print("version               ");
      Serial.println(version);
      Serial.print("structSize            ");
      Serial.println(structSize);
      Serial.print("hostname              ");
      Serial.println(hostname);
      Serial.print("password              ");
      Serial.println(password);
      Serial.print("brightnessMode        ");
      Serial.println(brightnessMode);
      Serial.print("maxBrightness         ");
      Serial.println(maxBrightness);
      Serial.print("minBrightness         ");
      Serial.println(minBrightness);
      Serial.print("brightnessStartHour   ");
      Serial.println(brightnessStartHour);
      Serial.print("brightnessEndHour     ");
      Serial.println(brightnessEndHour);
      Serial.print("checksum              ");
      Serial.println(checksum);
    }
  };

  typedef Configuration_v1 Configuration;

  extern Configuration config;

  void configurationSetup();

  void saveConfiguration();

  void loadConfiguration();

  void loadDefaultConfiguration();

  uint8_t calculateConfigChecksum();
} // namespace Wordclock closed