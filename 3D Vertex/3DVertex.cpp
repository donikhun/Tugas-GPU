#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
void mydisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(-0.5, -0.5, 1);
	glColor3f(0, 0, 1);
	glVertex3f(-0.75, 0, 1);
	glColor3f(1, 0, 0);
	glVertex3f(-0.5, 0.5, 1);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0.75, 1);
	glColor3f(0, 0, 1);
	glVertex3f(0.5, 0.5, -1);
	glColor3f(1, 0, 0);
	glVertex3f(0.75, 0, -1);
	glColor3f(0, 1, 0);
	glVertex3f(0.5, -0.5, -1);
	glColor3f(0, 0, 1);
	glVertex3f(0, -0.75, -1);
	glEnd();
	glFlush();
}
int main(int argc, char** argv){
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay);
	glutMainLoop();
}