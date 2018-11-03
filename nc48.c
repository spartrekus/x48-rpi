
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
int rows, cols;


////////////////////////////////
void nruncmd( char *thecmd   )
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
       reset_prog_mode();
}


void printdir()
{
   DIR *dirp; int posy = 0;  int posx, chr ; 
   posy = 1; posx = cols/2;
   char cwd[PATH_MAX];
   struct dirent *dp;
   dirp = opendir( "." );
   attron( A_REVERSE );
   mvprintw( posy, cols/2, "DIR: %s", getcwd( cwd, PATH_MAX ) );
   while  ((dp = readdir( dirp )) != NULL ) 
   if ( posy <= rows-3 )
   {
        if (  strcmp( dp->d_name, "." ) != 0 )
        if (  strcmp( dp->d_name, ".." ) != 0 )
        {
            posy++; 
            for ( chr = 0 ;  chr <= strlen(dp->d_name) ; chr++) 
            {
              if  ( dp->d_name[chr] == '\n' )
                  posx = cols/2;
              else if  ( dp->d_name[chr] == '\0' )
                  posx = cols/2;
              else
                 mvaddch( posy, posx++ , dp->d_name[chr] );
            }
        }
   }
   closedir( dirp );
}




void gfxframe( int y1, int x1, int y2, int x2 )
{
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
        mvaddch( fooy , foo , ACS_VLINE );
    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
         mvaddch( fooy , foo , ACS_VLINE );
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    mvaddch( y1 , x1 , ACS_ULCORNER );
    mvaddch( y1 , x2 , ACS_URCORNER );
    mvaddch( y2 , x1 , ACS_LLCORNER );
    mvaddch( y2 , x2 , ACS_LRCORNER );
}


void mvcenter( int myposypass, char *mytext )
{
      mvprintw( myposypass, cols/2 - strlen( mytext )/2  , "%s", mytext );
}

void gfxcls()
{
   attroff( A_REVERSE );
   attroff( A_BOLD );
   color_set( 0, NULL );
   int foo, fooy , foox ;
   int y1 = 0; int x1=0; 
   int y2 = rows-1; int x2= cols -1 ;
   foo = x1;
   color_set( 7, NULL ); attron( A_REVERSE ); 
   for( fooy = y1 ; fooy <= y2 ; fooy++) 
   for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );
   color_set( 7, NULL ); attron( A_REVERSE ); 
   gfxframe( 0, 0 , rows-1, cols -1 );
}

void main_draw()
{
    attroff( A_REVERSE );
    attroff( A_BOLD );
    color_set( 0, NULL );
    int foo, fooy , foox ;
    int y1 = 0; int x1=0; 
    int y2 = rows-1; int x2= cols -1 ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    for( foox = x1 ; foox <= x2 ; foox++) 
    {
      attron( A_REVERSE );
      color_set( 7, NULL );
      mvprintw( fooy , foox , "A" );
      refresh();
      attroff( A_REVERSE );
      color_set( 0, NULL );
      mvprintw( fooy , foox , " " );
    }
   color_set( 7, NULL ); attron( A_REVERSE ); 
   for( fooy = y1 ; fooy <= y2 ; fooy++) 
   for( foox = x1 ; foox <= x2 ; foox++) 
      mvprintw( fooy , foox , " " );
   color_set( 7, NULL ); attron( A_REVERSE ); 
   gfxframe( 0, 0 , rows-1, cols -1 );
   attroff( A_BOLD ); mvcenter( rows*50/100 -1, "[ NBLUE Press Key ]" );
   attroff( A_BOLD ); mvcenter( rows*50/100 -8, "[ F7: Fetch x48, Press Key ]" );
   attroff( A_BOLD ); mvcenter( rows*50/100 -7, "[ F5: Fetch rom, Press Key ]" );
   attroff( A_BOLD ); mvcenter( rows*50/100 -5, "[ F6: Show Dir, Press Key ]" );
   attroff( A_BOLD ); mvcenter( rows*50/100 -4, "[ x:  run x48, Press Key ]" );
}



int main()
{

  initscr();	
  getmaxyx( stdscr, rows, cols);
  curs_set( 0 );
  noecho();            
  keypad( stdscr, TRUE );  // for F...
  start_color();
  init_pair(0,  COLOR_WHITE,     COLOR_BLACK);
  init_pair(1,  COLOR_RED,     COLOR_BLACK);
  init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
  init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
  init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
  init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
  init_pair(7,  COLOR_BLUE,    COLOR_WHITE);


   int ch  ; 

   int gameover = 0;
   while ( gameover == 0 )
   {
      main_draw();
      ch = getch();
      switch ( ch ) 
      {
         case 'q':  
            gameover = 1; 
            break;
         case KEY_F(1):
         case KEY_F(2):
         case KEY_F(3):
         case KEY_F(4):
            mvprintw( 2,1, "%d", ch );
            break;

         case KEY_F(10):
            gameover = 1; 
            break;


         case KEY_F(5):
            // http://www.linuxfocus.org/common/src/article319/x48-gxrom-r.tar.gz
            nruncmd( " cd ; wget http://www.linuxfocus.org/common/src/article319/x48-gxrom-r.tar.gz -O x48-gxrom-r.tar.gz " ) ; 

            nruncmd( " cd ; tar xvpfz  x48-gxrom-r.tar.gz " ) ; 
            gfxcls();
            mvprintw( rows-2, 5, "Download completed" );
            getch(); 
            break;


         case KEY_F(6):
         case 'd':
            gfxcls();
            printdir();
            mvprintw( rows-2, 5, "Dir completed" );
            getch(); 
            break;

         case KEY_F(7):
            gfxcls();
            nruncmd(  "wget https://raw.githubusercontent.com/spartrekus/x48-rpi/master/x48.x86 -O x48 ; chmod uog+rx x48 " );
            mvprintw( rows-2, 5, "Dir completed" );
            getch(); 
            break;

         case 'x':
            gfxcls();
            //nruncmd(  " ./x48 ");
            nruncmd( " xlsfonts ; ./x48 -connFont -misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-1     -smallFont -misc-fixed-medium-r-semicondensed--0-0-75-75-c-0-iso8859-1   -mediumFont -misc-fixed-medium-r-semicondensed--13-100-100-100-c-60-iso8859-1  -largeFont -misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1     " );
            mvprintw( rows-2, 5, "x48 completed" );
            getch(); 
            break;

      }
    }


   curs_set( 4 ) ;
   attroff( A_BOLD ); attroff( A_REVERSE ); curs_set( 1 );
   endwin();		/* End curses mode  */
   return 0;
}





