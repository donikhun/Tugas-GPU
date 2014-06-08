#include <windows.h>
#include <stdio.h> // Header File For Standard Input/Output
#include <GL\GL.H> // Header File For The OpenGL32 Library
#include <GL\GLU.H> // Header File For The GLu32 Library
#include <GL\GLAUX.H> // Header File For The Glaux Library
#include "bmp.h"
#pragma comment ( lib, "glaux.lib")

AUX_RGBImageRec *LoadBMP(char *Filename) // Loads A Bitmap Image
{
FILE *File=NULL; //File Handle
if (!Filename)// Make Sure A Filename Was Given
{
return NULL;
// If Not Return NULL
}
File=fopen(Filename,"r"); //Check To See If The File Exists
if (File)
// Does The File Exist?
{
fclose(File);
// Close The Handle
//return auxDIBImageLoad(Filename); // Load The Bitmap And Return A Pointer
return auxDIBImageLoadA(Filename); // load The Bitmap and Return A pointer for Multicode type character
}
return NULL;
// If Load Failed Return NULL
}