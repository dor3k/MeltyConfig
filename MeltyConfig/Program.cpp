#include "Program.h"
#include <termios.h> 
#include <unistd.h> 
#include <stdio.h> 


// _getch() implementation
#ifdef _WIN32
#include <conio.h>
#include <tchar.h>
#endif //win32
#ifdef __unix__
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


// clearing the temrinal window implementation
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