#include <GL/freeglut.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "tower.h"

#define DISTANCE_OF_FLOOR 2

int level = 1;
int Game_state = 0;	//0 = 타이틀 1 = 고르기 2 = 줌인 3 = 게임 4 = 패배 5 = 승리 

Tower tower;
mouse_ first_click;
mouse_ moved_mouse;
bool drag = false;
double zoom = 0;

int timer = 0;

GLubyte *pBytes;
BITMAPINFO *info;
GLuint textures[1];


void main(int argc, char *argv[])
{
	//초기화 함수들
	tower.Initialize(EASY_1);
	Sound_SetUp();
	Play_Sound();
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정	// 더블버퍼링 && 3차원으로 그린다.
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("Example"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수의 지정
	glutMouseFunc(Mouse);	// 마우스 입력
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);		// 키보드 입력
	glutTimerFunc(100/3, Timerfunction, 1);
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색 깜장색! 깜장색!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 설정된 색으로 전체를 칠하기, 깊이버퍼 클리어

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_BLEND);
	glEnable(GL_AUTO_NORMAL);
	Draw_Background();
	glPushMatrix();
	{
		glTranslated(0, 0.5, -1);
		glRotated(30, 1, 0, 0);
		if (tower.Get_ball_camera_follow() == true)
		{
			glTranslated(0, -tower.Get_ball_y(), 0);
		}
		else
		{
			glTranslated(0, -tower.Get_ball_floor()*-DISTANCE_OF_FLOOR, 0);
		}
		Control_light();
		if (Game_state == 1 || Game_state == 2)
		{
			glTranslated(0, 1.5 - zoom * 0.075, 0);
			glScaled(0.2 + zoom * 0.04, 0.05 + zoom * 0.0475, 0.2 + zoom * 0.04);
		}
		tower.Draw_Tower();
	}
	glPopMatrix();

	Draw_Level();
	glutSwapBuffers();
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, w / h, 1, 200.0);
	glTranslated(0.0, -1.0, -3);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);	// 은면제거!
	glEnable(GL_AUTO_NORMAL);
	glLoadIdentity();
}
void Timerfunction(int value)
{
	if (Game_state == 0)
	{
		timer++;
		if (timer == 20)
		{
			Game_state = 1;
		}
	}

	if (Game_state == 2)
	{
		zoom += 1;
		if (zoom > 19)
		{
			Game_state = 3;
			zoom = 0;
		}
	}


	if (Game_state == 3)
	{
		tower.Update();
	}

	glutPostRedisplay();	// 화면 재출력
	glutTimerFunc(100/3, Timerfunction, 1);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Y':
		//glRotated(5, 1, 0, 0);
		tower.Rotate_plus();
		break;
	case'y':
		//glRotated(5, 0, 1, 0);
		tower.Rotate_minus();
		break;
	case '1':
		if (Game_state == 1)
		{
			level = 1;
			tower.Initialize(1);
		}
		break;
	case'2':
		if (Game_state == 1)
		{
			level = 2;
			tower.Initialize(2);
		}
		break;
	case'3':
		if (Game_state == 1)
		{
			level = 3;
			tower.Initialize(3);
		}
		break;
	case'4':
		if (Game_state == 1)
		{
			level = 4;
			tower.Initialize(4);
		}
		break;
	case'5':
		if (Game_state == 1)
		{
			level = 5;
			tower.Initialize(5);
		}
		break;
	case'6':
		if (Game_state == 1)
		{
			level = 6;
			tower.Initialize(6);
		}
		break;
	case's':
		if (Game_state == 1)
		{
			Game_state = 2;
		}
		break;
	case'x':
		if (Game_state == 4 || Game_state == 5)
		{
			Game_state = 1;
			tower.Initialize(level);
		}
		break;
	case'r':
		if (Game_state == 4 || Game_state == 5)
		{
			tower.Initialize(level);
			Game_state = 3;
		}
		break;
	}
	glutPostRedisplay();	// 화면 재출력!
}


void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		drag = true;
		first_click.x = x;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		drag = false;
		tower.Fix_degree();
	}
}

void Motion(int xp, int yp)
{
	moved_mouse.x = xp;

	if (drag == true)
	{
		int moving = moved_mouse.x - first_click.x;
		tower.Rotate_by_mouse(-moving);
	}

}

void Control_light()
{
	GLfloat AmbientLight[] = { 0.2f,  0.2f,  0.2f,  1.0f };
	GLfloat DiffuseLight[] = { 1.0, 1 , 1, 1.0 };
	GLfloat SpecularLight[] = { 0.5,0.5,0.5,1.0 };
	GLfloat lightPos_1[] = { 1, 1, 1, 1.0 };

	GLfloat AmbientLight2[] = { 0.2f,  0.2f ,  0.2f,  1.0f };
	GLfloat DiffuseLight2[] = { 1 , 1 , 1.0 , 1.0 };
	GLfloat SpecularLight2[] = { 0.5 ,0.5 ,0.5 ,1.0 };
	GLfloat lightPos_2[] = { -1, 1, 1, 1.0 };

	

	GLfloat gray[] = { 0.2f,0.2f,0.2f,1.0f };


	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos_1);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight2);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos_2);
	glEnable(GL_LIGHT1);

	
	
}
void Draw_Background()
{
	Init_Texture(pBytes, info, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glPushMatrix();
	glTranslated(0, 0, -10);

	glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glTexCoord2i(0, 1);
		glVertex3f(-10, 10, 0);

		glTexCoord2i(1, 1);
		glVertex3f(10, 10, 0);

		glTexCoord2i(1, 0);
		glVertex3f(10, -10, 0);

		glTexCoord2i(0, 0);
		glVertex3f(-10, -10, 0);
	glEnd();

	glPopMatrix();
}
void Draw_Level()
{
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1, 1, 1);
	char str[] = "Level : EASY_1";
	glRasterPos2f(-0.28, 2.3);

	int len = (int)strlen(str);

	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
}