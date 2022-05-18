// ------------------------------------------ -
//
//�G�\�����[enemy.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�}�N����`
#define MAX_ENEMY		(128)		//�G�̍ő吔

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//�ʏ���
	ENEMYSTATE_DAMAGE,			//�_���[�W���
	ENEMYSTATE_DATH,			//���S���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posDis;			//�����̍�
	D3DXVECTOR3 posOld;			//�O��̈ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 size;			//�T�C�Y
	D3DXVECTOR3 vtxMin;			//�ŏ����_�l
	D3DXVECTOR3 vtxMax;			//�ő咸�_�l
	LPD3DXMESH pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;				//�}�e���A�����̐�
	ENEMYSTATE state;			//��ԊǗ�
	int nCounter;				//�J�E���g
	int nCntState;				//��ԊǗ��J�E���^�[
	int nLife;					//���C�t
	bool bUse;					//�g�p���邩�ǂ���
	bool bDis;					//�\�����邩�ǂ���
	bool bRot;					//��]���邩�ǂ���
	bool bAttack;				
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);				//����������
void UninitEnemy(void);				//�I������
void UpdateEnemy(void);				//�X�V����
void DrawEnemy(void);				//�`�揈��
void SetEnemy(D3DXVECTOR3 pos);		//�ݒ菈��
void StateEnemy(int nCntEnemy);		//��ԊǗ�
void BlinkEnemy(int nCntEnemy,int ratio);		//�_�ŏ���
void HitEnemy(int nCntEnemy,int nDamage);		//�q�b�g����
void FollowEnemy(int nCntEnemy);	//�Ǐ]����
void CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);		//�����蔻��
Enemy * GetEnemy(void);				//���̎擾

#endif
