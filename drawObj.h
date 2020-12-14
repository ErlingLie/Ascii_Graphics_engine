#ifndef DRAWOBJ_H
#define DRAWOBJ_H

#include "draw.h"
#include "objreader.h"
class DrawObject : public ConsoleDrawer{
private:

    std::vector<Linalg::Vec3> vertexNormals;
    std::vector<Linalg::Vec3> transformedNormals;

    void processRaster(int xp, int yp,
     const Linalg::Vec3& lambda, iVec idxes);
    
    void resetBuffers();

    void rasterizeTriangle(iVec poly, const std::vector<Linalg::Vec3>&);
public:
    DrawObject() : ConsoleDrawer(){
        Object3d reader{"f16.obj"};
        vertexVector = reader.vertices;
        polygons = reader.polygons;
        vertexNormals = reader.vertexNormals;
        for (auto& v : polygons){
            std::cout << v[0] << " " << v[1] << " " << v[2] << "\n";
        }
    }



    void drawLoop();




};

#endif