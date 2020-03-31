#pragma once
#include <stdio.h>

class Vector {
public:
    double data[3];
    Vector();
    Vector(double, double);
    void Print(void);
    void Set(double d1,double d2);
    double GetX();
    double GetY();
};

class Matrix {
public:
    double data[3][3];
    Matrix();
    void Print(void);
    Matrix operator*(const Matrix);
    friend Vector operator*(const Matrix,const Vector);
};
