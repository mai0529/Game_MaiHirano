//-------------------------------------------
//
//�X�e�[�W���[stage.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _STAGE_H_
#define _STAGE_H_

//�C���N���[�h�t�@�C��
#include "main.h"
#include "maze.h"

//�}�N����`
#define MAX_STAGE		(MAX_STAGEX * MAX_STAGEX)				//�ǂ̍ő吔

//�X�e�[�W�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 size;			//�T�C�Y
	D3DXVECTOR3 vtxMin;			//�ŏ����_�l
	D3DXVECTOR3 vtxMax;			//�ő咸�_�l
	bool bUse;					//�g�p���Ă��邩�ǂ���
	LPD3DXMESH pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;				//�}�e���A�����̐�
}Stage;

//�v���g�^�C�v�錾
void InitStage(void);			//����������
void UninitStage(void);			//�I������
void UpdateStage(void);			//�X�V����
void DrawStage(void);			//�`�揈��
void SetStage(D3DXVECTOR3 pos);	//�ݒ菈��
void CollisionStage(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);		//�����蔻�菈��
void LoadStage(void);			//�O���t�@�C���̓ǂݍ���
Stage * GetStage(void);			//�����擾

#endif

