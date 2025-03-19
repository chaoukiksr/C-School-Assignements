#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
   char *valeur;
   int base;
} number;

void free_number(number nbr)
{
   if (nbr.valeur != NULL)
   {
      free(nbr.valeur);
   }
}

bool est_une_base_valide(int base)
{
   if (base == 2 || base == 4 || base == 8 || base == 16 || base == 32)
   {
      return true;
   }
   return false;
}

char *convertir_vers_base(unsigned int nbr, unsigned char base)
{
   const char degits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

   int length = 0;
   int rest = nbr;
   do
   {
      length++;
      rest = rest / base;
   } while (rest != 0);

   char *resultat = (char *)malloc((length + 1) * sizeof(char));
   if (resultat == NULL)
   {
      fprintf(stderr, "Erreur d'allocation de mémoire. \n");
      return NULL;
   }
   resultat[length] = '\0';
   for (int i = length - 1; i >= 0; i--)
   {
      resultat[i] = degits[nbr % base];
      nbr = nbr / base;
   }


   //supression des zeros inutils

   char* premier_char = resultat;
   while(*premier_char=='0' &&*(premier_char+1)!='\0'){
      premier_char++;
   }
   if(premier_char!=resultat){
      size_t new_resultat_length = strlen(premier_char)+1;
      char* new_resultat_memory_al = (char*)realloc(resultat,new_resultat_length);
      if(new_resultat_memory_al==NULL){
         printf("erreur lors d'allocation de memoire");
         return NULL;
      }else{
         memmove(new_resultat_memory_al,premier_char,new_resultat_length);
         return new_resultat_memory_al;
      }
   }
   return resultat;
}

unsigned int to_uint(number nbr)
{
   if (!est_une_base_valide(nbr.base))
   {
      printf("Base invalide");
      return 0;
   }
   char *endpointer;
   unsigned long resultat = strtoul(nbr.valeur, &endpointer, nbr.base);
   if (*endpointer != '\0')
   {
      printf("Erreur : transformation impossible, caractères non valides détectés\n");
      return 0;
   }
   if (resultat == 0)
   {
      printf("Erreur au niveau de la fonction, transformation impossible");
      return 0;
   }
   return (unsigned int)resultat;
}

number to_number(unsigned int nbr, unsigned char base)
{
   number number;
   if (!est_une_base_valide(base))
   {
      base = 16;
   }
   number.base = base;
   number.valeur = convertir_vers_base(nbr, base);
   return number;
}
char *to_string(number nbr)
{

   char *result = (char *)malloc((strlen(nbr.valeur) + 1) * sizeof(char));

   if (result != NULL)
   {
      return strcpy(result, nbr.valeur);
   }
   printf("Erreur d'allocation de memoire");
   return NULL;
}