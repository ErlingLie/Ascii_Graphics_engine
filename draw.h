#ifndef DRAW_H
#define DRAW_H

#include <Windows.h>
#include "linalg.h"
#include <vector>


using iVec = std::vector<int>;
class ConsoleDrawer{
protected:
    int screenWidth;
    int screenHeight;
    wchar_t* screenBuffer;
    double* zBuffer;

    HANDLE hConsole;
    DWORD dwBytesWritten;

    std::vector<Linalg::Vec4> vertexVector;
    std::vector<iVec> polygons;

    // std::vector<Linalg::Vec4> pyramid;
    // std::vector<iVec> pyramidPolygons;

    std::vector<Linalg::Vec4> modelPositions;

    Linalg::Vec3 lightDirection;


    double kd;
    double ks;
    

    void resetBuffers();
    bool backFaceCull(const Linalg::Vec3& normalVec);
    void processRaster(int xp, int yp,
    const Linalg::Vec3& lambda, int v0, int v1, int v2, const Linalg::Vec3& normalVec);
    void rasterizeTriangle(int a, int b, int c, const std::vector<Linalg::Vec3>&);

    Linalg::Vec3 makeLine(Linalg::Vec3 a, Linalg::Vec3 b);
public:
    ConsoleDrawer();

    

    void drawLoop();

    ~ConsoleDrawer(){
        delete[] zBuffer;
        delete[] screenBuffer;
    }
};


#endif

