#include <GL/freeglut_std.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_W 550
#define WINDOW_H 700

static GLfloat screenBottom = -74.5;
static const int FPS = 70;
static GLfloat stick1Position = -8;
static GLfloat stickThick = 1.5;
static GLfloat stickSize = 18;
static GLfloat moveXFactor = 0.1;
static GLfloat moveYFactor = 0.5;
const int brickAmount = 24;
int lives = 3;

struct ball
{
	GLfloat radius = 2.5;
	GLfloat X = 0;
	GLfloat Y = -57;
	double directionX = 1.7;
	double directionY = 1;
}ball;

struct bricks
{
	float x;
	float y;
	float width;
	float height;
	bool isAlive = true;
};
bricks bricksArray[brickAmount];

void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-60.0, 60.0, -60.0 / aspect, 60.0 / aspect, -1.0, 1.0);
}

void draw_circle(float x, float y, float radius)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	static const int circle_points = 100;
	static const float angle = 2.0f * 3.1416f / circle_points;

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0); glVertex3f(0.6, 0.5, 0);
	glColor3f(0.98, 0.64, 0.96); glVertex3f(0, 0.75, 0);
	double angle1 = 0.0;
	glVertex2d(radius * cos(0.0), radius * sin(0.0));
	int i;
	for (i = 0; i < circle_points; i++)
	{
		glVertex2d(radius * cos(angle1), radius * sin(angle1));
		angle1 += angle;
	}
	glEnd();
	glPopMatrix();
}

bool areBricksAlive()
{
	for (int i = 0; i < brickAmount; i++)
	{
		if (bricksArray[i].isAlive)
		{
			return true;
		}
	}
	return false;
}

void timer(int v)
{
	glutPostRedisplay();
	if (ball.X + ball.radius > 60 || ball.X - ball.radius < -60)
	{
		ball.directionX = ball.directionX * (-1);
	}

	if (ball.Y < screenBottom + stickThick + ball.radius - 0.1)
	{
		if (((stick1Position) < ball.X) && (ball.X < (stick1Position + stickSize)) && (ball.Y > screenBottom + stickThick))
		{
			ball.directionY = ball.directionY * (-1);
			moveYFactor = moveYFactor;
		}
		else if (ball.Y < screenBottom + stickThick)
		{
			lives -= 1;
			Sleep(1000);
			ball.X = 0;
			ball.Y = -57;                      
			stick1Position = -8;
			ball.directionY = 1;

			switch (lives) {
			case 0:
				printf("YOU ARE DEAD!!! \n");
				printf("You must try all over again\n");
				for (int i = 0; i < brickAmount; i++)
				{
					bricksArray[i].isAlive = true;                            // reset isAlive flag for all bricks
				}
				lives = 3;
				Sleep(3000);                                             // öldükten sonra brickler yerine gelmeli
				break;
			case 1:
				printf("YOU MISSED :( \n");
				printf("You have %d lives left\n", lives);
				break;
			case 2:
				printf("YOU MISSED :( \n");
				printf("You have %d lives left\n", lives);
				break;
			}

		}
	}

	if (ball.Y > 74)
	{
		ball.directionY = ball.directionY * -1;
	}

	if (!areBricksAlive() && lives != 0)
	{
		printf("YOU WON :)\n");		                                                 // Stop the game loop
		return;
	}

	ball.X = ball.X + (moveXFactor * ball.directionX);                
	ball.Y = ball.Y + (moveYFactor * ball.directionY);

	glutTimerFunc(1000 / FPS, timer, v);

}

void bricksAction()
{
	float brickX = -50, brickY = 55;

	for (int i = 0; i < brickAmount; i++)
	{
		if (brickX > 48.5)
		{
			brickX = -50;
			brickY -= 12;
		}
		bricksArray[i].x = brickX;
		bricksArray[i].y = brickY;
		bricksArray[i].width = 15;
		bricksArray[i].height = 10;
		brickX += 17;

		if (bricksArray[i].isAlive && lives != 0)
		{

			if (ball.X + ball.radius >= bricksArray[i].x && ball.X - ball.radius <= bricksArray[i].x + bricksArray[i].width &&
				ball.Y + ball.radius >= bricksArray[i].y && ball.Y - ball.radius <= bricksArray[i].y + bricksArray[i].height)
			{

				ball.directionX = ball.directionX * -1;
				ball.directionY = ball.directionY * -1;

				ball.X += moveXFactor * ball.directionX;
				ball.Y += moveYFactor * ball.directionY;
				bricksArray[i].isAlive = false;
			}
		}
	}
	glColor3f(0.49, 0.93, 0.97);
	glBegin(GL_QUADS);

	for (int i = 0; i < brickAmount; i++)
	{
		if (bricksArray[i].isAlive == true)
		{
			glVertex2f(bricksArray[i].x, bricksArray[i].y);
			glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y);
			glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y + bricksArray[i].height);
			glVertex2f(bricksArray[i].x, bricksArray[i].y + bricksArray[i].height);
		}
	}
	glEnd();
}

void display()
{
	glClearColor(0.18, 0, 0.22, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRectf(stick1Position, screenBottom, stick1Position + stickSize, screenBottom + stickThick);
	draw_circle(ball.X, ball.Y, ball.radius);
	bricksAction();
	glFlush();
	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (stick1Position > -60)
		{
			stick1Position = stick1Position - 4;
			//printf("%f\n", stick1Position);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (stick1Position + stickSize < 60)
		{
			stick1Position = stick1Position + 4;
			//printf("%f\n", stick1Position);
		}
	}
}

void kbSpecial(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		if (stick1Position > -60)
		{
			stick1Position = stick1Position - 1;
			//printf("%f\n", stick1Position);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (stick1Position + stickSize < 60)
		{
			stick1Position = stick1Position + 1;
			//printf("%f\n", stick1Position);
		}
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(360, 50);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutCreateWindow("layda's first project");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	glutMouseFunc(mouse);
	glutSpecialFunc(kbSpecial);
	glutMainLoop();
}
