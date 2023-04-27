#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <time.h>

#include "prg_serial_nonblock.h"

#define USB_DEVICE "/dev/ttyACM0"
#define TIMEOUT 0
#define READ_ERROR -1
#define INIT_CHAR 'i'
#define SEIRAL_OPEN_ERROR -1
#define TIMEOUT_SLOW 50
#define TIMEOUT_FAST 10
#define LED_ON_CHAR 'x'
#define LED_OFF_CHAR 'o'
#define NO_RESPONSE '?'
#define INIT_SEND ' '
#define COMPUTE_PERIOD 5.0f

void *input_thread(void *);
void *serial_thread(void *);
void *output_thread(void *);
void *compute_period_thread(void *);

typedef struct
{
    bool quit;
    int file_d;
    bool is_serial_open;
    unsigned char serial_in;
    unsigned char serial_out;
    bool led_status;
    int ticker;
    int period;
    bool reset_counter;
} Data;

//-----FUNCTIONS-----//
Data init_data();
void raw_mode(int reset);

//----GLOBAL VARIABLES----//
pthread_mutex_t mtx;
pthread_cond_t cond_redraw;
pthread_cond_t cond_count;

int main(int argc, char *argv[])
{
    //----INITIALIZATION----//
    Data data = init_data();
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond_redraw, NULL);
    pthread_cond_init(&cond_count, NULL);

    enum
    {
        INPUT,
        SERIAL,
        OUTPUT,
        COMPUTE,
        NUM_THREADS
    };
    void *(*thread_f[])(void *) = {input_thread, serial_thread, output_thread, compute_period_thread};
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&threads[i], NULL, thread_f[i], &data);
    }
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    //printf("\rMain Thread finished\n");
    return 0;
}

Data init_data()
{
    Data data;
    data.quit = false;
    data.file_d = SEIRAL_OPEN_ERROR;
    data.is_serial_open = false;
    data.serial_in = NO_RESPONSE;
    data.serial_out = INIT_SEND;
    data.led_status = false;
    data.ticker = 0;
    data.period = 0;
    data.reset_counter = false;
    return data;
}

void raw_mode(int reset)
{
    // 0 -> raw mode || 1 -> reset to normal
    static struct termios tio, tioOld;
    tcgetattr(STDIN_FILENO, &tio);
    if (reset)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
    }
    else
    {
        tioOld = tio; // backup
        cfmakeraw(&tio);
        tcsetattr(STDIN_FILENO, TCSANOW, &tio);
    }
}

void *input_thread(void *data_v)
{
    Data *data = (Data *)data_v;
    // set raw terminal mode
    raw_mode(0);
    bool q = false;
    bool serial_opened = false;
    while (!q && !serial_opened)
    {
        pthread_mutex_lock(&mtx);
        q = data->quit;
        serial_opened = data->is_serial_open;
        pthread_mutex_unlock(&mtx);
    }
    pthread_mutex_lock(&mtx);
    int file_d = data->file_d;
    pthread_mutex_unlock(&mtx);

    while (!q)
    {
        char c = getchar();
        if (c == 'q')
        {
            q = true;
            pthread_mutex_lock(&mtx);
            data->quit = true;
            pthread_cond_broadcast(&cond_redraw);
            pthread_cond_broadcast(&cond_count);
            pthread_mutex_unlock(&mtx);
        }
        else if ((c >= '1' && c <= '5') || c == 's' || c == 'e' || c == 'h' || c == 'b')
        {
            serial_putc(file_d, c);
            pthread_mutex_lock(&mtx);
            data->serial_out = c;
            data->serial_in = NO_RESPONSE;
            if (c != 'h' && c != 'b')
                data->reset_counter = true;
            pthread_cond_signal(&cond_redraw);
            pthread_mutex_unlock(&mtx);
        }
    }
    raw_mode(1);
    //printf("\nInput Thread finished!\n");
    return 0;
}

