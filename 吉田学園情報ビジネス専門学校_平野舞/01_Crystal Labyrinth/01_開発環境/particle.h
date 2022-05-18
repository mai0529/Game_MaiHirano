//-------------------------------------------
//
//�p�[�e�B�N�����[particle.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�G�t�F�N�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR col;			//�J���[
	float fSize;			//�T�C�Y
	float fGravity;			//�d��
	int nLife;				//���C�t
	int nType;				//���
	bool bUse;				//�g�p���Ă��邩�ǂ���
}Particle;

//�v���g�^�C�v�錾
void InitParticle(void);		//����������
void UninitParticle(void);		//�I������
void UpdateParticle(void);		//�X�V����
void DrawParticle(void);		//�`�揈��
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,int nLife,int nType, float fSize,float fGravity);	//�G�t�F�N�g�̐ݒ菈��

#endif