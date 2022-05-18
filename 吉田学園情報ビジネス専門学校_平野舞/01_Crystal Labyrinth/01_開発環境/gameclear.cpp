//------------------------------------------ -
//
//�Q�[���N���A����[gameclear.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "gameclear.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "controller.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureGameclear = NULL;				//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameclear = NULL;			//���_�o�b�t�@�ւ̃|�C���^
bool g_bClear;												//�Q�[���N���A����
bool g_bGameClearFade;										//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitGameclear(void)
{
	g_bClear = false;			//�N���A���Ă��Ȃ�
	g_bGameClearFade = false;	//�t�F�[�h���Ă��Ȃ�

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result001.jpg",
		&g_pTextureGameclear);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameclear,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameclear->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffGameclear->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitGameclear(void)
{
	//�T�E���h�̒�~
	StopSound();

	if (g_pTextureGameclear != NULL)
	{//�e�N�X�`���̔j��
		g_pTextureGameclear->Release();
		g_pTextureGameclear = NULL;
	}

	if (g_pVtxBuffGameclear != NULL)
	{//���_�o�b�t�@�̔j��
		g_pVtxBuffGameclear->Release();
		g_pVtxBuffGameclear = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateGameclear(void)
{
	//����L�[(ENTER�L�[)�������ꂽ���ǂ���
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bGameClearFade 
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bGameClearFade)
	{
			PlaySound(SOUND_LABEL_SE000);
			SetFade(MODE_RESULT);			//�����L���O��ʂɈڍs
			g_bClear = true;				//�Q�[���N���A
			g_bGameClearFade = false;		//�J�E���g��1�ɂ���
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawGameclear(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameclear, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureGameclear);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------
//�N���A�������ǂ����̏����l�����鏈��
//-------------------------------------------
bool GetClear(void)
{
	return g_bClear;
}