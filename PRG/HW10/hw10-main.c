/*
 * File name: hw10-main.c
 * Date:      2017/04/14 18:51
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <termios.h>
#include <unistd.h> // for STDIN_FILENO

#include <pthread.h>

#include "prg_serial_nonblock.h"
#include "messages.h"
#include "event_queue.h"

#define SERIAL_READ_TIMOUT_MS 500 // timeout for reading from serial port
#define MESSAGE_SIZE sizeof(message)
#define WRITE_FAIL -1

#ifndef DEBUG
#define DEBUG false
#endif

#define HELP_ROWS 9 

// shared data structure
typedef struct
{
   bool quit;
   int fd; // serial port file descriptor
} data_t;

pthread_mutex_t mtx;
pthread_cond_t cond;

void call_termios(int reset);

void *input_thread(void *);
void *serial_rx_thread(void *); // serial receive buffer

bool send_message(data_t *data, message *msg);


const char *help[] = {"---------------------------------------------\n",
                      "q - quit program\n",
                      "g - get program version from nucleo\n",
                      "r - reset chunk id\n",
                      "a - abort computation on nucleo\n",
                      "h - print this message\n",
                      "'1 - 5' - send new computation to nucleo with\n",
                      "          number of tasks = 'number' * 10\n",
                      "---------------------------------------------\n"
                     };


// - main ---------------------------------------------------------------------
int main(int argc, char *argv[])
{
   data_t data = {.quit = false, .fd = -1};
   const char *serial = argc > 1 ? argv[1] : "/dev/ttyACM0";
   data.fd = serial_open(serial);

   if (data.fd == -1)
   {
      fprintf(stderr, "ERROR: Cannot open serial port %s\n", serial);
      exit(100);
   }

   enum
   {
      INPUT,
      SERIAL_RX,
      NUM_THREADS
   };
   const char *threads_names[] = {"Input", "Serial In"};

   void *(*thr_functions[])(void *) = {input_thread, serial_rx_thread};

   pthread_t threads[NUM_THREADS];
   pthread_mutex_init(&mtx, NULL); // initialize mutex with default attributes
   pthread_cond_init(&cond, NULL); // initialize mutex with default attributes

   call_termios(0);

   for (int i = 0; i < NUM_THREADS; ++i)
   {
      int r = pthread_create(&threads[i], NULL, thr_functions[i], &data);
      fprintf(stderr, "INFO: Create thread '%s' %s\n", threads_names[i], (r == 0 ? "OK" : "FAIL"));
   }

   // example of local variables for computation and messaging
   struct
   {
      uint16_t chunk_id;
      uint16_t nbr_tasks;
      uint16_t task_id;
      bool computing;
   } computation = {0, 0, 0, false};
   message msg;

   bool quit = false;
   while (!quit)
   {
      // example of the event queue
      event ev = queue_pop();
      if (ev.source == EV_KEYBOARD)
      {
         msg.type = MSG_NBR;
         // handle keyboard events
         switch (ev.type)
         {
         case EV_GET_VERSION:
         { // prepare packet for get version
            msg.type = MSG_GET_VERSION;
            fprintf(stderr, "INFO: Get version requested\n");
         }
         break;
         case EV_ABORT:
         {
            if (computation.computing)
               msg.type = MSG_ABORT;
            else
            {
               fprintf(stderr, "WARN: Abort requested but it is not computing\n\r");
               msg.type = MSG_NBR;
            }
            break;
         }
         case EV_RESET_CHUNK:
         {
            if (!computation.computing)
            {
               computation.chunk_id = 0;
               fprintf(stderr, "INFO: Chunk reset request\n\r");
            }
            else
               fprintf(stderr, "WARN: Chunk reset request discarded, it is currently computing\n\r");
            break;
         }
         case EV_COMPUTE:
         {
            if (!computation.computing)
            {
               msg.type = MSG_COMPUTE;
               msg.data.compute.chunk_id = computation.chunk_id++;
               msg.data.compute.nbr_tasks = ev.data.param;
               fprintf(stderr, "INFO: New computation chunk id: %d no. of tasks: %d\n\r", msg.data.compute.chunk_id, msg.data.compute.nbr_tasks);
            }
            else
               fprintf(stderr, "WARN: New computation requested but it is discarded due on ongoing computation\n\r");
            break;
         }
         case EV_QUIT:
         {
            if(computation.computing)
               msg.type = MSG_ABORT;
            pthread_mutex_lock(&mtx);
            data.quit = true;
            pthread_mutex_unlock(&mtx);
            quit = true;
            break;
         }
         case EV_HELP:
            for(int i = 0; i < HELP_ROWS; ++i)
               printf("%s", help[i]);
            break;
         default:
            break;
         }
         if (msg.type != MSG_NBR)
         { // messge has been set
            if (!send_message(&data, &msg))
            {
               fprintf(stderr, "ERROR: send_message() does not send all bytes of the message!\n");
            }
         }
      }
      else if (ev.source == EV_NUCLEO)
      { // handle nucleo events
         if (ev.type == EV_SERIAL)
         {
            message *msg = ev.data.msg;
            switch (msg->type)
            {
            case MSG_STARTUP:
            {
               char str[STARTUP_MSG_LEN + 1];
               for (int i = 0; i < STARTUP_MSG_LEN; ++i)
               {
                  str[i] = msg->data.startup.message[i];
               }
               str[STARTUP_MSG_LEN] = '\0';
               fprintf(stderr, "INFO: Nucleo restarted - '%s'\n", str);
               break;
            }
            case MSG_VERSION:
            {
               if (msg->data.version.patch > 0)
               {
                  fprintf(stderr, "INFO: Nucleo firmware ver. %d.%d-p%d\n", msg->data.version.major, msg->data.version.minor, msg->data.version.patch);
               }
               else
               {
                  fprintf(stderr, "INFO: Nucleo firmware ver. %d.%d\n", msg->data.version.major, msg->data.version.minor);
               }
               break;
            }
            case MSG_OK:
            {
               fprintf(stderr, "INFO: Receive ok from Nucleo\r\n");
               computation.computing = !computation.computing;
               if (DEBUG)
                  fprintf(stderr, "DEBUG: computing: %d\r\n", computation.computing);
               break;
            }
            case MSG_ERROR:
            {
               fprintf(stderr, "WARN: Receive error from Nucleo\r\n");
               break;
            }
            case MSG_COMPUTE_DATA:
            {
               fprintf(stderr, "INFO: New data chunk id: %d, task id: %d - results %d\r\n", msg->data.compute_data.chunk_id, msg->data.compute_data.task_id, msg->data.compute_data.result);
               if (!computation.computing)
                  fprintf(stderr, "WARN: Nucleo sends new data without computing \r\n");
               break;
            }
            case MSG_DONE:
            {
               fprintf(stderr, "INFO: Nucleo reports the computation is done computing: %d\r\n", computation.computing);
               computation.computing = false;
               if (DEBUG)
                  fprintf(stderr, "DEBUG: computing: %d\r\n", computation.computing);
               break;
            }
            case MSG_ABORT:
            {
               fprintf(stderr, "INFO: Abort from Nucleo\r\n");
               computation.computing = false;
               if (DEBUG)
                  fprintf(stderr, "DEBUG: computing: %d\r\n", computation.computing);
               break;
            }

            default:
               break;
            }
            if (msg)
            {
               free(msg);
            }
         }
         else if (ev.type == EV_QUIT)
         {
            quit = true;
         }
         else
         {
            // ignore all other events
         }
      }
   }                // end main quit
   queue_cleanup(); // cleanup all events and free allocated memory for messages.
   for (int i = 0; i < NUM_THREADS; ++i)
   {
      fprintf(stderr, "INFO: Call join to the thread %s\n", threads_names[i]);
      int r = pthread_join(threads[i], NULL);
      fprintf(stderr, "INFO: Joining the thread %s has been %s\n", threads_names[i], (r == 0 ? "OK" : "FAIL"));
   }
   serial_close(data.fd);
   call_termios(1); // restore terminal settings
   return EXIT_SUCCESS;
}

// - function -----------------------------------------------------------------
void call_termios(int reset)
{
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
      tio.c_oflag |= OPOST;
      tcsetattr(STDIN_FILENO, TCSANOW, &tio);
   }
}

// - function -----------------------------------------------------------------
void *input_thread(void *d)
{
   data_t *data = (data_t *)d;
   bool end = false;
   int c;
   event ev = {.source = EV_KEYBOARD};
   while (!end && (c = getchar()))
   {
      ev.type = EV_TYPE_NUM;
      switch (c)
      {
      case 'g': // get version
         ev.type = EV_GET_VERSION;
         break;
      case 'q': // quit whole program
         end = true;
         break;
      case 'a': // abort computation
         ev.type = EV_ABORT;
         break;
      case 'r': // reset chunk id
         ev.type = EV_RESET_CHUNK;
         break;
      case 'h':
         ev.type = EV_HELP;
         break;
      default:
         if (c >= '1' && c <= '5') // set new computation
         {
            ev.type = EV_COMPUTE;
            ev.data.param = 10 * (c - '0');
         }
         break;
      }
      if (ev.type != EV_TYPE_NUM)
      { // new event
         queue_push(ev);
      }
      pthread_mutex_lock(&mtx);
      end = end || data->quit; // check for quit
      pthread_mutex_unlock(&mtx);
   }
   ev.type = EV_QUIT;
   queue_push(ev);
   fprintf(stderr, "INFO: Exit input thead %p\n", (void *)pthread_self());
   return NULL;
}

// - function -----------------------------------------------------------------
void *serial_rx_thread(void *d)
{ // read bytes from the serial and puts the parsed message to the queue
   data_t *data = (data_t *)d;
   uint8_t msg_buf[MESSAGE_SIZE]; // maximal buffer for all possible messages defined in messages.h
   event ev = {.source = EV_NUCLEO, .type = EV_SERIAL, .data.msg = NULL};
   bool end = false;
   unsigned char c;
   int buff_index = 0, message_len = -1;
   while (serial_getc_timeout(data->fd, SERIAL_READ_TIMOUT_MS, &c) > 0)
   {
   }; // discard garbage

   while (!end)
   {
      int r = serial_getc_timeout(data->fd, SERIAL_READ_TIMOUT_MS, &c);
      if (r > 0)
      { // character has been read
         msg_buf[buff_index++] = c;
         if (buff_index == 1)
         {
            if (!get_message_size(msg_buf[buff_index - 1], &message_len))
            {
               fprintf(stderr, "ERROR: Unknown message type has been received 0x%x\n - '%c'\r", c, c);
               buff_index = 0;
            }
         }
         if (buff_index == message_len)
         {
            message *msg = (message *)malloc(sizeof(message));
            if (parse_message_buf(msg_buf, message_len, msg))
            {
               ev.data.msg = msg;
               queue_push(ev);
            }
            else
            {
               free(msg);
               fprintf(stderr, "ERROR: Cannot parse message type %d\n\r", msg_buf[0]);
            }
            buff_index = 0;
         }
      }
      else if (r == 0)
      {
         if (buff_index > 0)
         {
            fprintf(stderr, "WARN: the packet has not been received discard what has been read\n\r");
            buff_index = 0;
         }
      }
      else
      {
         fprintf(stderr, "ERROR: Cannot receive data from the serial port\n");
         end = true;
      }
      pthread_mutex_lock(&mtx);
      end = data->quit;
      pthread_mutex_unlock(&mtx);
   }
   ev.type = EV_QUIT;
   queue_push(ev);
   fprintf(stderr, "INFO: Exit serial_rx_thread %p\n", (void *)pthread_self());
   return NULL;
}

// - function -----------------------------------------------------------------
bool send_message(data_t *data, message *msg)
{
   bool ret = true;
   uint8_t buffer[MESSAGE_SIZE];
   int msg_len;
   if (fill_message_buf(msg, buffer, MESSAGE_SIZE, &msg_len))
   {
      if (DEBUG)
      {
         fprintf(stderr, "DEBUG: Write message: ");
         for(int i = 0; i < msg_len; ++i)
            printf("%d ", buffer[i]);
         printf("\n");
      }
      for (int i = 0; i < msg_len; ++i)
      {
         int r = serial_putc(data->fd, buffer[i]);
         if (r == WRITE_FAIL)
         {
            ret = false;
            break;
         }
      }
   }
   else
      ret = false;
   return ret;
}

/* end of hw10-main.c */
