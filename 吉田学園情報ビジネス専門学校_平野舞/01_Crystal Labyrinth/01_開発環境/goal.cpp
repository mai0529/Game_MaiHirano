//-------------------------------------------
//
//�S�[������[goal.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "goal.h"
#include "item.h"
#include "particle.h"
#include "effect.h"
#include "stage.h"
#include "fade.h"
#include "map.h"

//�}�N����`
#define GOAL_X_BLOCK			(15)		//X�����̃u���b�N��
#define GOAL_Z_BLOCK			(15)		//X�����̃u���b�N��

#define GOAL_VERTEX_NUM			((GOAL_X_BLOCK + 1) * (GOAL_Z_BLOCK + 1))									//���_��
#define GOAL_INDEX_NUM			((((GOAL_X_BLOCK + 1) * 2) * GOAL_Z_BLOCK) + (2 * (GOAL_Z_BLOCK - 1)))		//�C���f�b�N�X��
#define GOAL_PRIMITIVE_NUM		((2 * GOAL_X_BLOCK * GOAL_Z_BLOCK) + (4 * (GOAL_Z_BLOCK - 1)))				//�|���S����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pldxBuffGoalField;				//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Goal g_aGoal;											//�S�[���̏��
bool g_bAttackGoal;										//�S�[���ɓ����������ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitGoal(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//������
	g_aGoal.pos = D3DXVECTOR3(1570.0f, 0.0f, 1320.0f);	//�ʒu
	g_aGoal.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_aGoal.bUse = false;								//�g�p���Ȃ�
	g_bAttackGoal = false;								//�S�[���ɓ������Ă��Ȃ�

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.png",
		&g_pTextureGoal);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GOAL_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * GOAL_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffGoalField,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < GOAL_Z_BLOCK + 1; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < GOAL_X_BLOCK + 1; nCntX++, nCount++)
		{//X���W

			g_aGoal.rot.y = ((D3DX_PI * 2.0f) / GOAL_X_BLOCK) * (float) nCntX;			//�p�x�ݒ�

			//�p�x���͈͂��璴�����ꍇ(�p�x�̐��K��)
			if (g_aGoal.rot.y  < -D3DX_PI)
			{//�p�x��-3.14�����������Ȃ����ꍇ
				g_aGoal.rot.y += D3DX_PI * 2;
			}
			else if (g_aGoal.rot.y  > D3DX_PI)
			{//�p�x��3.14�����傫���Ȃ����ꍇ
				g_aGoal.rot.y -= D3DX_PI * 2;
			}
			
			//���_���W�̐ݒ�
			pVtx[0].pos.x = sinf(g_aGoal.rot.y) * 40.0f;		//X���W
			pVtx[0].pos.y = (100.0f / GOAL_Z_BLOCK) * nCntZ;	//Y���W
			pVtx[0].pos.z = cosf(g_aGoal.rot.y) * 40.0f;		//Z���W

			//�e���_�̖@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, g_aGoal.rot.y, 0.0f);

			//���K������
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.9f, 0.2f, 0.1f, 1.0f);

			////�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / GOAL_X_BLOCK) * nCntX , 0.0f - (1.0f / GOAL_Z_BLOCK) * nCntZ);

			pVtx += 1;			//���_�f�[�^��1���i�߂�
		}
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffGoal->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD * pldx;
	g_pldxBuffGoalField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <GOAL_Z_BLOCK; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < GOAL_X_BLOCK + 1; nCntX++, nData++)
		{//X���W
		 //�C���f�b�N�X�̐ݒ�
			pldx[0] = (GOAL_X_BLOCK + 1) + nData;	//��
			pldx[1] = nData;						//��

			pldx += 2;		//�C���f�b�N�X�f�[�^��2���i�߂�
		}
		if (nCntZ != (GOAL_Z_BLOCK - 1))
		{//�܂�Ԃ�
			pldx[0] = nData - 1;					//�E��
			pldx[1] = nData + (GOAL_X_BLOCK + 1);	//����

			pldx += 2;			//�C���f�b�N�X�f�[�^��2���i�߂�
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pldxBuffGoalField->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitGoal(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureGoal != NULL)
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGoal != NULL)
	{
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pldxBuffGoalField != NULL)
	{
		g_pldxBuffGoalField->Release();
		g_pldxBuffGoalField = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateGoal(void)
{
	//�l�������A�C�e�����̎擾
	int nGoalItem = GetCntItem();

	if (4 == nGoalItem)
	{//�A�C�e����S���������
		SetGoal(D3DXVECTOR3(1130.0f, 0.0f, 880.0f));		//�S�[���̐ݒ�
		SetMap(D3DXVECTOR3(1130.0f, 0.0f, 880.0f), 4);

		g_aGoal.rot.y -= 0.05f;	//��]

		//�����_��
		int nRandX, nRandZ;
		nRandX = rand() % 40;		//X���̃����_��
		nRandZ = rand() % 40;		//Z���̃����_��

		//�p�[�e�B�N����ݒ�
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x + nRandX, 0.1f, g_aGoal.pos.z + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x - nRandX, 0.1f, g_aGoal.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x + 10 + nRandX, 0.1f, g_aGoal.pos.z - 10 - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x - 10 - nRandX, 0.1f, g_aGoal.pos.z + 10 + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 4.0f, -1.0f);
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawGoal(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	if (g_aGoal.bUse)
	{//�g�p���Ă�����
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aGoal.mtxWorld);

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pldxBuffGoalField);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoal.rot.y, g_aGoal.rot.x, g_aGoal.rot.z);
		D3DXMatrixMultiply(&g_aGoal.mtxWorld, &g_aGoal.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aGoal.pos.x, g_aGoal.pos.y, g_aGoal.pos.z);
		D3DXMatrixMultiply(&g_aGoal.mtxWorld, &g_aGoal.mtxWorld, &mtxTrans);

		//���C�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//���Z�����̐ݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aGoal.mtxWorld);

		//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffGoal, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//Z�o�b�t�@��L���ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureGoal);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, GOAL_VERTEX_NUM, 0, GOAL_PRIMITIVE_NUM);

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
//�ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu�̐ݒ�
//-------------------------------------------
void SetGoal(D3DXVECTOR3 pos)
{
	if (!g_aGoal.bUse)
	{//�g�p���Ă��Ȃ�������
		g_aGoal.pos = pos;		//�ʒu
		g_aGoal.bUse = true;	//�g�p����
	}
}

//-------------------------------------------
//�����蔻�菈��
//
//D3DXVECTOR3 * pPos ��	 �ʒu
//D3DXVECTOR3 size	 ��	 �傫��
//-------------------------------------------
void CollisionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	if (g_aGoal.bUse)
	{//�g�p���Ă�����
			float nDisX = pPos->x - g_aGoal.pos.x;							//X���̍��̋���
			float nDisZ = (pPos->z + (size.z / 2.0f)) - g_aGoal.pos.z;		//Z���̍��̋���
			float nData = sqrt((nDisX * nDisX) + (nDisZ * nDisZ));			//���̋���

			if (nData < 30.0f && !g_bAttackGoal)
			{//�~�ɓ�������A�~�ɓ������ĂȂ�������
					SetFade(MODE_GAMECLEAR);		//�N���A�\��
					g_bAttackGoal = true;			//��������
			}
	}
}