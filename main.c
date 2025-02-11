// ldd: -lncurses
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE 16
#define NB_PLAYERS 2
#define CHIP "XO"

char columns[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'};
void INITSCREEN(void)
{
   initscr();
   raw();
   keypad(stdscr, true);
   noecho();
}

void DONE_SCREEN(void)
{
   endwin();
   exit(0);
}

char board[SIZE][SIZE];

int current_player = 0;

void init_board(void)
{
   for (int i = 0; i < SIZE; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         board[i][j] = ' ';
      }
   }
}

void draw_board(void)
{
   clear();
   for (int i = 0; i < SIZE + 1; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         mvprintw(i, j * 2, "%c|", board[i][j]);
         mvprintw(SIZE, j * 2, "+-");
         mvprintw(SIZE + 1, j * 2 + 1, "%c", columns[j]);
      }
   }
   refresh();
}
int get_col(void)
{
   int ch = getch();
   while (ch != KEY_BACKSPACE)
   {
      if (ch >= 'A' && ch <= 'P')
      {
         return ch - 'A';
      }
   }
   return -1;
}
int add_coin(int col, int player)
{
   int row;
   for (row = SIZE - 1; row >= 0; row--)
   {
      if (board[row][col] == ' ')
      {
         board[row][col] = CHIP[player];
         return (player + 1) % NB_PLAYERS;
      }
   }
   if (row < 0)
   {
      mvprintw(SIZE + 3, 0, "Full column, Try another one");
      refresh();
   }
   return player;
}

bool board_is_full(void)
{
   bool full = true;
   for (int i = 0; i < SIZE; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         if (board[i][j] == ' ')
         {
            full = false;
         }
      }
   }
   return full;
}
bool check_horizontal_win(int row, int col)

{
   char chip = board[row][col];
   int count = 1;
   for (int i = col - 1; i >= 0; i--)
   {
      if (board[row][i] == chip)
      {
         count++;
      }
   }
   for (int i = col + 1; i <= SIZE; i++)
   {
      if (board[row][i] == chip)
      {
         count++;
      }
   }
   if (count >= 4)
   {
      return true;
   }
   return false;
}
bool check_Vertical_win(int row, int col)
{
   char chip = board[row][col];
   int count = 1;
   for (int i = row - 1; i >= 0; i--)
   {
      if (board[i][col] == chip)
      {
         count++;
      }
   }
   for (int i = row + 1; i < SIZE; i++)
   {
      if (board[i][col] == chip)
      {
         count++;
      }
   }
   if (count >= 4)
   {
      return true;
   }
   return false;
}

bool check_top_right_bottom_left_diagonal_win(int row, int col)
{
   char chip = board[row][col];
   int count = 1;
   for (int i = col + 1; i >= SIZE; i++)
   {
      for (int j = row + 1; j >= SIZE; j++)
      {
         if (board[i][j] == chip)
         {
            count++;
         }
      }
   }
   for (int i = col - 1; i >= SIZE; i++)
   {
      for (int j = row - 1; j >= SIZE; j++)
      {
         if (board[i][j] == chip)
         {
            count++;
         }
      }
   }
   if (count >= 4)
   {
      return true;
   }
   return false;
}
bool check_top_left_bottom_right_diagonal_win(int row, int col)
{
   char chip = board[row][col];
   int count = 1;
   for (int i = col + 1; i >= SIZE; i++)
   {
      for (int j = row - 1; j >= SIZE; j++)
      {
         if (board[i][j] == chip)
         {
            count++;
         }
      }
   }
   for (int i = col - 1; i >= SIZE; i++)
   {
      for (int j = row + 1; j >= SIZE; j++)
      {
         if (board[i][j] == chip)
         {
            count++;
         }
      }
   }

   if (count >= 4)
   {
      return true;
   }
   return false;
}

bool check_Win(int row, int col)
{
   if (check_horizontal_win(row, col) ||
       check_Vertical_win(row, col) || check_top_left_bottom_right_diagonal_win(row, col) || check_top_right_bottom_left_diagonal_win(row, col))
   {
      return true;
   }
   return false;
}

int game_over(void)
{
   for (int i = 0; i < SIZE; i++)
   {
      for (int j = 0; j < SIZE; j++)
      {
         if (board[i][j] != ' ')
         {

            char chip = board[i][j];
            if (check_Win(i, j))
            {
               return (int)chip;
            }
         }
      }
      if (board_is_full())
      {
         draw_board();
         mvprintw(SIZE + 2, 0, "It's a draw!");
         refresh();
         return 1;
      }
      return 0;
   }
}
void play(void)
{

   int col;
   bool game_over_status = false;

   while (!game_over_status)
   {
      draw_board();
      mvprintw(SIZE + 2, 0, "It's player's %d turn (%c)", current_player + 1, CHIP[current_player]);
      refresh();
      // get the column pressed by the user
      col = get_col();
      if (col < 0 || col > SIZE)
      {
         mvprintw(SIZE + 3, 0, "invalid choice, choose another column");
         refresh();
         continue;
      }
      // find the first empty row in the selected column
      current_player = add_coin(col, current_player);

      int result = game_over();
      if (result == 1)
      {
         draw_board();
         mvprintw(SIZE + 2, 0, "It's a draw");
         refresh();
         game_over_status = true;
      }
      else if (result != 0)
      {
         draw_board();
         mvprintw(SIZE + 2, 0, "player %d won", result);
         refresh();
         game_over_status = true;
      }
   }

   getch();
}
int main(void)
{
   INITSCREEN();
   init_board();
   play();
   DONE_SCREEN();
   return 0;
}