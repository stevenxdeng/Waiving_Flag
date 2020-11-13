/*
 * Author: Hsien-Wen "Steven" Deng
 * Subject: Bouncing Ball
 */
#include <iostream>
#include <vector>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "include/Triangle.h"
#include "include/Particle.h"
#include "Math_Matrix.h"
#include "Edge.h"
#include "Area.h"
#include "Bendable.h"

using namespace std;

char title[] = "3D Shapes with animation";
GLfloat dt = 0.1f; //time derivatives

//Viewing Cameras
GLfloat Zcamera = -50.0f;
GLfloat Xrotate = 0.0f;
GLfloat Yrotate = 0.0f;
GLfloat Zrotate = 0.0f;

GLfloat Coefficient_Stickiness = 0.5f;
GLfloat Coefficient_Restitution = 0.1f;
GLfloat k_Spring = 3000.0f;
GLfloat k_Friction = 150.0f;
GLfloat k_Area = 80.0f;
GLfloat k_Bend = 0.5f;

GLfloat wind = 2.0f;

//Particle and triangles objects declaration
int num = 0;
char Solver_mode = 'b';

std::vector<Triangle> triangles;
std::vector<Particle> Cloth;
std::vector<Edge> edges;
std::vector<Area> areas;
std::vector<Bendable> bendables;
int Cloth_ID = 0;
GLfloat particle_point_x = -10.0f;
GLfloat particle_point_y = -5.0f;

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void Keyboard(unsigned char key, int x, int y) {
    Particle particles = Particle();
    int a = 0;
    int b = 0;
    switch(key)
    {
        case 'T':
            dt *= 1.01f;
            cout << "faster and dt is "<< dt*10 <<endl;
            break;
        case 't':
            dt *= 0.99f;
            cout << "Slower and dt is "<< dt*10 <<endl;
            break;
        case 'Z':
            Zcamera += 0.5f;
            break;
        case 'z':
            Zcamera -= 0.5f;
            break;
        case 'w':
            Xrotate -= 5.0f;
            break;
        case 's':
            Xrotate += 5.0f;
            break;
        case 'a':
            Yrotate -= 5.0f;
            break;
        case 'd':
            Yrotate += 5.0f;
            break;
        case 'q':
            Zrotate -= 5.0f;
            break;
        case 'e':
            Zrotate += 5.0f;
            break;
        case 'h':
            Xrotate = 0.0f;
            Yrotate = 0.0f;
            Zrotate = 0.0f;
            Zcamera = -40.f;
            break;
        case 'r':
            //Xrotate = 0.0f;
            //Yrotate = 0.0f;
            //Zrotate = 0.0f;
            //Zcamera = -40.f;
            particle_point_x = -10.0f;
            particle_point_y = -5.0f;
            Cloth.clear();
            for(int i = 0;i < 20;i++)
            {
                for(int j = 0;j < 32;j++)
                {
                    if(i > 9)
                    {
                        particles.setcolor(0.0f,0.0f,1.0f);
                    }else{
                    particles.setcolor(1.0f,1.0f,1.0f);
                }
                    particles.setX(particle_point_x,particle_point_y,0.0f);
                    Cloth.push_back(particles);

                    particle_point_x += 0.5f;
                }
                particle_point_x = -10.0f;
                particle_point_y += 0.5f;
            }
            cout << "Reset" << endl;
            break;
        case 'X':
            wind *= 1.1f;
            cout << "Expand Wind Force to " << wind << endl;
            break;
        case 'x':
            wind *= 0.9f;
            cout << "Shrink Wind Force to " << wind << endl;
            break;
        case 'L':
            k_Spring *= 1.1f;
            cout << "Expand Spring Force to " << k_Spring << endl;
            break;
        case 'l':
            k_Spring *= 0.9f;
            cout << "Shrink Spring Force to " << k_Spring << endl;
            break;
        case 'K':
            k_Friction *= 1.1f;
            cout << "Expand Friction Force to " << k_Friction << endl;
            break;
        case 'k':
            k_Friction *= 0.9f;
            cout << "Shrink Friction Force to " << k_Friction << endl;
            break;
        case 'M':
            k_Area *= 1.1f;
            cout << "Expand Area Force to " << k_Area<< endl;
            break;
        case 'm':
            k_Area *= 0.9f;
            cout << "Shrink Area Force to " << k_Area << endl;
            break;
        case 'N':
            k_Bend *= 1.1f;
            cout << "Expand Bend Force to " << wind << endl;
            break;
        case 'n':
            k_Bend *= 0.9f;
            cout << "Shrink Bend Force to " << wind << endl;
            break;

    }

    if(Xrotate < 0)
    {
        Xrotate = 355.0f;
    }
    if(Xrotate > 360.0f)
    {
        Xrotate = 0.0f;
    }
    if(Yrotate < 0)
    {
        Yrotate = 355.0f;
    }
    if(Yrotate > 360.0f)
    {
        Yrotate = 0.0f;
    }
    if(Zrotate < 0)
    {
        Zrotate = 355.0f;
    }
    if(Zrotate > 360.0f)
    {
        Zrotate = 0.0f;
    }

    glutPostRedisplay();
}

