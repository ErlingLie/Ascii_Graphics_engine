#include "drawObj.h"
#include <chrono>

using namespace Linalg;

void DrawObject::resetBuffers(){
    ConsoleDrawer::resetBuffers();
    transformedNormals = {};
}

void DrawObject::processRaster(int xp, int yp,
     const Vec3& lambda, iVec idxes){
    Vec3 p0 = modelPositions[idxes[0]];
    Vec3 p1 = modelPositions[idxes[1]];
    Vec3 p2 = modelPositions[idxes[2]];
    Vec3 pos{lambda[0]*p0 + lambda[1]*p1 + lambda[2]*p2};
    Vec3 eyeVec = -pos;
    eyeVec.normalize();

    Vec3 n0 = transformedNormals[idxes[3]];
    Vec3 n1 = transformedNormals[idxes[4]];
    Vec3 n2 = transformedNormals[idxes[5]];

    Vec3 normalVec{lambda[0]*n0 + lambda[1]*n1 + lambda[2]*n2};
    normalVec.normalize();


    double diffuse = max(dot(normalVec, -lightDirection), 0.0);


    Vec3 h = (-lightDirection + eyeVec)*0.5;
    h.normalize();
    

    double specular = dot(normalVec, h);
    double spec = 1;
    for (int i{0}; i<2; ++i){
        spec*= specular;
    }
    int L = static_cast<int>(12*(kd*diffuse + ks*spec));
    if (L >= 0){
        // Screen coordinates
        int idx = xp + screenWidth*yp;
        if (idx > screenWidth*screenHeight || idx < 0){
            return;
        }
        if (pos[2] < zBuffer[idx]){
            zBuffer[idx] = pos[2];
            screenBuffer[idx] = L".,-~:;=!*#$@"[L];
        }
    }
}



// void DrawObject::rasterizeTriangle(iVec poly){

//     Vec3 a = screenVertices[poly[0]];
//     Vec3 b = screenVertices[poly[1]];
//     Vec3 c = screenVertices[poly[2]];


//     auto l0 = makeLine(a,b);
//     auto l1 = makeLine(b,c);
//     auto l2 = makeLine(c,a);

//     double e0, e1, e2, e0t, e1t, e2t;


//     int bbXMin = static_cast<int>(min(b[0], min(a[0], c[0])));
//     int bbXMax = static_cast<int>(max(a[0], max(b[0], c[0])));
//     int bbYMin = static_cast<int>(min(a[1], min(b[1], c[1])));
//     int bbYMax = static_cast<int>(max(a[1], max(b[1], c[1])));
//     double area = (l1[0]*a[0] + l1[1]*a[1] + l1[2]);

//     e0 = l0[0]*bbXMin + l0[1]*bbYMin + l0[2];
//     e1 = l1[0]*bbXMin + l1[1]*bbYMin + l1[2];
//     e2 = l2[0]*bbXMin + l2[1]*bbYMin + l2[2];

//     for (int y=bbYMin; y<=bbYMax; ++y){
//         e0t = e0; e1t = e1; e2t = e2;
//         for (int x=bbXMin; x<= bbXMax; ++x){
//             if (e0<0 == e1 < 0 && e0<0 ==e2 < 0){
//                 Vec3 lambda{e1/area, e2/area,e0/area};
//                 processRaster(x,y,lambda, poly);
//             }
//             e0 += l0[0];
//             e1 += l1[0];
//             e2 += l2[0];
//         }
//         e0 = e0t + l0[1];
//         e1 = e1t + l1[1];
//         e2 = e2t + l2[1];
//     }
// }


void DrawObject::transformVertices(const Mat4& totalMatrix, 
                 const Mat4& modelMatrix, const Mat3& normalMatrix){
    ConsoleDrawer::transformVertices(totalMatrix, modelMatrix);

    for(const auto& n: vertexNormals){
        transformedNormals.push_back(matmul(normalMatrix, n));
    }
}


constexpr double pi = 3.141592;
void DrawObject::drawLoop(){
    

    Mat4 scaleMatrix{
        3.0,.0,.0,.0,
        .0, 3.0,.0, 0.,
        .0,.0, 3.0, 0,
        .0,.0,.0,1.0
    };

    Mat4 translationMatrix{
            1.0,.0,.0,.0,
            .0,1.0,.0, -3,
            .0,.0,1.0, 10,
            .0,.0,.0,1.0
        };

    Mat4 camRotation{
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(-10.0*pi/180), -sin(-10.0*pi/180), 0.0,
        0.0, sin(-10.0*pi/180), cos(-10.0*pi/180), 0.0,
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
    while (true){
        resetBuffers();

        p2 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = p2 - p1;
        double elapsedTime = diff.count();
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
