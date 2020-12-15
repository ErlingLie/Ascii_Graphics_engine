#ifndef DRAWOBJ_H
#define DRAWOBJ_H

#include "draw.h"
#include "objreader.h"
class DrawObject : public ConsoleDrawer{
private:

    std::vector<Linalg::Vec3> vertexNormals;
    std::vector<Linalg::Vec3> transformedNormals;

    void resetBuffers() override;
    void processRaster(int xp, int yp,
        const Linalg::Vec3& lambda, iVec idxes) override;
    
    
    void transformVertices(const Linalg::Mat4& totalMatrix,
         const Linalg::Mat4& modelMatrix, const Linalg::Mat3& normalMatrix);
    //void rasterizeTriangle(iVec poly);
public:
    DrawObject(int width, int height, const char* filename) : ConsoleDrawer(width, height){
        Object3d reader{filename};
        vertexVector = reader.vertices;
        polygons = reader.polygons;
        vertexNormals = reader.vertexNormals;
    }

    void drawLoop() override;
};

#endif