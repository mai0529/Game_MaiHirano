//-------------------------------------------
//
//���U���g����[result.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "time.h"
#include "controller.h"

//�}�N����`
#define RESULT_WIDTH	(100.0f)		//���U���g�̕�
#define RESULT_HEIGHT	(150.0f)		//���U���g�̍���
#define RESULT_MAX		(2)				//���U���g�̍ő�摜
#define RESULT_BUFFMAX	(2)				//�o�b�t�@�̍ő吔
#define MAX_RESULT		(3)				//X�̍ő吔

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_MAX] = {};				//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult[RESULT_BUFFMAX] = {};		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posRank[MAX_RESULT];									//�^�C���̈ʒu
Result g_nResult;													//�X�R�A�̒l
bool g_bResultFade;													//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitResult(void)
{
	for (int nCount = 0; nCount < MAX_RESULT; nCount++)
	{//�X�R�A�̐ݒu
		g_posRank[nCount] = D3DXVECTOR3(520.0f + (120.0f * nCount), 480.0f, 0.0f);
	}
	g_bResultFade = false;		//�t�F�[�h���Ă��Ȃ�

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���(�w�i)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result003.jpg",
		&g_pTextureResult[0]);

	//�e�N�X�`���̓ǂݍ���(���l)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&g_pTextureResult[1]);

	//���_�o�b�t�@�̐���(�w�i)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult[0],
		NULL);

	//���_�o�b�t�@�̐���(���l)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult[1],
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//----------------------------------�w�i---------------------------------------------------------------
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[0]->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult[0]->Unlock();

	//----------------------------------�^�C��---------------------------------------------------------------
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_RESULT; nCount++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posRank[nCount].x - (RESULT_WIDTH / 2.0f), g_posRank[nCount].y - (RESULT_HEIGHT / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posRank[nCount].x + (RESULT_WIDTH / 2.0f), g_posRank[nCount].y - (RESULT_HEIGHT / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posRank[nCount].x - (RESULT_WIDTH / 2.0f), g_posRank[nCount].y + (RESULT_HEIGHT / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posRank[nCount].x + (RESULT_WIDTH / 2.0f), g_posRank[nCount].y + (RESULT_HEIGHT / 2.0f), 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult[1]->Unlock();

	//���U���g�̐ݒu
	SetResult();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int nCount = 0; nCount < RESULT_MAX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCount] != NULL)
		{
			g_pTextureResult[nCount]->Release();
			g_pTextureResult[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < RESULT_BUFFMAX; nCount++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffResult[nCount] != NULL)
		{
			g_pVtxBuffResult[nCount]->Release();
			g_pVtxBuffResult[nCount] = NULL;
		}
	}
}

//-------------------------------------------
//�X�V��������
//-------------------------------------------
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bResultFade
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bResultFade)
	{
		PlaySound(SOUND_LABEL_SE000);	//SE�̍Đ�
		SetFade(MODE_RANKING);			//���̉�ʐݒ�
		g_bResultFade = true;			//�t�F�[�h���Ă���
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawResult(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult[0], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�w�i
	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureResult[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�X�R�A
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult[1], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntX = 0; nCntX < 3; nCntX++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureResult[1]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntX * 4, 2);
	}
}

//-------------------------------------------
//���Z�b�g����
//-------------------------------------------
void ResetResult(void)
{
	g_nResult.nTime = 300;		//�^�C���̃��Z�b�g
}

//-------------------------------------------
//�ݒ菈��
//-------------------------------------------
void SetResult(void)
{
	//�X�R�A�̏��
	int nTime = GetTime();
	g_nResult.nTime = 300 - nTime;

	int aPosTexU[3];

	aPosTexU[0] = g_nResult.nTime % 1000 / 100;
	aPosTexU[1] = g_nResult.nTime % 100 / 10;
	aPosTexU[2] = g_nResult.nTime % 10 / 1;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult[1]->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	for (int nCount = 0; nCount < 3; nCount++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aPosTexU[nCount]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCount], 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aPosTexU[nCount]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult[1]->Unlock();
}