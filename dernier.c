#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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

// 3
dayname weekday(time_t when)
{
   int days = when/86400;
   int day = (days + 4) %7;
   return day == 0 ? 7 : (dayname)day;
}

int leapyear(unsigned int year)
{
   return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

date from_time(time_t when)
{
   struct tm *tminfo = localtime(&when);
   date d;


   d.weekday = weekday(when);
   d.day = tminfo->tm_mday;
   d.month = (monthname)(tminfo->tm_mon + 1);
   d.year = tminfo->tm_year + 1900;

   return d;
}

int main(void)
{
   // Obtenir le temps actuel
   time_t now = time(NULL);
   if (now == -1)
   {
      return 1;
   }

   date today = from_time(now);

   printf("Today is %s, %d %s %d\n",
          dayname_str(today.weekday),
          today.day,
          monthname_str(today.month),
          today.year);

   return 0;
}