#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Définition des énumérations et de la structure date
typedef enum
{
   MON = 1,
   TUE = 2,
   WED = 3,
   THU = 4,
   FRI = 5,
   SAT = 6,
   SUN = 7
} dayname;
typedef enum
{
   JAN = 1,
   FEB = 2,
   MAR = 3,
   APR = 4,
   MAY = 5,
   JUN = 6,
   JUL = 7,
   AUG = 8,
   SEP = 9,
   OCT = 10,
   NOV = 11,
   DEC = 12
} monthname;

typedef struct
{
   dayname weekday : 3;
   unsigned int day : 5;
   monthname month : 4;
   int year : 20;
} date;

// 1. Fonction pour renvoyer le nom du jour
char *dayname_str(dayname day)
{
   switch (day)
   {
   case MON:
      return "Monday";
   case TUE:
      return "Tuesday";
   case WED:
      return "Wednesday";
   case THU:
      return "Thursday";
   case FRI:
      return "Friday";
   case SAT:
      return "Saturday";
   case SUN:
      return "Sunday";
   default:
      return "Unknown";
   }
}

// 2. Fonction pour renvoyer le nom du mois
char *monthname_str(monthname month)
{
   switch (month)
   {
   case JAN:
      return "January";
   case FEB:
      return "February";
   case MAR:
      return "March";
   case APR:
      return "April";
   case MAY:
      return "May";
   case JUN:
      return "June";
   case JUL:
      return "July";
   case AUG:
      return "August";
   case SEP:
      return "September";
   case OCT:
      return "October";
   case NOV:
      return "November";
   case DEC:
      return "December";
   default:
      return "Unknown";
   }
}

// 3. Fonction pour renvoyer le jour de la semaine correspondant à une date en secondes depuis l'epoch
dayname weekday(time_t when)
{
   struct tm *tm = localtime(&when);
   if (tm == NULL)
   {
      return MON; // Valeur par défaut en cas d'erreur
   }
   return (dayname)(tm->tm_wday == 0 ? 7 : tm->tm_wday); // tm_wday : 0 (dimanche) à 6 (samedi)
}

// 4. Fonction pour vérifier si une année est bissextile
int leapyear(unsigned int year)
{
   return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// 5. Fonction pour convertir un time_t en date
date from_time(time_t when)
{
   struct tm *tm = localtime(&when);
   date d;

   if (tm == NULL)
   {
      // En cas d'erreur, retourne une date par défaut
      d.weekday = MON;
      d.day = 1;
      d.month = JAN;
      d.year = 1970;
      return d;
   }

   d.weekday = (dayname)(tm->tm_wday == 0 ? 7 : tm->tm_wday); // Conversion en dayname
   d.day = tm->tm_mday;
   d.month = (monthname)(tm->tm_mon + 1); // tm_mon : 0 (janvier) à 11 (décembre)
   d.year = tm->tm_year + 1900;           // tm_year : années depuis 1900

   return d;
}

// 6. Fonction main pour afficher la date du jour
int main()
{
   // Obtenir le temps actuel
   time_t now = time(NULL);
   if (now == -1)
   {
      perror("time");
      return 1;
   }

   // Convertir en date
   date today = from_time(now);

   // Afficher la date
   printf("Today is %s, %d %s %d\n",
          dayname_str(today.weekday),
          today.day,
          monthname_str(today.month),
          today.year);

   return 0;
}