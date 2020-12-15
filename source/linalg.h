#ifndef LINALG_H
#define LINALG_H

#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include <cmath>
namespace Linalg{
class Vec4 {
private:
    double* vec;
public:
    Vec4(){
        vec = new double[4]();
    }

    Vec4(double x, double y, double z){
        vec = new double[4];
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = 1.0;
    }

    Vec4(const Vec4& other){
        vec = new double[4];
        for(int i = 0; i< 4; ++i)
            vec[i] = other[i];
    }

    // Vec4(const Vec3& v){
    //     Vec4(v[0],v[1], v[2]);
    // }

    double& operator[](int i) const{
        return vec[i];
    }

    Vec4& operator/=(double other){
        vec[0] /= other;
        vec[1] /= other;
        vec[2] /= other;
        vec[3] /= other;
        return *this;
    }


    double length() const{
        return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2] + vec[3]*vec[3]);
    }
    Vec4 operator-(const Vec4& other){
        return Vec4{vec[0] - other[0], vec[1] - other[1], vec[2]- other[2]};
    }

    ~Vec4(){
        delete[] vec;
    }
};


class Vec3 {
private:
    double* vec;
public:
    Vec3(){
        vec = new double[3]();
    }

    Vec3(double x, double y, double z){
        vec = new double[3];
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }
    Vec3(const Vec3& v){
        vec = new double[3];
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
    }

    Vec3(const Vec4& v){
        vec = new double[3];
        vec[0] = v[0];
        vec[1] = v[1];
        vec[2] = v[2];
    }

    double& operator[](int i) const{
        return vec[i];
    }

    double length() const{
        return sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    }

    void normalize() {
        double len = this->length();
        vec[0] /= len;
        vec[1] /= len;
        vec[2] /= len;
    }

    Vec3 operator-(const Vec3& other){
        Vec3 a{};
        a[0] = this->vec[0] - other[0];
        a[1] = this->vec[1] - other[1];
        a[2] = this->vec[2] - other[2];
        return a;
    }

    Vec3& operator*=(const double a){
        vec[0]*=a;
        vec[1]*= a;
        vec[2]*=a;
        return *this;
    }

    Vec3 operator*(const double a) const{
        Vec3 result{*this};
        result *= a;
        return result;
    }

    Vec3 operator+(const Vec3& other){
        return Vec3{vec[0] + other[0], vec[1] + other[1], vec[2]+ other[2]};
    }

    Vec3 operator-(){
        return Vec3{-vec[0], -vec[1], -vec[2]};
    }

    ~Vec3(){
        delete[] vec;
    }
};

class Mat4{
private:
    double* mat;
public:
    Mat4(){
        mat = new double[16]();
    }

    Mat4(std::initializer_list<double> l );

    Mat4(const Mat4& other){
        mat = new double[16];
        for(int i = 0; i< 16; ++i)
            mat[i] = other.mat[i];
    }


    void set(int r, int c, double val){
        mat[r*4 + c] = val;
    }

    double& at(int r, int c) const{
        return mat[r*4 + c];
    }

    friend Vec4 matmul(const Mat4& l, const Vec4& r);
    ~Mat4(){
        delete[] mat;
    }

    friend Mat4 matmul(const Mat4& lhs, const Mat4& rhs);
};


class Mat3{
private:
    double* mat;
public:
    Mat3(){
        mat = new double[9]();
    }

    Mat3(std::initializer_list<double> l );

    Mat3(const Mat3& other){
        mat = new double[9];
        for(int i = 0; i< 9; ++i)
            mat[i] = other.mat[i];
    }

    Mat3(const Mat4& other): Mat3{}{
        for (int r = 0; r< 3; ++r){
            for(int c = 0; c<3; ++c){
                mat[r*3+c] = other.at(r,c);
            }
        }
    }
    void set(int r, int c, double val){
        mat[r*3 + c] = val;
    }

    double& at(int r, int c) const{
        return mat[r*3 + c];
    }

    friend Vec3 matmul(const Mat3& l, const Vec3& r);
    ~Mat3(){
        delete[] mat;
    }

    friend Mat3 matmul(const Mat3& lhs, const Mat3& rhs);
};


double dot(const Vec4& a, const Vec4& b);
double dot(const Vec3& a, const Vec3& b);

Vec4 matmul(const Mat4& l, const Vec4& r);
Vec3 matmul(const Mat3& l, const Vec3& r);
Mat4 operator*(const Mat4& lhs, const Mat4& rhs);
Mat3 operator*(const Mat3& lhs, const Mat3& rhs);

Vec3 operator*(double, const Vec3&);

Vec3 normal(const Vec3& lhs, const Vec3& rhs);

std::ostream& operator<<(std::ostream&, const Vec4& rhs);
std::ostream& operator<<(std::ostream&, const Vec3& rhs);
}

#endif