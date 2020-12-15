#include "draw.h"
#include "drawObj.h"

#ifdef __unix__
    #include <sys/ioctl.h>
    #include <unistd.h>
#else
    #include <windows.h>
#endif

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

    //ConsoleDrawer c{200, 100};
    DrawObject c{rows, columns, "bunny.obj"};
    c.drawLoop();
}
