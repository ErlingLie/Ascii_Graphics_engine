#include "objreader.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace Linalg;
Object3d::Object3d(const char* filename){
    std::ifstream ifs{filename};
    if (!ifs){
        throw "Could not open file";
    }
    string mode;
    ifs >> mode;
    while (ifs){
        if(mode == "v"){
            double x,y,z;
            ifs >> x>>y>>z;
            vertices.push_back(Vec4{x,y,z});
        }
        else if(mode == "vn"){
            double x,y,z;
            ifs >> x>>y>>z;
            vertexNormals.push_back(Vec3{x,y,z});
        }
        else if(mode == "f"){
            int x,y,z,xn,yn,zn, t;
            char tmp;
            ifs >> x>>tmp>>t>>tmp>>xn >> y>>tmp>>t>>tmp>>yn>>z>>tmp>>t>>tmp>>zn;
            polygons.push_back(iVec{x-1,y-1,z-1,xn-1,yn-1,zn-1});
        }
        ifs >> mode;

    }
    ifs.close();
}