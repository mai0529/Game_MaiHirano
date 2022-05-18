//-------------------------------------------
//
//�S�[�����[goal.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _GOAL_H_
#define _GOAL_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�S�[���̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Goal;

//�v���g�^�C�v�錾
void InitGoal(void);		//����������
void UninitGoal(void);		//�I������
void UpdateGoal(void);		//�X�V����
void DrawGoal(void);		//�`�揈��
void SetGoal(D3DXVECTOR3 pos);			//�S�[���̐ݒ菈��
void CollisionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//�S�[���̓����蔻��	

#endif
