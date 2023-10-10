#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include "MyCube.h"
#include "MyPyramid.h"
#include "MyTarget.h"

MyCube cube;
MyPyramid pyramid;
MyTarget target(&cube);

GLuint program;
GLuint uMat;

mat4 CTM;

bool bPlay = false;
bool bChasingTarget = false;
bool bDrawTarget = false;

float ang1 = 0;
float ang2 = 0;
float ang3 = 0;

float pi = 3.141592;

void myInit()
{
	cube.Init();
	pyramid.Init();

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
}

float timer = 0;

void drawRobotArm(float ang1, float ang2, float ang3)
{
	mat4 temp2 = CTM;
	mat4 temp = CTM;

	// BASE
	mat4 M(1.0);
	
	mat4 tmp = Translate(0, -0.4, 0) * RotateY(timer * 30);

	CTM = Translate(0, -0.4, 0) * RotateY(timer * 30) * Translate(0, 0, 0.075);
	M = Scale(0.3, 0.2, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	CTM = Translate(0, -0.4, 0) * RotateY(timer * 30) * Translate(0, 0, 0.0825);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = Translate(0, -0.4, 0) * RotateY(timer * 30) * Translate(0, 0, -0.075);
	M = Scale(0.3, 0.2, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	pyramid.Draw(program);

	CTM = Translate(0, -0.4, 0) * RotateY(timer * 30) * Translate(0, 0, -0.0825);
	M = Scale(0.05, 0.05, 0.05);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = tmp;

	// Upper Arm
	CTM *= RotateZ(ang1);
	M = Translate(0, 0.2, 0) *  Scale(0.1, 0.4, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	M = Scale(0.05, 0.05, 0.4);
	glUniformMatrix4fv(uMat, 1, true, CTM*Translate(0,0.35,0)*M);
	cube.Draw(program);

	tmp = CTM * Translate(0, 0.4, 0) * RotateZ(ang2);

	// Lower Arm
	mat4 tmp2 = CTM * Translate(0, 0.4, 0) * RotateZ(ang2);

	tmp = CTM;

	CTM *= Translate(0, 0.35, 0) * RotateZ(ang2) * Translate(0,0,0.1);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.6, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);
	CTM = tmp;

	CTM *= Translate(0, 0.35, 0) * RotateZ(ang2) * Translate(0, 0, -0.1);
	M = Translate(0, 0.2, 0) * Scale(0.1, 0.6, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	CTM = tmp * Translate(0, 0.35, 0) * RotateZ(ang2);


	// Hand
	M = Scale(0.05, 0.05, 0.4);
	glUniformMatrix4fv(uMat, 1, true, CTM*Translate(0, 0.4, 0)*M);
	cube.Draw(program);

	CTM *= Translate(0, 0.4, 0)	* RotateZ(ang3);
	M = Translate(0, 0, 0) * Scale(0.3, 0.2, 0.1);
	glUniformMatrix4fv(uMat, 1, true, CTM*M);
	cube.Draw(program);

	//CTM = temp;
	CTM = Translate(0, -0.4, 0) * RotateY(timer * 5);
	CTM *= RotateZ(ang1);
	CTM *= Translate(0, 0.4, 0) * RotateZ(ang2);
	CTM *= Translate(0, 0.4, 0)	* RotateZ(ang3);
	CTM = temp2;
}

void computeAngle()
{
	float tx = -target.currentPos.x, ty = target.currentPos.y;

	float offset = atan2(0.15, 0.75);

	float rad_ang1 = ang1 * pi / 180 + offset;
	float rad_ang2 = ang2 * pi / 180 + offset;
	float rad_ang3 = ang3 * pi / 180 + offset;

	float y = 0.4 * cos(rad_ang1) + 0.4 * cos(rad_ang1 + rad_ang2) + 0.1 * cos(rad_ang1 + rad_ang2 + rad_ang3);
	float x = 0.4 * sin(rad_ang1) + 0.4 * sin(rad_ang1 + rad_ang2) + 0.1 * sin(rad_ang1 + rad_ang2 + rad_ang3);


	float y_prime_ang1 = -0.4 * sin(rad_ang1) - 0.4 * sin(rad_ang1 + rad_ang2) - 0.1* sin(rad_ang1 + rad_ang2 + rad_ang3);
	float y_prime_ang2 = -0.4 * sin(rad_ang1 + rad_ang2) - 0.1 * sin(rad_ang1 + rad_ang2 + rad_ang3);
	float y_prime_ang3 = -0.1 * sin(rad_ang1 + rad_ang2 + rad_ang3);

	float x_prime_ang1 = 0.4 * cos(rad_ang1) + 0.4 * cos(rad_ang1 + rad_ang2) + 0.1 * cos(rad_ang1 + rad_ang2 + rad_ang3);
	float x_prime_ang2 = 0.4 * cos(rad_ang1 + rad_ang2) + 0.1 * cos(rad_ang1 + rad_ang2 + rad_ang3);
	float x_prime_ang3 = 0.1 * cos(rad_ang1 + rad_ang2 + rad_ang3);


	float dy_d_ang1 = 2 * x * x_prime_ang1 - 2 * tx * x_prime_ang1 + 2 * y * y_prime_ang1 - 2 * ty * y_prime_ang1;
	float dy_d_ang2 = 2 * x * x_prime_ang2 - 2 * tx * x_prime_ang2 + 2 * y * y_prime_ang2 - 2 * ty * y_prime_ang2;
	float dy_d_ang3 = 2 * x * x_prime_ang3 - 2 * tx * x_prime_ang3 + 2 * y * y_prime_ang3 - 2 * ty * y_prime_ang3;

	float l = 25;
	
	ang1 -= dy_d_ang1 * l + offset;
	ang2 -= dy_d_ang2 * l + offset;
	ang3 -= dy_d_ang3 * l + offset;
}


void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	GLuint uColor = glGetUniformLocation(program, "uColor");
	glUniform4f(uColor, -1, -1, -1, -1);


	uMat = glGetUniformLocation(program, "uMat");
	CTM = Translate(0, -0.4, 0) * RotateY(timer * 30);
	drawRobotArm(ang1, ang2, ang3);


	glUniform4f(uColor, 1, 0, 0, 1);
	if (bDrawTarget == true)
		target.Draw(program, CTM, timer);

	glutSwapBuffers();
}

void myIdle()
{
	if (bPlay)
	{
		timer += 1 / 60.0f;
		Sleep(1 / 60.0f * 1000);

		if (bChasingTarget == false)
		{
			ang1 = 45 * sin(timer*3.141592);
			ang2 = 60 * sin(timer * 2 * 3.141592);
			ang3 = 30 * sin(timer*3.141592);
		}
		else
			computeAngle();

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char c, int x, int y)
{

	switch (c)
	{
	case '1':
		bChasingTarget = !bChasingTarget;
		break;
	case '2':
		bDrawTarget = !bDrawTarget;
		break;
	case '3':
		target.toggleRandom();
		break;
	case ' ':
		bPlay = !bPlay;
		break;
	default:
		break;
	}
}


int main(int argc, char ** argv)
{
	printf("Press 'space' to play\n");
	printf("Press '1' to chase target\n");
	printf("Press '2' to show target\n");
	printf("Press '3' to replace target on random position\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Simple Robot Arm");
	
	glewExperimental = true;
	glewInit();

	myInit();
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);

	glutMainLoop();

	return 0;
}