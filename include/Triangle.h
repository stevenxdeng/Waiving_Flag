#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

class Triangle
{
   public:
        Triangle();
        int ID_1;
        int ID_2;
        int ID_3;

        GLfloat A[3]; //P0
        GLfloat B[3]; //P1
        GLfloat C[3]; //P2
        GLfloat color[3];

        //Normal and Barycentric Coordinate
        GLfloat N[3];
        GLfloat e1[3];
        GLfloat e2[3];
        bool collide = false;

        GLfloat crosse2e1[3];
        GLfloat crosse2e1_sqr;
        GLfloat crosse1e2[3];
        GLfloat crosse1e2_sqr;

        void setIDs(int A,int B,int C);
        void setPoints(GLfloat Ax,GLfloat Ay,GLfloat Az,GLfloat Bx,GLfloat By,GLfloat Bz,GLfloat Cx,GLfloat Cy,GLfloat Cz);
        void setcolor(GLfloat R,GLfloat G,GLfloat B);
        void ComputeN(void);
};

#endif // TRIANGLE_H
