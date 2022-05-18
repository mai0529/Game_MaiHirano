//-------------------------------------------
//
//�Q�[������[game.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "game.h"
#include "map.h"
#include "fade.h"
#include "time.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "floor.h"
#include "start.h"
#include "effect.h"
#include "particle.h"
#include "item.h"
#include "goal.h"
#include "time.h"
#include "stage.h"
#include "enemy.h"
#include "life.h"
#include "sound.h"

//-------------------------------------------
//����������
//-------------------------------------------
void InitGame(void)
{
	//�^�C���̏�����
	InitTime();

	//�e�̏�����
	InitShadow();

	//���C�t�̏�����
	InitLife();

	//�}�b�v�̏�����
	InitMap();

	//�A�C�e���̏�����
	InitItem();

	//�G�̏�����
	InitEnemy();

	//�v���C���[�̏�����
	InitPlayer();

	//�e�̏�����
	InitBullet();

	//�ǂ̏�����
	InitStage();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�p�[�e�B�N���̏�����
	InitParticle();

	//���̏�����
	InitFloor();

	//�X�^�[�g�̏�����
	InitStart();

	//�S�[���̏�����
	InitGoal();

	//�J�����̏�����
	InitCamera();

	//���C�g�̏�����
	InitLight();
	
	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�^�C���̏I��
	UninitTime();

	//���̏I��
	UninitFloor();

	//�X�^�[�g�̏I��
	UninitStart();

	//�S�[���̏I��
	UninitGoal();

	//�v���C���[�̏I��
	UninitPlayer();

	//�e�̏I��
	UninitBullet();

	//�G�̏I��
	UninitEnemy();

	//���C�t�̏I��
	UninitLife();

	//�A�C�e���̏I��
	UninitItem();

	//�G�t�F�N�g�̏I��
	UninitEffect();

	//�p�[�e�B�N���̏I��
	UninitParticle();

	//�e�̏I��
	UninitShadow();

	//�}�b�v�̏I��
	UninitMap();

	//�ǂ̏I��
	UninitStage();

	//�J�����̏I��
	UninitCamera();

	//���C�g�̏I��
	UninitLight();
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateGame(void)
{
	//�^�C���̍X�V
	UpdateTime();

	//���̍X�V
	UpdateFloor();

	//�v���C���[�̍X�V
	UpdatePlayer();

	//�e�̍X�V
	UpdateBullet();

	//�G�̍X�V
	UpdateEnemy();

	//���C�t�̍X�V
	UpdateLife();

	//�A�C�e���̍X�V
	UpdateItem();

	//�X�^�[�g�̍X�V
	UpdateStart();

	//�S�[���̍X�V
	UpdateGoal();

	//�G�t�F�N�g�̍X�V
	UpdateEffect();

	//�p�[�e�B�N���̍X�V
	UpdateParticle();

	//�e�̍X�V
	UpdateShadow();

	//�ǂ̍X�V
	UpdateStage();

	//�}�b�v�̍X�V
	UpdateMap();

	//�J�����̍X�V
	UpdateCamera();

	//���C�g�̍X�V
	UpdateLight();
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawGame(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�{�[�g
	D3DVIEWPORT9 viewport;

	//�r���[�{�[�g�̐ݒ�
	pDevice->GetViewport(&viewport);

	//�J�������̎擾
	Camera * pCamera = GetCamera();

	for (int nCount = 0; nCount < MAX_CAMERA; nCount++, pCamera++)
	{
		//�r���[�|�[�g�̐ݒ�
		pDevice->SetViewport(&pCamera->viewport);

		//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		//�J�����̐ݒ�
		SetCamera(nCount);

		switch (nCount)
		{
		case 0:
			//�^�C���̕`��
			DrawTime();

			//�A�C�e���̕`��
			DrawItem();

			//�G�̕`��
			DrawEnemy();

			//���̕`��
			DrawFloor();

			//�ǂ̕`��
			DrawStage();

			//�v���C���[�̕`��
			DrawPlayer();

			//�X�^�[�g�̕`��
			DrawStart();

			//�S�[���̕`��
			DrawGoal();

			//�e�̕`��
			DrawShadow();

			//�e�̕`��
			DrawBullet();

			//���C�t�̕`��
			DrawLife();

			//�G�t�F�N�g�̕`��
			DrawEffect();
			break;

		case 1:
			//���̕`��
			DrawFloor();

			//�}�b�v�̕`��
			DrawMap();
			break;

		default:
			break;
		}
	}

	//�r���[�{�[�g�̐ݒ�
	pDevice->SetViewport(&viewport);
}