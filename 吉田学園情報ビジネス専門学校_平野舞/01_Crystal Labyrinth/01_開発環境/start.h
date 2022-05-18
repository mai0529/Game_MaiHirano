//-------------------------------------------
//
//�~�����[start.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _START_H_
#define _START_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�X�^�[�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Start;

//�v���g�^�C�v�錾
void InitStart(void);		//����������
void UninitStart(void);		//�I������
void UpdateStart(void);		//�X�V����
void DrawStart(void);		//�`�揈��
void CollisionStart(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//�����蔻��
Start * GetStart(void);		//�����擾

#endif
