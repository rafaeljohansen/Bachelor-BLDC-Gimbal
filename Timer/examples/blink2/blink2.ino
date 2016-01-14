//Flash two LEDs at different rates using Simon Monk's Timer library
//http://www.doctormonk.com/2012/01/arduino-timer-library.html
//
//Jack Christensen 30Sep2013
//
//Beerware license: Free for any and all purposes, but if you find it
//useful AND we actually meet someday, you can buy me a beer!

#include "Timer.h"                     //http://github.com/JChristensen/Timer

const int LED1 = 12;                    //connect one LED to this pin (with appropriate current-limiting resistor of course)
const int LED2 = 13;                    //connect another LED to this pin (don't forget the resistor)
const unsigned long PERIOD1 = 100000;   // 1/10th second
const unsigned long PERIOD2 = 1000000;  //One second (1 * 1000 * 1000)
Timer t;                                //instantiate the timer object

void setup(void)
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    t.oscillate(LED1, PERIOD1, HIGH);
    t.oscillate(LED2, PERIOD2, HIGH);
}

void loop(void)
{
    t.update();
}

