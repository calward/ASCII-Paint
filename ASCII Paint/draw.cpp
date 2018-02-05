#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 40
#define WIDTH  80
#define BORDER '/'

using namespace std;

typedef enum color{
   placeholder, //needed to increment the colors by 1
   white,
   green,
   yellow,
   blue,
   magenta,
   red,
   cyan
} color_t;

class Cursor {
public:
  int x;
  int y;
  int quit;
  bool painting;
  char screen[HEIGHT][WIDTH];
  color_t color[HEIGHT][WIDTH];
  color_t av_color;
  char avatar;
  Cursor(int, int);
};

Cursor::Cursor(int x, int y){
    this->x = x;
    this->y = y;
    this->quit = 0;
    this->painting = 0;
    this->avatar = '@';
    this->av_color = white;
}
 
void init_screen(Cursor *cursor);

void quit(Cursor *cursor);

void handle_input(Cursor *cursor);

void print_screen(Cursor *cursor);

void print_messages(Cursor *cursor);

void paint(Cursor *cursor, int dir);

void move(Cursor *cursor, int dir);

int main(int argc, char *argv[])
{
  Cursor cursor(1,1);
 
  init_screen(&cursor);
   
  while(1)
    {
      handle_input(&cursor);

      if(cursor.quit)
	{
	  return -1;
	}
    }
    
  endwin();

  return 0;
}

