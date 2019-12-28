// Project 2
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>      // (or others, depending on the system in use)



// Defining region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int w;

class scrPt {
   public:
      GLint x, y;
};

#define width  450
#define height 450

double poly1[3][2] = { {100,150},{200,150},{150,250} };
double poly2[4][2] = { {250,150},{350,150},{400,250},{300,250} };
double poly3[5][2] = { {100,300}, {150,300}, {175,350}, {125,400}, {75,350} };
double poly4[4][2] = { {300,300}, {350,300}, {375,350}, {325,400} };
double poly5[4][2] = { {200,10}, {275,50}, {225,100}, {175,50} };

scrPt leftbottom,righttop;

int translate = 0, resize = 0;
int prev_x, prev_y;
int win1, win2;

GLfloat xt;
GLfloat yt;

GLfloat sx;
GLfloat sy;

void init (void)
{
    glClearColor (1.0, 1.0, 1.0, 0.0);  // Set display-window color to white.

    glMatrixMode (GL_PROJECTION);       // Set projection parameters.
    gluOrtho2D (0.0, 200.0, 0.0, 150.0);
}

void drawPolygon(){
    glLineWidth(1);
	//glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (int i = 0; i<3; i++) glVertex2dv(poly1[i]);
	glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i<4; i++) glVertex2dv(poly2[i]);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i<5; i++) glVertex2dv(poly3[i]);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i<4; i++) glVertex2dv(poly4[i]);
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i<4; i++) glVertex2dv(poly5[i]);
    glEnd();

	glFlush();     // Process all OpenGL routines as quickly as possible.
	glutSwapBuffers();
}

// Function to compute region code for a point(x, y)

int calCode(double x, double y)
{
    // initialized as being inside
    int code = INSIDE;

    if (x < leftbottom.x)       // to the left of rectangle
        code |= LEFT;
    else if (x > righttop.x)  // to the right of rectangle
        code |= RIGHT;
    if (y < leftbottom.y)       // below the rectangle
        code |= BOTTOM;
    else if (y > righttop.y)  // above the rectangle
        code |= TOP;

    return code;
}

// Implementing Cohen-Sutherland algorithm
// Clipping a line from P1 = (x2, y2) to P2 = (x2, y2)

float CS_clip(double poly[2], double polyy[2])
{
    double x1,  y1,  x2,  y2;
    x1 = poly[0];
    y1 = poly[1];

    x2 = polyy[0];
    y2 = polyy[1];
    // Compute region codes for P1, P2
    int code1 = calCode(x1, y1);
    int code2 = calCode(x2, y2);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & TOP)
            {
                // point is above the clip rectangle
                x = x1 + (x2 - x1) * (righttop.y - y1) / (y2 - y1);
                y = righttop.y;
            }
            else if (code_out & BOTTOM)
            {
                // point is below the rectangle
                x = x1 + (x2 - x1) * (leftbottom.y - y1) / (y2 - y1);
                y = leftbottom.y;
            }
            else if (code_out & RIGHT)
            {
                // point is to the right of rectangle
                y = y1 + (y2 - y1) * (righttop.x - x1) / (x2 - x1);
                x = righttop.x;
            }
            else if (code_out & LEFT)
            {
                // point is to the left of rectangle
                y = y1 + (y2 - y1) * (leftbottom.x - x1) / (x2 - x1);
                x = leftbottom.x;
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                x1 = x;
                y1 = y;
                code1 = calCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = calCode(x2, y2);
            }
        }
    }
    if (accept)
    {
        printf("Line accepted from %1.1f %1.1f to %1.1f %1.1f \n", x1, y1, x2, y2);

    if ( w==1){
        glLineWidth(3);
        glColor3f(0, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        glFlush();
        glutSwapBuffers();
    }
    else {
        glLineWidth(3);
        glColor3f(0, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        sx = width/(righttop.x - leftbottom.x);
        sy = height/(righttop.y - leftbottom.y);

        xt = - leftbottom.x;
        yt = - leftbottom.y;

        glTranslatef(xt*sx, yt*sy, 0);
        glScalef(sx, sy, 0);

        glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();


        glScalef(1/sx, 1/sy, 0);
        glTranslatef(-xt*sx, -yt*sy, 0);

        glFlush();     // Process all OpenGL routines as quickly as possible.
        glutSwapBuffers();
    }

    }
    else
        printf("Line rejected\n");
}


void call_clip(void){
    CS_clip(poly1[0], poly1[1]);
	CS_clip(poly1[1], poly1[2]);
	CS_clip(poly1[2], poly1[0]);

	CS_clip(poly2[0], poly2[1]);
	CS_clip(poly2[1], poly2[2]);
	CS_clip(poly2[2], poly2[3]);
	CS_clip(poly2[3], poly2[0]);

    CS_clip(poly3[0], poly3[1]);
	CS_clip(poly3[1], poly3[2]);
	CS_clip(poly3[2], poly3[3]);
	CS_clip(poly3[3], poly3[4]);
	CS_clip(poly3[4], poly3[0]);

    CS_clip(poly4[0], poly4[1]);
	CS_clip(poly4[1], poly4[2]);
	CS_clip(poly4[2], poly4[3]);
	CS_clip(poly4[3], poly4[0]);

    CS_clip(poly5[0], poly5[1]);
	CS_clip(poly5[1], poly5[2]);
	CS_clip(poly5[2], poly5[3]);
	CS_clip(poly5[3], poly5[0]);
}

void drawfunc1(void){
    w = 1;
    glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.

	glColor3f (1.0, 0.0, 1.0);
    glLineWidth(1);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glRecti (leftbottom.x,leftbottom.y,righttop.x,righttop.y);

	drawPolygon();

    call_clip();

    glFlush ( );     // Process all OpenGL routines as quickly as possible.
}

void drawfunc2 (void)
{
    w = 2;
    glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.

	glColor3f (1.0, 0.0, 1.0);
    glLineWidth(1);
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

    call_clip();

    glFlush ( );     // Process all OpenGL routines as quickly as possible.
}

void MousePress(int button, int state, int x, int y)
{

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
		translate = 1;
		prev_x = x;
		prev_y = y;
    }
  else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
      resize = 1;
	  prev_x = x;
	  prev_y = y;
    }
  else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
      translate = 0;
    }
  else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
      resize = 0;
    }
}

