#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

GLfloat Length(GLfloat input[3]);
GLfloat Distance(GLfloat A[3],GLfloat B[3]);

GLfloat dotproduct(GLfloat inputa[3],GLfloat inputb[3]);
void crossproduct(GLfloat inputa[3],GLfloat inputb[3],GLfloat output[3]);
void Outerproduct(GLfloat inputa[3],GLfloat inputb[3],GLfloat output[3][3]);

void Rotation(GLfloat RotateMatrix[3][3],GLfloat input[3]);

void Scale(GLfloat inputa[3],GLfloat inputb,GLfloat output[3]);
void Scale3X3(GLfloat input[3][3],GLfloat scaler,GLfloat output[3][3]);
void Translation(char mode,GLfloat inputa[3],GLfloat scalea,GLfloat inputb[3],GLfloat scaleb,GLfloat output[3]);
void Translation3X3(char mode,GLfloat inputa[3][3],GLfloat inputb[3][3],GLfloat output[3][3]);

#endif // MATH_MATRIX_H
