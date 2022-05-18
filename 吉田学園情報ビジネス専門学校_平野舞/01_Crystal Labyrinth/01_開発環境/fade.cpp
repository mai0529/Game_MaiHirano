//-------------------------------------------
//
//�t�F�[�h����[fade.cpp](������)
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "fade.h"

//�}�N����`
#define FADE_WIDTH		(640.0f)		//��
#define FADE_HEIGHT		(360.0f)		//����

//�O���[�o���ϐ�
//LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
FADE g_fade;										//�t�F�[�h�̏��
MODE g_modeNext;									//���̉��(���[�h)
D3DXCOLOR g_colorFade;								//�|���S��(�t�F�[�h)�̐F

//-------------------------------------------
//����������
//
//MODE modeNext �� ���̉�ʃ��[�h���w��
//-------------------------------------------
void InitFade(MODE modeNext)
{
	g_fade = FADE_IN;									//�t�F�[�h�C����Ԃ�
	g_modeNext = modeNext;								//���̉��(���[�h)��ݒ�
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S��(�s����)�ɂ��Ă���

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(640.0f - FADE_WIDTH, 360.0f - FADE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + FADE_WIDTH, 360.0f - FADE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - FADE_WIDTH, 360.0f + FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + FADE_WIDTH, 360.0f + FADE_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateFade(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����
			g_colorFade.a -= 0.05f;				//�|���S���𓧖��ɂ��Ă���
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;				//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			g_colorFade.a += 0.05f;				//�|���S����s�����ɂ��Ă���
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;				//�t�F�[�h�C����Ԃ�
												//���[�h�ݒ�
				SetMode(g_modeNext);			//��ʑJ�ڂ̐ݒ�
			}
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawFade(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------
//�t�F�[�h�̐ݒ�
//
//MODE modeNext �� ���̉�ʂ��w��(main.h��enum�^�Ŏw��)
//-------------------------------------------
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;									//�t�F�[�h�A�E�g��Ԃ�
	g_modeNext = modeNext;								//���̉��(���[�h)��ݒ�
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�����|���S��(����)�ɂ��Ă���
}

//-------------------------------------------
//�t�F�[�h�̎擾
//-------------------------------------------
FADE GetFade(void)
{
	return g_fade;
}