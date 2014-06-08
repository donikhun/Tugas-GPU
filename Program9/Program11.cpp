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
float z_pos = -5.0f;
float rot = 0.0f;
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
/* array to hold texture handles */
GLuint  filter;  // Which Filter To Use
GLuint  texture[3]; // Storage For 3 Textures
int LoadGLTextures()  //Load Bitmaps And Convert To Textures
{
	int Status = FALSE; //Status Indicator
		AUX_RGBImageRec *TextureImage[1];  // CreateStorage Space For The Texture
		memset(TextureImage, 0, sizeof(void *)* 1); // Set ThePointer To NULL
		// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("images.bmp"))
	{
		Status = TRUE;
		// Set The Status To TRUE
		glGenTextures(3, &texture[0]);  //Create Three Textures
			// Create Nearest Filtered Texture
			glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
			TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0] ->data);
		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
			TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0] ->data);
		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX,
			TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	if (TextureImage[0])  //If Texture Exists
	{
		if (TextureImage[0]->data)  //If Texture Image Exists
		{
			free(TextureImage[0]->data); // FreeThe Texture Image Memory
		}
		free(TextureImage[0]); //Free The Image Structure
	}
	return Status;
	// Return The Status
}
void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width / (float)height, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	rot += 10;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
	glTranslatef(0.0, 0.0f, z_pos);
	glRotatef(rot, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texture[filter]);
	glEnable(GL_BLEND); // Turn Blending On
	glDisable(GL_DEPTH_TEST); // Turn Depth Testing Off
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top Face
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom Face
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glFlush();
	glutSwapBuffers();
}
void init()
{
	if (!LoadGLTextures())
		// Jump To Texture Loading Routine
	{
		return;
		// If Texture Didn't Load Return FALSE
	}
	glEnable(GL_TEXTURE_2D);  //Enable Texture Mapping
		glShadeModel(GL_SMOOTH);  //Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);  // BlackBackground
		glClearDepth(1.0f);
	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);  //Enables Depth Testing
		glDepthFunc(GL_LEQUAL);
	// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really NicePerspective Calculations
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // Setup TheAmbient Light
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // Setup TheDiffuse Light
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);  // Position TheLight
		glEnable(GL_LIGHT1);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5); // FullBrightness. 50 % Alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	return;
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("program 11");
	// callbacks
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, myTimeOut, 0);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
	return 0;
}