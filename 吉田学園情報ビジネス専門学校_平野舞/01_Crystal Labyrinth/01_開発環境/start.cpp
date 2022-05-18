//-------------------------------------------
//
//�~������[cylinder.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "start.h"
#include "particle.h"
#include "effect.h"

//�}�N����`
#define START_X_BLOCK			(15)		//X�����̃u���b�N��
#define START_Z_BLOCK			(15)		//X�����̃u���b�N��

#define START_VERTEX_NUM			((START_X_BLOCK + 1) * (START_Z_BLOCK + 1))									//���_��
#define START_INDEX_NUM			((((START_X_BLOCK + 1) * 2) * START_Z_BLOCK) + (2 * (START_Z_BLOCK - 1)))		//�C���f�b�N�X��
#define START_PRIMITIVE_NUM		((2 * START_X_BLOCK * START_Z_BLOCK) + (4 * (START_Z_BLOCK - 1)))				//�|���S����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureStart = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pldxBuffClinderField;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Start g_aStart;										//�X�^�[�g�̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitStart(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aStart.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);	//�ʒu
	g_aStart.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_aStart.bUse = true;								//�g�p����

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.png",
		&g_pTextureStart);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * START_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * START_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffClinderField,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < START_Z_BLOCK + 1; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < START_X_BLOCK + 1; nCntX++, nCount++)
		{//X���W

			g_aStart.rot.y = ((D3DX_PI * 2.0f) / START_X_BLOCK) * (float) nCntX;			//�p�x�ݒ�

			//�p�x���͈͂��璴�����ꍇ(�p�x�̐��K��)
			if (g_aStart.rot.y  < -D3DX_PI)
			{//�p�x��-3.14�����������Ȃ����ꍇ
				g_aStart.rot.y += D3DX_PI * 2;
			}
			else if (g_aStart.rot.y  > D3DX_PI)
			{//�p�x��3.14�����傫���Ȃ����ꍇ
				g_aStart.rot.y -= D3DX_PI * 2;
			}
			
			//���_���W�̐ݒ�
			pVtx[0].pos.x = sinf(g_aStart.rot.y) * 40.0f;			//X���W
			pVtx[0].pos.y = (100.0f / START_Z_BLOCK) * nCntZ;	//Y���W
			pVtx[0].pos.z = cosf(g_aStart.rot.y) * 40.0f;			//Z���W

			//�e���_�̖@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, g_aStart.rot.y, 0.0f);

			//���K������
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.1f, 0.4f, 0.7f, 1.0f);

			////�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / START_X_BLOCK) * nCntX , 0.0f - (1.0f / START_Z_BLOCK) * nCntZ);

			pVtx += 1;			//���_�f�[�^��1���i�߂�
		}
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffStart->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD * pldx;
	g_pldxBuffClinderField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <START_Z_BLOCK; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < START_X_BLOCK + 1; nCntX++, nData++)
		{//X���W
		 //�C���f�b�N�X�̐ݒ�
			pldx[0] = (START_X_BLOCK + 1) + nData;		//��
			pldx[1] = nData;							//��

			pldx += 2;		//�C���f�b�N�X�f�[�^��2���i�߂�
		}
		if (nCntZ != (START_Z_BLOCK - 1))
		{//�܂�Ԃ�
			pldx[0] = nData - 1;						//�E��
			pldx[1] = nData + (START_X_BLOCK + 1);		//����

			pldx += 2;			//�C���f�b�N�X�f�[�^��2���i�߂�
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pldxBuffClinderField->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitStart(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pldxBuffClinderField != NULL)
	{
		g_pldxBuffClinderField->Release();
		g_pldxBuffClinderField = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateStart(void)
{
	g_aStart.rot.y -= 0.05;	//��]

	if (g_aStart.bUse)
	{
			//�����_��
			int nRandX, nRandZ;
			nRandX = rand() % 40;		//X���̃����_��
			nRandZ = rand() % 40;		//Z���̃����_��

				//�p�[�e�B�N����ݒ� 
			SetParticle(D3DXVECTOR3(g_aStart.pos.x + nRandX, 0.1f, g_aStart.pos.z + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x - nRandX, 0.1f, g_aStart.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x + 10 + nRandX, 0.1f, g_aStart.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 8.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x - 10 - nRandX, 0.1f, g_aStart.pos.z + 10 + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 8.0f, -1.0f);
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawStart(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	if (g_aStart.bUse)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aStart.mtxWorld);

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pldxBuffClinderField);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStart.rot.y, g_aStart.rot.x, g_aStart.rot.z);
		D3DXMatrixMultiply(&g_aStart.mtxWorld, &g_aStart.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aStart.pos.x, g_aStart.pos.y, g_aStart.pos.z);
		D3DXMatrixMultiply(&g_aStart.mtxWorld, &g_aStart.mtxWorld, &mtxTrans);

		//���C�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aStart.mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//Z�o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureStart);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, START_VERTEX_NUM, 0, START_PRIMITIVE_NUM);

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

//-------------------------------------------
//�����蔻�菈��
//
//D3DXVECTOR3 * pPos �� �ʒu
//D3DXVECTOR3 size	 �� �傫��
//-------------------------------------------
void CollisionStart(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	if (g_aStart.bUse)
	{
		float nDisX = pPos->x - g_aStart.pos.x;							//X���̍��̋���
		float nDisZ = (pPos->z + (size.z / 2.0f)) - g_aStart.pos.z;		//Z���̍��̋���
		float nData = sqrt((nDisX * nDisX) + (nDisZ * nDisZ));			//���̋���

		if (nData >= 40.0f)
		{//�~����o����
			g_aStart.bUse = false;		//�g��Ȃ�
		}
	}
}

//-------------------------------------------
//�X�^�[�g���̎擾
//-------------------------------------------
Start * GetStart(void)
{
	return &g_aStart;
}