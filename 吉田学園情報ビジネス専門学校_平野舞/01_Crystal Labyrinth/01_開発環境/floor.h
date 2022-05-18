//-------------------------------------------
//
//���\�����[floor.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fWidth;			//��
	float fHeigth;			//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Floor;

//�v���g�^�C�v�錾
void InitFloor(void);			//����������
void UninitFloor(void);			//�I������
void UpdateFloor(void);			//�X�V����
void DrawFloor(void);			//�`�揈��
void SetFloor(D3DXVECTOR3 pos, float fWidth, float fHeigth);		//�ݒ菈��

#endif