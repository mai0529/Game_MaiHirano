//-------------------------------------------
//
//�G�̏���[enemy.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "stage.h"
#include "life.h"
#include "start.h"
#include "map.h"
#include "item.h"

//�}�N����`
#define ENEMY_STATERATIO		(20)		//�_�ł����銄��
#define ENEMY_CNTSTATE			(100)		//��ԑJ�ڃJ�E���^�[
#define ENEMY_FOLLOWSTATE		(180)		//�Ǐ]�p�̃J�E���^�[
#define ENEMYLIFE_HEIGTH		(70.0f)		//���C�t�Q�[�W�̍���

//�O���[�o���ϐ�
Enemy g_aEnemy[MAX_ENEMY];		//�G�̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitEnemy(void)
{
	//�G���̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aEnemy[nCntEnemy].posDis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����̍�
		g_aEnemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�T�C�Y
		g_aEnemy[nCntEnemy].vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);		//�ŏ����_�l
		g_aEnemy[nCntEnemy].vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);	//�ő咸�_�l
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					//���
		g_aEnemy[nCntEnemy].nCounter = 180;								//�J�E���^�[
		g_aEnemy[nCntEnemy].nCntState = 100;							//��ԊǗ��J�E���^�[
		g_aEnemy[nCntEnemy].nLife = 5;									//���C�t
		g_aEnemy[nCntEnemy].bUse = false;								//�g�p���Ȃ�
		g_aEnemy[nCntEnemy].bDis = false;								//�g�p���Ȃ�
		g_aEnemy[nCntEnemy].bRot = true;								//��]����
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX("data/MODEL/enemy.x",		
			D3DXMESH_SYSTEMMEM,						
			pDevice,								
			NULL,									
			&g_aEnemy[nCntEnemy].pBuffMat,			
			NULL,									
			&g_aEnemy[nCntEnemy].nNumMat,			
			&g_aEnemy[nCntEnemy].pMesh);			
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���̎擾
		nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x�l
			if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
			{//���_�l��������������
				g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
			{//���_�l���傫��������
				g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;		//�ő咸�_�l�ɑ��
			}

			//y�l
			if (vtx.y < g_aEnemy[nCntEnemy].vtxMin.y)
			{//���_�l��������������
				g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.y > g_aEnemy[nCntEnemy].vtxMax.y)
			{//���_�l���傫��������
				g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;		//�ő咸�_�l�ɑ��
			}

			//z�l
			if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
			{//���_�l��������������
				g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
			{//���_�l���傫��������
				g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;		//�ő咸�_�l�ɑ��
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			g_aEnemy[nCntEnemy].size.x = g_aEnemy[nCntEnemy].vtxMax.x - g_aEnemy[nCntEnemy].vtxMin.x;		//x�̃T�C�Y���擾
			g_aEnemy[nCntEnemy].size.y = g_aEnemy[nCntEnemy].vtxMax.y - g_aEnemy[nCntEnemy].vtxMin.y;		//y�̃T�C�Y���擾
			g_aEnemy[nCntEnemy].size.z = g_aEnemy[nCntEnemy].vtxMax.z - g_aEnemy[nCntEnemy].vtxMin.z;		//z�̃T�C�Y���擾
		}

		//���_�o�b�t�@�̃A�����b�N
		g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
	}
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//���b�V���̔j��
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//�}�e���A���̔j��
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateEnemy(void)
{
	Start * pStart = GetStart();		//�S�[���̏����擾

	if (!pStart->bUse)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_aEnemy[nCntEnemy].bUse)
			{//�g�p���Ă�����
				//�O��̈ʒu��ۑ�
				g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

				//�v���C���[�ɒǏ]����
				FollowEnemy(nCntEnemy);

				//�ǂƂ̓����蔻��
				CollisionStage(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].size);

				//���C�t�Q�[�W�̈ʒu�X�V
				SetPositionLife(1 + nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMYLIFE_HEIGTH, g_aEnemy[nCntEnemy].pos.z));

				//�G�}�b�v�̈ʒu�X�V
				SetPositionEnemyMap(nCntEnemy, g_aEnemy[nCntEnemy].pos);

				//��ԊǗ�
				StateEnemy(nCntEnemy);
			}
		}
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL * pMat;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse && g_aEnemy[nCntEnemy].bDis)
		{//�G���g�p���Ă�����
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A����ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
			}

			//�ێ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//�ݒ菈��
