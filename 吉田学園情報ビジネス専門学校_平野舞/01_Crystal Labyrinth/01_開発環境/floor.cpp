//-------------------------------------------
//
//���\������[floor.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "floor.h"

//�}�N����`
#define FLOOR_X_BLOCK			(8)		//X�����̃u���b�N��
#define FLOOR_Z_BLOCK			(8)		//X�����̃u���b�N��

#define FLOOR_VERTEX_NUM		((FLOOR_X_BLOCK + 1) * (FLOOR_Z_BLOCK + 1))										//���_��
#define FLOOR_INDEX_NUM			((((FLOOR_X_BLOCK + 1) * 2) * FLOOR_Z_BLOCK) + (2 * (FLOOR_Z_BLOCK - 1)))		//�C���f�b�N�X��
#define FLOOR_PRIMITIVE_NUM		((2 * FLOOR_X_BLOCK * FLOOR_Z_BLOCK) + (4 * (FLOOR_Z_BLOCK - 1)))				//�|���S����

#define MAX_FLOOR		(128)			//���b�V���t�B�[���h�̍ő吔

//�O���[�o���ϐ��錾
//LPDIRECT3DTEXTURE9 g_pTextureFloor = NULL;		//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pldxBuffMeshField;			//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posFloor;								//�ʒu
D3DXVECTOR3 g_rotFloor;								//����
D3DXMATRIX g_mtxWorldFloor;							//���[���h�}�g���b�N�X
Floor g_aFloor[MAX_FLOOR];							//���b�V���t�B�[���h�̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitFloor(void)
{
	//�����ݒ�
	for (int nCntFloor = 0; nCntFloor < MAX_FLOOR; nCntFloor++)
	{
		g_aFloor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aFloor[nCntFloor].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);		//����	
		g_aFloor[nCntFloor].fWidth = 0.0f;							//��
		g_aFloor[nCntFloor].fHeigth = 0.0f;							//����
		g_aFloor[nCntFloor].bUse = false;							//�g�p����
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * FLOOR_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFloor,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * FLOOR_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffMeshField,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D * pVtx = NULL;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < FLOOR_Z_BLOCK + 1; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < FLOOR_X_BLOCK + 1; nCntX++)
		{//X���W
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((600 / 2.0f) + ((1350.0f / FLOOR_X_BLOCK) * nCntX), 0.0f, (2850.0f / 2.0f) - ((1400.0f / FLOOR_Z_BLOCK)* nCntZ));

			//�e���_�̖@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 1;			//���_�f�[�^��1���i�߂�
		}
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffFloor->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD * pldx;
	g_pldxBuffMeshField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <FLOOR_Z_BLOCK; nCntZ++)
	{//Z���W
		for (int nCntX = 0; nCntX < FLOOR_X_BLOCK + 1; nCntX++, nData++)
		{//X���W
		//�C���f�b�N�X�̐ݒ�
			pldx[0] = (FLOOR_X_BLOCK + 1) + nData;		//��
			pldx[1] = nData;							//��

			pldx += 2;		//�C���f�b�N�X�f�[�^��2���i�߂�
		}
		if (nCntZ != (FLOOR_Z_BLOCK - 1))
		{//�܂�Ԃ�
			pldx[0] = nData - 1;						//�E��
			pldx[1] = nData + (FLOOR_X_BLOCK + 1);		//����

			pldx += 2;			//�C���f�b�N�X�f�[�^��2���i�߂�
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pldxBuffMeshField->Unlock();
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitFloor(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̊J��
	if (g_pldxBuffMeshField != NULL)
	{
		g_pldxBuffMeshField->Release();
		g_pldxBuffMeshField = NULL;
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateFloor(void)
{
	//���͂Ȃ�
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawFloor(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldFloor);

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pldxBuffMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotFloor.y, g_rotFloor.x, g_rotFloor.z);
	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posFloor.x, g_posFloor.y, g_posFloor.z);
	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFloor);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0, 												//���_�o�b�t�@
		0, 												//���_�o�b�t�@�̊J�n�ʒu
		FLOOR_VERTEX_NUM, 								//�ǂ͈̔͂̒��_�o�b�t�@���g�p���邩
		0, 												//���_�C���f�b�N�X�o�b�t�@�̊J�n�ʒu
		FLOOR_PRIMITIVE_NUM);							//�����v���~�e�B�u��`�悷�邩
}

//-------------------------------------------
//�ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu
//float fWidth	  �� ��
//float fHeigth	  �� ����
//-------------------------------------------
void SetFloor(D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFloor = 0 ; nCntFloor < MAX_FLOOR ; nCntFloor++)
	{
		if (!g_aFloor[nCntFloor].bUse)
		{
			g_aFloor[nCntFloor].pos = pos;				//�ʒu
			g_aFloor[nCntFloor].fWidth = fWidth;		//��
			g_aFloor[nCntFloor].fHeigth = fHeigth;		//����
			g_aFloor[nCntFloor].bUse = true;			//�g�p����

			break;
		}
		pVtx += 4;			//���_�f�[�^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFloor->Unlock();
}