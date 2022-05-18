//-------------------------------------------
//
//���U���g���[result.h]
//Author:���앑
//
//-------------------------------------------

#ifndef _RESULT_H_
#define _RESULT_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//���U���g�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nTime;			//�X�R�A
}Result;

//�v���g�^�C�v�錾
void InitResult(void);		//����������
void UninitResult(void);	//�I������
void UpdateResult(void);	//�X�V����
void DrawResult(void);		//�`�揈��
void ResetResult(void);		//���Z�b�g����
void SetResult(void);		//�ݒ菈��

#endif