#include "Arduino.h"
#include <Wire.h>
#include <RTClib.h>

namespace wordclock
{
    class RealTimeClock
    {
    public:
        RealTimeClock();

        bool init();

        bool hasNewTime();

        bool setDateTime(
            uint16_t year,
            uint8_t month,
            uint8_t day,
            uint8_t hour,
            uint8_t minute
        );

        DateTime now();

        void toSerial(DateTime time) const;

    private:
        RTC_DS3231  m_rtc;
        uint16_t    m_min;
        uint16_t    m_hour;
        bool        m_isInitialized;
    };

    extern RealTimeClock rtc;
}