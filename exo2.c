#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BASE 64

// Définition du type number
typedef struct
{
   unsigned char *digits; // Tableau de chiffres
   size_t length;         // Nombre de chiffres
   unsigned char base;    // Base du nombre (2, 4, 8, 16, 32, 64)
} number;

// Fonction pour libérer la mémoire d'un nombre
void free_number(number nbr)
{
   if (nbr.digits != NULL)
   {
      free(nbr.digits);
   }
}

// Fonction pour convertir un entier en nombre dans une base donnée
number to_number(unsigned int nbr, unsigned char base)
{
   number result;
   result.digits = NULL;
   result.length = 0;
   result.base = (base == 2 || base == 4 || base == 8 || base == 16 || base == 32 || base == 64) ? base : 16;

   // Cas spécial pour 0
   if (nbr == 0)
   {
      result.digits = (unsigned char *)malloc(1);
      result.digits[0] = '0';
      result.length = 1;
      return result;
   }

   // Calcul du nombre de chiffres nécessaires
   unsigned int temp = nbr;
   size_t length = 0;
   while (temp > 0)
   {
      temp /= result.base;
      length++;
   }

   // Allocation de la mémoire pour les chiffres
   result.digits = (unsigned char *)malloc(length);
   result.length = length;

   // Conversion du nombre en base donnée
   const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
   for (size_t i = 0; i < length; i++)
   {
      result.digits[length - 1 - i] = digits[nbr % result.base];
      nbr /= result.base;
   }

   return result;
}

// Fonction pour convertir un nombre en base quelconque en entier non signé
unsigned int to_uint(number nbr)
{
   unsigned int result = 0;
   const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";

   for (size_t i = 0; i < nbr.length; i++)
   {
      unsigned char digit = nbr.digits[i];
      unsigned int value = 0;

      // Trouver la valeur du chiffre
      for (unsigned char j = 0; j < nbr.base; j++)
      {
         if (digits[j] == digit)
         {
            value = j;
            break;
         }
      }

      result = result * nbr.base + value;
   }

   return result;
}

// Fonction pour convertir un nombre en chaîne de caractères
char *to_string(number nbr)
{
   char *str = (char *)malloc(nbr.length + 1); // +1 pour le caractère nul
   if (str == NULL)
   {
      return NULL;
   }

   for (size_t i = 0; i < nbr.length; i++)
   {
      str[i] = nbr.digits[i];
   }
   str[nbr.length] = '\0';

   return str;
}

// Exemple d'utilisation
int main()
{
   // Convertit 255 en base 16
   number nbr = to_number(255, 16);

   // Convertit le nombre en chaîne de caractères
   char *str = to_string(nbr);
   printf("Nombre en base 16: %s\n", str);

   // Convertit le nombre en entier non signé
   unsigned int value = to_uint(nbr);
   printf("Valeur en entier: %u\n", value);

   // Libère la mémoire
   free(str);
   free_number(nbr);

   return 0;
}