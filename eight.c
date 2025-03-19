#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define DEFAULT_PLAYER 2
#define DEFAULT_WIDTH 4
#define DEFAULT_HEIGHT 4
#define MAX_WIDTH 26
#define MIN_WIDTH 4
#define MAX_HEIGHT 16
#define MIN_HEIGHT 4
#define MAX_PLAYERS 8
#define MIN_PLAYERS 2

// Vérifie si une chaîne est un nombre valide
bool is_valid_number(const char *str)
{
   while (*str)
   {
      if (!isdigit(*str))
         return false;
      str++;
   }
   return true;
}

// Récupère une valeur à partir des arguments ou des variables d'environnement
int get_value(int argc, char *argv[], int index, const char *env_var, int default_val, int min, int max)
{
   int value = default_val;
   char *env_value;

   if (argc > index && is_valid_number(argv[index]))
   {
      value = atoi(argv[index]);
   }
   else if ((env_value = getenv(env_var)) && is_valid_number(env_value))
   {
      value = atoi(env_value);
   }

   if (value < min || value > max)
   {
      value = default_val;
   }

   return value;
}

int main(int argc, char *argv[])
{
   if (argc > 1 && strcmp(argv[1], "--help") == 0)
   {
      printf("Utilisation : %s [hauteur] [largeur] [joueurs]\n", argv[0]);
      printf("Exemple : %s 8 10 4\n", argv[0]);
      printf("Si les paramètres sont absents ou invalides, les valeurs par défaut ou celles des variables d'environnement seront utilisées.\n");
      return EXIT_SUCCESS;
   }

   int height = get_value(argc, argv, 1, "P4HEIGHT", DEFAULT_HEIGHT, MIN_HEIGHT, MAX_HEIGHT);
   int width = get_value(argc, argv, 2, "P4WIDTH", DEFAULT_WIDTH, MIN_WIDTH, MAX_WIDTH);
   int players = get_value(argc, argv, 3, "P4PLAYERS", DEFAULT_PLAYER, MIN_PLAYERS, MAX_PLAYERS);

   printf("Largeur de la grille : %d\n", width);
   printf("Hauteur de la grille : %d\n", height);
   printf("Nombre de joueurs   : %d\n", players);

   return EXIT_SUCCESS;
}
