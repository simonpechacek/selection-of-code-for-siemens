#include "mbed.h"

DigitalOut my_led(LED1);
Serial serial(SERIAL_TX, SERIAL_RX);
Ticker led_ticker;

#define INIT_CHAR 'i'
#define DEFAULT_RESPONSE 'a'
#define HELLO_RESPONSE 'h'
#define LEAVE_RESPONSE 'b'
#define UNDEFINED_INPUT 'u'
#define LED_ON 1
#define LED_OFF 0
#define LED_BLINKING 2
#define LED_ON_CHAR 'x'
#define LED_OFF_CHAR 'o'
#define BAUD_RATE 115200
#define OK 0

void flip()
{
    my_led = !my_led;
}

unsigned char handle_input(unsigned char input, float *periods, int *led_serial)
{
    unsigned char response;
    if (input >= '1' && input <= '5')
    {
        led_ticker.detach();
        led_ticker.attach(&flip, periods[input - '1']);
        response = DEFAULT_RESPONSE;
        *led_serial = LED_BLINKING;
    }
    else
    {
        switch (input)
        {
        case 's':
            led_ticker.detach();
            my_led = LED_ON;
            response = DEFAULT_RESPONSE;
            *led_serial = LED_ON;
            break;
        case 'e':
            led_ticker.detach();
            my_led = LED_OFF;
            response = DEFAULT_RESPONSE;
            *led_serial = LED_OFF;
            break;
        case 'h':
            response = HELLO_RESPONSE;
            break;
        case 'b':
            response = LEAVE_RESPONSE;
            break;
        default:
            response = UNDEFINED_INPUT;
            break;
        }
    }

    return response;
}

int main()
{
    //------CONSTANTS AND VARIABLES------//
    unsigned char inc_char, response;
    float periods[] = {0.05, 0.1, 0.2, 0.5, 1.0}; // periods in seconds
    int led_serial = LED_OFF;                     // used for printing led status on serial port
    int prev_led_state = LED_OFF;
    my_led = LED_OFF; // turn off led
    serial.baud(BAUD_RATE);
    // read out input buffer
    while (serial.readable())
    {
        serial.getc();
    }
    while (!serial.writeable())
    {
    };
    // initialize with sending 'i'
    serial.putc(INIT_CHAR);

    // main loop
    while (true)
    {
        if (serial.readable())
        {
            inc_char = serial.getc();
            response = handle_input(inc_char, periods, &led_serial);
            while(!serial.writeable()){}
            serial.putc(response);
            if (response == LEAVE_RESPONSE)
                break;
        }

        if (led_serial == LED_BLINKING && prev_led_state != my_led)
        {
            // send led status
            response = my_led ? LED_ON_CHAR : LED_OFF_CHAR;
            while(!serial.writeable()){}
            serial.putc(response);
            prev_led_state = my_led;
        }
    }
    // turn off led and exit
    my_led = LED_OFF;
    return OK;
}