//-------------------------------------------
//
//�e���[bullet.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

//�C���N���[�h�t�@�C��
#include "main.h"
#include "shadow.h"
#include "player.h"

//�}�N����`
#define MAX_BULLET		(300)		//�e�̍ő吔

//�ǂ̍\����
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	int nIdxShadow;					//�e
	int nLife;						//���C�t
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);					//����������
void UninitBullet(void);				//�I������
void UpdateBullet(void);				//�X�V����
void DrawBullet(void);					//�`�揈��
void SetBullet(D3DXVECTOR3 pos);		//�e�̐ݒ菈��

#endif