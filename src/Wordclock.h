#ifndef WORDCLOCK_H_INCLUDED
#define WORDCLOCK_H_INCLUDED

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>

template <int X, int Y, int M=4>
class TLedMatrix
{
public:
    static const int SIZE_X = X;
    static const int SIZE_Y = Y;
    static const int NUM_MINS = M;
    static const int SIZE = SIZE_X * SIZE_Y + NUM_MINS;

    TLedMatrix()
    {
        clear();
    }

    TLedMatrix(const TLedMatrix<X,Y>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] = f_other.m_ledState[n];
        }
    }

    void clear()
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n]=0u;
        }
    }

    void setLeds(int numActiveLeds, ...)
    {
        va_list leds;
        va_start(leds, numActiveLeds);
        for (int n=0; n<numActiveLeds; ++n)
        {
            int idx = va_arg(leds, int);
            if ((idx >= 0) && (idx < SIZE))
            {
                m_ledState[idx] = 1u;
            }
        } 
        va_end(leds);     
    }

    bool isLedActive(int idx)
    {
        if ((idx >=0) && (idx < SIZE))
        {
            return (m_ledState[idx] > 0);
        }
        return false;
    }

    TLedMatrix<X,Y,M>& operator= (const TLedMatrix<X,Y,M>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] = f_other.m_ledState[n];
        }
        return *this;
    }

    TLedMatrix<X,Y,M>& operator+= (const TLedMatrix<X,Y,M>& f_other)
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledState[n] += f_other.m_ledState[n];
        }
        return *this;
    }

protected:
    uint8_t m_ledState[SIZE];
};

//---------------------------------------------------------------------------------------------------------------------

template <class LED_MATRIX_CONFIG>
class Wordclock
{
public:
    typedef LED_MATRIX_CONFIG led_matrix_config_t;
    typedef typename led_matrix_config_t::led_matrix_t led_matrix_t;

    static const int SIZE = led_matrix_config_t::SIZE;

    Wordclock()
    {
    }

    const led_matrix_t& getLedMatrix() const;

    template <class LedMatrixCtrl, class Color>
    void setTime(uint8_t hour, uint8_t minutes, LedMatrixCtrl& matrixCtrl, Color color)
    {
        led_matrix_config_t::setTime(hour, minutes, m_ledMatrix);
        for (int n=0; n<led_matrix_t::SIZE; ++n)
        {
            if (m_ledMatrix.isLedActive(n))
            {
                matrixCtrl.SetPixelColor(n, color);
            }
        }
        // set the four corner minute LEDs
    }

protected:
    led_matrix_t m_ledMatrix;
};


//---------------------------------------------------------------------------------------------------------------------

class LedMatrixConfig_DE_11x10
{
public:
    typedef TLedMatrix<11,10,4> led_matrix_t;

    static const char *MatrixLetters;
    static const int  MatrixLedPositions[led_matrix_t::SIZE];

    static void setTime(uint8_t hour, uint8_t minutes, led_matrix_t& matrix) 
    {
        matrix.clear();
        matrix.setLeds(5,0,1,3,4,5); // es ist
        
        // set minutes and increment the hour if needed
        const uint8_t validMinutes = minutes % 60;
        const uint8_t minutesBy5 = (minutes / 5) * 5;
        bool fullHour = false;
        switch (minutesBy5)
        {
            case 0 : matrix.setLeds(3,107,108,109); fullHour=true; break;
            case 5 : matrix.setLeds(8,7,8,9,10,40,41,42,43); break;
            case 10: matrix.setLeds(8,11,12,13,14,40,41,42,43); break;
            case 15: matrix.setLeds(11,26,27,28,29,30,31,32,40,41,42,43); break;
            case 20: matrix.setLeds(11,15,16,17,18,19,20,21,40,41,42,43); break;
            case 25: matrix.setLeds(11,7,8,9,10,33,34,35,44,45,46,47); hour++; break;
            case 30: matrix.setLeds(4,44,45,46,47); hour++; break;
            case 35: matrix.setLeds(12,7,8,9,10,40,41,42,43,44,45,46,47); hour++; break;
            case 40: matrix.setLeds(10,15,16,17,18,19,20,21,33,34,35); hour++; break;
            case 45: matrix.setLeds(10,26,27,28,29,30,31,32,33,34,35); hour++; break;
            case 50: matrix.setLeds(7,11,12,13,14,33,34,35); hour++; break;
            case 55: matrix.setLeds(7,7,8,9,10,33,34,35); hour++; break;
        }

        const uint8_t remainingMinutes = validMinutes - minutesBy5;
        switch (remainingMinutes)
        {
            case 1: matrix.setLeds(1,110); break;
            case 2: matrix.setLeds(2,110,111); break;
            case 3: matrix.setLeds(3,110,111,112); break;
            case 4: matrix.setLeds(4,110,111,112,113); break;
            default: break;
        }


        const uint8_t hour12 = hour % 12;
        switch (hour12)
        {
            case 0: matrix.setLeds(5,94,95,96,97,98); break;
            case 1: fullHour ? matrix.setLeds(3,55,56,57) : matrix.setLeds(4,55,56,57,58); break;
            case 2: matrix.setLeds(4,62,63,64,65); break;
            case 3: matrix.setLeds(4,66,67,68,69); break;
            case 4: matrix.setLeds(4,73,74,75,76); break;
            case 5: matrix.setLeds(4,51,52,53,54); break;
            case 6: matrix.setLeds(5,77,78,79,80,81); break;
            case 7: matrix.setLeds(6,88,89,90,91,92,93); break;
            case 8: matrix.setLeds(4,84,85,86,87); break;
            case 9: matrix.setLeds(4,102,103,104,105); break;
            case 10: matrix.setLeds(4,99,100,101,102); break;
            case 11: matrix.setLeds(3,49,50,51); break;
        }
    }
};