//
//D3DXVECTOR3 pos �� �ʒu�̎w��
//-------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{//�G���g�p���Ă��Ȃ�������
			g_aEnemy[nCntEnemy].pos = pos;		//�ʒu
			g_aEnemy[nCntEnemy].bUse = true;	//�g�p����
			g_aEnemy[nCntEnemy].bDis = true;	//�g�p����

			//���C�t�Q�[�W�̐ݒ�
			SetLife(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMYLIFE_HEIGTH, g_aEnemy[nCntEnemy].pos.z), 5, g_aEnemy[nCntEnemy].nLife);
			break;
		}
	}
}

//-------------------------------------------
//��ԊǗ�����
//
//int nCntEnemy �� ���Ԗڂ̓G���w��
//-------------------------------------------
void StateEnemy(int nCntEnemy)
{
	switch (g_aEnemy[nCntEnemy].state)
	{
	case ENEMYSTATE_NORMAL:		//�ʏ���
		break;
	case ENEMYSTATE_DAMAGE:		//�_���[�W���
		g_aEnemy[nCntEnemy].nCntState--;					//�J�E���^�[�����炷
		//�_�ł��ă_���[�W�����������悤�Ɍ�����
		BlinkEnemy(nCntEnemy, ENEMY_STATERATIO);
		break;
	case ENEMYSTATE_DATH:		//���S���
		g_aEnemy[nCntEnemy].bUse = false;						//�g�p���Ȃ�
		break;
	default:
		break;
	}
}

//-------------------------------------------
//��ԊǗ�����
//
//int nCntEnemy �� ���Ԗڂ̓G���w��
//int ratio     �� �_�ł̊���
//-------------------------------------------
void BlinkEnemy(int nCntEnemy,int ratio)
{
	if (0 == g_aEnemy[nCntEnemy].nCntState % ratio)
	{
		g_aEnemy[nCntEnemy].bDis = true;				//�\������
	}
	if (10 == g_aEnemy[nCntEnemy].nCntState % ratio)
	{
		g_aEnemy[nCntEnemy].bDis = false;				//�\�����Ȃ�
	}
	if (g_aEnemy[nCntEnemy].nCntState == 0)
	{//��ԊǗ��J�E���^�[��0�ɂȂ�����
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;		//�ʏ��Ԃɂ���
		g_aEnemy[nCntEnemy].bDis = true;					//�g�p����
	}
}

//-------------------------------------------
//�q�b�g����
//
//int nCntEnemy �� ���Ԗڂ̓G���w��
//int nDamage	�� �_���[�W��
//-------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	if (g_aEnemy[nCntEnemy].bUse)
	{//�����g�p���Ă�����
  		g_aEnemy[nCntEnemy].nLife -= nDamage;		//�G�̃��C�t�����炷
		SubLife(1 + nCntEnemy,nDamage);				//���C�t�Q�[�W�����炷

		if (g_aEnemy[nCntEnemy].nLife == 0)
		{//���C�t��0�ɂȂ�����
			SetItem(g_aEnemy[nCntEnemy].pos);					//�A�C�e����ݒu
			SetItemMap(g_aEnemy[nCntEnemy].pos);				//�}�b�v�ɃA�C�e����\��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DATH;		//���S��Ԃɂ���
		}
		else
		{
			g_aEnemy[nCntEnemy].nCntState = ENEMY_CNTSTATE;		//�J�E���^�[��ݒ肷��
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		//�_���[�W��Ԃɂ���
		}
	}
}

