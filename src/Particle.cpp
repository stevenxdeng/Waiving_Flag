#include <iostream>
#include <math.h>
#include "Triangle.h"
#include "Particle.h"
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Math_Matrix.h"
using namespace std;

Particle::Particle()
{
}

void Particle::setX(GLfloat x,GLfloat y,GLfloat z)
{
    X[0] = x;
    X[1] = y;
    X[2] = z;

    Home_X[0] = x;
    Home_X[1] = y;
    Home_X[2] = z;
}

void Particle::setV(GLfloat vx, GLfloat vy, GLfloat vz)
{
    V[0] = vx;
    V[1] = vy;
    V[2] = vz;
}

void Particle::updateX(GLfloat dt)
{
    Translation('+',X,1,V,dt,X);
}

void Particle::updateV(GLfloat dt)
{
    Translation('+',V,1,F,dt,V);
}

void Particle::updateF(GLfloat wind)
{
    F[0] += wind;
    F[1] += F_external[1];
    F[2] += F_external[2];
}

void Particle::setcolor(GLfloat R, GLfloat G, GLfloat B)
{
    color[0] = R;
    color[1] = G;
    color[2] = B;
}

void Particle::CollisionHandle(GLfloat C_Stickiness,GLfloat C_Restitution,Triangle triangle,GLfloat Dt)
{
    //compute Xf = v * dt
    Translation('+',X,1.0f,V,Dt,Xf);

    fX = (X[0] - triangle.A[0]) * triangle.N[0] + (X[1] - triangle.A[1]) * triangle.N[1] + (X[2] - triangle.A[2]) * triangle.N[2];
    fXf = (Xf[0] - triangle.A[0]) * triangle.N[0] + (Xf[1] - triangle.A[1]) * triangle.N[1] + (Xf[2] - triangle.A[2]) * triangle.N[2];

    if(fX * fXf <= 0)//collide with the plane
    {
        dt = fX / (V[0] * triangle.N[0] + V[1] * triangle.N[1] + V[2] * triangle.N[2]) * (-1);
        if(dt > 0 && dt < Dt)
        {
            //Inside test via Barycentric Coordinate
            Translation('-',X,1.0f,V,dt,Xc);
            Translation('-',Xc,1.0f,triangle.A,1.0f,XcP0);

            crossproduct(triangle.e2,XcP0,e2XXcP0);
            crossproduct(triangle.e1,XcP0,e1XXcP0);

            u = dotproduct(triangle.crosse2e1,e2XXcP0);
            u /= triangle.crosse2e1_sqr;

            v = dotproduct(triangle.crosse1e2,e1XXcP0);
            v /= triangle.crosse1e2_sqr;

            if((u>=0 && u <=1) && (v>=0 && v <=1) && (u+v>=0 && u+v <=1))//collide with the triangle
            {
                collide = true;
                triangle.collide = true;

                //Set response velocity
                Scale(triangle.N, (dotproduct(V,triangle.N)), Vn);
                Translation('-',V,1.0f,Vn,1.0f,Vp);

                //Update velocity with Coefficient Stickiness 0.99 and Coefficient Restriction 0.99
                Translation('-',Vp,C_Stickiness,Vn,C_Restitution,V);
                //Translation('+',Output_V,1.0f,V,1.0f,Output_V);

                //Update Location
                Translation('+',Xc,1.0f,V,dt,X);

            }
        }
    }
}

//Constrain particles max force
void Particle::Acceleration_Prioritization()
{
    if(Length(F)>F_max)
    {
        Scale(F,F_max/Length(F),F);
    }
}

void Particle::Reset()//Reset particle state for every time stamp
{
    collide = false;
    u = 1.0f;
    v = 1.0f;
    fX = 0.0f;
    fXf = 0.0f;
    GLfloat dt = -100.0f;

    //Initialize the force
    F[0] = 0.0f;
    F[1] = 0.0f;
    F[2] = 0.0f;
}
