#include<math.h>
#include "Triangle.h"
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Math_Matrix.h"

Triangle::Triangle()
{
}

void Triangle::setIDs(int A,int B,int C)
{
    ID_1 = A;
    ID_2 = B;
    ID_3 = C;
}

void Triangle::setPoints(GLfloat Ax,GLfloat Ay,GLfloat Az,GLfloat Bx,GLfloat By,GLfloat Bz,GLfloat Cx,GLfloat Cy,GLfloat Cz)
{
    A[0] = Ax;
    A[1] = Ay;
    A[2] = Az;
    B[0] = Bx;
    B[1] = By;
    B[2] = Bz;
    C[0] = Cx;
    C[1] = Cy;
    C[2] = Cz;
}

void Triangle::setcolor(GLfloat R,GLfloat G,GLfloat B)
{
    color[0] = R;
    color[1] = G;
    color[2] = B;
}

void Triangle::ComputeN()//Compute normal and barycentric coordinates
{
  Translation('-',B,1.0f,A,1.0f,e1);
  Translation('-',C,1.0f,A,1.0f,e2);
  crossproduct(e1,e2,N);
  Scale(N,1.0f/Length(N),N);

  crosse2e1[0] = e2[1] * e1[2] - e2[2] * e1[1];
  crosse2e1[1] = e2[2] * e1[0] - e2[0] * e1[2];
  crosse2e1[2] = e2[0] * e1[1] - e2[1] * e1[0];

  crosse1e2[0] = e1[1] * e2[2] - e1[2] * e2[1];
  crosse1e2[1] = e1[2] * e2[0] - e1[0] * e2[2];
  crosse1e2[2] = e1[0] * e2[1] - e1[1] * e2[0];

  crosse2e1_sqr = pow(crosse2e1[0],2) + pow(crosse2e1[1],2) + pow(crosse2e1[2],2);
  crosse1e2_sqr = pow(crosse1e2[0],2) + pow(crosse1e2[1],2) + pow(crosse1e2[2],2);
}


