// ------------------------------------------
//
//�v���C���[�\�����[player.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�C���N���[�h�t�@�C��
#include "main.h"

//�}�N����`
#define PLAYER_DIS		(3.5f)		//���f���̈ړ�����
#define MAX_MODELPARTS	(30)		//�ő僂�f���̃p�[�c��
#define MAX_KEY			(20)		//�ő�Key����
#define MAX_KEYSET		(10)		//�ő�KeySet����

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//�ʏ���
	PLAYERSTATE_DAMAGE,			//�_���[�W���
	PLAYERSTATE_DATH,			//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//-----------------------------------------------------
//�O���t�@�C���ǂݍ��ޗp
//-----------------------------------------------------
//�O���t�@�C���̍\����
typedef struct
{
	int nIndex;			//�C���f�b�N�X
	int nParent;		//�e�C���f�b�N�X
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
}PlayerFile;

//-----------------------------------------------------
//�v���C���[�̃��f���p�[�c
//-----------------------------------------------------
//���f���̍\����
typedef struct
{
	LPD3DXMESH pMesh;				//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;			//�}�e���A�����ւ̃|�C���^
	DWORD nNumMat;					//�}�e���A�����̐�
	LPDIRECT3DTEXTURE9 pTexture;	//�e�N�X�`���̓ǂݍ���
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//����
	int nIdxModelParent;			//�e���f���̃C���f�b�N�X
}ModelParts;

//-----------------------------------------------------
//�v���C���[
//-----------------------------------------------------
//�v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 posOld;			//�O�̈ʒu
	D3DXVECTOR3 rot;			//����
	D3DXVECTOR3 rotDest;		//�ړI�̊p�x
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 size;			//�T�C�Y
	D3DXVECTOR3 vtxMin;			//�ŏ����_�l
	D3DXVECTOR3 vtxMax;			//�ő咸�_�l
	ModelParts aModel[MAX_MODELPARTS];		//���f���̃p�[�c��
	bool bUse;					//�g�p���邩�ǂ���
	PLAYERSTATE state;			//��ԊǗ�
	int nCntState;				//��ԊǗ��J�E���^�[
	int nLife;					//���C�t
	int nIdxShadow;				//�e�̔ԍ�
}Player;

//-----------------------------------------------------
//���[�V����
//-----------------------------------------------------
//Key�\���́@
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
}Key;

//KeySet�̍\����
typedef struct
{
	int nFrame;				//�t���[����
	Key g_Key[MAX_KEY];		//Key�̏��
}KeySet;

//Motion�\����
typedef struct
{
	int nLoop;		//���[�v����
	int nNumKey;	//�L�[��
	KeySet g_KeySet[MAX_KEYSET];		//KeySet�̏��
}MotionSet;


//�v���g�^�C�v�錾
void InitPlayer(void);				//����������
void UninitPlayer(void);			//�I������
void UpdatePlayer(void);			//�X�V����
void DrawPlayer(void);				//�`�揈��
void MovePlayer(void);				//�v���C���[�̈ړ�����
void MotionPlayer(int nMotion);		//���[�V��������
void StatePlayer(void);				//��ԏ���
void BlinkPlayer(int ratio);		//�_�ŏ���
void HitPlayer(int nDamage);		//�q�b�g����
float GetPlayerRot(void);			//�v���C���[�̊p�x���擾
Player * GetPlayer(void);			//�v���C���[���̎擾
void LoadPlayerFile(void);			//�v���C���[���f���̊O���t�@�C�����̓ǂݍ���
void LoadMotionPlayerFile(void);	//�v���C���[���[�V�����̊O���t�@�C�����̓ǂݍ���

#endif