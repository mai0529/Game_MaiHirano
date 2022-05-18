//-------------------------------------------
//
//�J�������[camera.h]
//Author:���앑
//
//-------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�C���N���[�h�t�@�C��
#include "main.h"
#include "input.h"
#include "player.h"

//�}�N����`
#define MAX_CAMERA		(2)			//�J�����̍ő吔

//�\����
typedef struct
{
	D3DXVECTOR3 posV;				//���_
	D3DXVECTOR3 posR;				//�����_
	D3DXVECTOR3 posVDest;			//�ړI�̎��_
	D3DXVECTOR3 posRDest;			//�ړI�̒����_
	D3DXVECTOR3 vecU;				//������x�N�g��
	D3DXVECTOR3 rot;				//����(�p�x)
	D3DXVECTOR3 rotDest;			//�ړI�̌���(�p�x)
	float fDistance;				//���_���璍���_�̋���
	D3DXMATRIX mtxProjection;		//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X
	int nCounter;					//�J�E���^�[
	D3DVIEWPORT9 viewport;			//�r���[�|�[�g
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);				//����������
void UninitCamera(void);			//�I������
void UpdateCamera(void);			//�X�V����
void SetCamera(int nCntCamere);		//�Z�b�g����
Camera * GetCamera(void);			//�|�C���^�̎擾

#endif