//Solver X: update particle's position
void SolverX(GLfloat time)
{
    for(int i = 0;i < Cloth.size();i++)
    {
        Cloth[i].Reset();
        /*for(int j = 0;j < triangles.size();j++)
        {
            if(i != triangles[j].ID_1 && i != triangles[j].ID_2 && i != triangles[j].ID_3)
            {
                Cloth[i].CollisionHandle(Coefficient_Stickiness,Coefficient_Restitution,triangles[j],dt);
            }
        }*/
        if(Cloth[i].collide == false)
        {
            Cloth[i].updateX(dt);
        }
    }
}

//SolverV: Update particle's velocity
void SolverV(GLfloat time)
{
    //Compute edge force
   for(int j = 0;j < edges.size();j++)
   {
       edges[j].Edge_Force_Reset();
       edges[j].Compute_Edge_Force(k_Spring,k_Friction,Cloth[edges[j].P1],Cloth[edges[j].P2]);
       Translation('+',Cloth[edges[j].P1].F,1.0f,edges[j].F,1.0f,Cloth[edges[j].P1].F);
       Translation('-',Cloth[edges[j].P2].F,1.0f,edges[j].F,1.0f,Cloth[edges[j].P2].F);
   }

   //Compute area force
   for(int k = 0;k < areas.size();k++)
   {
       areas[k].Reset_Area_Force();
       areas[k].Compute_Area_Force(k_Area,Cloth[areas[k].P0],Cloth[areas[k].P1],Cloth[areas[k].P2]);
       Translation('+',Cloth[areas[k].P0].F,1.0f,areas[k].F_P0,1.0f,Cloth[areas[k].P0].F);
       Translation('+',Cloth[areas[k].P1].F,1.0f,areas[k].F_P1,1.0f,Cloth[areas[k].P1].F);
       Translation('+',Cloth[areas[k].P2].F,1.0f,areas[k].F_P2,1.0f,Cloth[areas[k].P2].F);
   }

   //compute bend force
   for(int l = 0;l < bendables.size();l++)
   {
       bendables[l].Reset_Bend_Force();
       bendables[l].Compute_Bend_Force(k_Bend,Cloth[bendables[l].P0],Cloth[bendables[l].P1],Cloth[bendables[l].P2],Cloth[bendables[l].P3]);
       Translation('+',Cloth[bendables[l].P0].F,1.0f,bendables[l].F_P0,1.0f,Cloth[bendables[l].P0].F);
       Translation('+',Cloth[bendables[l].P1].F,1.0f,bendables[l].F_P1,1.0f,Cloth[bendables[l].P1].F);
       Translation('+',Cloth[bendables[l].P2].F,1.0f,bendables[l].F_P2,1.0f,Cloth[bendables[l].P2].F);
       Translation('+',Cloth[bendables[l].P3].F,1.0f,bendables[l].F_P3,1.0f,Cloth[bendables[l].P3].F);
   }
   //calculate particle's total force & update velocity
   for(int m = 0;m < Cloth.size();m++)
   {
       if(m%32 != 0)
       {
         Cloth[m].updateF(wind);
         Cloth[m].Acceleration_Prioritization();
         Cloth[m].updateV(dt);
       }
   }
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display()
{
   glClearColor(0.6,0.9,0.92,0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(0.0f, 0.0f, Zcamera);  // Zooming camera
   glRotatef(Xrotate, 1.0f, 0.0f, 0.0f);  // Rotate camera
   glRotatef(Yrotate, 0.0f, 1.0f, 0.0f);
   glRotatef(Zrotate, 0.0f, 0.0f, 1.0f);

   //Leap Frog
   SolverX(dt/2.0f);
   SolverV(dt);
   SolverX(dt/2.0f);

   //Draw particles
   glPointSize(1.0f);
   glBegin(GL_POINTS);
    for(int i = 0; i < Cloth.size(); i++)
    {
        glColor3f(Cloth[i].color[0],Cloth[i].color[1],Cloth[i].color[2]);
        glVertex3f(Cloth[i].X[0],Cloth[i].X[1],Cloth[i].X[2]);
    }
   glEnd();  // End of drawing color-cube

   //Draw Lines between particles
   glLineWidth(0.5f);
   glBegin(GL_LINES);

   glColor3f(1.0f,1.0f,1.0f);
   glVertex3f(-10.0f,-50.0f,0.0f);
   glVertex3f(-10.0f,0.0f,0.0f);

   Cloth_ID = 0;
   for(int i = 0;i < 20;i++)
   {
       for(int j = 0;j < 31;j++)
       {
           if(i < 4){
             glColor3f(0.0f,0.0f,0.1f);
           }else if(i >= 4 && i < 8){
             glColor3f(1.0f,1.0f,1.0f);
           }else if(i >= 8 && i < 12)
           {
             glColor3f(0.0f,0.0f,1.0f);
           }else if(i >= 12 && i < 16)
           {
             glColor3f(1.0f,1.0f,0.0f);
           }else{
             glColor3f(1.0f,0.0f,0.0f);
           }
           glVertex3f(Cloth[Cloth_ID].X[0],Cloth[Cloth_ID].X[1],Cloth[Cloth_ID].X[2]);
           glVertex3f(Cloth[Cloth_ID+1].X[0],Cloth[Cloth_ID+1].X[1],Cloth[Cloth_ID+1].X[2]);
           Cloth_ID++;
       }
       Cloth_ID++;
   }
   Cloth_ID = 0;
   for(int k = 0;k < 19;k++)
   {
       for(int l = 0;l < 32;l++)
       {
           if(k < 4){
             glColor3f(0.0f,0.0f,0.1f);
           }else if(k >= 4 && k < 8){
             glColor3f(1.0f,1.0f,1.0f);
           }else if(k >= 8 && k < 12)
           {
             glColor3f(0.0f,0.0f,1.0f);
           }else if(k >= 12 && k < 16)
           {
             glColor3f(1.0f,1.0f,0.0f);
           }else{
             glColor3f(1.0f,0.0f,0.0f);
           }
           glVertex3f(Cloth[Cloth_ID].X[0],Cloth[Cloth_ID].X[1],Cloth[Cloth_ID].X[2]);
           glVertex3f(Cloth[Cloth_ID+32].X[0],Cloth[Cloth_ID+32].X[1],Cloth[Cloth_ID+32].X[2]);
           Cloth_ID++;
       }
   }
   Cloth_ID = 0;
   for(int i = 0;i < 19;i++)
   {
       for(int j = 0;j < 31;j++)
       {
           if(i < 4){
             glColor3f(0.0f,0.0f,0.1f);
           }else if(i >= 4 && i < 8){
             glColor3f(1.0f,1.0f,1.0f);
           }else if(i >= 8 && i < 12)
           {
             glColor3f(0.0f,0.0f,1.0f);
           }else if(i >= 12 && i < 16)
           {
             glColor3f(1.0f,1.0f,0.0f);
           }else{
             glColor3f(1.0f,0.0f,0.0f);
           }
           glVertex3f(Cloth[Cloth_ID].X[0],Cloth[Cloth_ID].X[1],Cloth[Cloth_ID].X[2]);
           glVertex3f(Cloth[Cloth_ID+33].X[0],Cloth[Cloth_ID+33].X[1],Cloth[Cloth_ID+33].X[2]);
           Cloth_ID++;
       }
       Cloth_ID++;
   }
   Cloth_ID = 0;
   for(int i = 0;i < 19;i++)
   {
       for(int j = 0;j < 31;j++)
       {
           if(i < 4){
             glColor3f(0.0f,0.0f,0.1f);
           }else if(i >= 4 && i < 8){
             glColor3f(1.0f,1.0f,1.0f);
           }else if(i >= 8 && i < 12)
           {
             glColor3f(0.0f,0.0f,1.0f);
           }else if(i >= 12 && i < 16)
           {
             glColor3f(1.0f,1.0f,0.0f);
           }else{
             glColor3f(1.0f,0.0f,0.0f);
           }
           glVertex3f(Cloth[Cloth_ID+1].X[0],Cloth[Cloth_ID+1].X[1],Cloth[Cloth_ID+1].X[2]);
           glVertex3f(Cloth[Cloth_ID+32].X[0],Cloth[Cloth_ID+32].X[1],Cloth[Cloth_ID+32].X[2]);
           Cloth_ID++;
       }
       Cloth_ID++;
   }
   glEnd();

   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

}

/* Called back when timer expired [NEW] */
void timer(int value) {
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(1, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 200.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

    //Build Particle Objects
    Particle particle = Particle();
    for(int i = 0;i < 20;i++)
    {
        for(int j = 0;j < 32;j++)
        {
            if(i > 9)
            {
              particle.setcolor(0.0f,0.0f,1.0f);
            }else{
              particle.setcolor(1.0f,1.0f,1.0f);
            }
            particle.setX(particle_point_x,particle_point_y,0.0f);
            Cloth.push_back(particle);

            particle_point_x += 0.5f;
        }
        particle_point_x = -10.0f;
        particle_point_y += 0.5f;
    }

   //Build edges
   Edge edge = Edge();
   Cloth_ID = 0;
   for(int i = 0;i < 20;i++)
   {
       for(int j = 0;j < 31;j++)
       {
           edge.SetEdge(Cloth_ID,Cloth_ID+1);
           edges.push_back(edge);
           Cloth_ID++;
       }
       Cloth_ID++;
   }
   Cloth_ID = 0;
   for(int k = 0;k < 19;k++)
   {
       for(int l = 0;l < 32;l++)
       {
           edge.SetEdge(Cloth_ID,Cloth_ID+32);
           edges.push_back(edge);
           Cloth_ID++;
       }
   }

   //Build areas & bendables
   Triangle triangle = Triangle();
   Area area = Area();
   Bendable bendable = Bendable();
   Cloth_ID = 0;
   for(int i = 0;i < 19;i++)
   {
       for(int j = 0;j < 31;j++)
       {
           edge.SetEdge(Cloth_ID,Cloth_ID+33);
           edges.push_back(edge);
           edge.SetEdge(Cloth_ID+1,Cloth_ID+32);
           edges.push_back(edge);

           area.setArea(Cloth_ID,Cloth_ID+1,Cloth_ID+32);
           areas.push_back(area);
           area.setArea(Cloth_ID+1,Cloth_ID+32,Cloth_ID+33);
           areas.push_back(area);
           area.setArea(Cloth_ID,Cloth_ID+32,Cloth_ID+33);
           areas.push_back(area);
           area.setArea(Cloth_ID+1,Cloth_ID+1,Cloth_ID+33);
           areas.push_back(area);

           bendable.setBendable(Cloth_ID,Cloth_ID+1,Cloth_ID+32,Cloth_ID+33);
           bendables.push_back(bendable);
           bendable.setBendable(Cloth_ID+1,Cloth_ID,Cloth_ID+33,Cloth_ID+32);
           bendables.push_back(bendable);

           triangle.setIDs(Cloth_ID,Cloth_ID+1,Cloth_ID+32);
           triangle.setPoints(Cloth[Cloth_ID].X[0],Cloth[Cloth_ID].X[1],Cloth[Cloth_ID].X[2],Cloth[Cloth_ID+1].X[0],Cloth[Cloth_ID+1].X[1],Cloth[Cloth_ID+1].X[2],Cloth[Cloth_ID+32].X[0],Cloth[Cloth_ID+32].X[1],Cloth[Cloth_ID+32].X[2]);
           triangle.ComputeN();
           triangles.push_back(triangle);
           triangle.setIDs(Cloth_ID+1,Cloth_ID+32,Cloth_ID+33);
           triangle.setPoints(Cloth[Cloth_ID+1].X[0],Cloth[Cloth_ID+1].X[1],Cloth[Cloth_ID+1].X[2],Cloth[Cloth_ID+32].X[0],Cloth[Cloth_ID+32].X[1],Cloth[Cloth_ID+32].X[2],Cloth[Cloth_ID+33].X[0],Cloth[Cloth_ID+33].X[1],Cloth[Cloth_ID+33].X[2]);
           triangle.ComputeN();
           triangles.push_back(triangle);


           Cloth_ID++;
       }
       Cloth_ID++;
   }

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(720, 480);   // Set the window's initial width & height
    glutInitWindowPosition(300, 200); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(Keyboard);     //Register Keyboard functions

    cout << "Rotate View:" << endl << "By X-axis: 'w' and 's'" << endl << "By Y-axis: 'a' and 'd'" << endl << "By Z-axis: 'q' and 'e'" << endl;
    cout << "Zoom-in: 'Z'" << endl << "Zoom-out: 'z'" << endl << "Home view: 'h'" << endl << "Reset: 'r'" << endl;
    cout << endl << "Cloth Parameters:" << endl;
    cout << "Expand Wind Force: " << "'X'" << endl;
    cout << "Shrink Wind Force: " << "'x'" << endl;
    cout << "Expand Spring Force: " << "'L'" << endl;
    cout << "Shrink Spring Force: " << "'l'" << endl;
    cout << "Expand Friction Force: " << "'K'" << endl;
    cout << "Shrink Friction Force: " << "'k'" << endl;
    cout << "Expand Area Force: " << "'M'" << endl;
    cout << "Shrink Area Force: " << "'m'" << endl;
    cout << "Expand Bend Force: " << "'N'" << endl;
    cout << "Shrink Bend Force: " << "'n'" << endl;



    initGL();                       // Our own OpenGL initialization
    glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
    glutMainLoop();                 // Enter the infinite event-processing loop

return 0;
}
