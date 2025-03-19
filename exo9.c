#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdatomic.h>

// Use atomic variables for safe signal handling
volatile atomic_int state = 0;      // 0 = normal, 1 = waiting
volatile atomic_time_t first_press; // Timestamp of first Ctrl+C

// Async-safe write wrapper
#define safe_write(msg) write(STDOUT_FILENO, msg, sizeof(msg) - 1)

// Signal handler for Ctrl+C
void handle_sigint(int sig)
{
   (void)sig;
   time_t now = time(NULL);

   if (atomic_exchange(&state, 1) == 0)
   { // First press
      atomic_store(&first_press, now);
      safe_write("\nPress Ctrl+C again within 2 seconds to exit\n");
      alarm(2);
   }
   else
   { // Second press
      if (difftime(now, atomic_load(&first_press)) <= 2.0)
      {
         safe_write("\nGoodbye!\n");
         exit(0);
      }
   }
}

// Signal handler for alarm
void handle_alarm(int sig)
{
   (void)sig;
   safe_write("\nResuming normal operation\n");
   atomic_store(&state, 0);
}

int main()
{
   struct sigaction sa = {0};
   time_t last_print = time(NULL);

   // Setup signal handlers
   sa.sa_handler = handle_sigint;
   sigaction(SIGINT, &sa, NULL);

   sa.sa_handler = handle_alarm;
   sigaction(SIGALRM, &sa, NULL);

   while (1)
   {
      if (atomic_load(&state) == 0)
      { // Normal operation
         time_t now = time(NULL);
         if (difftime(now, last_print) >= 1.0)
         {
            printf("working...\n");
            fflush(stdout);
            last_print = now;
         }
         sleep(1);
      }
      else
      {                  // Waiting state
         usleep(100000); // 100ms sleep to reduce CPU usage
      }
   }

   return 0;
}