void *serial_thread(void *data_v)
{
    Data *data = (Data *)data_v;

    const char *serial = USB_DEVICE;
    bool q = false;
    int file_d = serial_open(serial);
    if (file_d == SEIRAL_OPEN_ERROR)
    {
        pthread_mutex_lock(&mtx);
        data->quit = true;
        pthread_cond_broadcast(&cond_redraw);
        pthread_cond_broadcast(&cond_count);
        pthread_mutex_unlock(&mtx);
        q = true;
    }
    else
    {
        pthread_mutex_lock(&mtx);
        data->file_d = file_d;
        data->is_serial_open = true;
        pthread_mutex_unlock(&mtx);
    }
    // read until 'i' char comes than go to main loop
    bool i_arrived = false;
    unsigned char tmp_char;
    while (!i_arrived && !q)
    {
        int ret_v = serial_getc_timeout(file_d, TIMEOUT_SLOW, &tmp_char);
        if (ret_v != READ_ERROR && ret_v != TIMEOUT)
        {
            if (tmp_char == INIT_CHAR)
            {
                i_arrived = true;
                pthread_mutex_lock(&mtx);
                data->serial_in = tmp_char;
                pthread_cond_signal(&cond_redraw);
                pthread_mutex_unlock(&mtx);
            }
        }
        pthread_mutex_lock(&mtx);
        q = data->quit;
        pthread_mutex_unlock(&mtx);
    }

    unsigned char c;
    while (!q)
    {
        int ret = serial_getc_timeout(file_d, TIMEOUT_FAST, &c);
        if (ret != READ_ERROR && ret != TIMEOUT)
        {

            if (c == LED_OFF_CHAR || c == LED_ON_CHAR)
            {
                //  signal both output and count thread
                pthread_mutex_lock(&mtx);
                data->serial_in = c;
                pthread_cond_signal(&cond_count);
                pthread_cond_signal(&cond_redraw);
                pthread_mutex_unlock(&mtx);
            }
            else
            {
                //  signal just output thread
                pthread_mutex_lock(&mtx);
                data->serial_in = c;
                if (data->serial_out == 's' && c == 'a')
                    data->led_status = true;
                else if(data->serial_out == 'e' && c == 'a')
                    data->led_status = false;
                pthread_cond_signal(&cond_redraw);
                pthread_mutex_unlock(&mtx);
            }
        }
        pthread_mutex_lock(&mtx);
        q = data->quit;
        pthread_mutex_unlock(&mtx);
    }
    if (file_d != SEIRAL_OPEN_ERROR)
    {
        serial_close(file_d);
    }
    //printf("\rSerial Thread finished\n");
    return 0;
}

void *output_thread(void *data_v)
{
    Data *data = (Data *)data_v;
    bool q = false;
    // Initial print with initial values
    printf("\rLED %3s ", "OFF");
    printf("send: ' ' received: '?', T = %4d ms, ticker = %4d  ", 0, 0);
    fflush(stdout);
    while (!q)
    {
        pthread_mutex_lock(&mtx);
        pthread_cond_wait(&cond_redraw, &mtx);
        bool led = data->led_status;
        unsigned char last_in = data->serial_in;
        unsigned char last_out = data->serial_out;
        int ticker = data->ticker;
        int period = data->period;
        pthread_mutex_unlock(&mtx);
        if (last_in == LED_ON_CHAR)
            led = true;
        else if (last_in == LED_OFF_CHAR)
            led = false;
        printf("\rLED %3s ", led ? "ON" : "OFF");
        printf("send: '%c' received: '%c', T = %4d ms, ticker = %4d  ", last_out, last_in, period, ticker);
        fflush(stdout);
        pthread_mutex_lock(&mtx);
        q = data->quit;
        pthread_mutex_unlock(&mtx);
    }
    //printf("\rOutput thread finished\n");
    return 0;
}

void *compute_period_thread(void *data_v)
{
    Data *data = (Data *)data_v;

    bool q = false;
    int counter = 0;
    time_t start, current;
    double diff_time;
    time(&start);
    while (!q)
    {
        pthread_mutex_lock(&mtx);
        pthread_cond_wait(&cond_count, &mtx);
        unsigned char in = data->serial_in;
        bool reset = data->reset_counter;
        pthread_mutex_unlock(&mtx);
        if (in == 'x')
        {
            pthread_mutex_lock(&mtx);
            data->ticker++;
            pthread_mutex_unlock(&mtx);
        }
        if (reset)
        {
            counter = 0;
            time(&start);
        }
        counter++;
        time(&current);
        diff_time = difftime(current, start);
        if (diff_time >= COMPUTE_PERIOD)
        {
            float period = 0.0f;
            if (counter != 0)
                period = ((diff_time * 1000) / counter);
            counter = 0;
            time(&start);
            pthread_mutex_lock(&mtx);
            data->period = (int)period;
            pthread_mutex_unlock(&mtx);
        }
        pthread_mutex_lock(&mtx);
        q = data->quit;
        data->reset_counter = false;
        pthread_mutex_unlock(&mtx);
    }
    //printf("\rCompute thread finished!\n");
    return 0;
}