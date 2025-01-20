//
//  ArUco-OpenGL.cpp
//
//  Created by Jean-Marie Normand on 28/02/13.
//  Copyright (c) 2013 Centrale Nantes. All rights reserved.
//


#include "ArUco-OpenGL.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\calib3d.hpp>
#include "Sphere.h"

// JPEG Lib

// TEXTURE
unsigned long x;
unsigned long y;
unsigned short int bpp; //bits per pixels   unsigned short int
GLbyte* texData;             //the data of the image
GLuint ID;                //the id ogl gives it
unsigned long size_txt;     //length of the file
int channels;      //the channels of the image 3 = RGA 4 = RGBA
GLuint type;
GLuint textureID = 0;


// Constructor
ArUco::ArUco(string intrinFileName, float markerSize) {
   // Initializing attributes
   m_IntrinsicFile= intrinFileName;
   m_MarkerSize   = markerSize;
   // read camera parameters if passed
   m_CameraParams.readFromXMLFile(intrinFileName);
  
}

// Destructor
ArUco::~ArUco() {}

void ArUco::resizeCameraParams(cv::Size newSize) {
	m_CameraParams.resize(newSize);
}

// Detect marker and draw things
void ArUco::doWork(Mat inputImg) {
   m_InputImage   = inputImg;
   m_GlWindowSize = m_InputImage.size();
   m_CameraParams.resize(m_InputImage.size());
   resize(m_GlWindowSize.width, m_GlWindowSize.height);
}