void init_screen(Cursor *cursor)
{
  int i, j;

  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  start_color();
  init_pair(1,COLOR_WHITE, COLOR_BLACK);
  init_pair(2,COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);


  for(i = 0; i < HEIGHT; i++)
    {
      for(j = 0; j < WIDTH; j++)
	{
          cursor->color[i][j] = white;

	  if(i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
	    {
              cursor->color[i][j] = cyan;
	      cursor->screen[i][j] = BORDER;
	    }

	  else
	  {
	    cursor->screen[i][j] = ' ';
	  }
	}
    }

  print_screen(cursor);
}

void quit(Cursor *cursor)
{
  int i, j;
  endwin();	 
  for(i = 0; i < HEIGHT; i++)
     {
      for(j = 0; j < WIDTH; j++)
         {
          if(cursor->screen[i][j] != BORDER)
            {
              putchar(cursor->screen[i][j]);
            }
          }
          putchar('\n');
      }
}

void handle_input(Cursor *cursor)
{
  int key;

    switch(key = getch())
	{
	case 'Q':
	  quit(cursor);
	  cursor->quit = 1;
	  break;
	case 'C':
          cursor->avatar = '@';
          cursor->y = 1;
          cursor->x = 1;
          cursor->av_color = white;
	  cursor->painting = 0;
	  init_screen(cursor);
	  break;

        case 'g':
          cursor->av_color = green;
          print_screen(cursor);
        break;

        case 'b':
          cursor->av_color = blue;
          print_screen(cursor);
        break;

        case 'r':
          cursor->av_color = red;
          print_screen(cursor);
        break;

        case 'w':
          cursor->av_color = white;
          print_screen(cursor);
        break;

        case 'c':
          cursor->av_color = cyan;
          print_screen(cursor);
        break;

        case 'm':
          cursor->av_color = magenta;
          print_screen(cursor);
        break;

        case 'y':
          cursor->av_color = yellow;
	  print_screen(cursor);
        break;

	case 'p':
	  if(cursor->painting == 0)
	    {
	      cursor->painting = 1;
              cursor->screen[cursor->y][cursor->x] = cursor->avatar;
	    }
	  else
	    {
	      cursor->painting = 0;
	    }

	  cursor->color[cursor->y][cursor->x] = cursor->av_color;
	  print_screen(cursor);
	  break;

	case KEY_DOWN:
	  if(cursor->painting == 1)
	    {
	      paint(cursor, 1);
	    }
	  else
	    {
	      move(cursor, 1);	      
	    }
	  	       
	   break;
	case KEY_UP:
	  if(cursor->painting == 1)
	    {
	      paint(cursor, 2);
	    }
	  else
	    {
	      move(cursor, 2);	      
	    }
	  	       
	   break;
	case KEY_LEFT:
	   if(cursor->painting == 1)
	    {
	      paint(cursor, 3);
	    }
	  else
	    {
	      move(cursor, 3);	      
	    }
	  	       
	   break;
	case KEY_RIGHT:
	   if(cursor->painting == 1)
	    {
	      paint(cursor, 4);
	    }
	  else
	    {
	      move(cursor, 4);	      
	    }
	  	       
	   break;

	 default:

           if(int(key) > 32 && int(key) < 127)
            {
              cursor->avatar = key;
            } 
      
            print_screen(cursor);	   
	}
}

void print_screen(Cursor *cursor)
{
  int i, j;

   for(i = 0; i < HEIGHT; i++)
    {
      for(j = 0; j < WIDTH; j++)
	{
          attron(COLOR_PAIR(cursor->color[i][j])); 
	  mvaddch(i, j, cursor->screen[i][j]);
          attroff(COLOR_PAIR(cursor->color[i][j]));

	  if(i == cursor->y && j == cursor->x)
	    {
              attron(COLOR_PAIR(cursor->av_color));
	      mvaddch(i, j, cursor->avatar);
              attroff(COLOR_PAIR(cursor->av_color));
	    }
	}

      mvaddch(i, j, '\n');
    }  
   
   print_messages(cursor);
 
   refresh();
}

void print_messages(Cursor *cursor)
{
   attron(COLOR_PAIR(1));
   mvprintw(HEIGHT/4 + 6, WIDTH + 15, "Press w to change color to white");
   
   attron(COLOR_PAIR(2));
   mvprintw(HEIGHT/4 + 7, WIDTH + 15, "Press g to change color to green");

   attron(COLOR_PAIR(3));
   mvprintw(HEIGHT/4 + 8, WIDTH + 15, "Press y to change color to yellow");
   
   attron(COLOR_PAIR(4));
   mvprintw(HEIGHT/4 + 9, WIDTH + 15, "Press b to change color to blue");

   attron(COLOR_PAIR(5));
   mvprintw(HEIGHT/4 + 10, WIDTH + 15, "Press m to change color to magenta");

   attron(COLOR_PAIR(6));
   mvprintw(HEIGHT/4 + 11, WIDTH + 15, "Press r to change color to red");
 
   attron(COLOR_PAIR(7));
   mvprintw(HEIGHT/4 + 12, WIDTH + 15, "Press c to change color to cyan");   

   attron(COLOR_PAIR(1));
   mvprintw(HEIGHT / 4 + 1, WIDTH + 15, "Use arrow keys to move your cursor");

   mvprintw(HEIGHT / 4 + 2, WIDTH + 15, "Press any printable character to change your cursor");

   mvprintw(HEIGHT / 4 + 3, WIDTH + 15, "Press p to toggle painting");

   mvprintw(5, WIDTH + 15, "Painting is");

   attron(COLOR_PAIR(1));
   mvprintw(HEIGHT/4 + 14, WIDTH + 15, "Press C to clear");

   mvprintw(HEIGHT/4 + 15, WIDTH + 15, "Press Q to quit"); 
  
  if(cursor->painting == 1)
  {
    attron(COLOR_PAIR(2));
    mvprintw(5, WIDTH + 30, "ON");
  }

  else
  {
    attron(COLOR_PAIR(6));
    mvprintw(5, WIDTH + 30, "OFF");
  }

   attroff(COLOR_PAIR(1));  
}

void move(Cursor *cursor, int dir)
{  
  if(dir == 1 && cursor->y + 1 < HEIGHT - 1)
    {
      cursor->y += 1;	 
    }
    
  else if(dir == 2 && cursor->y - 1 > 0)
    {
      cursor->y -= 1;	 
    }
  else if(dir == 3 && cursor->x - 1 > 0)
    {
      cursor->x -= 1;	 
    }
   else if(dir == 4 && cursor->x + 1 < WIDTH - 1)
    {
      cursor->x += 1;	 
    }

  print_screen(cursor);
}

void paint(Cursor *cursor, int dir)
{
  cursor->screen[cursor->y][cursor->x] = cursor->avatar;
  
  cursor->color[cursor->y][cursor->x] = cursor->av_color;

  if(dir == 1 && cursor->y + 1 < HEIGHT - 1) //down
    {
      cursor->screen[cursor->y + 1][cursor->x] = cursor->avatar;
      cursor->y += 1;
     
    }
  else if(dir == 2 && cursor->y - 1 > 0)// up
    {
      cursor->screen[cursor->y - 1][cursor->x] = cursor->avatar;
      cursor->y -= 1;
    }

  else if(dir == 3 && cursor->x - 1 > 0) //left
    {
      cursor->screen[cursor->y][cursor->x - 1] = cursor->avatar;
      cursor->x -= 1;
    }

  else if(dir == 4 && cursor->x + 1 < WIDTH - 1)//right
    {
      cursor->screen[cursor->y][cursor->x + 1] = cursor->avatar;
      cursor->x += 1;
    }

  cursor->color[cursor->y][cursor->x] = cursor->av_color;
  print_screen(cursor);
}
