//-------------------------------------------
//
//���H���[maze.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _MAZE_H_
#define _MAZE_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�}�N����`
#define MAX_STAGEX		(13)		//X�T�C�Y
#define MAX_STAGEZ		(13)		//Z�T�C�Y

void InitMaze(void);			//����������
void MakeMaze(void);			//���H�\������
void DigMaze(int X, int Z);		//�����@�鏈��
void CreateMaze(void);			//���H��������

#endif
