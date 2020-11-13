#ifndef AREA_H
#define AREA_H

#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Particle.h"


class Area
{
    public:
        Area();
        //index of area particles
        int P0;
        int P1;
        int P2;

        //Area forces
        GLfloat F_P0[3];
        GLfloat F_P1[3];
        GLfloat F_P2[3];
        GLfloat F_max = 0.5f;

        //components
        GLfloat d0[3];
        GLfloat d1[3];
        GLfloat d2[3];
        GLfloat vd0[3];
        GLfloat vd1[3];
        GLfloat vd2[3];

        void setArea(int a,int b,int c);
        void Compute_Area_Force(GLfloat Kaf,Particle A,Particle B,Particle C);
        void Restrain_Area_Force();
        void Reset_Area_Force();
};

#endif // AREA_H
