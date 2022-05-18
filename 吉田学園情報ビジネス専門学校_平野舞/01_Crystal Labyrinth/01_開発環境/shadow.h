//-------------------------------------------
//
//�e���[Shadow.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 size;		//�T�C�Y
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Shadow;

//�v���g�^�C�v�錾
void InitShadow(void);			//����������
void UninitShadow(void);		//�I������
void UpdateShadow(void);		//�X�V����
void DrawShadow(void);			//�`�揈��
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);		//�e�̐ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);	//�e�̈ʒu�X�V����

#endif