void MouseDrag(int x, int y)
{
  int tx, ty;

	if (translate) {
		tx = x - prev_x;
		ty = prev_y - y;
		prev_x = x;
		prev_y = y;
		leftbottom.x += tx;
		leftbottom.y += ty;
		righttop.x += tx;
		righttop.y += ty;

		drawfunc1 ();
		glutSetWindow(win2);
	}

	if (resize) {
		tx = x - prev_x;
		ty = prev_y - y;
		prev_x = x;
		prev_y = y;
		righttop.x += tx;
		if (righttop.x < leftbottom.x + 5) // make sure your rectangle maintain a correct order of vertices
			righttop.x = leftbottom.x + 5;
		righttop.y += ty;
		if (righttop.y < leftbottom.y + 5) // make sure your rectangle maintain a correct order of vertices
			righttop.y = leftbottom.y + 5;


		drawfunc1 ();
		glutSetWindow(win2);
		drawfunc2 ();
		glutSetWindow(win1);
	}

  glutPostRedisplay();
}


void winReshapeFcn (int newWidth, int newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (0.0, (GLdouble) newWidth, 0.0, (GLdouble) newHeight);

    glClear (GL_COLOR_BUFFER_BIT);
}

// Driver code
int main(int argc, char** argv)
{

    /////
    glutInit (&argc, argv);                         // Initialize GLUT.
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowSize (width, height);      // Set display-window width and height.

	//first window
    glutInitWindowPosition (50, 100);   // Set top-left display-window position.
    win1 = glutCreateWindow ("Window One\n"); // Create display window.
    init ( );                            // Execute initialization procedure.

	leftbottom.x = 150;
	leftbottom.y = 120;
	righttop.x = 325;
	righttop.y = 200;


    glutDisplayFunc (drawfunc1);       // Send graphics to display window.
	glutReshapeFunc (winReshapeFcn);
    glutMouseFunc(MousePress);
	glutMotionFunc(MouseDrag);

    win2 = glutCreateWindow ("Window Two\n"); // Create display window.
	glutPositionWindow(550,100);
	init ( );                            // Execute initialization procedure.
	glutDisplayFunc (drawfunc2);       // Send graphics to display window.
	glutReshapeFunc (winReshapeFcn);

    glutMainLoop ( );                    // Display everything and wait.
}
