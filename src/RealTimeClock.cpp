#include "RealTimeClock.h"
#include "Wire.h"

namespace wordclock
{
    RealTimeClock::RealTimeClock()
    : m_min(0)
    , m_hour(0)
    , m_isInitialized(false)
    {}

    bool RealTimeClock::init()
    {
        Serial.println("Initializing RealTimeClock");
        Wire.begin();
        Wire.status();
        m_isInitialized = m_rtc.begin();
        if (m_isInitialized)
        {
            DateTime now = m_rtc.now();
            m_hour = now.hour();
            m_min = now.minute();
            Serial.print(now.year(), DEC);
            Serial.print('/');
            Serial.print(now.month(), DEC);
            Serial.print('/');
            Serial.print(now.day(), DEC);
            Serial.print(" ");
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.println();

        }
        return true;
    }

    void RealTimeClock::toSerial(DateTime time ) const
    {
            Serial.print(time.year(), DEC);
            Serial.print('/');
            Serial.print(time.month(), DEC);
            Serial.print('/');
            Serial.print(time.day(), DEC);
            Serial.print(" ");
            Serial.print(time.hour(), DEC);
            Serial.print(':');
            Serial.print(time.minute(), DEC);
            Serial.println();
    }

    bool RealTimeClock::hasNewTime()
    {
        bool retValue = false;
        if (m_isInitialized)
        {
            DateTime now = m_rtc.now();
            if ((now.hour() != m_hour) || (now.minute() != m_min))
            {
                m_hour = now.hour();
                m_min = now.minute();
                retValue = true;
            }
        }
        return retValue;
    }

    DateTime RealTimeClock::now()
    {
        if (m_isInitialized)
        {
            return m_rtc.now();
        }
        return DateTime();
    }

    bool RealTimeClock::setDateTime(
        uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute
    )
    {
        bool retValue = true;
        Serial.println("Trying to adjust time:");
        Serial.print(year, DEC);
        Serial.print('/');
        Serial.print(month, DEC);
        Serial.print('/');
        Serial.print(day, DEC);
        Serial.print(hour, DEC);
        Serial.print(':');
        Serial.print(minute, DEC);
        Serial.println();
        if (m_isInitialized)
        {
            DateTime now = m_rtc.now();
            if (year == 0)
            {
                year = now.year();
            }
            if (month == 0)
            {
                month = now.month();
            }
            if (day == 0)
            {
                day = now.day();
            }
            if (hour >= 24)
            {
                hour = now.hour();
            }
            if (minute >= 60)
            {
                minute = now.minute();
            }
            if ((hour   < 24)       &&
                (minute < 60)       &&
                (year   > 2000)     &&
                (month  > 0)        && 
                (month  < 13)       &&
                (day    > 0)        &&
                (day    < 32))
            {
                m_rtc.adjust(DateTime(year, month, day, hour, minute ));
                Serial.println("RTC adjusted");
            }
        }
        return retValue;
    }

    RealTimeClock rtc;
}