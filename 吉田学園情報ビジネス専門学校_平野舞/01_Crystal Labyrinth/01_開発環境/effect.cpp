//-------------------------------------------
//
//�G�t�F�N�g����[effect.cpp]
//Author:���앑
//
//------------------------------------------- 

//�C���N���[�h�t�@�C��
#include "effect.h"

//�}�N����`
#define MAX_EFFECT		(4096)			//�G�t�F�N�g�̍ő吔
#define MAX_TEXEFFECT	(7)				//�G�t�F�N�g�摜�̍ő吔

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_TEXEFFECT] = {};		//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];						//�G�t�F�N�g�̏��

//�O���t�@�C���p�̃O���[�o���ϐ�
char g_cReadEffect[256];				//�����ǂݍ��ݗp
char g_cTexName[30][256];				//�摜���ǂݍ��ݗp
int g_nNumEffect;						//�摜��ǂݍ��ސ���ǂݍ��ޗp

//-------------------------------------------
//����������
//-------------------------------------------
void InitEffect(void)
{
	//�O���t�@�C���ǂݍ���
	LoadEffectFile();

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�G�t�F�N�g���̏�����
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);	//�J���[�̏�����
		g_aEffect[nCntEffect].fSize = 0.0f;								//�T�C�Y�̏�����
		g_aEffect[nCntEffect].nLife = 0;								//���C�t�̏�����
		g_aEffect[nCntEffect].nType = EFFECT_CIRCLE;					//��ނ̏�����
		g_aEffect[nCntEffect].bUse = false;								//�g�p���Ă��邩�ǂ���
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			&g_cTexName[nCount][0],
			&g_pTextureEffect[nCount]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(10.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (10.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(10.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (10.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

		//�e���_�̖@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

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
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitEffect(void)
{
	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{//�e�N�X�`���̔j��
		if (g_pTextureEffect[nCount] != NULL)
		{
			g_pTextureEffect[nCount]->Release();
			g_pTextureEffect[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateEffect(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//�G�t�F�N�g���g�p����Ă���
			//����(�\������)
			g_aEffect[nCntEffect].nLife -= 1;				//���C�t�����炵�Ă���
			if (g_aEffect[nCntEffect].nLife == 0)
			{//���C�t��0�ɂȂ�����
				g_aEffect[nCntEffect].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawEffect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//�g�p���Ă�����
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			D3DXMATRIX mtxView;

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//Z�o�b�t�@��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

			//Z�o�b�t�@�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//�ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//-------------------------------------------
//�G�t�F�N�g�̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//D3DXCOLOR col	  �� �F
//int nLife		  �� ����
//float fSize	  �� �傫��
//int nType		  �� ���(effect.h��enum�^�Ŏw��)
//-------------------------------------------
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife,float fSize, int nType)
{
	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aEffect[nCntEffect].pos = pos;		//�ʒu
			g_aEffect[nCntEffect].col = col;		//�J���[
			g_aEffect[nCntEffect].nLife = nLife;	//���C�t
			g_aEffect[nCntEffect].fSize = fSize;	//�T�C�Y
			g_aEffect[nCntEffect].nType = nType;	//�T�C�Y
			g_aEffect[nCntEffect].bUse = true;		//�g�p����

			//�T�C�Y�̍X�V
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fSize / 2.0f),  (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3( (g_aEffect[nCntEffect].fSize / 2.0f),  (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fSize / 2.0f), -(g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3( (g_aEffect[nCntEffect].fSize / 2.0f), -(g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);

			//���_�J���[�̍X�V
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;		//�f�[�^����i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//�O���t�@�C���ǂݍ��ݏ���
//-------------------------------------------
void LoadEffectFile(void)
{
	FILE * pFile;		//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data/effect.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		while (1)
		{
			fscanf(pFile, "%s", &g_cReadEffect[0]);		//������̓ǂݍ���

			if (strcmp(&g_cReadEffect[0], "NUM_EFFECT") == 0)
			{//���f���̐��ǂݍ���
				fscanf(pFile, "%s", &g_cReadEffect[0]);
				fscanf(pFile, "%d", &g_nNumEffect);		//������̓ǂݍ���
			}

			if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
			{//���f���t�@�C���ǂݍ���
				for (int nCount = 0; nCount < g_nNumEffect; nCount++)
				{
					while (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") != 0)
					{//���f���t�@�C���l�[���ł͂Ȃ��ꍇ
						fscanf(pFile, "%s", &g_cReadEffect[0]);
					}

					if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
					{//���f���t�@�C���l�[����������
						fscanf(pFile, "%s", &g_cReadEffect[0]);
						fscanf(pFile, "%s", &g_cTexName[nCount][0]);		//������̓ǂݍ���
					}
				}
			}
			if (strcmp(&g_cReadEffect[0], "END_SCRIPT") == 0)
			{//���̕����������甲����
				break;
			}
		}
		fclose(pFile);									//�t�@�C�������
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł���");
	}
}
