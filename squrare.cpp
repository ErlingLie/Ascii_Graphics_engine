// #include <iostream>
// #include <cstdlib>
// #include <Windows.h>
// #include <vector>
// #include <cmath>
// #include <cstdio>
// #include "linalg.h"
// #include <chrono>


// constexpr double pi = 3.141592;

// using namespace Linalg;
// using std::vector;

// using ivec = vector<int>;
// int main(){
    
//     int screenWidth = 100;
//     int screenHeight = 50;


//     const double kd = .8;
//     const double ks = 1.0-kd;

//     wchar_t* screenBuffer = new wchar_t[screenHeight*screenWidth];
//     HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr);
//     SetConsoleActiveScreenBuffer(hConsole);
//     DWORD dwBytesWritten = 0;

//     Vec3 lightDir{1.0, -0.5, 1.0};
//     lightDir.normalize();


//     vector<Vec4> cube{Vec4{1.0, 1.0, 1.0},
//                          Vec4{1.0, 1.0, -1.0},
//                          Vec4{1.0, -1.0, 1.0},
//                          Vec4{-1.0, 1.0, 1.0},
//                          Vec4{-1.0, 1.0, -1.0},
//                          Vec4{-1.0, -1.0, 1.0},
//                          Vec4{1.0, -1.0, -1.0},
//                          Vec4{-1.0, -1.0, -1.0}};
//     vector<ivec> polygons{ivec{0,1,2}, ivec{1,6,2}, ivec{1,7,6},
//                             ivec{1,4,7}, ivec{0,3,1}, ivec{3,4,1},
//                             ivec{0,2,5}, ivec{0,5,3}, ivec{4,5,7}, ivec{4,3,5}};


//     std::vector<std::vector<double>> zBuffer(screenWidth,std::vector<double>(screenHeight, -10.0));

//     double theta = 0;


//     Mat4 translationMatrix{
//             1.0,.0,.0,.0,
//             .0,1.0,.0,.0,
//             .0,.0,1.0,10,
//             .0,.0,.0,1.0
//         };

//     Mat4 perspectiveMatrix{
//         1.0, 0.0, 0.0, 0.0,
//         0.0, 1.0, 0.0, 0.0,
//         0.0, 0.0, 1.0, 0.0,
//         0.0, 0.0, 1.0, 0.0
//     };

//     auto p1 = std::chrono::system_clock::now();
//     auto p2 = std::chrono::system_clock::now();
//     while (true){
//         //Clear buffers
//         for (auto& column : zBuffer){
//             for (auto& pixel : column){
//                 pixel = -10.0;
//             }
//         }

//         for (int i = 0; i < screenWidth*screenHeight; ++i){
//             screenBuffer[i] = L' ';
//         }


//         p2 = std::chrono::system_clock::now();
//         std::chrono::duration<double> diff = p2 - p1;
//         double elapsedTime = diff.count();
//         p1 = p2;
//         theta = theta< 2*pi ? theta + 1*elapsedTime : 0.0;


//         Mat4 rotationMatrix{
//             cos(theta), 0.0, sin(theta), .0,
//             .0, 1.0, .0, .0,
//             -sin(theta), 0, cos(theta), 0,
//             0.0, 0.0, 0.0, 1.0
//         };

//         Mat4 totalMatrix = perspectiveMatrix*translationMatrix* rotationMatrix;

//         // for(auto& vertex : cube){
//         //     //std::cout <<"Original vertex: " << vertex <<"\n";
//         //     Vec4 v = matmul(totalMatrix, vertex);
//         //    // std::cout << "Transformed vertex: " << v << "\n";
//         //     newPositions.push_back(matmul(totalMatrix, vertex));
//         // }

//         for(auto& vertex: cube){
//             Vec4 pVec = matmul(totalMatrix, vertex);
//             //std::cout << pVec << "\n";
//             pVec /= pVec[3];
//             int x = screenWidth/2 + static_cast<int>(pVec[0]*screenWidth);
//             int y = screenHeight/2 - static_cast<int>(pVec[1]*screenWidth);
//             screenBuffer[x+ y*screenWidth] = L'#';
//         }



//                 // // Lighting
//                 // std::vector<double> normal{x/radius, y/radius, z/radius};
//                 // double diffuse = max(dot(normal, light), 0.0);

//                 // std::vector<double> v = normalized_vector(-x, -y, -z - K2);

//                 // std::vector<double> h = normalized_vector((light[0] + v[0])/2, 
//                 //                                          (light[1] + v[1])/2,
//                 //                                          (light[2] + v[2])/2);
                

//                 // double specular = dot(normal, h);
//                 // double spec = 1;
//                 // for (int i{0}; i<10; ++i){
//                 //     spec*= specular;
//                 // }
//                 // int L = static_cast<int>(12*(kd*diffuse + ks*spec));
//                 // if (L >= 0){
//                 //     // Screen coordinates
//                 //     double ooz = 1/(z + K2);
//                 //     int xp = static_cast<int>(screenWidth/2 + K1*ooz*x);
//                 //     int yp = static_cast<int>(screenHeight/2 - K3*ooz*y);
//                 //     if (ooz > zBuffer[xp][yp]){
//                 //         zBuffer[xp][yp] = ooz;
//                 //         screenBuffer[xp + yp*screenWidth] = L".,-~:;=!*#$@"[L];
//                 //     }
//                 // }

//         screenBuffer[screenWidth*screenHeight - 1] = '\0';
//         WriteConsoleOutputCharacterW(hConsole, screenBuffer, screenWidth*screenHeight, {0,0}, &dwBytesWritten);
//     }
//     delete[] screenBuffer;
//     return 0;

// }