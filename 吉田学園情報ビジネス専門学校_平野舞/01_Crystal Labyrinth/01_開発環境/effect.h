//-------------------------------------------
//
//�G�t�F�N�g���[effect.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�G�t�F�N�g�̎��
typedef enum
{
	EFFECT_CIRCLE = 0,		//�܂�
	EFFECT_SUMMON,			//�����o�������Ȃ��
	EFFECT_RING,			//���F�̗�
	EFFECT_BLUE_RING,		//�̂ڂ�ڂ�
	EFFECT_BLUE_CIRCLE,		//�̂܂�
	EFFECT_TWINKLE,			//�L���L��
	EFFECT_FIREWORK,		//�ԉ�
	EFFECT_MAX
}EFFECT;

//�G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR col;			//�J���[
	float fSize;			//�T�C�Y
	int nLife;				//���C�t
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Effect;

//�v���g�^�C�v�錾
void InitEffect(void);			//����������
void UninitEffect(void);		//�I������
void UpdateEffect(void);		//�X�V����
void DrawEffect(void);			//�`�揈��
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife,float fSize,int nType);	//�G�t�F�N�g�̐ݒ菈��
void LoadEffectFile(void);		//�O���t�@�C�����̓ǂݍ���

#endif