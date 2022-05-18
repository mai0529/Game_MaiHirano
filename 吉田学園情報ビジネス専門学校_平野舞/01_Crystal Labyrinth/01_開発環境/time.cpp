//-------------------------------------------
//
//�^�C�}�[����[time.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "time.h"
#include "fade.h"
#include "start.h"

//�}�N����`
#define TIME_WIDTH		(20.0f)		//�^�C���̕�
#define TIME_HEIGHT		(45.0f)		//�^�C���̍���
#define TIME_MAX		(3)			//�^�C���̍ő包��

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTime[TIME_MAX];					//�^�C���̈ʒu
int g_nTime;										//�^�C���̒l
int g_nCntTime;										//���t���[����
bool g_bTimeFade;									//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//�^�C�}�[�̏���������
//-------------------------------------------
void InitTime(void)
{
	for (int nCount = 0; nCount < TIME_MAX; nCount++)
	{//�ʒu�̏�����
		g_posTime[nCount] = D3DXVECTOR3(600.0f + (25.0f * nCount), 30.0f, 0.0f);
	}
	g_nTime = 300;						//���b�Ԃ�
	int g_nCntTime = 60;				//60�t���[��
	g_bTimeFade = false;				//�t�F�[�h���Ă��Ȃ�

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&g_pTextureTime);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < TIME_MAX; nCount++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posTime[nCount].x - (TIME_WIDTH / 2.0f), g_posTime[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[1].pos = D3DXVECTOR3(g_posTime[nCount].x + (TIME_WIDTH / 2.0f), g_posTime[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[2].pos = D3DXVECTOR3(g_posTime[nCount].x - (TIME_WIDTH / 2.0f), g_posTime[nCount].y + (TIME_HEIGHT / 2.0f), 0);
		pVtx[3].pos = D3DXVECTOR3(g_posTime[nCount].x + (TIME_WIDTH / 2.0f), g_posTime[nCount].y + (TIME_HEIGHT / 2.0f), 0);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//�f�[�^��4���i�߂�
	}
		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffTime->Unlock();
}

//-------------------------------------------
//�^�C�}�[�̏I������
//-------------------------------------------
void UninitTime(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//-------------------------------------------
//�^�C�}�[�̍X�V����
//-------------------------------------------
void UpdateTime(void)
{
	int nTimePos[3];		//�e�N�X�`���ݒ�p

	//�X�^�[�g���̎擾
	Start * pStart = GetStart();

	if (!pStart->bUse)
	{//�X�^�[�g��false�ɂȂ�����^�C�}�[�J�n
		if (g_nTime != 0)
		{//�^�C�}��0�ł͂Ȃ�������t���[���������炷
			g_nCntTime--;
		}

		if (g_nCntTime <= 0)
		{//�t���[����0�ɂȂ�����^�C�}�����炷
			g_nTime--;
			g_nCntTime = 60;
		}
	}

	nTimePos[0] = g_nTime % 1000 / 100;
	nTimePos[1] = g_nTime % 100 / 10;
	nTimePos[2] = g_nTime % 10;

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;		//�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

	if (g_nTime == 0 && !g_bTimeFade)
	{
			SetFade(MODE_GAMEOVER);		//�Q�[���I�[�o�[��ʂɈڍs
			g_bTimeFade = true;			//�t�F�[�h���Ă���
	}
}

//-------------------------------------------
//�^�C�}�[�̕`�揈��
//-------------------------------------------
void DrawTime(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTime);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

	//�A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//-------------------------------------------
//�^�C�}�[�̎擾
//-------------------------------------------
int GetTime(void)
{
	return g_nTime;
}