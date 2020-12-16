#include "draw.h"
#include "drawObj.h"
#include <signal.h>

#ifdef __unix__
    #include <sys/ioctl.h>
    #include <unistd.h>
#else
    #include <windows.h>
#endif


void my_handler(int s){
    //Shows cursor again
    printf("\e[?25h");
    exit(1);
}
int main(int argc, char* argv[]){
    int columns, rows;

    #ifdef __unix__
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        rows = w.ws_col;
        columns = w.ws_row;
    #else
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #endif
    signal (SIGINT,my_handler);
    //ConsoleDrawer c{200, 100};

    //Remove cursor
    printf("\e[?25l");

    //Run Program
    DrawObject c{rows, columns, "f16.obj"};
    c.drawLoop();
}
