// ------------------------------------------
//
//���C�t�\�����[life.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _LIFE_H_
#define _LIFE_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//���C�t�̍\���́@
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int nMaxLife;				//�ő僉�C�t��
	int nNowLife;				//���̃��C�t��
	bool bUse;					//�g�p���邩�ǂ���
}Life;

//�v���g�^�C�v�錾
void InitLife(void);		//����������
void UninitLife(void);		//�I������
void UpdateLife(void);		//�X�V����
void DrawLife(void);		//�`�揈��
void SetLife(D3DXVECTOR3 pos,int nMax,int nNow);			//�ݒ菈��
void SubLife(int nCntLife, int nReduce);					//���Z����
void SetPositionLife(int nIdxLife, D3DXVECTOR3 pos);		//�ʒu�X�V�̏���

#endif