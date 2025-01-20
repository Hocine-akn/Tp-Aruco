#include <Windows.h>

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <iostream>

#include <fstream>
#include <sstream>

#include "aruco/aruco.h"

// Stores the sphere's texture  
//GLuint texture[1];

// Angle of rotation for the sphere  
double angle = 0;

// Vertex structure to store position and texture coordinates  
typedef struct {
    float X, Y, Z;  // Vertex coordinates  
    double U, V;    // Texture coordinates  
} VERTICES;

// Constants for calculations  
const double PI = 3.1415926535897; // Value of PI  
const int space = 10;              // Angular step size for sphere segments  
const int VertexCount = (90 / space) * (360 / space) * 4; // Total vertices  
static VERTICES VERTEX[VertexCount];      // Array to store sphere vertices  


void CreateSphere(double R, double H, double K, double Z) {
    int n = 0; // Vertex counter  
    for (double b = 0; b <= 90 - space; b += space) {
        for (double a = 0; a <= 360 - space; a += space) {
            // Vertex 1  
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].U = (a) / 360;
            VERTEX[n].V = (2 * b) / 360;
            n++;

            // Vertex 2  
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].U = (a) / 360;
            VERTEX[n].V = (2 * (b + space)) / 360;
            n++;

            // Vertex 3  
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].U = (a + space) / 360;
            VERTEX[n].V = (2 * b) / 360;
            n++;

            // Vertex 4  
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].U = (a + space) / 360;
            VERTEX[n].V = (2 * (b + space)) / 360;
            n++;
        }
    }
}

void DisplaySphere(double R ){ //, GLuint texture) {
    glScalef(0.0125 * R, 0.0125 * R, 0.0125 * R); // Scale the sphere  
    glRotatef(90, 1, 0, 0); // Rotate to correct orientation  

    //glBindTexture(GL_TEXTURE_2D, texture); // Bind the texture  
    
    for (int b = 0; b <= VertexCount; b++) {
        //glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
        glBegin(GL_LINE_LOOP);
        glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
        glEnd();
    }
    for (int b = 0; b <= VertexCount; b++) {
        //glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);
        glBegin(GL_LINE_LOOP);
        glVertex3f(VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
        glEnd();
    }
    
}
