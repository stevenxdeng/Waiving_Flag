#include "Math_Matrix.h"
#include <iostream>
#include <math.h>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
using namespace std;


GLfloat Length(GLfloat input[3])//Length of vector
{
    return sqrt(pow(input[0],2) + pow(input[1],2) + pow(input[2],2));
}

GLfloat Distance(GLfloat A[3],GLfloat B[3])//Distance of two particles
{
    return sqrt(pow((B[0]-A[0]),2) + pow((B[1]-A[1]),2) + pow((B[2]-A[2]),2));
}

GLfloat dotproduct(GLfloat inputa[3],GLfloat inputb[3])
{
    return (inputa[0] * inputb[0] + inputa[1] * inputb[1] + inputa[2] * inputb[2]);
}

void crossproduct(GLfloat inputa[3],GLfloat inputb[3],GLfloat output[3])
{
    output[0] = inputa[1] * inputb[2] - inputa[2] * inputb[1];
    output[1] = inputa[2] * inputb[0] - inputa[0] * inputb[2];
    output[2] = inputa[0] * inputb[1] - inputa[1] * inputb[0];
}

void Outerproduct(GLfloat inputa[3],GLfloat inputb[3],GLfloat output[3][3])
{
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            output[i][j] = inputa[i] * inputb[j];
        }
    }
}

void Rotation(GLfloat RotateMatrix[3][3],GLfloat input[3])
{
    input[0] = RotateMatrix[0][0] * input[0] + RotateMatrix[0][1] *  input[1] + RotateMatrix[0][2] * input[2];
    input[1] = RotateMatrix[1][0] * input[0] + RotateMatrix[1][1] *  input[1] + RotateMatrix[1][2] * input[2];
    input[2] = RotateMatrix[2][0] * input[0] + RotateMatrix[2][1] *  input[1] + RotateMatrix[2][2] * input[2];
}

void Scale(GLfloat inputa[3],GLfloat inputb,GLfloat output[3])
{
    output[0] = inputa[0] * inputb;
    output[1] = inputa[1] * inputb;
    output[2] = inputa[2] * inputb;
}

void Scale3X3(GLfloat input[3][3],GLfloat scaler,GLfloat output[3][3])
{
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            output[i][j] = input[i][j] * scaler;
        }
    }
}

//Translate the particle
void Translation(char mode,GLfloat inputa[3],GLfloat scalea,GLfloat inputb[3],GLfloat scaleb,GLfloat output[3])
{
    if(mode == '+')
    {
        output[0] = inputa[0] * scalea + inputb[0] * scaleb;
        output[1] = inputa[1] * scalea + inputb[1] * scaleb;
        output[2] = inputa[2] * scalea + inputb[2] * scaleb;
    }else if(mode == '-'){
        output[0] = inputa[0] * scalea - inputb[0] * scaleb;
        output[1] = inputa[1] * scalea - inputb[1] * scaleb;
        output[2] = inputa[2] * scalea - inputb[2] * scaleb;
    }else{
        cout << "Illegal Translation" << endl;
    }
}

void Translation3X3(char mode,GLfloat inputa[3][3],GLfloat inputb[3][3],GLfloat output[3][3])
{
    if(mode == '+')
    {
        for(int i = 0;i < 3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                output[i][j] = inputa[i][j] + inputb[i][j];
            }
        }
    }else if(mode == '-'){
        for(int i = 0;i < 3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                output[i][j] = inputa[i][j] - inputb[i][j];
            }
        }
    }else{
        cout << "Illegal Translation" << endl;
    }
}

