#include "Configuration.h"

namespace wordclock
{
    Configuration config;

    void configurationSetup() {
        Serial.print("Configuration consists of ");
        Serial.print(sizeof(Configuration));
        Serial.println(" bytes");
        EEPROM.begin(sizeof(Configuration));
        loadConfiguration();
    }

    void saveConfiguration() {
        Serial.println("Need to save configuration to EEPROM");
        config.checksum = calculateConfigChecksum();
        EEPROM.put(0, config);
        EEPROM.commit();
    }

    void loadConfiguration() {
        Serial.println("Loading configuration from EEPROM");
        EEPROM.get(0, config);

        uint8_t expected_checksum = calculateConfigChecksum();
        if (config.checksum != expected_checksum) {
            Serial.print("EEPROM checksum not valid, got ");
            Serial.print(config.checksum);
            Serial.print(", expected ");
            Serial.println(expected_checksum);
            loadDefaultConfiguration();
        }
    }

    void loadDefaultConfiguration() {
        Serial.println("Loading default configuration");
        new(&config) Configuration();
    }

    uint8_t calculateConfigChecksum() {
        uint8_t previousChecksum = config.checksum;
        config.checksum = 0;
        
        unsigned char *data = (unsigned char *)&config;
        uint8_t checksum = sizeof(Configuration);
        for(int i = 0; i < sizeof(Configuration); i++) {
            checksum ^= data[i];
        }

        config.checksum = previousChecksum;
        return checksum;
    }
}