//-------------------------------------------
//
//�Q�[���^�C�g������[title.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "title.h"
#include "fade.h"
#include "sound.h"
#include "input.h"
#include "controller.h"

//�}�N����`
#define MAX_TITLE		(2)					//�w�i�摜�̍ő吔
#define ENTER_WIDTH		(500.0f)			//��
#define ENTER_HEIGHT	(100.0f)			//����
#define TITLE_RATIO		(30)				//�_�ł̊���
#define TITLE_BLINKCOUNTER		(60)		//�_�ŃJ�E���^�[

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE] = {};			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle[MAX_TITLE] = {};	//���_�o�b�t�@�ւ̃|�C���^
int g_nCounter;												//�_�ŗp�J�E���^�[
bool g_bTitleFade;											//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitTitle(void)
{
	//������
	g_nCounter = TITLE_BLINKCOUNTER;	//�_�ŗp
	g_bTitleFade = false;				//�t�F�[�h�p
							
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���(�w�i)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title001.jpg",
		&g_pTextureTitle[0]);

	//�e�N�X�`���̓ǂݍ���(ENTER_PRESS)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/press_enter.png",
		&g_pTextureTitle[1]);

	for (int nCount = 0; nCount < MAX_TITLE; nCount++)
	{//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffTitle[nCount],
			NULL);
	}

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//-----------------------�w�i-------------------------------------
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
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
	g_pVtxBuffTitle[0]->Unlock();

	//-----------------------ENTER_PRESS-------------------------------------
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0) - (ENTER_WIDTH / 2.0f), 500.0f - (ENTER_HEIGHT / 2.0f), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0) + (ENTER_WIDTH / 2.0f), 500.0f - (ENTER_HEIGHT / 2.0f), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0) - (ENTER_WIDTH / 2.0f), 500.0f + (ENTER_HEIGHT / 2.0f), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((SCREEN_WIDTH / 2.0) + (ENTER_WIDTH / 2.0f), 500.0f + (ENTER_HEIGHT / 2.0f), 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle[1]->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitTitle(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int nCount = 0; nCount < MAX_TITLE; nCount++)
	{//�e�N�X�`���̔j��
		if (g_pTextureTitle[nCount] != NULL)
		{
			g_pTextureTitle[nCount]->Release();
			g_pTextureTitle[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TITLE; nCount++)
	{//���_�o�b�t�@�̔j��
		if (g_pVtxBuffTitle[nCount] != NULL)
		{
			g_pVtxBuffTitle[nCount]->Release();
			g_pVtxBuffTitle[nCount] = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateTitle(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle[1]->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounter--;		//�J�E���g�����炷

	//EnterPress�̓_�ŗp
	BlinkTitle(pVtx, TITLE_RATIO);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle[1]->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) && !g_bTitleFade
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_START) && !g_bTitleFade)
	{//ENTER�L�[�A�R���g���[���[��START�������ꂽ��
			PlaySound(SOUND_LABEL_SE000);		//SE�̍Đ�
			SetFade(MODE_TUTORIAL);				//�`���[�g���A�����[�h�Ɉڍs
			g_bTitleFade = true;				//�t�F�[�h���Ă���
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawTitle(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCount = 0; nCount < MAX_TITLE; nCount++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle[nCount], 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCount]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-------------------------------------------
//�_�ŏ���
//
//int ratio �� �_�ł̊���
//-------------------------------------------
void BlinkTitle(VERTEX_2D *pVtx,int ratio)
{
	if (0 == g_nCounter % ratio)
	{//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	}

	if (10 == g_nCounter % ratio)
	{//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.0f);
	}

	if (g_nCounter == 0)
	{//�J�E���^�[��0�ɂȂ�����
		g_nCounter = TITLE_BLINKCOUNTER;		//60�ɖ߂�
	}
}