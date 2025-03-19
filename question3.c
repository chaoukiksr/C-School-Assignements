#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
   pid_t child_pid;
   int status;

   child_pid = fork();
   for (size_t i = 0; i < 10; i++)
   {

      if (child_pid < 0)
      {
         perror("impossible de créer un processus enfant");
         exit(EXIT_FAILURE);
      }
      else if (child_pid == 0)
      {
         bruteforce(i, 1, 6);
      }
      else
      {
         waitpid(child_pid, &status, 0);
         if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
         {
            char filename[50];
            sprintf(filename, "found.PID.%d", child_pid);
            FILE *file = fopen(filename, "r");
            if (file == NULL)
            {
               perror("erreur lors de l'ouverture");
               exit(EXIT_FAILURE);
            }
            int integer;
            fscanf(file, "%d", &integer);
            printf("la valeur de l'entier trouvé est %d\n", integer);
            fclose(file);

            remove(filename);
         }
         else
         {
            printf("Aucune correspondance trouvée.\n"); // Inform if no match found
         }
      }
   }

   return 0;
}