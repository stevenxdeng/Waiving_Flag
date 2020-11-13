#ifndef PARTICLE_H
#define PARTICLE_H

#include "Triangle.h"
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

class Particle
{
    public:
        Particle();
        GLfloat X[3]; //position
        GLfloat V[3]; //velocity
        GLfloat F[3];
        GLfloat F_max = 0.8f;
        GLfloat F_external[3]={0.0f,-0.9f,0.0f};
        GLfloat color[3];
        bool collide = false;

        //GLfloat Identity[3][3] = {1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f};

        GLfloat Xf[3]; //Xf = X + v*dt
        GLfloat fX = 0.0f; //f(x)
        GLfloat fXf = 0.0f; //f(xf)

        GLfloat dt = -100.0f; //time to collide
        GLfloat Xc[3]; //location to collide

        //parameters to computer u and v by using Barycentric Coordinates
        GLfloat XcP0[3];
        GLfloat e2XXcP0[3];
        GLfloat e1XXcP0[3];
        GLfloat u = 1.0f;
        GLfloat v = 1.0f;
        //Response Velocity
        GLfloat Vn[3]; //Vr perpendicular
        GLfloat Vp[3]; //Vr parallel

        GLfloat Home_X[3];

        //Basic function
        void setX(GLfloat x,GLfloat y,GLfloat z);
        void setV(GLfloat vx,GLfloat vy,GLfloat vz);
        void updateX(GLfloat dt);
        void updateV(GLfloat dt);
        void updateF(GLfloat wind);
        void setcolor(GLfloat R,GLfloat G,GLfloat B);

        //Collision Handle function
        void CollisionHandle(GLfloat C_Stickiness,GLfloat C_Restitution,Triangle triangle, GLfloat Dt);
        void Acceleration_Prioritization(void);

        //Matrices math function
        void Reset();
};

#endif // PARTICLE_H
