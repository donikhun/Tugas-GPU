#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
void mydisplay(){
	glClear(GL_COLOR_BUFFER_BIT); // Menghapus layar
	glBegin(GL_POLYGON);
	glVertex2f(-0.5, -0.5);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, -0.5);
	glEnd();
	glFlush();
}
void resize(int w, int h)
{
	if (w >= h)
		glViewport(0, 0, (GLsizei)h, (GLsizei)h);
	else
		glViewport(0, 0, (GLsizei)w, (GLsizei)w);
}
int main(int argc, char** argv)
{
	printf("Hello World, this is 2D Rectangle");
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(resize);
	glutMainLoop();
}