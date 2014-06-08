#include <windows.h>
#include <math.h> // Math Library Header File
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
#include <GL/glaux.h>

#pragma comment(lib, "Glaux.lib")
const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos;
GLfloat  yrot; // Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
GLfloat  z = 0.0f;  // Depth Into The Screen
GLuint  filter;  // Which Filter To Use
GLuint  texture[3]; // Storage For 3 Textures
typedef struct tagVERTEX
{
	float x, y, z;
	float u, v;
} VERTEX;
typedef struct tagTRIANGLE
{
	VERTEX vertex[3];
} TRIANGLE;
typedef struct tagSECTOR
{
	int numtriangles;
	TRIANGLE* triangle;
} SECTOR;
SECTOR sector1;  // Our Model Goes Here:
void readstr(FILE *f, char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
	return;
}
void SetupWorld()
{
	float x, y, z, u, v;
	int numtriangles;
	FILE *filein;
	char oneline[255];
	filein = fopen("data/world.txt", "rt");  // FileTo Load World Data From
		readstr(filein, oneline);
	sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);
	sector1.triangle = new TRIANGLE[numtriangles];
	sector1.numtriangles = numtriangles;
	for (int loop = 0; loop < numtriangles; loop++)
	{
		for (int vert = 0; vert < 3; vert++)
		{
			readstr(filein, oneline);
			sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
			sector1.triangle[loop].vertex[vert].x = x;
			sector1.triangle[loop].vertex[vert].y = y;
			sector1.triangle[loop].vertex[vert].z = z;
			sector1.triangle[loop].vertex[vert].u = u;
			sector1.triangle[loop].vertex[vert].v = v;
		}
	}
	fclose(filein);
	return;
}
AUX_RGBImageRec *LoadBMP(char *Filename) // Loads A BitmapImage
{
	FILE *File = NULL; // File Handle
	if (!Filename) // Make Sure AFilename Was Given
	{
		return NULL; // If Not Return NULL
	}
	File = fopen(Filename, "r"); // Check To See IfThe File Exists
	if (File) // Does The FileExist ?
	{
		fclose(File); // Close The Handle
		return auxDIBImageLoadA(Filename); // Load The Bitmap AndReturn A Pointer
	}
	return NULL; // If Load FailedReturn NULL
}
int LoadGLTextures() // Load Bitmaps And Convert To Textures
{
	int Status = FALSE; // Status Indicator
	AUX_RGBImageRec *TextureImage[1]; // Create Storage Space For The Texture
		memset(TextureImage, 0, sizeof(void *)* 1); // Set The Pointer ToNULL
		// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP("Data/Mud.bmp"))
	{
		Status = TRUE; // Set The Status ToTRUE
			glGenTextures(3, &texture[0]); // Create ThreeTextures
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
	if (TextureImage[0]) // If Texture Exists
	{
		if (TextureImage[0]->data) // If Texture ImageExists
		{
			free(TextureImage[0]->data); // Free The Texture ImageMemory
		}
		free(TextureImage[0]); // Free The ImageStructure
	}
	return Status; // Return The Status
}
void resize(int width, int height) // Resize And Initialize The GLWindow
{
	if (height == 0)
	// Prevent A Divide By Zero By
	{
		height = 1;
		// Making Height Equal One
	}
	glViewport(0, 0, width, height); // ResetThe Current Viewport
		glMatrixMode(GL_PROJECTION); // SelectThe Projection Matrix
		glLoadIdentity(); //Reset The Projection Matrix
		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);  //Select The Modelview Matrix
		glLoadIdentity(); //Reset The Modelview Matrix
}
void init() // AllSetup For OpenGL Goes Here
{
	if (!LoadGLTextures()) //Jump To Texture Loading Routine
	{
		return;  //If Texture Didn't Load Return FALSE
	}
	glEnable(GL_TEXTURE_2D);  //Enable Texture Mapping
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);  // SetThe Blending Function For Translucency
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // ThisWill Clear The Background Color To Black
		glClearDepth(1.0);
	// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);  //The Type Of Depth Test To Do
		glEnable(GL_DEPTH_TEST);  //Enables Depth Testing
		glShadeModel(GL_SMOOTH);  //Enables Smooth Color Shading
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really NicePerspective Calculations
		SetupWorld();
	return;  //Initialization Went OK
}
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	//rot+=1;
	glutPostRedisplay(); // request redisplay
	glutTimerFunc(100, myTimeOut, 0);  // request next timer event
}
void myKeyboard(unsigned char key, int x, int y)
{
}
void mySpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		xpos -= (float)sin(heading*piover180) * 0.05f;
		zpos -= (float)cos(heading*piover180) * 0.05f;
		if (walkbiasangle >= 359.0f)
		{
			walkbiasangle = 0.0f;
		}
		else
		{
			walkbiasangle += 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		xpos += (float)sin(heading*piover180) * 0.05f;
		zpos += (float)cos(heading*piover180) * 0.05f;
		if (walkbiasangle <= 1.0f)
		{
			walkbiasangle = 359.0f;
		}
		else
		{
			walkbiasangle -= 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		heading -= 1.0f;
		yrot = heading;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		heading += 1.0f;
		yrot = heading;
	}
	else if (key == GLUT_KEY_PAGE_UP)
	{
		z -= 0.02f;
		lookupdown -= 1.0f;
	}
	else if (key == GLUT_KEY_PAGE_DOWN)
	{
		z += 0.02f;
		lookupdown += 1.0f;
	}
}
void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear TheScreen And The Depth Buffer
		glLoadIdentity(); //Reset The View
		GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias - 0.25f;
	GLfloat sceneroty = 360.0f - yrot;
	int numtriangles;
	glRotatef(lookupdown, 1.0f, 0, 0);
	glRotatef(sceneroty, 0, 1.0f, 0);
	glTranslatef(xtrans, ytrans, ztrans);
	glBindTexture(GL_TEXTURE_2D, texture[filter]);
	numtriangles = sector1.numtriangles;
	// Process Each Triangle
	for (int loop_m = 0; loop_m < numtriangles; loop_m++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(0.0f, 0.0f, 1.0f);
		x_m = sector1.triangle[loop_m].vertex[0].x;
		y_m = sector1.triangle[loop_m].vertex[0].y;
		z_m = sector1.triangle[loop_m].vertex[0].z;
		u_m = sector1.triangle[loop_m].vertex[0].u;
		v_m = sector1.triangle[loop_m].vertex[0].v;
		glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);
		x_m = sector1.triangle[loop_m].vertex[1].x;
		y_m = sector1.triangle[loop_m].vertex[1].y;
		z_m = sector1.triangle[loop_m].vertex[1].z;
		u_m = sector1.triangle[loop_m].vertex[1].u;
		v_m = sector1.triangle[loop_m].vertex[1].v;
		glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);
		x_m = sector1.triangle[loop_m].vertex[2].x;
		y_m = sector1.triangle[loop_m].vertex[2].y;
		z_m = sector1.triangle[loop_m].vertex[2].z;
		u_m = sector1.triangle[loop_m].vertex[2].u;
		v_m = sector1.triangle[loop_m].vertex[2].v;
		glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
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