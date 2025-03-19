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

char hash[1 + 2 * MD5_DIGEST_LENGTH] = {0};

char *md5hash(char *str)
{
   unsigned char md5[MD5_DIGEST_LENGTH] = {0};
   MD5((const unsigned char *)str, strlen(str), md5); // Added casting and const
   for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
   {
      sprintf(hash + 2 * i, "%02x", md5[i]);
   }
   return hash;
}

int zeros(char *s, int n)
{
   for (int i = 0; i < n; i++)
   {
      if (s[i] != '0')
      {
         return 0;
      }
   }
   return 1;
}

void bruteforce(int first, int step, int zero)
{
   char filename[128];
   pid_t child;
   unsigned long long i;
   sprintf(filename, "found.PID.%d", getpid()); // Corrected filename format
   FILE *file = fopen(filename, "w");           // Corrected file name in fopen
   if (file == NULL)
   {
      perror("impossible d'ouvrir");
      exit(EXIT_FAILURE);
   }
   char str[128];
   for (int i = 0; i < 10; i++)
   {
      child = fork();
      if (child == 0)
      {
         bruteforce(i, 10, 6);
      }
      else
      {
         exit(EXIT_FAILURE);
      }
   }
   while (true)
   {

      sprintf(str, "%d", first);
      md5hash(str);
      if (zeros(hash, zero))
      {
         break;
      }
      first += step;
   }
   sprintf(str, "found.%i", getpid());
   int out;
   if ((out = open(str, O_WRONLY || O_CREAT || O_TRUNC, 0600)) == -1)
   {
      perror("error has been occured");
      exit(EXIT_FAILURE);
   }
   if (write(out, &i, sizeof(unsigned long long)) < sizeof(unsigned long long))
   {
      perror("write");
      exit(EXIT_FAILURE);
   }
   if (close(out) == -1)
   {
      perror("close");
      exit(EXIT_FAILURE);
   }
   exit(0);
}

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