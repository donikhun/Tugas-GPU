#include <windows.h>
#include <math.h>  // Math Library Header File
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#pragma comment ( lib, "glaux.lib")
float points[45][45][3]; // The Array For The Points On The Grid Of Our"Wave"
int wiggle_count = 0;  // Counter Used To Control How Fast Flag Waves
GLfloat  xrot; // X Rotation ( NEW )
GLfloat  yrot; // Y Rotation ( NEW )
GLfloat  zrot; // Z Rotation ( NEW )
GLfloat hold; // Temporarily Holds A Floating Point Value
GLuint texture[1]; // Storage For One Texture ( NEW )
AUX_RGBImageRec *LoadBMP(char *Filename)  // Loads ABitmap Image
{
	FILE *File = NULL;
	// File Handle
	if (!Filename)
		// Make Sure A Filename Was Given
	{
		return NULL;
		// If Not Return NULL
	}
	File = fopen(Filename, "r"); //Check To See If The File Exists
	if (File)
		// Does The File Exist?
	{
		fclose(File);
		// Close The Handle
		return auxDIBImageLoadA(Filename); // Load The Bitmap And Return A Pointer
	}
	return NULL;
	// If Load Failed Return NULL
}
int LoadGLTextures()
// Load Bitmaps And Convert To Textures
{
	int Status = FALSE;
	// Status Indicator
	AUX_RGBImageRec *TextureImage[1]; // Create Storage Space For The Texture
		memset(TextureImage, 0, sizeof(void *)* 1); // Set The Pointer To NULL
		// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("images.bmp"))
	{
		Status = TRUE;
		// Set The Status To TRUE
		glGenTextures(1, &texture[0]);  //Create The Texture
			// Typical Texture Generation Using Data From The Bitmap
			glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
			TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[0])
		// If Texture Exists
	{
		if (TextureImage[0]->data)
			// If Texture Image Exists
		{
			free(TextureImage[0]->data);
			// Free The Texture Image Memory
		}
		free(TextureImage[0]);
		// Free The Image Structure
	}
	return Status;
	// Return The Status
}
void resize(GLsizei width, GLsizei height)  // Resize And Initialize The GL Window
{
	if (height == 0)
	// Prevent A Divide By Zero By
	{
		height = 1;
		// Making Height Equal One
	}
	glViewport(0, 0, width, height);  //Reset The Current Viewport
		glMatrixMode(GL_PROJECTION);  //Select The Projection Matrix
		glLoadIdentity();
	// Reset The Projection Matrix
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	// Select The Modelview Matrix
	glLoadIdentity();
	// Reset The Modelview Matrix
}
int init(GLvoid)
// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())
		// Jump To Texture Loading Routine ( NEW )
	{
		return FALSE;
		// If Texture Didn't Load Return FALSE
	}
	glEnable(GL_TEXTURE_2D);  //Enable Texture Mapping(NEW)
		glShadeModel(GL_SMOOTH);  //Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);  // Black Background
		glClearDepth(1.0f);
	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);  //Enables Depth Testing
		glDepthFunc(GL_LEQUAL);
	// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
		glPolygonMode(GL_BACK, GL_FILL);  //Back Face Is Solid
		glPolygonMode(GL_FRONT, GL_LINE);  //Front Face Is Made Of Lines
	for (int x = 0; x<45; x++)
	{
		for (int y = 0; y<45; y++)
		{
			points[x][y][0] = float((x / 5.0f) - 4.5f);
			points[x][y][1] = float((y / 5.0f) - 4.5f);
			points[x][y][2] = float(sin((((x / 5.0f)*40.0f) / 360.0f)*3.141592654*2.0f));
		}
	}
	return TRUE;
	// Initialization Went OK
}
void mydisplay(GLvoid)
// Here's Where We Do All The Drawing
{
	int x, y;
	float float_x, float_y, float_xb, float_yb;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
		glLoadIdentity();
	// Reset The View
	glTranslatef(0.0f, 0.0f, -12.0f);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glRotatef(zrot, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	for (x = 0; x < 44; x++)
	{
		for (y = 0; y < 44; y++)
		{
			float_x = float(x) / 44.0f;
			float_y = float(y) / 44.0f;
			float_xb = float(x + 1) / 44.0f;
			float_yb = float(y + 1) / 44.0f;
			glTexCoord2f(float_x, float_y);
			glVertex3f(points[x][y][0], points[x][y][1],
				points[x][y][2]);
			glTexCoord2f(float_x, float_yb);
			glVertex3f(points[x][y + 1][0], points[x][y + 1][1],
				points[x][y + 1][2]);
			glTexCoord2f(float_xb, float_yb);
			glVertex3f(points[x + 1][y + 1][0], points[x + 1][y + 1][1],
				points[x + 1][y + 1][2]);
			glTexCoord2f(float_xb, float_y);
			glVertex3f(points[x + 1][y][0], points[x + 1][y][1],
				points[x + 1][y][2]);
		}
	}
	glEnd();
	if (wiggle_count == 2)
	{
		for (y = 0; y < 45; y++)
		{
			hold = points[0][y][2];
			for (x = 0; x < 44; x++)
			{
				points[x][y][2] = points[x + 1][y][2];
			}
			points[44][y][2] = hold;
		}
		wiggle_count = 0;
	}
	wiggle_count++;
	xrot += 0.3f;
	yrot += 0.2f;
	zrot += 0.4f;
	glFlush();
	glutSwapBuffers();
}
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	//rot+=1;
	glutPostRedisplay(); // request redisplay
	glutTimerFunc(10, myTimeOut, 0); // request next timer event
}
void myKeyboard(unsigned char key, int x, int y)
{
}
void mySpecialKeyboard(int key, int x, int y)
{
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("program19");
	// callbacks
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKeyboard);
	glutTimerFunc(100, myTimeOut, 0);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
	return 0;
}
