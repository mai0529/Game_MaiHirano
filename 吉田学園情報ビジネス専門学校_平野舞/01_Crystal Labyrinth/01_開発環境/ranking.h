//-------------------------------------------
//
//�����L���O���[ranking.h]
//Author:���앑
//
//-------------------------------------------

#ifndef _RANKING_H_
#define _RANKING_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�����L���O�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nTime;			//����
}Rank;

//�v���g�^�C�v�錾
void InitRanking(void);		//����������
void UninitRanking(void);	//�I������
void UpdateRanking(void);	//�X�V����
void DrawRanking(void);		//�`�揈��
void ResetRanking(void);	//�����L���O�̃��Z�b�g����
void SetRanking(void);		//�ݒ菈��

#endif