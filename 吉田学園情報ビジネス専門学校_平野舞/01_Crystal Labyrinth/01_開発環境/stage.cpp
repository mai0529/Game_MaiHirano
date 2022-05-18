//-------------------------------------------
//
//�X�e�[�W����[stage.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "stage.h"
#include "item.h"
#include "enemy.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureStage = NULL;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Stage g_aStage[MAX_STAGE];							//�ǂ̏��
int g_nCntMapItem;									//�A�C�e���ݒu��
int g_nCntZero;										//0�̐����擾

//�O���t�@�C���p
int g_nStageDate[MAX_STAGEZ][MAX_STAGEX];			//���H�}�b�v��ǂݍ��ޗp
char g_cStage[256];									//���̑�

//-------------------------------------------
//����������
//-------------------------------------------
void InitStage(void)
{
	//�O���t�@�C���ǂݍ���
	LoadStage();

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		//���f�����̏����ݒ�
		g_aStage[nCount].pos = D3DXVECTOR3(70.0f + (70.0f * nCount), 0.0f, 30.0f);		//�ʒu
		g_aStage[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//����
		g_aStage[nCount].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//�T�C�Y
		g_aStage[nCount].vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);					//�ŏ����_�l
		g_aStage[nCount].vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);				//�ő咸�_�l
		g_aStage[nCount].bUse = false;													//�g�p���Ȃ�
	}
	g_nCntMapItem = 0;		//�A�C�e���J�E���g��������
	g_nCntZero = 0;			//0�̐���������

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/wall.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aStage[nCount].pBuffMat,
			NULL,
			&g_aStage[nCount].nNumMat,
			&g_aStage[nCount].pMesh);
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//���_���̎擾
		nNumVtx = g_aStage[nCntStage].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aStage[nCntStage].pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_aStage[nCntStage].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x�l
			if (vtx.x < g_aStage[nCntStage].vtxMin.x)
			{//���_�l��������������
				g_aStage[nCntStage].vtxMin.x = vtx.x;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.x > g_aStage[nCntStage].vtxMax.x)
			{//���_�l���傫��������
				g_aStage[nCntStage].vtxMax.x = vtx.x;		//�ő咸�_�l�ɑ��
			}

			//y�l
			if (vtx.y < g_aStage[nCntStage].vtxMin.y)
			{//���_�l��������������
				g_aStage[nCntStage].vtxMin.y = vtx.y;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.y > g_aStage[nCntStage].vtxMax.y)
			{//���_�l���傫��������
				g_aStage[nCntStage].vtxMax.y = vtx.y;		//�ő咸�_�l�ɑ��
			}

			//z�l
			if (vtx.z < g_aStage[nCntStage].vtxMin.z)
			{//���_�l��������������
				g_aStage[nCntStage].vtxMin.z = vtx.z;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.z > g_aStage[nCntStage].vtxMax.z)
			{//���_�l���傫��������
				g_aStage[nCntStage].vtxMax.z = vtx.z;		//�ő咸�_�l�ɑ��
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		g_aStage[nCntStage].pMesh->UnlockVertexBuffer();
	}

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		g_aStage[nCntStage].size.x = g_aStage[nCntStage].vtxMax.x - g_aStage[nCntStage].vtxMin.x;		//X�̃T�C�Y�̎擾
		g_aStage[nCntStage].size.y = g_aStage[nCntStage].vtxMax.y - g_aStage[nCntStage].vtxMin.y;		//Y�̃T�C�Y�̎擾
		g_aStage[nCntStage].size.z = g_aStage[nCntStage].vtxMax.z - g_aStage[nCntStage].vtxMin.z;		//Z�̃T�C�Y�̎擾
	}

	//���f���u���b�N�ƃA�C�e���̐ݒu
	for (int nCountZ = 0; nCountZ < MAX_STAGEZ; nCountZ++)
	{//Z����
		for (int nCountX = 0; nCountX < MAX_STAGEX; nCountX++)
		{//X����
			switch (g_nStageDate[nCountZ][nCountX])
			{
			case 0:		//0�������ꍇ
				g_nCntZero++;		//�����J�E���g����
				if (7 == g_nCntZero || 23 == g_nCntZero || 37 == g_nCntZero || 53 == g_nCntZero)
				{//�G��ݒu
					SetEnemy(D3DXVECTOR3(300.0f + (75.0f * nCountX), 10.0f, 50.0f + (75.0f * nCountZ)));
				}
				break;
			case 1:		//1�������ꍇ���f���u���b�N��u��
				SetStage(D3DXVECTOR3(300.0f + (75.0f * nCountX), 0.0f, 50.0f + (75.0f * nCountZ)));
				break;
			default:
				break;
			}
		}
	}
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitStage(void)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//���b�V���̔j��
		if (g_aStage[nCntStage].pMesh != NULL)
		{
			g_aStage[nCntStage].pMesh->Release();
			g_aStage[nCntStage].pMesh = NULL;
		}
	}

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//�}�e���A���̔j��
		if (g_aStage[nCntStage].pBuffMat != NULL)
		{
			g_aStage[nCntStage].pBuffMat->Release();
			g_aStage[nCntStage].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateStage(void)
{
	//���͂Ȃ�
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawStage(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL * pMat;

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (g_aStage[nCntStage].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aStage[nCntStage].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStage[nCntStage].rot.y, g_aStage[nCntStage].rot.x, g_aStage[nCntStage].rot.z);
			D3DXMatrixMultiply(&g_aStage[nCntStage].mtxWorld, &g_aStage[nCntStage].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aStage[nCntStage].pos.x, g_aStage[nCntStage].pos.y, g_aStage[nCntStage].pos.z);
			D3DXMatrixMultiply(&g_aStage[nCntStage].mtxWorld, &g_aStage[nCntStage].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aStage[nCntStage].mtxWorld);

			//���݂̃}�e���A����ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_aStage[nCntStage].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aStage[nCntStage].nNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_aStage[nCntStage].pMesh->DrawSubset(nCntMat);
			}

			//�ێ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//�ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//-------------------------------------------
void SetStage(D3DXVECTOR3 pos)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (!g_aStage[nCntStage].bUse)
		{
			g_aStage[nCntStage].pos = pos;			//�ʒu
			g_aStage[nCntStage].bUse = true;		//�g�p����

			break;
		}
	}
}

