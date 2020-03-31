#include "vecmat.h"

Vector::Vector() {
 data[0]=0.0; data[1]=0.0; data[2]=1.0;
}

Vector::Vector(double d1, double d2) {
    Set(d1, d2);
}

void Vector::Print(void) {
 printf("(%2.3lf,%2.3lf,%2.3lf)\n",data[0],data[1],data[2]);
}

void Vector::Set(double d1,double d2) {
 data[0]=d1; data[1]=d2;
}

double Vector::GetX() {
 return data[0];
}

double Vector::GetY() {
 return data[1];
}

Vector operator*(const Matrix gMatrix,const Vector gVector)
{
 unsigned int i,j;
 Vector tmp;

 for (i=0;i<3;i++)
  {
   tmp.data[i]=0.0;
   for (j=0;j<3;j++) tmp.data[i]=tmp.data[i]+(gMatrix.data[i][j]*gVector.data[j]);
  }
 return tmp;
}
