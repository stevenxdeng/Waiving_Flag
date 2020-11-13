#ifndef EDGE_H
#define EDGE_H

#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Particle.h"

class Edge
{
    public:
       Edge();
       int P1;
       int P2;

       GLfloat F[3];
       GLfloat F_max = 5.0f;

       GLfloat F_Spring[3];
       GLfloat F_Spring_Scaler;

       GLfloat F_Friction[3];
       GLfloat F_Friction_Scaler;
       GLfloat nXab[3];
       GLfloat dvab[3];

       void SetEdge(int a,int b);
       void Compute_Edge_Force(GLfloat Ks,GLfloat Kf,Particle A,Particle B);
       void Compute_Spring_Force(GLfloat Ks,Particle A,Particle B);
       void Compute_Friction_Force(GLfloat Kf,Particle A,Particle B);
       void Restrain_Edge_Force();
       void Edge_Force_Reset();
};

#endif // EDGE_H
