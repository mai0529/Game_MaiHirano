//-------------------------------------------
//
//�A�C�e���̏���[item.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "shadow.h"
#include "player.h"
#include "map.h"
#include "sound.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Item g_aItem[MAX_ITEM];								//�A�C�e���̏��
int g_nIdxItemShadow;								//�e�̔ԍ�
int g_nCntItem;										//�J�E���^�[
int g_RandmPos;										//�ʒu�̃����_��
int g_RandmMove;									//�ړ��ʂ̃����_��

//-------------------------------------------
//����������
//-------------------------------------------
void InitItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//�e���̏�����
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu�̏�����
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
		g_aItem[nCntItem].bUse = false;									//�g�p���Ă��邩�ǂ���
	}
	g_nCntItem = 0;														//�J�E���^�[�̏�����

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_pTextureItem);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(20.0f / 2.0f),  (30.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (20.0f / 2.0f),  (30.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(20.0f / 2.0f), -(30.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (20.0f / 2.0f), -(30.0f / 2.0f), 0.0f);

		//�e���_�̖@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;				//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffItem->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureItem!= NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem= NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem!= NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem= NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateItem(void)
{
	g_RandmPos = rand() % 10;
	g_RandmMove = rand() % 5;
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawItem(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{//�g�p���Ă�����
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			D3DXMATRIX mtxView;

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aItem[nCntItem].mtxWorld._11 = mtxView._11;
			g_aItem[nCntItem].mtxWorld._12 = mtxView._21;
			g_aItem[nCntItem].mtxWorld._13 = mtxView._31;
			g_aItem[nCntItem].mtxWorld._21 = mtxView._12;
			g_aItem[nCntItem].mtxWorld._22 = mtxView._22;
			g_aItem[nCntItem].mtxWorld._23 = mtxView._32;
			g_aItem[nCntItem].mtxWorld._31 = mtxView._13;
			g_aItem[nCntItem].mtxWorld._32 = mtxView._23;
			g_aItem[nCntItem].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureItem);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//-------------------------------------------
//�A�C�e���̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetItem(D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	//float Model = GetPlayerRot();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aItem[nCntItem].pos = pos;		//�ʒu
			g_aItem[nCntItem].bUse = true;		//�g�p����
			break;
		}
		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//-------------------------------------------
//�A�C�e���̓����蔻��
//
//D3DXVECTOR3 * pPos �� �ʒu
//D3DXVECTOR3 size	 �� �傫��
//-------------------------------------------
void CollisionItem(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{//�A�C�e�����g���Ă���Ƃ�
			if ((pPos->x - (size.x / 2.0f)) <= g_aItem[nCntItem].pos.x + (14.0f / 2.0f)
				&& (pPos->x + (size.x / 2.0f)) >= g_aItem[nCntItem].pos.x - (14.0f / 2.0f)
				&& (pPos->z - (size.z / 2.0f)) <= g_aItem[nCntItem].pos.z
				&& (pPos->z + (size.z / 2.0f)) >= g_aItem[nCntItem].pos.z)
			{
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f,-3.0f + g_RandmMove), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�G�t�F�N�g��\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f,-3.0f + g_RandmMove), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z + g_RandmPos), D3DXVECTOR3(0.0f, 0.0f, 3.0f - g_RandmMove), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z - g_RandmPos), D3DXVECTOR3(0.0f, 0.0f, 3.0f - g_RandmMove), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��

				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z + g_RandmPos), D3DXVECTOR3(-3.0f - g_RandmMove, 0.0f, 0.0f),  D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z - g_RandmPos), D3DXVECTOR3(-3.0f + g_RandmMove, 0.0f, 0.0f),  D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(3.0f  - g_RandmMove, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(3.0f  + g_RandmMove, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//�p�[�e�B�N����\��

				g_nCntItem++;		//�J�E���^�[��1���₷
				g_aItem[nCntItem].bUse = false;		//�g�p���Ȃ�
				PlaySound(SOUND_LABEL_SE_ITEM);		//SE���Đ�

				UseItemMap(nCntItem);		//�A�C�e���}�b�v���g�p���Ȃ�
			}
		}
	}
}

//-------------------------------------------
//�A�C�e�����l�����������擾
//-------------------------------------------
int GetCntItem(void)
{
	return g_nCntItem;
}

//-------------------------------------------
//�A�C�e���̏����擾
//-------------------------------------------
Item * GetItem(void)
{
	return &g_aItem[0];
}