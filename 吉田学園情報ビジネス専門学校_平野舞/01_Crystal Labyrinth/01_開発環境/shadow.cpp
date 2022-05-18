//-------------------------------------------
//
//�e����[shadow.cpp]
//Author:���앑
//
//------------------------------------------- 

//�C���N���[�h�t�@�C��
#include "shadow.h"

//�}�N����`
#define MAX_SHADOW		(124)				//�e�̍ő吔
#define SHADOW_WIDTH	(16.0f)				//�e�̕�
#define SHADOW_HEIGHT	(16.0f)				//�e�̍���

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];						//�e�̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//�e���̏�����
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����
		g_aShadow[nCntShadow].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�T�C�Y�̏�����
		g_aShadow[nCntShadow].bUse = false;								//�g�p���Ă��邩�ǂ���
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(SHADOW_WIDTH / 2.0f), 0.0f,  (SHADOW_HEIGHT / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (SHADOW_WIDTH / 2.0f), 0.0f,  (SHADOW_HEIGHT / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(SHADOW_WIDTH / 2.0f), 0.0f, -(SHADOW_HEIGHT / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (SHADOW_WIDTH / 2.0f), 0.0f, -(SHADOW_HEIGHT / 2.0f));

		//�e���_�̖@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateShadow(void)
{
	//���͂Ȃ�
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse)
		{//�g�p���Ă�����
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

			//�ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//-------------------------------------------
//�e�̐ݒ菈��
//
//D3DXVECTOR3 pos  �� �ʒu
//D3DXVECTOR3 rot  �� ����
//D3DXVECTOR3 size �� �傫��
//-------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	int nCntShadow;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aShadow[nCntShadow].pos = pos;		//�ʒu
			g_aShadow[nCntShadow].rot = rot;		//����
			g_aShadow[nCntShadow].size = size;		//�T�C�Y
			g_aShadow[nCntShadow].bUse = true;		//�g�p����

			//�T�C�Y�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].size.x / 2.0f),0.0f, (g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[1].pos = D3DXVECTOR3((g_aShadow[nCntShadow].size.x / 2.0f), 0.0f, (g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].size.x / 2.0f),0.0f, -(g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[3].pos = D3DXVECTOR3((g_aShadow[nCntShadow].size.x / 2.0f), 0.0f, -(g_aShadow[nCntShadow].size.z / 2.0f));

			break;
		}
		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;				//�e�̔ԍ���Ԃ�
}

//-------------------------------------------
//�e�̈ʒu�̍X�V����
//
//int nIdxShadow  �� ���Ԗڂ̉e���w��
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos = pos;		//�ʒu�̍X�V
}