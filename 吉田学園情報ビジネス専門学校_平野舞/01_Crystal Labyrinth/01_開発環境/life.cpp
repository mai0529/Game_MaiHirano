// ------------------------------------------
//
//���C�t�\������[life.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "life.h"

//�}�N����`
#define MAX_LIFE		(64)		//�ő僉�C�t
#define MATLIFE_TEX		(2)			//�e�N�X�`���̍ő吔
#define MAXLIFE_VTX		(2)			//�o�b�t�@�̍ő吔

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureLife[MATLIFE_TEX] = {};			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife[MAXLIFE_VTX] = {};		//���_�o�b�t�@�ւ̃|�C���^
Life g_aLife[MAX_LIFE];											//���C�t���

//-------------------------------------------
//����������
//-------------------------------------------
void InitLife(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{//������
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aLife[nCntLife].nMaxLife = 0;								//�ő僉�C�t��
		g_aLife[nCntLife].nNowLife = 0;								//���̃��C�t��
		g_aLife[nCntLife].bUse = false;								//�g�p���Ȃ�
	}

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���(�Q�[�W)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/life001.png",
		&g_apTextureLife[0]);

	//�e�N�X�`���̓ǂݍ���(�g)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/life000.png",
		&g_apTextureLife[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife[0],
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife[1],
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//----------------------------------�Q�[�W--------------------------------------------------------
	//���_�o�b�t�@�����b�N
	g_pVtxBuffLife[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(28.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (28.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(28.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (28.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife[0]->Unlock();

	//----------------------------------�g--------------------------------------------------------
	//���_�o�b�t�@�����b�N
	g_pVtxBuffLife[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(30.0f / 2.0f), (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(( 30.0f / 2.0f), (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(30.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (30.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife[1]->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitLife(void)
{
	for (int nCntLife = 0; nCntLife < MATLIFE_TEX; nCntLife++)
	{//�e�N�X�`���̔j��
		if (g_apTextureLife[nCntLife] != NULL)
		{
			g_apTextureLife[nCntLife]->Release();
			g_apTextureLife[nCntLife] = NULL;
		}
	}

	for (int nCntLife = 0; nCntLife < MAXLIFE_VTX; nCntLife++)
	{//���_�o�b�t�@�̔j��
		if (g_pVtxBuffLife[nCntLife] != NULL)
		{
			g_pVtxBuffLife[nCntLife]->Release();
			g_pVtxBuffLife[nCntLife] = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateLife(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffLife[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse)
		{//�g�p���Ă�����
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 1.0f);

			if (g_aLife[nCntLife].nNowLife == 0)
			{//���C�t��0�ɂȂ�����
				g_aLife[nCntLife].bUse = false;		//�g�p���Ȃ�
			}
		}
		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife[0]->Unlock();
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawLife(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse)
		{//�g�p���Ă�����
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aLife[nCntLife].mtxWorld);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aLife[nCntLife].pos.x, g_aLife[nCntLife].pos.y, g_aLife[nCntLife].pos.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aLife[nCntLife].mtxWorld);

			for (int nCount = 0; nCount < 2; nCount++)
			{
				//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_pVtxBuffLife[nCount], 0, sizeof(VERTEX_3D));

				//���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, g_apTextureLife[nCount]);

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);

				//���C�g��L���ɖ߂�
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}
		}
	}
}

//-------------------------------------------
//�ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//int nMax		  �� ���C�t�̍ő吔
//int nNow		  �� ���݂̍ő吔
//-------------------------------------------
void SetLife(D3DXVECTOR3 pos, int nMax, int nNow)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (!g_aLife[nCntLife].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aLife[nCntLife].pos = pos;		//�ʒu
			g_aLife[nCntLife].nMaxLife = nMax;	//�ő僉�C�t��
			g_aLife[nCntLife].nNowLife = nNow;	//���̃��C�t��
			g_aLife[nCntLife].bUse = true;		//�g�p����

			break;
		}
	}
}

//-------------------------------------------
//���C�t�̌��Z����
//
//int nCntLife �� ���Ԗڂ̃��C�t�Q�[�W���w��
//int nReduce  �� ���C�t������l���w��
//-------------------------------------------
void SubLife(int nCntLife,int nReduce)
{
		if (g_aLife[nCntLife].bUse)
		{
			g_aLife[nCntLife].nNowLife -= nReduce;
		}
}

//-------------------------------------------
//���C�t�Q�[�W�̈ʒu�̍X�V����
//
//int nIdxLife	  �� ���Ԗڂ̃��C�t�Q�[�W���w��
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetPositionLife(int nIdxLife, D3DXVECTOR3 pos)
{
	g_aLife[nIdxLife].pos = pos;
}