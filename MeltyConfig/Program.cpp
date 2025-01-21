#include "Program.h"

#include <termios.h> 
#include <unistd.h> 
#include <stdio.h> 


//Cross-platform getch implementation
char _getch(void) 
{ 
    struct termios oldattr, newattr; 
    char ch; 
    tcgetattr(STDIN_FILENO, &oldattr); 
    newattr = oldattr; 
    newattr.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); 
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); 
    return ch; 
}

