//-------------------------------------------
//
//�Q�[����������[tutorial.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "tutorial.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "controller.h"

//�}�N����`
#define TITLE_WIDTH		(600.0f)	//��
#define TITLE_HEIGHT	(320.0f)	//����

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTutorial;								//�^�C�g���̈ʒu
bool g_bTutorialFade;									//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitTutorial(void)
{
	g_posTutorial = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���W�̏�����
	g_bTutorialFade = false;							//�t�F�[�h���Ă��Ȃ�

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_pTextureTutorial);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorial->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e�N�X�`���̔j��
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateTutorial(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bTutorialFade
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bTutorialFade)
	{//ENTER�L�[�A�R���g���[���[��A�L�[�������ꂽ��
			PlaySound(SOUND_LABEL_SE000);	//SE�̍Đ�
			SetFade(MODE_GAME);				//�Q�[�����[�h�Ɉڍs
			g_bTutorialFade = true;			//�t�F�[�h���Ă���
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawTutorial(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}