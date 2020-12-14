#include "linalg.h"
using namespace Linalg;
double Linalg::dot(const Vec4& a, const Vec4& b){
    double result = 0.0;
    for(unsigned int i = 0; i<4; ++i){
        result += a[i]*b[i];
    }
    return result;
}

double Linalg::dot(const Vec3& a,const Vec3& b){
    double result = 0.0;
    for(unsigned int i = 0; i<3; ++i){
        result += a[i]*b[i];
    }
    return result;
}

Mat4::Mat4(std::initializer_list<double> l){
    if (l.size() != 16){
        throw std::invalid_argument("Expected 16 doubles");
    }
    mat = new double[16];
    int i = 0;
    for (auto& el : l){
        mat[i++] = el;
    }
}

Mat3::Mat3(std::initializer_list<double> l){
        if (l.size() != 9){
            throw std::invalid_argument("Expected 16 doubles");
        }
        mat = new double[9];
        int i = 0;
        for (auto& el : l){
            mat[i++] = el;
        }
    }

Vec4 Linalg::matmul(const Mat4& lhs, const Vec4& rhs){
    Vec4 result{};

    for (int r = 0; r<4; ++r){
        for(int j = 0; j < 4; ++j){
            result[r] += lhs.at(r,j)*rhs[j];
        }
    }
    return result;
}

Vec3 Linalg::matmul(const Mat3& lhs, const Vec3& rhs){
    Vec3 result{};

    for (int r = 0; r<3; ++r){
        for(int j = 0; j < 3; ++j){
            result[r] += lhs.at(r,j)*rhs[j];
        }
    }
    return result;
}

Mat4 Linalg::operator*(const Mat4& lhs, const Mat4& rhs){
    Mat4 result{};

    for (int r = 0; r<4; ++r){
        for(int c = 0; c < 4; ++c){
            for(int j = 0; j < 4; ++j){
                result.at(r,c) += lhs.at(r,j)*rhs.at(j,c);
            }
        }
    }
    return result;
}

Mat3 Linalg::operator*(const Mat3& lhs, const Mat3& rhs){
    Mat3 result{};

    for (int r = 0; r<3; ++r){
        for(int c = 0; c < 3; ++c){
            for(int j = 0; j < 3; ++j){
                result.at(r,c) += lhs.at(r,j)*rhs.at(j,c);
            }
        }
    }
    return result;
}

std::ostream& Linalg::operator<<(std::ostream& os, const Vec4& rhs){
    os << rhs[0] << " " << rhs[1] << " " << rhs[2] << " " << rhs[3];
    return os;
}

std::ostream& Linalg::operator<<(std::ostream& os, const Vec3& rhs){
    os << rhs[0] << " " << rhs[1] << " " << rhs[2];
    return os;
}

Vec3 Linalg::operator*(double d, const Vec3& v){
    return v*d;
}

Vec3 Linalg::normal(const Vec3& lhs, const Vec3& rhs){
    return Vec3{lhs[1]*rhs[2] - lhs[2]*rhs[1],
                lhs[2]*rhs[0] - lhs[0]*rhs[2],
                lhs[0]*rhs[1] - lhs[1]*rhs[0]};
}