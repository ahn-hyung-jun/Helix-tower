#include "ball.h"
#include "tower.h"
#include <iostream>
extern int Game_state;
extern Tower tower;
#define GRAVITY 9.8

#define EMPTY_TILE 0
#define NORMAL_TILE 1
#define KILL_TILE 2
#define GRAVITY_FREE_TILE 3
#define BLIND_TILE 4
#define BLINK_TILE 5
#define ROTATE_TILE 6
#define END_TILE 9

Ball::Ball()
{
	x = 0;
	y = 0.5;
	z = 0.5;
	size = 0.07;
	floor = 0;
	speed = 0;
	Squeeze_timer = 1;
	Squeeze = 0;
	camera_follow = false;
	life = true;
	wire_size = 0.07;
}

void Ball::Draw()
{
	Draw_ink();
	glPushMatrix();
	{
		glColor3f(1, 0.9 - (speed * 2), 0.9 - (speed * 2));
		glTranslated(x, y, z);
		glScaled(1, Squeeze_timer, 1);
		glutSolidSphere(size, 10, 10);
		glutWireSphere(wire_size, 20, 20);
	}
	glPopMatrix();
}

void Ball::Update()
{
	speed += 0.0005*GRAVITY;
	y -= speed;

	for (int i = 0; i < 3; i++)
	{
		if (ink[i].exist == true)
		{
			ink[i].timer++;
			if (ink[i].timer > 60)
			{
				ink[i].timer = 0;
				ink[i].exist = false;
			}
		}
	}

	if (Squeeze == 2)
	{
		Squeeze_timer -= 0.1;
		if (Squeeze_timer < 0.5)
		{
			Squeeze = 1;
		}
	}
	else if (Squeeze == 1)
	{
		Squeeze_timer += 0.1;
		if (Squeeze_timer > 1)
		{
			Squeeze = 0;
			Squeeze_timer = 1;
		}
	}

	if (life == false)
	{
		wire_size += 0.1;
		if (wire_size > 5)
		{
			Fail();
		}
	}
}

bool Ball::Collide(int floor, int tile_state)
{
	if (speed < 0.25)
	{
		if (y < floor + 0.2 && tile_state == NORMAL_TILE)
		{
			speed = -0.1;
			camera_follow = false;
			Squeeze = 2;
			//PlaySound("Sound/bounce.wav", NULL, SND_ASYNC);
		}
		else if (y < floor + 0.2 && tile_state == ROTATE_TILE)
		{
			speed = -0.1;
			camera_follow = false;
			Squeeze = 2;
			tower.Rotate_half();
			
			//PlaySound("Sound/bounce.wav", NULL, SND_ASYNC);
		}
		else if (y < floor + 0.2 && tile_state == BLIND_TILE)
		{
			speed = -0.1;
			camera_follow = false;
			Squeeze = 2;

			for (int i = 0; i < 3; i++)
			{
				if (ink[i].exist == false)
				{
					ink[i].exist = true;
					ink[i].x1 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].y1 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].x2 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].y2 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].x3 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].y3 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].x4 = (rand() / (double)RAND_MAX) * 2 - 1;
					ink[i].y4 = (rand() / (double)RAND_MAX) * 2 - 1;
					break;
				}
			}
		}
		else if (y < floor + 0.4 && tile_state == GRAVITY_FREE_TILE)
		{
			speed = -0.0007*GRAVITY;

		}
		else if (y < floor + 0.1 && tile_state == EMPTY_TILE)
		{
			this->floor += 1;
			camera_follow = true;
		}
		else if (y < floor + 0.1 && tile_state == 8)
		{
			this->floor += 1;
			camera_follow = true;
		}
		else if (y < floor + 0.1 && tile_state == KILL_TILE)
		{
			if (life == true)
			{
				//PlaySound("Sound/die.wav", NULL, SND_ASYNC);
			}
			speed = 0;
			life = false;
		}
		else if (y < floor + 0.1 && tile_state == BLINK_TILE)
		{
			if (life == true)
			{
				//PlaySound("Sound/die.wav", NULL, SND_ASYNC);
			}
			speed = 0;
			life = false;
		}
		else if (y < floor + 0.1 && tile_state == END_TILE)
		{
			Victory();
		}
	}
	if (speed >= 0.25)
	{
		if (y < floor + 0.1 && tile_state == EMPTY_TILE)
		{
			this->floor += 1;
			camera_follow = true;
		}
		else if (y < floor + 0.1&&tile_state != EMPTY_TILE)
		{
			speed = -0.1;
			camera_follow = true;
			//PlaySound("Sound/break.wav", NULL, SND_ASYNC);
			return true;
		}
	}

			
	return false;
}

int Ball::Get_floor()
{
	return floor;
}

double Ball::Get_y()
{
	return y;
}

bool Ball::Get_camera()
{
	return camera_follow;
}

void Ball::Draw_ink()
{
	glPushMatrix();
	{
		glLoadIdentity();
		for (int j = 0; j < 3; j++)
		{
			if (ink[j].exist == true)
			{
				glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++)
				{
					glColor3f(0, 0, 0);
					glVertex3f(ink[j].x1 + 0.1*cos(i), ink[j].y1 + 0.1*sin(i), 0);
				}
				glEnd();
				glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++)
				{
					glColor3f(0, 0, 0);
					glVertex3f(ink[j].x2 + 0.2*cos(i), ink[j].y2 + 0.2*sin(i), 0);
				}
				glEnd();
				glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++)
				{
					glColor3f(0, 0, 0);
					glVertex3f(ink[j].x3 + 0.3*cos(i), ink[j].y3 + 0.3*sin(i), 0);
				}
				glEnd();
				glBegin(GL_POLYGON);
				for (int i = 0; i < 360; i++)
				{
					glColor3f(0, 0, 0);
					glVertex3f(ink[j].x4 + 0.4*cos(i), ink[j].y4 + 0.4*sin(i), 0);
				}
				glEnd();
			}
		}
	}
	glPopMatrix();
}

void Ball::Fail()
{
	Game_state = 4;
}

void Ball::Victory()
{
	Game_state = 5;
}
