#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <GL/glut.h>
float z_pos = 0.0f;
float rot = 0.0f;
void mydisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0f, z_pos);
	glRotatef(rot, 0, 0, 1);
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(-0.5, -0.5, -5);
	glColor3f(0, 0, 1);
	glVertex3f(-0.75, 0, -5);
	glColor3f(1, 0, 0);
	glVertex3f(-0.5, 0.5, -5);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0.75, -5);
	glColor3f(0, 0, 1);
	glVertex3f(0.5, 0.5, -5);
	glColor3f(1, 0, 0);
	glVertex3f(0.75, 0, -5);
	glColor3f(0, 1, 0);
	glVertex3f(0.5, -0.5, -5);
	glColor3f(0, 0, 1);
	glVertex3f(0, -0.75, -5);
	glEnd();
	glFlush();
	glutSwapBuffers();
}
void init(void)
{
	glClearColor(1.0, 0.0, 0.0, 1.0); // A Background Clear Color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)500.0 / (GLdouble)500.0, 0, 100);
	glMatrixMode(GL_MODELVIEW);
}
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)w / (GLdouble)h, 0, 100);
	glMatrixMode(GL_MODELVIEW);
}
void myTimeOut(int id)
{
	// called if timer event
	// ...advance the state of animation incrementally...
	rot += 10;
	glutPostRedisplay(); // request redisplay
	glutTimerFunc(100, myTimeOut, 0); // request next timer event
}
void myKeyboard(unsigned char key, int x, int y)
{
	if ((key == '<') || (key == ',')) z_pos -= 0.1f;
	if ((key == '>') || (key == '.')) z_pos += 0.1f;
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	// callbacks
	glutDisplayFunc(mydisplay);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, myTimeOut, 0);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
}