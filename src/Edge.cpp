#include <iostream>
#include <math.h>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Edge.h"
#include "Particle.h"
#include "Math_Matrix.h"
using namespace std;

Edge::Edge()
{
}

void Edge::SetEdge(int a,int b)
{
    P1 = a;
    P2 = b;
}

void Edge::Compute_Edge_Force(GLfloat Ks,GLfloat Kf,Particle A,Particle B)
{
    Compute_Spring_Force(Ks,A,B);
    Compute_Friction_Force(Kf,A,B);
    Translation('+',F_Spring,1.0f,F_Friction,1.0f,F);
    //Restrain_Edge_Force();
}

void Edge::Compute_Spring_Force(GLfloat Ks,Particle A,Particle B)
{
    Translation('-',A.X,1.0f,B.X,1.0f,F_Spring);
    F_Spring_Scaler = (-1.0f*Ks) * (Distance(A.X,B.X) - Distance(A.Home_X,B.Home_X)) / Distance(A.X,B.X);
    Scale(F_Spring,F_Spring_Scaler,F_Spring);
}

void Edge::Compute_Friction_Force(GLfloat Kf,Particle A,Particle B)
{
    Translation('-',A.X,1.0f,B.X,1.0f,F_Friction);
    Scale(F_Friction,1.0f/Distance(A.X,B.X),nXab);
    Translation('-',A.V,1.0f,B.V,1.0f,dvab);
    F_Friction_Scaler = (-1.0f*Kf) * dotproduct(nXab,dvab) / Distance(A.X,B.X);
    Scale(F_Friction,F_Friction_Scaler,F_Friction);
}

void Edge::Restrain_Edge_Force()
{
    if(Length(F)>F_max)
    {
        Scale(F,F_max/Length(F),F);
    }
}

void Edge::Edge_Force_Reset()
{
    F[0] = 0.0f;
    F[1] = 0.0f;
    F[2] = 0.0f;
}
