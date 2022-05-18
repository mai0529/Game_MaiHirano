//-------------------------------------------
//
//�}�b�v����[map.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "map.h"
#include "maze.h"
#include "enemy.h"
#include "item.h"

//�}�N����`
#define MAX_MAP		(MAX_STAGEX * MAX_STAGEX)				//�ǂ̍ő吔
#define NUM_MAP		(5)										//�ő�e�N�X�`����
#define MAX_BUFFMAP	(4)										//�ő�o�b�t�@��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureMap[NUM_MAP] = {};			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMap[MAX_BUFFMAP] = {};	//���_�o�b�t�@�ւ̃|�C���^
Map g_aMap[MAX_MAP];										//�}�b�v���
PlayerMap g_playerMap;										//�v���C���[���
EnemyMap g_aEnemyMap[MAX_ENEMY];							//�G���
ItemMap g_aItemMap[MAX_ITEM];								//�A�C�e�����
int g_nCntZeroMap;											//0�̐��𐔂��鏉����

//�O���t�@�C����ǂݍ��ޗp�̃O���[�o���ϐ��錾
int g_nMapDate[MAX_STAGEZ][MAX_STAGEX];				//�ʒu����ǂݍ��ޗp
char g_cMap[256];									//���̑��p

//-------------------------------------------
//����������
//-------------------------------------------
void InitMap(void)
{
	//�O���t�@�C���ǂݍ���
	LoadMap();

	//�}�b�v���̏�����
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		g_aMap[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̏�����
		g_aMap[nCntMap].nType = 0;								//�ʒu�̏�����
		g_aMap[nCntMap].bUse = false;							//�g�p���Ȃ�
	}

	//�v���C���[�}�b�v���̏�����
	g_playerMap.pos = D3DXVECTOR3(-980.0f, 0.0f, 20.0f);	//�ʒu�̏�����
	g_playerMap.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̏�����

	//�G�}�b�v���̏�����
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		g_aEnemyMap[nCntEnemyMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̏�����
		g_aEnemyMap[nCntEnemyMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����̏�����
		g_aEnemyMap[nCntEnemyMap].bUse = false;							//�g�p���Ȃ�
	}

	//�A�C�e���}�b�v���̏�����
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		g_aItemMap[nCntItemMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̏�����
		g_aItemMap[nCntItemMap].bUse = false;							//�g�p���Ȃ�
	}

	g_nCntZeroMap = 0;		//�J�E���g�̏�����

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���(��)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wall000.jpg",
		&g_apTextureMap[0]);

	//�e�N�X�`���̓ǂݍ���(�A�C�e��)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_apTextureMap[1]);

	//�e�N�X�`���̓ǂݍ���(�v���C���[)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_apTextureMap[2]);

	//�e�N�X�`���̓ǂݍ���(�G)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureMap[3]);

	//�e�N�X�`���̓ǂݍ���(�S�[��)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/goal000.png",
		&g_apTextureMap[4]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[0],
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMap[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[0]->Unlock();

	//------------------------Player�p-------------------------------------------
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[1],
		NULL);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMap[1]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
	pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
	pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
	pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  -(75.0f / 2.0f));

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMap[1]->Unlock();

	//------------------------Enemy�p-------------------------------------------
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[2],
		NULL);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMap[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_ENEMY; nCntMap++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[2]->Unlock();

	//------------------------Enemy�p-------------------------------------------
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[3],
		NULL);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMap[3]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_ITEM; nCntMap++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[3]->Unlock();

	//�}�b�v�̐ݒu
	for (int nCountZ = 0; nCountZ < MAX_STAGEZ; nCountZ++)
	{//Z����
		for (int nCountX = 0; nCountX < MAX_STAGEX; nCountX++)
		{//X����
			switch (g_nMapDate[nCountZ][nCountX])
			{
			case 0:		//0�������ꍇ
				g_nCntZeroMap++;
				if (7 == g_nCntZeroMap || 23 == g_nCntZeroMap || 37 == g_nCntZeroMap || 53 == g_nCntZeroMap)
				{//�G��z�u

					SetEnemyMap(D3DXVECTOR3(300.0f + (75.0f * nCountX), 10.0f, 50.0f + (75.0f * nCountZ)));
				}
				break;
			case 1:		//1�������ꍇ�u���b�N��u��
				SetMap(D3DXVECTOR3(300.0f + (75.0f * nCountX), 0.0f, 50.0f + (75.0f * nCountZ)),0);
				break;
			default:
				break;
			}
		}
	}

	//�v���C���[�}�b�v�̐ݒu
	g_playerMap.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitMap(void)
{
	for (int nCntMap = 0; nCntMap < NUM_MAP; nCntMap++)
	{//�e�N�X�`���̔j��
		if (g_apTextureMap[nCntMap] != NULL)
		{
			g_apTextureMap[nCntMap]->Release();
			g_apTextureMap[nCntMap] = NULL;
		}
	}
	
	for (int nCntMap = 0; nCntMap < MAX_BUFFMAP; nCntMap++)
	{//���_�o�b�t�@�̔j��
		if (g_pVtxBuffMap[nCntMap] != NULL)
		{
			g_pVtxBuffMap[nCntMap]->Release();
			g_pVtxBuffMap[nCntMap] = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateMap(void)
{
	//�G�����擾
	Enemy * pEnemy = GetEnemy();

	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++, pEnemy++)
	{
		if (pEnemy->nLife == 0)
		{//�����G�̃��C�t���Ȃ��Ȃ�����
			g_aEnemyMap[nCntEnemyMap].bUse = false;		//�g�p���Ȃ�
		}
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawMap(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		if (g_aMap[nCntMap].bUse)
		{//�g�p���Ă�����
		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMap[nCntMap].mtxWorld);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMap[nCntMap].pos.x, g_aMap[nCntMap].pos.y, g_aMap[nCntMap].pos.z);
			D3DXMatrixMultiply(&g_aMap[nCntMap].mtxWorld, &g_aMap[nCntMap].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMap[nCntMap].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMap[0], 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTextureMap[g_aMap[nCntMap].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMap * 4, 2);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	//----------------------------------Player�p--------------------------------------------------------------
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_playerMap.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_playerMap.rot.y, g_playerMap.rot.x, g_playerMap.rot.z);
	D3DXMatrixMultiply(&g_playerMap.mtxWorld, &g_playerMap.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_playerMap.pos.x, g_playerMap.pos.y, g_playerMap.pos.z);
	D3DXMatrixMultiply(&g_playerMap.mtxWorld, &g_playerMap.mtxWorld, &mtxTrans);

	//���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_playerMap.mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMap[1], 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_apTextureMap[2]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//----------------------------------Enemy�p--------------------------------------------------------------
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		if (g_aEnemyMap[nCntEnemyMap].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemyMap[nCntEnemyMap].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemyMap[nCntEnemyMap].rot.y, g_aEnemyMap[nCntEnemyMap].rot.x, g_aEnemyMap[nCntEnemyMap].rot.z);
			D3DXMatrixMultiply(&g_aEnemyMap[nCntEnemyMap].mtxWorld, &g_aEnemyMap[nCntEnemyMap].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyMap[nCntEnemyMap].pos.x, g_aEnemyMap[nCntEnemyMap].pos.y, g_aEnemyMap[nCntEnemyMap].pos.z);
			D3DXMatrixMultiply(&g_aEnemyMap[nCntEnemyMap].mtxWorld, &g_aEnemyMap[nCntEnemyMap].mtxWorld, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyMap[nCntEnemyMap].mtxWorld);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMap[2], 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTextureMap[3]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyMap * 4, 2);

			//���C�g��L���ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//----------------------------------Item�p--------------------------------------------------------------
			for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
			{
				if (g_aItemMap[nCntItemMap].bUse)
				{
					//���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&g_aItemMap[nCntItemMap].mtxWorld);

					//�ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans, g_aItemMap[nCntItemMap].pos.x, g_aItemMap[nCntItemMap].pos.y, g_aItemMap[nCntItemMap].pos.z);
					D3DXMatrixMultiply(&g_aItemMap[nCntItemMap].mtxWorld, &g_aItemMap[nCntItemMap].mtxWorld, &mtxTrans);

					//���C�g�𖳌��ɂ���
					pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

					//���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform(D3DTS_WORLD, &g_aItemMap[nCntItemMap].mtxWorld);

					//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
					pDevice->SetStreamSource(0, g_pVtxBuffMap[3], 0, sizeof(VERTEX_3D));

					//���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);

					//�e�N�X�`���ݒ�
					pDevice->SetTexture(0, g_apTextureMap[1]);

					//�|���S���̕`��
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItemMap * 4, 2);

					//���C�g��L���ɖ߂�
					pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				}
			}
		}
	}
}