//-------------------------------------------
//�ǂ̓����蔻�菈��
//-------------------------------------------
void CollisionStage(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (g_aStage[nCntStage].bUse)
		{
			if ((pPos->y - (size.y / 2.0f)) <= ((g_aStage[nCntStage].pos.y + g_aStage[nCntStage].vtxMax.y) / 2.0) &&	//��
				(pPos->y + (size.y / 2.0f)) >= ((g_aStage[nCntStage].pos.y + g_aStage[nCntStage].vtxMin.y) / 2.0))		//��
			{//y���̓����蔻��
			 //���E�̓����蔻��
				if ((pPos->z - (size.z / 2.0f)) < (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z) &&		//��O
					(pPos->z + (size.z / 2.0f)) > (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z))		//��
				{//z�͈͂̓����蔻��
					if ((pPosOld->x + (size.x / 2.0f)) <= (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) &&			//�O��̈ʒu
						(pPos->x + (size.x / 2.0f)) > (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x))				//���݂̈ʒu
					{//�����̓����蔻��
						pPos->x = (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) - (size.x / 2.0f);		//�����߂�
					}
					else if ((pPosOld->x - (size.x / 2.0f)) >= (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x) &&	//�O��̈ʒu
						(pPos->x - (size.x / 2.0f)) < (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x))				//���݂̈ʒu
					{//�E���̓����蔻��
						pPos->x = (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x) + (size.x / 2.0f);		//�����߂�
					}
				}

				//��O���̓����蔻��
				if ((pPos->x + (size.x / 2.0f)) > (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) &&	//��
					(pPos->x - (size.x / 2.0f)) < (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x))		//�E
				{//x���̓����蔻��
					if ((pPosOld->z + (size.z / 2.0f)) <= (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z) &&			//�O��̈ʒu
						(pPos->z + (size.z / 2.0f)) > (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z))				//���݂̈ʒu
					{//��O�̓����蔻��
						pPos->z = (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z) - (size.z / 2.0f);		//�����߂�
					}
					else if ((pPosOld->z - (size.z / 2.0f)) >= g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z &&		//�O��̈ʒu
						(pPos->z - (size.z / 2.0f)) < (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z))				//���݂̈ʒu
					{//���̓����蔻��
						pPos->z = (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z) + (size.z / 2.0f);		//�����߂�
					}
				}
			}
		}
	}
}

//-------------------------------------------
//�O���t�@�C���̓ǂݍ��ݏ���
//-------------------------------------------
void LoadStage(void)
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
				fscanf(pFile, "%1d", &g_nStageDate[nCntZ][nCntX]);		//�����̓ǂݍ���
			}
			fscanf(pFile, "%*[^\n]", &g_cStage[0]);						//���s���΂�
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}
}

//-------------------------------------------
//�X�e�[�W�����擾���鏈��
//-------------------------------------------
Stage * GetStage(void)
{
	return &g_aStage[0];
}