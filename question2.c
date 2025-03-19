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
