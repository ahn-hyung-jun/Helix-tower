#include "tile.h"
#include <iostream>
#include <cmath>

#define ANGLE -30
#define LEFT_SIDE_DEGREE 90
#define RIGHT_SIDE_DEGREE 120
#define MID_CTRL_DEGREE 7.5

#define EMPTY_TILE 0
#define NORMAL_TILE 1
#define KILL_TILE 2
#define GRAVITY_FREE_TILE 3
#define BLIND_TILE 4
#define BLINK_TILE 5
#define ROTATE_TILE 6
#define END_TILE 9

const double mid_degree = (LEFT_SIDE_DEGREE + RIGHT_SIDE_DEGREE) / 2;

void Tile::Initialize(int degree)
{
	x = 0, z = 0;
	y = 0;
	timer = 0;
	blink = true;
	this->degree = degree;
	half_height = 0.1;

	up_side = gluNewQuadric();
	down_side = gluNewQuadric();

	distroy_degree = 0;
	Distroy_timer = 0;

	alpha = 1;
}
void Tile::Draw_Tile()
{
	if (state == 5)
	{
		timer++;
		if (timer % 10 == 0)
		{
			if (blink == false)
			{
				blink = true;
			}
			else if (blink == true)
			{
				blink = false;
			}
		}
	}
	switch (state)
	{
	case EMPTY_TILE:
		return;
	case NORMAL_TILE:
		glColor3f(0, 0, 0);
		break;
	case KILL_TILE:
		glColor3f(1, 0, 0);
		break;
	case GRAVITY_FREE_TILE:
		glColor3f(0, 0.5, 0);
		break;
	case BLIND_TILE:
		glColor3f(0.5, 0.5, 0.5);
		break;
	case BLINK_TILE:
		if (blink == true)
		{
			glColor3f(1, 0, 0);
		}
		else if (blink == false)
		{
			return;
		}
		break;
	case ROTATE_TILE:
		glColor3f(1, 1, 0);
		break;
	case END_TILE:
		glColor3f(0, 0, 1);
		break;
	case 8:
		glColor4f(0, 0, 1, alpha);
	}


	glPushMatrix();
		glRotated(degree, 0, 1, 0);	// y�� ȸ���Ͽ� ���ڸ��� �ȴ´�
	glTranslated(x, y, z);
	glRotated(distroy_degree, 1, 0, 0);
		glRotated(90, 1, 0, 0);
		glPushMatrix();
		glTranslated(0, 0, half_height);	// ���� ���� �׸���
			gluPartialDisk(up_side, 0, 1.0, 26, 13, 0.0, ANGLE);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, -half_height);	// �Ʒ��� �׸���
			gluPartialDisk(down_side, 0, 1.0, 26, 13, 0.0, ANGLE);
		glPopMatrix();
		

		Draw_Side(LEFT_SIDE_DEGREE);
		Draw_Side(RIGHT_SIDE_DEGREE);
		// ���� �׸���

		Draw_Surface(LEFT_SIDE_DEGREE, LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 1);
		Draw_Surface(LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 1, LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 2);
		Draw_Surface(LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 2, LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 3);
		Draw_Surface(LEFT_SIDE_DEGREE + MID_CTRL_DEGREE * 3, RIGHT_SIDE_DEGREE);
		// 4���� ������� ����� ���� ����� �׸���.

	glPopMatrix();
}
void Tile::Draw_Side(int degree)
{
	double radian = Your_Radian(degree);

	glPushMatrix();
	glTranslated(0, 0, 0);
		glBegin(GL_POLYGON);
		glNormal3f(0.0, -1, 0);
			glVertex3f(cos(radian), sin(radian), half_height);
			glVertex3f(0.0, 0.0, half_height);
			glVertex3f(0.0, 0.0, -half_height);
			glVertex3f(cos(radian), sin(radian), -half_height);
		glEnd();
	glPopMatrix();
}
void Tile::Draw_Surface(double left_degree, double right_degree)
{
	double radian_L{ Your_Radian(left_degree) }, 
		   radian_R{ Your_Radian(right_degree) };
	double radian_mid = Your_Radian((left_degree + right_degree) / 2);	// �߰� ������ ����

	GLfloat ctrlpoints[3][3][3] = { {{cos(radian_L), sin(radian_L), half_height},  {cos(radian_mid), sin(radian_mid), half_height}, {cos(radian_R), sin(radian_R), half_height}},
									{{cos(radian_L), sin(radian_L), 0.0},  {cos(radian_mid), sin(radian_mid), 0.0}, {cos(radian_R), sin(radian_R), 0.0}},
									{{cos(radian_L), sin(radian_L), -half_height},  {cos(radian_mid), sin(radian_mid), -half_height}, {cos(radian_R), sin(radian_R), -half_height}}};
	// ��Ʈ������Ʈ �ʱ�ȭ

	glPushMatrix();
	glTranslated(0, 0, 0);
		glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 3, 0.0, 1.0, 9, 3, &ctrlpoints[0][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
		glEvalMesh2(GL_FILL, 0, 10, 0, 10);
		// ���׸���
	glPopMatrix();
}
double Tile::Your_Radian(int degree)
{
	const double PI = 3.141592;
	return degree * PI / 180;
}

void Tile::Load_Data(int data)
{
	state = data;
	// �����Ͱ� ����
}


int Tile::Get_state()
{
	return state;
}

void Tile::Update()
{
	
}

//for i in range(0, 100 + 1, 2) :
//	t = i / 100
//	x = (1 - t)*p1[0] + t * p2[0]
//	y = (1 - t)*p1[1] + t * p2[1]
//	draw_point((x, y))
void Tile::Distroy()
{
	state = 8;
	alpha -= 0.05;
	distroy_degree += 5;
	double t = Distroy_timer / 100;
	std::cout << t << std::endl;
	z = (2 * t*t - 3 * t + 1) * 0 + (-4 * t*t + 4 * t)*2.7 + (2 * t*t - t) * 4;
	y = (2 * t*t - 3 * t + 1) * 0 + (-4 * t*t + 4 * t)*-2.2 + (2 * t*t - t)*-4;
	Distroy_timer+=2;
	if (Distroy_timer == 100)
	{
		state = 0;
	}
	
}