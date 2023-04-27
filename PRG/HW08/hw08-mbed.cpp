#include "mbed.h"

DigitalOut myled(LED1);
DigitalIn button(USER_BUTTON);
InterruptIn mybutton(USER_BUTTON);

bool pressIgnore = false;

Ticker ledTicker;
Ticker btnTicker;
Ticker btnPressTicker;

int counter = 0;
float blink_periods[] = {1, 0.5, 0.4, 0.3, 0.2, 0.1, 0.05};
int period_set = 0;

void flip()
{
    myled = !myled;
}

void controlBtn()
{
    if (button == 1)
    {
        btnPressTicker.detach();
        counter = 0;
        return;
    }
    counter++;
    if (counter == 20)
    {
        ledTicker.detach();
        period_set = 0;
        ledTicker.attach(&flip, blink_periods[period_set]);
        btnPressTicker.detach();
        counter = 0;
        return;
    }
}

void clearIgnore()
{
    pressIgnore = false;
}

void pressed()
{
    if (!pressIgnore)
    {
        ledTicker.detach();
        period_set++;
        if (period_set > 6)
        {
            period_set = 7;
            myled = 1;
        }
        else
            ledTicker.attach(&flip, blink_periods[period_set]);
        btnTicker.attach(&clearIgnore, 0.2);
        counter = 0;
        btnPressTicker.attach(&controlBtn, 0.01);
        pressIgnore = true;
    }
}

int main()
{
    ledTicker.attach(&flip, blink_periods[period_set]);
    mybutton.fall(&pressed);
}