#ifndef BENDABLE_H
#define BENDABLE_H
#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Particle.h"

class Bendable
{
    public:
        Bendable();
        //Index of Bendable particles
        int P0;
        int P1;
        int P2;
        int P3;

        //Bend forces
        GLfloat F_P0[3];
        GLfloat F_P1[3];
        GLfloat F_P2[3];
        GLfloat F_P3[3];
        GLfloat F_max = 0.2f;

        //Bend angle
        GLfloat theta;
        GLfloat theta_h = 3.14159f;

        //Center of mass & torque
        GLfloat Xcm[3];
        GLfloat r0[3];
        GLfloat r1[3];
        GLfloat r2[3];
        GLfloat r3[3];

        //Components
        GLfloat P1_P[3];
        GLfloat P2_P[3];
        GLfloat n0[3];
        GLfloat n1[3];
        GLfloat q0;
        GLfloat q3;
        GLfloat costheta;
        GLfloat sintheta;

        void setBendable(int a,int b,int c,int d);
        void Compute_Bend_Force(GLfloat Kb,Particle A,Particle B,Particle C,Particle D);
        void Restrain_Bend_Force();
        void Reset_Bend_Force();
};

#endif // BENDABLE_H
