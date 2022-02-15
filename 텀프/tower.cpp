#include "tower.h"
#include <iostream>

extern int Game_state;

void Tower::Initialize(int state)
{
	this->state = state;

	if (this->state == EASY_1 || this->state == EASY_2)
		num_of_stages = 20;
	else if (this->state == NORMAL_1 || this->state == NORMAL_2)
		num_of_stages = 30;
	else if (this->state == HARD_1 || this->state == HARD_2)
		num_of_stages = 40;

	stage = new Stage[num_of_stages];
	ball = new Ball();
	current_degree = 0;
	rotate_degree = 0;
	for (int i = 0; i < num_of_stages; i++)
		stage[i].Initialize(i);	// ���κ��� ���° ���ΰ�

	Load_Map();	// ������ �ε�
}
void Tower::Draw_Tower()
{
	if (Game_state == 3)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glPushMatrix();
		glRotated(90, 1, 0, 0);
		glutSolidCylinder(0.3, 2.0 * num_of_stages, 20, 20);
		glPopMatrix();
		glRotated(-current_degree - rotate_degree, 0, 1, 0);
		stage[0].Draw_Stage();
		stage[1].Draw_Stage();
		stage[2].Draw_Stage();
		stage[3].Draw_Stage();
		stage[4].Draw_Stage();
		stage[5].Draw_Stage();
		if (ball->Get_floor() > 3)
		{
			for (int i = ball->Get_floor() - 3; i < ball->Get_floor() + 3; i++)
				stage[i].Draw_Stage();
		}
		glPopMatrix();
		ball->Draw();
	}
	else
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glPushMatrix();
		glRotated(90, 1, 0, 0);
		glutSolidCylinder(0.3, 2.0 * num_of_stages, 20, 20);
		glPopMatrix();
		glRotated(-current_degree - rotate_degree, 0, 1, 0);
		for (int i = 0; i < num_of_stages; i++)
			stage[i].Draw_Stage();
		glPopMatrix();
		ball->Draw();
	}
}


void Tower::Update()
{
	int current_floor = ball->Get_floor();
	rotate_degree = rotate_degree % 360;

	ball->Update();
	
	for (int i = 0; i < num_of_stages; i++)
	{
		stage[i].Update();
	}
	
	if (ball->Collide(stage[current_floor].Get_y(), stage[current_floor].Get_state_of_tile(current_degree + rotate_degree)) == true)
	{
		stage[current_floor].Distroy();
	}
	

}

void Tower::Rotate_plus()
{
	current_degree++;

}

void Tower::Rotate_minus()
{
	current_degree--;

}

double Tower::Get_ball_y()
{
	return ball->Get_y();
}

bool Tower::Get_ball_camera_follow()
{
	return ball->Get_camera();
}

int Tower::Get_ball_floor()
{
	return ball->Get_floor();
}

void Tower::Rotate_by_mouse(int degree)
{
	rotate_degree = degree;
}

void Tower::Fix_degree()
{
	current_degree = current_degree + rotate_degree;
	rotate_degree = 0;
}

void Tower::Rotate_half()
{
	current_degree += 180;
}