// Draw axis function
void ArUco::drawAxis(float axisSize) {
   // X
   glColor3f (1,0,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(axisSize,0.0f, 0.0f); // ending point of the line
   glEnd( );
   
   // Y
   glColor3f (0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, axisSize, 0.0f); // ending point of the line
   glEnd( );
   
   // Z
   glColor3f (0,0,1);
   glBegin(GL_LINES);
   glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
   glVertex3f(0.0f, 0.0f, axisSize); // ending point of the line
   glEnd( );
}

void ArUco::drawWireCube(GLdouble size) {
    drawBox(size, GL_LINE_LOOP);
}

// Fonction qui dessine un cube de différentes manières (type)
void ArUco::drawBox(GLfloat size, GLenum type)
{
		static const GLfloat n[6][3] =
		{
		  {-1.0, 0.0, 0.0},
		  {0.0, 1.0, 0.0},
		  {1.0, 0.0, 0.0},
		  {0.0, -1.0, 0.0},
		  {0.0, 0.0, 1.0},
		  {0.0, 0.0, -1.0}
		};
		static const GLint faces[6][4] =
		{
		  {0, 1, 2, 3},
		  {3, 2, 6, 7},
		  {7, 6, 5, 4},
		  {4, 5, 1, 0},
		  {5, 6, 2, 1},
		  {7, 4, 0, 3}
		};
		GLfloat v[8][3];
		GLint i;

		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

		for (i = 5; i >= 0; i--) {
			glBegin(type);
			glNormal3fv(&n[i][0]);
			glVertex3fv(&v[faces[i][0]][0]);
			glVertex3fv(&v[faces[i][1]][0]);
			glVertex3fv(&v[faces[i][2]][0]);
			glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		}
}
void ArUco::drawPanda() {
	// On va dessiner en blanc
	glColor3f(1.0f, 1.0f, 1.0f);
	cout << "snowmannn" << endl;
	// Corps du Panda
	//glTranslatef(0.0f, 0.75f, 0.0f);
	//DisplaySphere(1.f);
	drawBox(0.75f*m_MarkerSize, GL_QUADS);
	//glutSolidSphere(0.75f, 20, 20);
	
	// Tete du panda
	glColor3f(0.9f, 0.9f, 0.9f);
	glTranslatef(0.0f, 0.0f, 0.75f * m_MarkerSize);
	drawBox(m_MarkerSize*0.5, GL_QUADS);
	//DisplaySphere(0.25f);
	
	// gluSolidSphere(0.25f, 20, 20);

	// Yeux du bonhomme du Panda : deux spheres noires
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(m_MarkerSize*0.1f, m_MarkerSize*0.25f, m_MarkerSize*0.10f);
	drawBox(m_MarkerSize * 0.1f, GL_QUADS);
	//glutSolidSphere(0.05f, 10, 10);
	glTranslatef(m_MarkerSize *-0.2f, 0.0f, 0.0f);
	drawBox(m_MarkerSize * 0.1f, GL_QUADS);
	//glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	//oreilles du panda
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(m_MarkerSize*0.25f , 0 , m_MarkerSize * 0.10f);
	drawBox(m_MarkerSize * 0.1f, GL_QUADS);


	glTranslatef(m_MarkerSize*-0.5f,0,0);
	drawBox(m_MarkerSize * 0.1f, GL_QUADS);
	glPopMatrix();

	
}

// GLUT functionnalities

// Drawing function
void ArUco::drawScene() {
   // If we do not have an image we don't do anyhting
   if (m_ResizedImage.rows==0)
      return;
   
   // On "reset" les matrices OpenGL de ModelView et de Projection
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // On deinit une vue orthographique de la taille de l'image OpenGL
   glOrtho(0, m_GlWindowSize.width, 0, m_GlWindowSize.height, -1.0, 1.0);
   // on definit le viewport correspond a un rendu "plein ecran"
   glViewport(0, 0, m_GlWindowSize.width , m_GlWindowSize.height);

   // on desactive les textures
   glDisable(GL_TEXTURE_2D);

   // On "flippe" l'axe des Y car OpenCV et OpenGL on un axe Y inverse pour les images/textures
   glPixelZoom( 1, -1);

   // On definit la position ou l'on va ecrire dans l'image
   glRasterPos3f(0, m_GlWindowSize.height, -1.0f);
   
   // On "dessine" les pixels contenus dans l'image OpenCV m_ResizedImage (donc l'image de la Webcam qui nous sert de fond)
   glDrawPixels (m_GlWindowSize.width, m_GlWindowSize.height, GL_RGB, GL_UNSIGNED_BYTE, m_ResizedImage.ptr(0));
   
   // On active ensuite le depth test pour les objets 3D
   glEnable(GL_DEPTH_TEST);
   
   // On passe en mode projection pour definir la bonne projection calculee par ArUco
   glMatrixMode(GL_PROJECTION);
   double proj_matrix[16];
   m_CameraParams.glGetProjectionMatrix(m_ResizedImage.size(),m_GlWindowSize,proj_matrix,0.01,100);
   glLoadIdentity();
   // on charge la matrice d'ArUco 
   glLoadMatrixd(proj_matrix);
   
   // On affiche le nombre de marqueurs (ne sert a rien)
   double modelview_matrix[16];
   std::cout << "Number of markers: " << m_Markers.size() << std::endl;
   
   // On desactive le depth test
   glDisable(GL_DEPTH_TEST);

   // Pour chaque marqueur detecte
   for (unsigned int m=0;m<m_Markers.size();m++)
   {
	   // On recupere la matrice de modelview qui correspond au marqueur [m]
       m_Markers[m].glGetModelViewMatrix(modelview_matrix);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
	   // on charge cette matrice pour se placer dans le repere de ce marqueur [m] 
       glLoadMatrixd(modelview_matrix);
      
      // On dessine les axes X Y Z
      //drawAxis(m_MarkerSize);

      // On se deplace sur Z de la moitie du marqueur pour dessiner "sur" le plan du marqueur
      glTranslatef(0, 0, m_MarkerSize/2.);
     
	  // On sauvegarde la matrice courante
      glPushMatrix();
	  // on choisit une couleur
	  glColor3f(1, 0.4f, 0.4f);

	  //On dessine une cube en fil de fer
      //drawWireCube( m_MarkerSize );

      // Ajouter votre code ici !!
	  //drawpanda;
	  //glBegin(GL_LINES);
	 // glVertex2f(0, 0);
	 // glVertex2f(30, 0); // Longueur du bras
	  //glEnd();
      
	 

      // On re=charge la matrice que l'on a sauvegarde
      glPopMatrix();
   }
   if (m_Markers.size() >= 2) {
	   // récupérer les deux premiers marqueurs
	   cv::Point2f marker1 = m_Markers[0].getCenter();
	   cv::Point2f marker2 = m_Markers[1].getCenter();

	   // distance entre les deux marqueurs
	   float distance = cv::norm(marker1 - marker2);
	   cout << "distance=" << distance << endl;

	   // Calculer l'angle de la pince en fonction de la distance
	   float max_distance = 500.0f; // Distance maximale pour que le ciseau soit completement ouverte
	   float clamp_distance = std::min(distance, max_distance);
	   float angle_ouverture = clamp_distance / max_distance;

	   // Dessiner les bras du ciseau
	   glColor3f(1.0f, 0.4f, 0.4f);
	   // On recupere la matrice de modelview qui correspond au marqueur 0
	   m_Markers[0].glGetModelViewMatrix(modelview_matrix);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   // on charge cette matrice pour se placer dans le repere de ce marqueur [m] 
	   glLoadMatrixd(modelview_matrix);

	   glPushMatrix();

	 // on dessine le premier bras du ciseau
	   glTranslatef(0.f, 0.f, 0.f);  // Centrer sur le premier marqueur
	   glRotatef(angle_ouverture * 90.0f, 0, 0, 1);    // Rotation des bras en fonction de la distance
	   glBegin(GL_QUADS);
	   glVertex2f(0, -m_MarkerSize * 0.125f);   
	   glVertex2f(m_MarkerSize * 2, -m_MarkerSize * 0.125f);  
	   glVertex2f(m_MarkerSize * 2, m_MarkerSize * 0.125f);   
	   glVertex2f(0, m_MarkerSize * 0.125f);   
	   glEnd(); 

	   glTranslatef(m_MarkerSize * 2, 0.0f, 0.0f);
	   glBegin(GL_TRIANGLE_FAN); // Dessin d'un petit cercle
	   glVertex2f(0.0f, 0.0f);  
	   for (int i = 0; i <= 360; i++) {
		   float angle = i * 3.14159f / 180.0f;  // Conversion en radians
		   float x = cos(angle) * m_MarkerSize*0.25f ; // Rayon du cercle
		   float y = sin(angle) * m_MarkerSize*0.25f ;
		   glVertex2f(x, y);
	   }
	   glEnd();
	   glPopMatrix();


	   //on recommence pour la deuxieme pince
	   // On recupere la matrice de modelview qui correspond au marqueur 1
	   m_Markers[1].glGetModelViewMatrix(modelview_matrix);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   // on charge cette matrice pour se placer dans le repere de ce marqueur  
	   glLoadMatrixd(modelview_matrix);
	   // Bras droit (segment)
	   glPushMatrix();
	   //second bras
	   /*glTranslatef(0.f, 0.f, 0.f);  // Centrer sur le second marqueur
	   glRotatef(-angle_ouverture * 90.0f, 0, 0, 1);   // Rotation en sens inverse
	   glBegin(GL_LINES);
	   glVertex2f(0, 0);  // Premier point de la ligne 
	   glVertex2f(30, 0); // Deuxième point de la ligne */
	   glTranslatef(0.f, 0.f, 0.f);  // Centrer sur le premier marqueur
	   glRotatef(-angle_ouverture * 90.0f, 0, 0, 1);    // Rotation des bras en fonction de la distance
	   glBegin(GL_QUADS);
	   glVertex2f(0, -m_MarkerSize * 0.125f);
	   glVertex2f(m_MarkerSize * 2, -m_MarkerSize * 0.125f);
	   glVertex2f(m_MarkerSize * 2, m_MarkerSize * 0.125f);
	   glVertex2f(0, m_MarkerSize * 0.125f);
	   glEnd();

	   glTranslatef(m_MarkerSize * 2, 0.0f, 0.0f);
	   glBegin(GL_TRIANGLE_FAN); // Dessin d'un petit cercle
	   glVertex2f(0.0f, 0.0f);
	   for (int i = 0; i <= 360; i++) {
		   float angle = i * 3.14159f / 180.0f;  // Conversion en radians
		   float x = cos(angle) * m_MarkerSize * 0.25f; // Rayon du cercle
		   float y = sin(angle) * m_MarkerSize * 0.25f;
		   glVertex2f(x, y);
	   }
	   glEnd();
	   glPopMatrix();


   }
   // Desactivation du depth test
   glDisable(GL_DEPTH_TEST);
}






// Idle function
void ArUco::idle(Mat newImage) {
   // Getting new image
   m_InputImage = newImage.clone();
  
   // Undistort image based on distorsion parameters
   m_UndInputImage.create(m_InputImage.size(),CV_8UC3);
   
   //transform color that by default is BGR to RGB because windows systems do not allow reading BGR images with opengl properly
   cv::cvtColor(m_InputImage,m_InputImage,cv::COLOR_BGR2RGB);
   
   //remove distorion in image ==> does not work very well (the YML file is not that of my camera)
   //cv::undistort(m_InputImage,m_UndInputImage, m_CameraParams.CameraMatrix, m_CameraParams.Distorsion);
   m_UndInputImage = m_InputImage.clone();
   
   //resize the image to the size of the GL window
   cv::resize(m_UndInputImage,m_ResizedImage,m_GlWindowSize);

   //detect markers
   m_Markers_old = m_Markers;
   
   m_PPDetector.detect(m_ResizedImage, m_Markers, m_CameraParams, m_MarkerSize, false);

}

// Resize function
void ArUco::resize(GLsizei iWidth, GLsizei iHeight) {
   m_GlWindowSize=Size(iWidth,iHeight);
   
   //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
   if (iWidth*3%4!=0) {
      iWidth+=iWidth*3%4;//resize to avoid padding
      resize(iWidth, m_GlWindowSize.height);
   }
   else {
      //resize the image to the size of the GL window
      if (m_UndInputImage.rows!=0)
         cv::resize(m_UndInputImage, m_ResizedImage, m_GlWindowSize);
   }

}

// Test using ArUco to display a 3D cube in OpenCV
void ArUco::draw3DCube(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dCube(img, m_Markers[markerInd], m_CameraParams); 
   }
}

void ArUco::draw3DAxis(cv::Mat img, int markerInd) {
   if(m_Markers.size() > markerInd) {
      aruco::CvDrawingUtils::draw3dAxis(img, m_Markers[markerInd], m_CameraParams); 
   }
   
}
