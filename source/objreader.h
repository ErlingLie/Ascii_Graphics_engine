#include "linalg.h"
#include <vector>


using iVec = std::vector<int>;
class Object3d{


public:
    std::vector<Linalg::Vec4> vertices;
    std::vector<Linalg::Vec3> vertexNormals;
    std::vector<iVec> polygons;
    Object3d(const char* fileName);
};