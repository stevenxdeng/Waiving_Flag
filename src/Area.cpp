#include <iostream>
#include <math.h>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>
#include "Area.h"
#include "Math_Matrix.h"
using namespace std;

Area::Area()
{
}

void Area::setArea(int a,int b,int c)
{
    P0 = a;
    P1 = b;
    P2 = c;
}

void Area::Compute_Area_Force(GLfloat Kaf,Particle A,Particle B,Particle C)
{
    //Get d0
    Translation('+',B.X,0.5f,C.X,0.5f,d0);
    Translation('-',A.X,1.0f,d0,1.0f,d0);
    Scale(d0,Length(d0),d0);

    //Get d1
    Translation('+',A.X,0.5f,C.X,0.5f,d1);
    Translation('-',B.X,1.0f,d1,1.0f,d1);
    Scale(d1,Length(d1),d1);

    //Get d2
    Translation('+',A.X,0.5f,B.X,0.5f,d2);
    Translation('-',C.X,1.0f,d2,1.0f,d2);
    Scale(d2,Length(d2),d2);

    //Get vd0,vd1,vd2
    Translation('+',B.V,0.5f,C.V,0.5f,vd0);
    Translation('-',A.V,1.0f,vd0,1.0f,vd0);
    Translation('+',A.V,0.5f,C.V,0.5f,vd1);
    Translation('-',B.V,1.0f,vd1,1.0f,vd1);
    Translation('+',A.V,0.5f,B.V,0.5f,vd2);
    Translation('-',C.V,1.0f,vd2,1.0f,vd2);

    //Calculate Force
    Scale(d0,(-1.0f*Kaf)*dotproduct(d0,vd0),F_P0);
    Scale(d1,(-1.0f*Kaf)*dotproduct(d1,vd1),F_P1);
    Scale(d2,(-1.0f*Kaf)*dotproduct(d0,vd2),F_P2);

    //Restrain_Area_Force();
}

void Area::Restrain_Area_Force()
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
}

void Area::Reset_Area_Force()
{
    F_P0[0] = 0.0f;
    F_P0[1] = 0.0f;
    F_P0[2] = 0.0f;
    F_P1[0] = 0.0f;
    F_P1[1] = 0.0f;
    F_P1[2] = 0.0f;
    F_P2[0] = 0.0f;
    F_P2[1] = 0.0f;
    F_P2[2] = 0.0f;
}
