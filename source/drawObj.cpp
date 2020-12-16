#include "drawObj.h"
#include <chrono>

using namespace Linalg;

void DrawObject::resetBuffers(){
    ConsoleDrawer::resetBuffers();
    transformedNormals = {};
}

Vec3 DrawObject::calculateNormal(const iVec& poly, const Linalg::Vec3& lambda){
    if (transformedNormals.size() == 0){
        return ConsoleDrawer::calculateNormal(poly, lambda);
    }
    Vec3 n0 = transformedNormals[poly[3]];
    Vec3 n1 = transformedNormals[poly[4]];
    Vec3 n2 = transformedNormals[poly[5]];

    Vec3 normalVec{lambda[0]*n0 + lambda[1]*n1 + lambda[2]*n2};
    normalVec.normalize();
    return normalVec;
}



void DrawObject::transformVertices(const Mat4& totalMatrix, 
                 const Mat4& modelMatrix, const Mat3& normalMatrix){
    ConsoleDrawer::transformVertices(totalMatrix, modelMatrix);
    for(const auto& n: vertexNormals){
        transformedNormals.push_back(matmul(normalMatrix, n));
    }
}


constexpr double pi = 3.141592;
void DrawObject::drawLoop(){
    
    double scale = 2;
    Mat4 scaleMatrix{
        scale,.0,.0,.0,
        .0, scale,.0, 0.,
        .0,.0, scale, 0,
        .0,.0,.0,1.0
    };

    Mat4 translationMatrix{
            1.0,.0,.0,.0,
            .0,1.0,.0, -8,
            .0,.0,1.0, 10,
            .0,.0,.0,1.0
        };

    const double viewAngle = -35;
    Mat4 camRotation{
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(viewAngle*pi/180), -sin(viewAngle*pi/180), 0.0,
        0.0, sin(viewAngle*pi/180), cos(viewAngle*pi/180), 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    Mat4 perspectiveMatrix = Mat4{
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0
    }*camRotation;

    auto p1 = std::chrono::system_clock::now();
    auto p2 = std::chrono::system_clock::now();
    double theta = 0;

    double fps = 60.0;
    double low_pass_alpha = 0.9;
    while (true){
        resetBuffers();
        p2 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = p2 - p1;
        double elapsedTime = diff.count();
        fps = low_pass_alpha*fps + (1-low_pass_alpha)*1/elapsedTime;
        printf("\033]0; Asci graphics (FPS: %.2f)\007", fps);

        p1 = p2;
        theta = theta< 2*pi ? theta + 0.3*elapsedTime : 0.0;


        Mat4 rotationMatrix{
            cos(theta), 0.0, sin(theta), .0,
            .0, 1.0, .0, .0,
            -sin(theta), 0, cos(theta), 0,
            0.0, 0.0, 0.0, 1.0
        };

        Mat4 modelMatrix = translationMatrix* rotationMatrix*scaleMatrix;
        Mat4 totalMatrix = perspectiveMatrix*modelMatrix;
        Mat3 normalMatrix{modelMatrix};
        
        transformVertices(totalMatrix, modelMatrix, normalMatrix);

        for(const auto& poly : polygons){
            rasterizeTriangle(poly);
        }
        writeBuffer();

    }
}