const char* LedMatrixConfig_DE_11x10::MatrixLetters = "ESKISTAFUNFZEHNZWANZIGDREIVIERTELVORMCLONACHHALBDELFUNFEINSOAMZWEIDREILARVIERSECHSNLACHTSIEBENZWOLFZEHNEUNKUHR....";
const int   LedMatrixConfig_DE_11x10::MatrixLedPositions[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
    66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
    88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    99,100,101,102,103,104,105,106,1070,108,109,
    110,111,112,113
};

//---------------------------------------------------------------------------------------------------------------------

class LedMatrixConfig_DE_11x10_reversed
{
public:
    typedef TLedMatrix<11,10,4> led_matrix_t;

    static const int SIZE = led_matrix_t::SIZE;

    static const char *MatrixLetters;
    static const int  MatrixLedPositions[led_matrix_t::SIZE];


    static void setTime(uint8_t hour, uint8_t minutes, led_matrix_t& matrix) 
    {
        matrix.clear();
        matrix.setLeds(5,102,103,105,106,107); // es ist
        
        // set minutes and increment the hour if needed
        const uint8_t validMinutes = minutes % 60;
        const uint8_t minutesBy5 = (validMinutes / 5) * 5;
        bool fullHour = false;
        switch (minutesBy5)
        {
            case 0 : matrix.setLeds(3,1,2,3); fullHour=true; break; //uhr
            case 5 : matrix.setLeds(8,109,110,111,112,71,70,69,68); break; //fünf nach
            case 10: matrix.setLeds(8,100,99,98,97,71,70,69,68); break; // zehn nach
            case 15: matrix.setLeds(11,83,84,85,86,87,88,89,71,70,69,68); break; // viertel nach
            case 20: matrix.setLeds(11,96,95,94,93,92,91,90,71,70,69,68); break; //zwanzig nach
            case 25: matrix.setLeds(11,109,110,111,112,78,77,76,57,58,59,60); hour++; break; // fünf vor halb
            case 30: matrix.setLeds(4,57,58,59,60); hour++; break; //halb
            case 35: matrix.setLeds(12,109,110,111,112,71,70,69,68,57,58,59,60); hour++; break; // fünf nach halb
            case 40: matrix.setLeds(10,96,95,94,93,92,91,90,78,77,76); hour++; break; // zwanzig vor
            case 45: matrix.setLeds(10,83,84,85,86,87,88,89,78,77,76); hour++; break; // viertel vor
            case 50: matrix.setLeds(7,100,99,98,97,78,77,76); hour++; break; // zehn vor
            case 55: matrix.setLeds(7,109,110,111,112,78,77,76); hour++; break; // fünf vor
        }

        const uint8_t remainingMinutes = validMinutes - minutesBy5;
        switch (remainingMinutes)
        {
            case 1: matrix.setLeds(1,101); break;
            case 2: matrix.setLeds(2,101,113); break;
            case 3: matrix.setLeds(3,101,113,0); break;
            case 4: matrix.setLeds(4,101,113,0,12); break;
            default: break;
        }

        const uint8_t hour12 = hour % 12;
        switch (hour12)
        {
            case 0: matrix.setLeds(5,19,20,21,22,23); break; // zwölf
            case 1: fullHour ? matrix.setLeds(3,56,55,54) : matrix.setLeds(4,56,55,54,53); break; // ein / eins
            case 2: matrix.setLeds(4,49,48,47,46); break; // zwei
            case 3: matrix.setLeds(4,35,36,37,38); break; // drei
            case 4: matrix.setLeds(4,42,43,44,45); break; // vier
            case 5: matrix.setLeds(4,64,65,66,67); break; // fünf
            case 6: matrix.setLeds(5,34,33,32,31,30); break; // sechs
            case 7: matrix.setLeds(6,13,14,15,16,17,18); break; // sieben
            case 8: matrix.setLeds(4,27,26,25,24); break; // acht
            case 9: matrix.setLeds(4,8,7,6,5); break; // neun
            case 10: matrix.setLeds(4,11,10,9,8); break; // zehn
            case 11: matrix.setLeds(3,62,63,64); break; // elf
        }
    }
};

const char* LedMatrixConfig_DE_11x10_reversed::MatrixLetters = "ESKISTAFUNFZEHNZWANZIGDREIVIERTELVORMCLONACHHALBDELFUNFEINSOAMZWEIDREILARVIERSECHSNLACHTSIEBENZWOLFZEHNEUNKUHR....";
const int   LedMatrixConfig_DE_11x10_reversed::MatrixLedPositions[] = {
    113, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 112,
    88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77,
    66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
    110, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 111
};


//---------------------------------------------------------------------------------------------------------------------
template<class LED_MATRIX_CONFIG>
class MatrixEmulator
{
public:
    MatrixEmulator()
    {
    }

    static const int SIZE = LED_MATRIX_CONFIG::SIZE;

    template <class color_t>
    void SetPixelColor(int idx, color_t c)
    {
        if ((idx >=0) && (idx < SIZE))
        {
            int pos = LED_MATRIX_CONFIG::MatrixLedPositions[idx];
            if ((pos >= 0) && (pos < SIZE))
                m_ledText[pos] = LED_MATRIX_CONFIG::MatrixLetters[pos];
        }
    }

    void print()
    {
        for (int n=0; n<SIZE; ++n)
        {
            Serial.print(m_ledText[n]);
        }
        Serial.println("");
        Serial.flush();
    }

    void clear()
    {
        for (int n=0; n<SIZE; ++n)
        {
            m_ledText[n]=' ';
        }
    }

private:
    char m_ledText[SIZE];
};


#endif // WORDCLOCK_H_INCLUDED