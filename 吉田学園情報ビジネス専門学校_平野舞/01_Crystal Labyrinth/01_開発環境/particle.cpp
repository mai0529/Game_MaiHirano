//-------------------------------------------
//
//�p�[�e�B�N������[particle.cpp]
//Author:���앑
//
//------------------------------------------- 

//�C���N���[�h�t�@�C��
#include "particle.h"
#include "effect.h"
#include <stdlib.h>

//�}�N����`
#define MAX_PARTICLE		(256)				//�p�[�e�B�N���̍ő吔

//�O���[�o���ϐ��錾
//LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Particle g_aParticle[MAX_PARTICLE];						//�p�[�e�B�N���̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//�G�t�F�N�g���̏�����
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏�����
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�J���[�̏�����
		g_aParticle[nCntParticle].fSize = 0.0f;								//�T�C�Y�̏�����
		g_aParticle[nCntParticle].fGravity = 0.0f;							//�d�͂̏�����
		g_aParticle[nCntParticle].nLife = 0;								//���C�t�̏�����
		g_aParticle[nCntParticle].nType = EFFECT_CIRCLE;					//��ނ̏�����
		g_aParticle[nCntParticle].bUse = false;								//�g�p���Ă��邩�ǂ���
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(1.0f / 2.0f),  (1.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (1.0f / 2.0f),  (1.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(1.0f / 2.0f), -(1.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (1.0f / 2.0f), -(1.0f / 2.0f), 0.0f);

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

		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffParticle->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitParticle(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].move.y -= g_aParticle[nCntParticle].fGravity;

			//�ʒu�����X�V
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//�G�t�F�N�g��ݒ�
			SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].nLife, g_aParticle[nCntParticle].fSize, g_aParticle[nCntParticle].nType);

			//a�l�����炵�Ă���
			g_aParticle[nCntParticle].col.a -= 0.1;

			//����(�\������)
			g_aParticle[nCntParticle].nLife -= 1;			//���C�t�����炵�Ă���
			if (g_aParticle[nCntParticle].nLife == 0)
			{//���C�t��0�ɂȂ�����
				g_aParticle[nCntParticle].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
				g_aParticle[nCntParticle].move.y = 0;
			}
		}
	}
}

//-------------------------------------------
//�`�揈������
//-------------------------------------------
void DrawParticle(void)
{
	//�Ȃ�
}

//-------------------------------------------
//�ݒ菈��
//
//D3DXVECTOR3 pos  �� �ʒu
//D3DXVECTOR3 move �� �ړ���
//D3DXCOLOR col	   �� �F
//int nLife		   �� ����
//int nType		   �� ���(effect.h��enum�^�ŋL������)
//float fSize	   �� �傫��
//float fGravity   �� �d��
//-------------------------------------------
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, int nType, float fSize, float fGravity)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aParticle[nCntParticle].pos = pos;				//�ʒu
			g_aParticle[nCntParticle].move = move;				//�ʒu
			g_aParticle[nCntParticle].col = col;				//�J���[
			g_aParticle[nCntParticle].nLife = nLife;			//���C�t
			g_aParticle[nCntParticle].nType = nType;			//���
			g_aParticle[nCntParticle].fSize = fSize;			//�T�C�Y
			g_aParticle[nCntParticle].fGravity = fGravity;		//�d��
			g_aParticle[nCntParticle].bUse = true;				//�g�p����

			break;
		}
	}
}