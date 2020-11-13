#include <iostream>
#include <math.h>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Bendable.h"
#include "Math_Matrix.h"
using namespace std;

Bendable::Bendable()
{
}

 void Bendable::setBendable(int a,int b,int c,int d)
 {
     P0 = a;
     P1 = b;
     P2 = c;
     P3 = d;
 }

 void Bendable::Compute_Bend_Force(GLfloat Kb,Particle A,Particle B,Particle C,Particle D)
 {
     //Get Center of mass position
     Translation('+',Xcm,1.0f,A.X,1.0f,Xcm);
     Translation('+',Xcm,1.0f,B.X,1.0f,Xcm);
     Translation('+',Xcm,1.0f,C.X,1.0f,Xcm);
     Translation('+',Xcm,1.0f,D.X,1.0f,Xcm);
     Scale(Xcm,0.25f,Xcm);

     //Get r0,r1,r2,r3
     Translation('-',A.X,1.0f,Xcm,1.0f,r0);
     Translation('-',B.X,1.0f,Xcm,1.0f,r1);
     Translation('-',C.X,1.0f,Xcm,1.0f,r2);
     Translation('-',D.X,1.0f,Xcm,1.0f,r3);

     //Get n0
     Translation('-',B.X,1.0f,A.X,1.0f,P1_P);
     Translation('-',C.X,1.0f,A.X,1.0f,P2_P);
     crossproduct(P1_P,P2_P,n0);
     Scale(n0,Length(n0),n0);

     //Get n1
     Translation('-',B.X,1.0f,D.X,1.0f,P1_P);
     Translation('-',C.X,1.0f,D.X,1.0f,P2_P);
     crossproduct(P1_P,P2_P,n1);
     Scale(n1,Length(n1),n1);

     //Get Theta
     costheta = dotproduct(n0,n1);
     Translation('-',C.X,1.0f,B.X,1.0f,P1_P);
     Scale(P1_P,Length(P1_P),P1_P);
     crossproduct(n0,n1,P2_P);
     sintheta = dotproduct(P1_P,P2_P);
     theta = atan2(sintheta,costheta);

     //Get q0,q3
     Translation('-',r1,1.0f,r2,1.0f,P1_P);
     Translation('-',r2,1.0f,r0,1.0f,P2_P);
     q0 = dotproduct(P1_P,P2_P) / pow(Distance(r1,r2),2);
     Translation('-',r2,1.0f,r3,1.0f,P2_P);
     q3 = dotproduct(P1_P,P2_P) / pow(Distance(r1,r2),2);

     //Get Bend forces
     Scale(n0,Kb*(theta-theta_h),F_P0);
     Scale(n1,Kb*(theta-theta_h),F_P3);
     Translation('+',F_P0,q0,F_P3,q3,F_P1);
     Translation('+',F_P0,(q0+1.0f)*(-1.0f),F_P3,(q3+1.0f)*(-1.0f),F_P2);

     //Restrain_Bend_Force();
 }

void Bendable::Restrain_Bend_Force()
{
    if(Length(F_P0)>F_max)
    {
        Scale(F_P0,F_max/Length(F_P0),F_P0);
    }
    if(Length(F_P1)>F_max)
    {
        Scale(F_P1,F_max/Length(F_P1),F_P1);
    }
    if(Length(F_P2)>F_max)
    {
        Scale(F_P2,F_max/Length(F_P2),F_P2);
    }
    if(Length(F_P3)>F_max)
    {
        Scale(F_P3,F_max/Length(F_P3),F_P3);
    }
}

void Bendable::Reset_Bend_Force()
{
    for(int i = 0;i < 3;i++)
    {
        F_P0[i] = 0.0f;
        F_P1[i] = 0.0f;
        F_P2[i] = 0.0f;
        F_P3[i] = 0.0f;
        Xcm[i] = 0.0f;
    }
}
