//-------------------------------------------
//
//���H����[maze.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//�}�N����`
#define PATH			(0)			//��
#define WALL			(1)			//��
#define CRYSTAL			(2)			//�A�C�e��
#define MAX_ITEM		(20)		//�A�C�e���̍ő吔

//�@������ƃA�C�e���ݒu�ꏊ
typedef enum
{
	UP = 0,		//��
	DOWN,		//��O
	LEFT,		//��
	RIGHT,		//�E
	ITEM,		//�A�C�e��
	MAX
}DIRECTION;

//�e�}�X�̏����i�[����z��
static int maze[MAX_STAGEZ][MAX_STAGEX] = {};

//�e�L�X�g�t�@�C���ɏ������ޗp
int g_nMazeData;
char g_cMaze[256];

//-------------------------------------------
//����������
//-------------------------------------------
void InitMaze(void)
{
	time_t randam;

	//���݂̎������擾
	randam = time(NULL);

	//���݂̎����𗐐��̎�ɂ��ė����̐����n(�x�N�g��)��ύX����
	srand((unsigned int)randam);
}

//-------------------------------------------
//���H�\������
//-------------------------------------------
void MakeMaze(void)
{
	FILE * pFile;			//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data/maze.txt", "w");		//�O���t�@�C���ɏ�������
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
		{//Z�̃}�X����
			for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
			{//X�̃}�X����
				if (maze[nCntZ][nCntX] == PATH)
				{//�����@��
					fprintf(pFile, "0", g_nMazeData);		//�O���t�@�C���ɏ�������
				}

				if (maze[nCntZ][nCntX] == WALL)
				{//�ǂ����
					fprintf(pFile, "1", g_nMazeData);		//�O���t�@�C���ɏ�������
				}

				if (maze[nCntZ][nCntX] == CRYSTAL)
				{//�A�C�e����ݒu����
					fprintf(pFile, "2", g_nMazeData);		//�O���t�@�C���ɏ�������
				}
			}
			fprintf(pFile, "\n", g_nMazeData);			//�O���t�@�C���ɏ�������
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}
}

//-------------------------------------------
//�����@�鏈��
//-------------------------------------------
void DigMaze(int X, int Z)
{
	//�ǂ̕������@�낤�Ƃ������o���Ă����ϐ�
	int Up, Down, Left, Right;

	//������
	Up = 0;
	Down = 0;
	Left = 0;
	Right = 0;
	
	//�S���������܂Ń��[�v����
	while (Up == 0 || Down == 0 || Left == 0 || Right == 0)
	{
		//0�`5�܂ł������_���Ŏ擾
		int Date = rand() % 5;

		switch (Date)
		{
		case UP:		//���������@��
			if (Z + 2 >= 0 && Z + 2 < MAX_STAGEZ)
			{
				if (maze[Z + 2][X] == WALL)
				{//�����ǂȂ�
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z + 2][X] = PATH;		//�����@��
						maze[Z + 1][X] = PATH;		//�����@��
						DigMaze(X, Z + 2);
					}
				}
			}
			Up++;	//Up�̃J�E���g�𑝂₷
			break;
		case DOWN:		//��O�����Ɍ����@��
			if (Z - 2 >= 0 && Z - 2 < MAX_STAGEZ)
			{
				if (maze[Z - 2][X] == WALL)
				{//�����ǂȂ�
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z - 2][X] = PATH;		//�����@��
						maze[Z - 1][X] = PATH;		//�����@��
						DigMaze(X, Z - 2);
					}
				}
			}
			Down++;		//Down�̃J�E���g�𑝂₷
			break;
		case LEFT:		//�������Ɍ����@��
			if (X - 2 >= 0 && X - 2 < MAX_STAGEX)
			{
				if (maze[Z][X - 2] == WALL)
				{
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z][X - 2] = PATH;		//�����@��
						maze[Z][X - 1] = PATH;		//�����@��
						DigMaze(X - 2, Z);
					}
				}
			}
			Left++;		//Left�̃J�E���g�𑝂₷
			break;
		case RIGHT:		//�E�����Ɍ����@��
			if (X + 2 >= 0 && X + 2 < MAX_STAGEX)
			{
				if (maze[Z][X + 2] == WALL)
				{
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z][X + 2] = PATH;		//�����@��
						maze[Z][X + 1] = PATH;		//�����@��
						DigMaze(X + 2, Z);
					}
				}
			}
			Right++;	//Right�̃J�E���g�𑝂₷
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------
//���H��������
//-------------------------------------------
void CreateMaze(void)
{
	for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
		{
			maze[nCntZ][nCntX] = WALL;
		}
	}

	//�J�n�n�_�������_���ɐݒ肷��
	int DataX = 2 * (rand() % (MAX_STAGEX / 2)) + 1;
	int DataZ = 2 * (rand() % (MAX_STAGEZ / 2)) + 1;

	//DataX,DataZ��ʘH�ɐݒ肷��
	maze[DataZ][DataX] = PATH;

	//DataX,DataZ�����ݒn�Ƃ��Č����@��
	DigMaze(DataX, DataZ);
}