//-------------------------------------------
//
//�A�C�e�����[item.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

//�C���N���[�h�t�@�C��
#include "main.h"
#include "maze.h"

//�}�N����`
#define MAX_ITEM		(128)		//�A�C�e���̍ő吔

//�A�C�e���̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Item;

//�v���g�^�C�v�錾
void InitItem(void);				//����������
void UninitItem(void);				//�I������
void UpdateItem(void);				//�X�V����
void DrawItem(void);				//�`�揈��
void SetItem(D3DXVECTOR3 pos);		//�A�C�e���̐ݒ菈��
void CollisionItem(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//�A�C�e���̓����蔻��
int GetCntItem(void);				//�J�E���^�[�̎擾
Item * GetItem(void);				//�A�C�e���̏����擾

#endif