//-------------------------------------------
//�Ǐ]����
//
//int nCntEnemy �� ���Ԗڂ̓G���w��
//-------------------------------------------
void FollowEnemy(int nCntEnemy)
{
	Player * pPlayer = GetPlayer();		//�v���C���[�̏����擾

	g_aEnemy[nCntEnemy].nCounter--;		//�J�E���g�����炵�Ă���

	if (g_aEnemy[nCntEnemy].bRot && 0 == g_aEnemy[nCntEnemy].nCounter)
	{//��]���Ă�����A�J�E���^�[��0�ɂȂ�����
	 //��莞�ԂŌ�����ς���
			g_aEnemy[nCntEnemy].rot.y += D3DX_PI / 2.0f;		//������ς���
			g_aEnemy[nCntEnemy].nCounter = ENEMY_FOLLOWSTATE;	//�J�E���g��߂�
	}

	//�����̐��K��
	if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{//3.14���傫��������
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//�v���C���[��ǔ�
	//�v���C���[�ƓG�̋����̍�
	g_aEnemy[nCntEnemy].posDis.x = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
	g_aEnemy[nCntEnemy].posDis.z = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;

	float fLength = D3DXVec3Length(&g_aEnemy[nCntEnemy].posDis);

	D3DXVec3Normalize(&g_aEnemy[nCntEnemy].posDis, &g_aEnemy[nCntEnemy].posDis);

	//�p�x�̍�
	float fAngle = atan2(g_aEnemy[nCntEnemy].posDis.z, g_aEnemy[nCntEnemy].posDis.x);

	//���E
	if (g_aEnemy[nCntEnemy].rot.y - ((D3DX_PI / 180.0f) * 30.0f) < fAngle - (D3DX_PI / 2.0f) &&
		g_aEnemy[nCntEnemy].rot.y + ((D3DX_PI / 180.0f) * 30.0f) > fAngle - (D3DX_PI / 2.0f))
	{//���E�ɓ�������
		if (fLength < 150.0f)
		{//�͈͓���������
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].posDis * 1.0f;		//�ǔ�����
			g_aEnemy[nCntEnemy].bRot = false;									//��]���Ȃ�
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].bRot = true;		//��]����
	}

	//��O��
	if (g_aEnemy[nCntEnemy].rot.y - ((D3DX_PI / 180.0f) * 30.0f) < fAngle + (D3DX_PI / 2.0f) &&
		g_aEnemy[nCntEnemy].rot.y + ((D3DX_PI / 180.0f) * 30.0f) > fAngle + (D3DX_PI / 2.0f))
	{//���E�ɓ�������
		if (fLength < 150.0f)
		{//�͈͓���������
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].posDis * 1.0f;		//�ǔ�����
			g_aEnemy[nCntEnemy].bRot = false;									//��]���Ȃ�
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].bRot = true;		//��]����
	}
}

//-------------------------------------------
//�G�̓����蔻��
//-------------------------------------------
void CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			if ((pPos->y - (size.y / 2.0f)) <= ((g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].vtxMax.y) / 2.0) &&	//��
				(pPos->y + (size.y / 2.0f)) >= ((g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].vtxMin.y) / 2.0))		//��
			{//y���̓����蔻��
			 //���E�̓����蔻��
				if ((pPos->z - (size.z / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z) &&	//��O
					(pPos->z + (size.z / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z))		//��
				{//z�͈͂̓����蔻��
					if ((pPosOld->x + (size.x / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) &&		//�O��̈ʒu
						(pPos->x + (size.x / 2.0f)) > (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x))			//���݂̈ʒu
					{//�����̓����蔻��
						pPos->x = (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) - (size.x / 2.0f);		//�����߂�
						HitPlayer(1);		//�_���[�W����
					}
					else if ((pPosOld->x - (size.x / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x) &&	//�O��̈ʒu
						(pPos->x - (size.x / 2.0f)) < (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x))				//���݂̈ʒu
					{//�E���̓����蔻��
						pPos->x = (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x) + (size.x / 2.0f);		//�����߂�
						HitPlayer(1);		//�_���[�W����
					}
				}

				//��O���̓����蔻��
				else if ((pPos->x + (size.x / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) &&		//��
					(pPos->x - (size.x / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x))		//�E
				{//x���̓����蔻��
					if ((pPosOld->z + (size.z / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z) &&		//�O��̈ʒu
						(pPos->z + (size.z / 2.0f)) > (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z))				//���݂̈ʒu
					{//��O�̓����蔻��
						pPos->z = (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z) - (size.z / 2.0f);		//�����߂�
							HitPlayer(1);		//�_���[�W����
					}
					else if ((pPosOld->z - (size.z / 2.0f)) >= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z &&	//�O��̈ʒu
						(pPos->z - (size.z / 2.0f)) < (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z))				//���݂̈ʒu
					{//���̓����蔻��
						pPos->z = (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z) + (size.z / 2.0f);		//�����߂�
						HitPlayer(1);		//�_���[�W����
					}
				}
			}
		}
	}
}

//-------------------------------------------
//���̎擾����
//-------------------------------------------
Enemy * GetEnemy(void)
{
	return &g_aEnemy[0];
}