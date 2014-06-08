// OpenGL
// - Texture Mapping Magnification Filter
//  filter=0 --> Nearest Filtered Texture
// filter=1 --> Linear Interpolation Texture
//  filter=2 --> Mipmapped Texture
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include "bmp.h"
#include "particle.h"
float z_pos = -5.0f;
float rot = 0.0f;
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
/* array to hold texture handles */
GLuint  filter;  // Which Filter To Use
GLuint  texture[1]; // Storage For 3 Textures
/* Particle System */
extern const int maxparticle = 2000; // set maximum number ofparticles
extern PARTICLE particle[maxparticle];
int LoadGLTextures()  //Load Bitmaps And Convert To Textures
{
	int Status = FALSE; //Status Indicator
		AUX_RGBImageRec *TextureImage[1];  // CreateStorage Space For The Texture
		memset(TextureImage, 0, sizeof(void *)* 1); // Set ThePointer To NULL
		// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("particle.bmp"))
	{
		Status = TRUE;
		// Set The Status To TRUE
		glGenTextures(1, &texture[0]);  //Create Three Textures
			// Create Nearest Filtered Texture
			glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
			TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	if (TextureImage[0])  // IfTexture Exists
	{
		if (TextureImage[0]->data)  // IfTexture Image Exists
		{
			free(TextureImage[0]->data); // Free TheTexture Image Memory
		}
		free(TextureImage[0]); //Free The Image Structure
	}
	glEnable(GL_TEXTURE_2D);  //Enable Texture Mapping
		return Status;
	// Return The Status
}
void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0, (float)width/(float)height, 0.0, 300.0);
	glMatrixMode(GL_PROJECTION); // the following operations affect theprojection matrix
		glLoadIdentity(); // restore matrix to original state
	glOrtho(-0.60, 0.60, -0.20, 0.60, -0.60, 0.60); // defines the viewing volume
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	rot += 1;
	glutPostRedisplay(); // request redisplay
	glutTimerFunc(100, myTimeOut, 0);  // request next timer event
}
void myKeyboard(unsigned char key, int x, int y)
{
	if ((key == '<') || (key == ',')) z_pos -= 0.1f;
	else if ((key == '>') || (key == '.')) z_pos += 0.1f;
	else if ((key == 'F') || (key = 'f'))
	{
		filter += 1;
		if (filter>2)
		{
			filter = 0;
		}
		printf("filter: %i", filter);
	}
}
void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
	glRotatef(50.0, 1.0, 0.0, 0.0); // show scene from top front
	glBindTexture(GL_TEXTURE_2D, texture[0]); // choose particletexture
	for (int i = 0; i <= maxparticle; i++){
		if (particle[i].ypos<0.0) particle[i].lifetime = 0.0;
		if ((particle[i].active == true) && (particle[i].lifetime>0.0)){
			glColor3f(particle[i].r, particle[i].g, particle[i].b);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0, 1.0); glVertex3f(particle[i].xpos + 0.005,
				particle[i].ypos + 0.005, particle[i].zpos + 0.0); // top right
			glTexCoord2f(0.0, 0.0); glVertex3f(particle[i].xpos - 0.005,
				particle[i].ypos + 0.005, particle[i].zpos + 0.0); // top left
			glTexCoord2f(1.0, 1.0); glVertex3f(particle[i].xpos + 0.005,
				particle[i].ypos - 0.005, particle[i].zpos + 0.0); // bottom right
			glTexCoord2f(1.0, 0.0); glVertex3f(particle[i].xpos - 0.005,
				particle[i].ypos - 0.005, particle[i].zpos + 0.0); // bottom left
			glEnd();
		}
		else CreateParticle(i);
	}
	EvolveParticle();
	glFlush();
	glutSwapBuffers();
}
void init()
{
	glDisable(GL_DEPTH_TEST); // deactivate hidden surface removal
	glDisable(GL_CULL_FACE); // show backside of polygons
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // BlackBackground
		glColor4f(1.0f, 1.0f, 1.0f, 0.5);  // FullBrightness. 50 % Alpha
	if (!LoadGLTextures()) //Jump To Texture Loading Routine
	{
		return;  // IfTexture Didn't Load Return FALSE
	}
	InitParticle();
	glMatrixMode(GL_MODELVIEW);
	return;
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("particle System");
	// callbacks
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, myTimeOut, 0);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
	return 0;
}