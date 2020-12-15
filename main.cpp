#include "draw.h"
#include "drawObj.h"

int main(){
    //ConsoleDrawer c{200, 100};
    DrawObject c{200, 100, "bunny.obj"};
    c.drawLoop();

}