// Contains system agnostic implementations of functions for _getch() and clearScreen()
#include "Program.h"
#include <stdio.h> 

// _getch() implementation for unix
// Pause the execution and wait for an unbuffered keyboard input, then return the character and resume
// _getch() is implemented in conio.h on Windows and is declared in Program.h

#ifdef __unix__
#include <termios.h> 
#include <unistd.h> 

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
#endif //unix

// clearScreen() implementation
// the function clears the window using the appropriate shell command
#ifdef _WIN32
void clearScreen(void){
    std::system("cls");
}
#endif /* _WIN32 */

#ifdef __unix__
void clearScreen(void) {
    std::system("clear");
}
#endif /* __unix__ */