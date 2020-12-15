#ifndef DRAW_H
#define DRAW_H

#include "linalg.h"
#include <vector>


using iVec = std::vector<int>;
class ConsoleDrawer{
protected:
    int screenWidth;
    int screenHeight;
    wchar_t* screenBuffer;
    double* zBuffer;

    std::vector<Linalg::Vec4> vertexVector;
    std::vector<iVec> polygons;

    std::vector<Linalg::Vec4> modelPositions;
    std::vector<Linalg::Vec3> screenVertices;

    Linalg::Vec3 lightDirection;

    double kd;
    double ks;

    virtual void resetBuffers();

    virtual void processRaster(int xp, int yp,
        const Linalg::Vec3& lambda, const iVec& poly);

    void rasterizeTriangle(const iVec& poly);

    virtual Linalg::Vec3 calculateNormal(const iVec& poly, const Linalg::Vec3& lambda);

    void transformVertices(const Linalg::Mat4& totalMatrix, const Linalg::Mat4& modelMatrix);

    Linalg::Vec3 makeLine(const Linalg::Vec3& a, const Linalg::Vec3& b);

    void writeBuffer();
public:
    ConsoleDrawer(int width, int height);
    virtual void drawLoop();

    ~ConsoleDrawer(){
        delete[] zBuffer;
        delete[] screenBuffer;
    }
};


#endif

