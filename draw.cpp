#include "draw.h"
#include "linalg.h"
#include <chrono>


using namespace Linalg;


ConsoleDrawer::ConsoleDrawer(int width, int height):
screenWidth{width},
screenHeight{height},
screenBuffer{new wchar_t[screenHeight*screenWidth]},
zBuffer{new double[screenHeight*screenWidth]},
hConsole{CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr)},
dwBytesWritten{0},
lightDirection{1, -10, 5},
kd{0.8},
ks{1-kd}
{
    SetConsoleActiveScreenBuffer(hConsole);

    // vertexVector = std::vector<Vec4>{Vec4{1.0, 1.0, 1.0},
    //                      Vec4{1.0, 1.0, -1.0},
    //                      Vec4{1.0, -1.0, 1.0},
    //                      Vec4{-1.0, 1.0, 1.0},
    //                      Vec4{-1.0, 1.0, -1.0},
    //                      Vec4{-1.0, -1.0, 1.0},
    //                      Vec4{1.0, -1.0, -1.0},
    //                      Vec4{-1.0, -1.0, -1.0}};
    // polygons = std::vector<iVec>{iVec{1,4,7}, iVec{0,1,2}, iVec{1,6,2}, iVec{1,7,6},
    //                      iVec{0,3,1}, iVec{3,4,1},
    //                     iVec{0,2,5}, iVec{0,5,3}, iVec{4,5,7}, iVec{4,3,5}};


    vertexVector = std::vector<Vec4>{Vec4{1.0, -1.0, 1.0},
                         Vec4{-1.0, -1.0, 1.0},
                         Vec4{1.0, -1.0, -1.0},
                         Vec4{-1.0, -1.0, -1.0},
                         Vec4{0, 1.0, 0}};
    polygons = std::vector<iVec>{iVec{0, 3, 2}, iVec{0, 1, 3},
                                    iVec{2, 3, 4}, iVec{0, 2, 4},
                                    iVec{1, 4, 3}, iVec{0, 4, 1}};
    lightDirection.normalize();
}


void ConsoleDrawer::resetBuffers(){
    for (int i = 0; i < screenWidth*screenHeight; ++i){
        screenBuffer[i] = L' ';
        zBuffer[i] = 100.0;
    }
    modelPositions = std::vector<Vec4>{};
    screenVertices = std::vector<Vec3>{};
}

Vec3 ConsoleDrawer::makeLine(const Vec3& a, const Vec3& b){
    Vec3 diff{ b[1] - a[1], a[0] - b[0], 0};
    diff[2] = -diff[0]*a[0] - diff[1]*a[1];
    return diff;
}

Linalg::Vec3 ConsoleDrawer::calculateNormal(const iVec& poly, const Linalg::Vec3& lambda){
    Vec3 p0 = modelPositions[poly[0]];
    Vec3 p1 = modelPositions[poly[1]];
    Vec3 p2 = modelPositions[poly[2]];
    Vec3 normalVec = normal(p1-p0, p2-p0);
    normalVec.normalize();
    return normalVec;
}

void ConsoleDrawer::processRaster(int xp, int yp,
     const Vec3& lambda, const iVec& poly){
    Vec3 p0 = modelPositions[poly[0]];
    Vec3 p1 = modelPositions[poly[1]];
    Vec3 p2 = modelPositions[poly[2]];
    Vec3 pos{lambda[0]*p0 + lambda[1]*p1 + lambda[2]*p2};

    Vec3 normalVec{calculateNormal(poly, lambda)};    

    Vec3 eyeVec = -pos;
    eyeVec.normalize();


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


void ConsoleDrawer::rasterizeTriangle(const iVec& poly){
    Vec3 a = screenVertices[poly[0]];
    Vec3 b = screenVertices[poly[1]];
    Vec3 c = screenVertices[poly[2]];

    auto l0 = makeLine(a,b);
    auto l1 = makeLine(b,c);
    auto l2 = makeLine(c,a);

    double e0, e1, e2, e0t, e1t, e2t;

    int bbXMin = static_cast<int>(min(b[0], min(a[0], c[0])));
    int bbXMax = static_cast<int>(max(a[0], max(b[0], c[0])));
    int bbYMin = static_cast<int>(min(a[1], min(b[1], c[1])));
    int bbYMax = static_cast<int>(max(a[1], max(b[1], c[1])));
    double area = (l1[0]*a[0] + l1[1]*a[1] + l1[2]);

    e0 = l0[0]*bbXMin + l0[1]*bbYMin + l0[2];
    e1 = l1[0]*bbXMin + l1[1]*bbYMin + l1[2];
    e2 = l2[0]*bbXMin + l2[1]*bbYMin + l2[2];

    for (int y=bbYMin; y<=bbYMax; ++y){
        e0t = e0; e1t = e1; e2t = e2;
        for (int x=bbXMin; x<= bbXMax; ++x){
            if (e0<0 == e1 < 0 && e0<0 ==e2 < 0){
                Vec3 lambda{e1/area, e2/area,e0/area};
                processRaster(x,y,lambda,poly);
            }
            e0 += l0[0];
            e1 += l1[0];
            e2 += l2[0];
        }
        e0 = e0t + l0[1];
        e1 = e1t + l1[1];
        e2 = e2t + l2[1];
    }
}


void ConsoleDrawer::writeBuffer(){
    screenBuffer[screenWidth*screenHeight - 1] = '\0';
    WriteConsoleOutputCharacterW(hConsole, screenBuffer, screenWidth*screenHeight, {0,0}, &dwBytesWritten);
}

void ConsoleDrawer::transformVertices(const Mat4& totalMatrix, const Mat4& modelMatrix){
    for(const auto& vertex: vertexVector){
        modelPositions.push_back(matmul(modelMatrix, vertex));
        Vec4 pVec = matmul(totalMatrix, vertex);
        pVec /= pVec[3];
        double x = screenWidth/2 - pVec[0]*screenWidth;
        double y = screenHeight/2 - pVec[1]*screenHeight;
        screenVertices.push_back(Vec3{x,y, pVec[2]});
    }
}


constexpr double pi = 3.141592;
void ConsoleDrawer::drawLoop(){
    
    Mat4 scaleMatrix{
        30.0,.0,.0,.0,
        .0, 30.0,.0, 0.,
        .0,.0, 30.0, 0,
        .0,.0,.0,1.0
    };
    Mat4 translationMatrix{
            1.0,.0,.0,.0,
            .0,1.0,.0, -5,
            .0,.0,1.0, 10,
            .0,.0,.0,1.0
        };

    Mat4 camRotation{
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(-20.0*pi/180), -sin(-20.0*pi/180), 0.0,
        0.0, sin(-20.0*pi/180), cos(-20.0*pi/180), 0.0,
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

    double theta = -pi/4;
    while (true){
        resetBuffers();

        p2 = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = p2 - p1;
        double elapsedTime = diff.count();
        p1 = p2;
        theta = theta< 2*pi ? theta + 0.1*elapsedTime : 0.0;


        Mat4 rotationMatrix{
            cos(theta), 0.0, sin(theta), .0,
            .0, 1.0, .0, .0,
            -sin(theta), 0, cos(theta), 0,
            0.0, 0.0, 0.0, 1.0
        };

        Mat4 modelMatrix = translationMatrix* rotationMatrix*scaleMatrix;
        Mat4 totalMatrix = perspectiveMatrix*modelMatrix;

        transformVertices(totalMatrix, modelMatrix);

        for(const auto& poly : polygons){
            rasterizeTriangle(poly);
        }
        writeBuffer();
    }
}
