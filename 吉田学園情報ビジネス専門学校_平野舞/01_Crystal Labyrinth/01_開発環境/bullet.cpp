//-------------------------------------------
//
//�e�̏���[bullet.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "stage.h"
#include "sound.h"

//�}�N����`
#define BULLET_WIDTH	(30.0f)		//��
#define BULLET_HEIGTH	(30.0f)		//����
#define BULLET_DIS		(5.0f)		//�e�̑��x
#define BULLET_MAXLIFE	(15)		//���C�t��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];						//�r���{�[�h�̏��
int g_nIdxBulletShadow;								//�e�̔ԍ�

//-------------------------------------------
//����������
//-------------------------------------------
void InitBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�e���̏�����
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu�̏�����
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ̏�����
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
		g_aBullet[nCntBullet].nLife = 0;									//���C�t�̏�����
		g_aBullet[nCntBullet].bUse = false;									//�g�p���Ă��邩�ǂ���
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet001.png",
		&g_pTextureBullet);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2.0f),  (BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (BULLET_WIDTH / 2.0f),  (BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2.0f), -(BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (BULLET_WIDTH / 2.0f), -(BULLET_HEIGTH / 2.0f), 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet!= NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet= NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet!= NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet= NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateBullet(void)
{
	Enemy * pEnemy = GetEnemy();		//�G���̎擾
	Stage * pStage = GetStage();		//�Ǐ��̎擾

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//�g�p���Ă�����
			//�ʒu���̍X�V
			g_aBullet[nCntBullet].pos.x -= sinf(g_aBullet[nCntBullet].rot.x) * BULLET_DIS;
			g_aBullet[nCntBullet].pos.z -= cosf(g_aBullet[nCntBullet].rot.z) * BULLET_DIS;

			//�G�t�F�N�g��ǉ�
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.7f,0.5f,0.0f,1.0f),20,25.0f, EFFECT_FIREWORK);

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse)
				{//�G���g�p����Ă���
					if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - (pEnemy->size.x)
						&& g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + (pEnemy->size.x)
						&& g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + (pEnemy->size.y)
						&& g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - (pEnemy->size.y)
						&& g_aBullet[nCntBullet].pos.z <= pEnemy->pos.z + (pEnemy->size.z)
						&& g_aBullet[nCntBullet].pos.z >= pEnemy->pos.z - (pEnemy->size.z))
					{//�G�̓����蔻��
						HitEnemy(nCntEnemy, 1);		//�q�b�g����
						g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
					}
				}
			}

			g_aBullet[nCntBullet].nLife -= 1;			//���C�t�����炵�Ă���
			if (g_aBullet[nCntBullet].nLife == 0)
			{//���C�t��0�ɂȂ�����
				g_aBullet[nCntBullet].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//�g�p���Ă�����
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			D3DXMATRIX mtxView;

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//-------------------------------------------
//�e�̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu�̐ݒ�
//-------------------------------------------
void SetBullet(D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	float Model = GetPlayerRot();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//�g�p���Ă��Ȃ�������
			g_aBullet[nCntBullet].pos = pos;		//�ʒu

			g_aBullet[nCntBullet].rot.x = Model;				//�e��x���̌���
			g_aBullet[nCntBullet].rot.z = Model;				//�e��z���̌���
			g_aBullet[nCntBullet].nLife = BULLET_MAXLIFE;		//���C�t�̐ݒ�

			PlaySound(SOUND_LABEL_SE_SHOT);		//SE�̍Đ�

			g_aBullet[nCntBullet].bUse = true;		//�g�p����
			break;
		}
		pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}