//-------------------------------------------
//�}�b�v�̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//int nType		  �� ���
//-------------------------------------------
void SetMap(D3DXVECTOR3 pos, int nType)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		if (!g_aMap[nCntMap].bUse)
		{
			g_aMap[nCntMap].pos = pos;			//�ʒu
			g_aMap[nCntMap].nType = nType;		//���
			g_aMap[nCntMap].bUse = true;		//�g�p����

			break;
		}
	}
}

//-------------------------------------------
//�G�̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetEnemyMap(D3DXVECTOR3 pos)
{
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		if (!g_aEnemyMap[nCntEnemyMap].bUse)
		{
			g_aEnemyMap[nCntEnemyMap].pos = pos;		//�ʒu
			g_aEnemyMap[nCntEnemyMap].bUse = true;		//�g�p����

			break;
		}
	}
}

//-------------------------------------------
//�A�C�e���̐ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetItemMap(D3DXVECTOR3 pos)
{
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		if (!g_aItemMap[nCntItemMap].bUse)
		{
			g_aItemMap[nCntItemMap].pos = pos;		//�ʒu
			g_aItemMap[nCntItemMap].bUse = true;	//�g�p����

			break;
		}
	}
}

//-------------------------------------------
//�v���C���[�}�b�v�̈ʒu�X�V����
//
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetPositionPlayerMap(D3DXVECTOR3 pos)
{
	g_playerMap.pos = pos;		//�ʒu�̍X�V
}

