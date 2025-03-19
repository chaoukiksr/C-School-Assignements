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
