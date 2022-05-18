//-------------------------------------------
//
//�}�b�v���[map.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _MAP_H_
#define _MAP_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�u���b�N�}�b�v�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	int nType;					//���
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Map;

//�v���C���[�}�b�v�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
}PlayerMap;

//�G�}�b�v�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
}EnemyMap;

//�A�C�e���}�b�v�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
}ItemMap;

//�v���g�^�C�v�錾
void InitMap(void);					//����������
void UninitMap(void);				//�I������
void UpdateMap(void);				//�X�V����
void DrawMap(void);					//�`�揈��
void SetMap(D3DXVECTOR3 pos, int nType);	//�ݒ菈��
void SetEnemyMap(D3DXVECTOR3 pos);	//�G�̐ݒ菈��
void SetItemMap(D3DXVECTOR3 pos);	//�A�C�e���̐ݒ菈��
void SetPositionPlayerMap(D3DXVECTOR3 pos);						//�v���C���[�}�b�v�̈ʒu�X�V����
void SetPositionEnemyMap(int nIdxEnemy, D3DXVECTOR3 pos);		//�G�}�b�v�̈ʒu�X�V����
void UseItemMap(int nCntItemMap);	//�A�C�e���̊l������
void LoadMap(void);					//�O���t�@�C���̓ǂݍ���					
	
#endif