//-------------------------------------------
//�G�}�b�v�̈ʒu�X�V����
//
//int nIdxEnemy	  �� ���Ԗڂ̓G���w��
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetPositionEnemyMap(int nIdxEnemy, D3DXVECTOR3 pos)
{
	g_aEnemyMap[nIdxEnemy].pos = pos;		//�ʒu�̍X�V
}

//-------------------------------------------
//�A�C�e���}�b�v�̊l������
//
//int nCntItemMap �� ���Ԗڂ̃A�C�e�����w��
//-------------------------------------------	
void UseItemMap(int nCntItemMap)
{
	if (g_aItemMap[nCntItemMap].bUse)
	{//�A�C�e�����g���Ă���Ƃ�
		g_aItemMap[nCntItemMap].bUse = false;		//�g�p���Ȃ�
	}
}

//-------------------------------------------
//�A�C�e���̓����蔻�菈��
//-------------------------------------------
void CollisionItemMap(void)
{
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		if (g_aItemMap[nCntItemMap].bUse)
		{//�A�C�e�����g���Ă���Ƃ�
			if (g_playerMap.pos.x - (60.0f / 2.0f) <= g_aItemMap[nCntItemMap].pos.x + (60.0f / 2.0f)
				&& g_playerMap.pos.x + (60.0f / 2.0f) >= g_aItemMap[nCntItemMap].pos.x - (60.0f / 2.0f)
				&& g_playerMap.pos.z - (60.0f / 2.0f) <= g_aItemMap[nCntItemMap].pos.z + (60.0f / 2.0f)
				&& g_playerMap.pos.z + (60.0f / 2.0f) >= g_aItemMap[nCntItemMap].pos.z - (60.0f / 2.0f))
			{
				g_aItemMap[nCntItemMap].bUse = false;		//�g�p���Ȃ�
			}
		}
	}
}

//-------------------------------------------
//�O���t�@�C����ǂݍ��ޏ���
//-------------------------------------------
void LoadMap(void)
{
	FILE * pFile;		//�t�@�C���|�C���^

	//�t�@�C�����J��
	pFile = fopen("data/maze.txt", "r");
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
		{//Z����
			for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
			{//X����
				fscanf(pFile, "%1d", &g_nMapDate[nCntZ][nCntX]);		//�����̓ǂݍ���
			}
			fscanf(pFile, "%*[^\n]", &g_cMap[0]);						//���s���΂�
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}
}