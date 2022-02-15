#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "tower.h"

void Tower::Load_Map()
{
	FILE *fp = nullptr;

	switch (state)
	{
	case EASY_1:
		fp = fopen("Map/easy1.txt", "rt");
		break;
	case EASY_2:
		fp = fopen("Map/easy2.txt", "rt");
		break;
	case NORMAL_1:
		fp = fopen("Map/normal1.txt", "rt");
		break;
	case NORMAL_2:
		fp = fopen("Map/normal2.txt", "rt");
		break;
	case HARD_1:
		fp = fopen("Map/hard1.txt", "rt");
		break;
	case HARD_2:
		break;
	}
	// ���̵��� ���� ������ �ҷ�����

	if (fp == NULL)
	{
		std::cout << "Failed";
		return;
	}

	int tile_data;

	for (int i = 0; i < num_of_stages; i++)
	{
		for (int j = 0; j < NUM_OF_TILES; j++)
		{
			fscanf(fp, "%d", &tile_data);
			stage[i].Load_Tile_Data(tile_data, j);
			// ���������� Ÿ���� �ѹ��� �����͸� ����
		}
	}
}