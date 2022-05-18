//-------------------------------------------
//
//�v���C���[�\������[player.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "player.h"
#include "effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "item.h"
#include "start.h"
#include "stage.h"
#include "goal.h"
#include "enemy.h"
#include "fade.h"
#include "life.h"
#include "map.h"
#include "controller.h"

//�}�N����`
#define MAX_MOTION			(10)		//�ő�Motion��
#define PLAYER_RATIO		(20)		//�_�ł̊���
#define PLAYERLIFE_HEIGHT	(60.0f)		//���C�t�Q�[�W�̍���
#define PLAYERSHADOW_HEIGHT	(0.1f)		//�e�̍���
#define PLAYER_ROTATION		(0.2f)		//��]�����炩�ɂ���
#define PLAYER_STATE		(100)		//��Ԃ̃J�E���^�[

//�O���[�o���ϐ�
Player g_player;							//�v���C���[���
ModelParts g_ModelParts[MAX_MODELPARTS];	//���f���p�[�c�̏��
bool g_PlayerFade;							//�t�F�[�h���Ă��邩�ǂ���

//�O���t�@�C����ǂݍ��ޗp�̃O���[�o���ϐ��錾
PlayerFile g_PlayerFile[MAX_MODELPARTS];	//�O���t�@�C���̏��
int g_nNumPlayer;							//�p�[�c���̓ǂݍ��ݗp
char g_cPlayerFileName[30][256];			//�v���C���[�t�@�C�����̓ǂݍ��ݗp
char g_cPlayer[256];						//�������ǂݍ��ޗp

//���[�V�����p�̃O���[�o���ϐ��錾
int g_nNowFrame;							//���݂̃t���[����
int g_nNowKeySet;							//���ݍĐ�����KeySet�ԍ�
int g_nNextKeySet;							//���ɍĐ�����KeySet�ԍ�
int g_nNowMotion;							//���ݍĐ�����Motion�ԍ�
MotionSet g_Motion[MAX_MOTION];				//MotionSet�̏��

//-------------------------------------------
//����������
//-------------------------------------------
void InitPlayer(void)
{
	//���[�V�����̏�����
	g_nNowFrame = 0;							//���݂̃t���[�����̏�����
	g_nNowKeySet = 0;							//���݂̃L�[���̏�����
	g_nNowMotion = 0;							//���݂̃��[�V�����ԍ��̏�����
	g_nNextKeySet = g_nNowKeySet + 1;			//���ɍĐ�����KeySet�ԍ��̏�����

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//���[�V������
		for (int nCntKeySet = 0; nCntKeySet < MAX_KEYSET; nCntKeySet++)
		{//KeySet��
			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{//Key��
				g_Motion[nCntMotion].nLoop = 0;			//���[�v����
				g_Motion[nCntMotion].nNumKey = 0;		//�L�[��
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].nFrame = 0;		//�t���[����
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
			}
		}
	}

	//�O���t�@�C���ǂݍ��ݏ���
	LoadPlayerFile();
	LoadMotionPlayerFile();

	//�v���C���[���̏�����
	g_player.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);	//�ʒu
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O�ʒu
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړI�̊p�x
	g_player.size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�T�C�Y
	g_player.vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);		//�ŏ����_�l
	g_player.vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);	//�ő咸�_�l
	g_player.bUse = true;								//�g�p����
	g_player.state = PLAYERSTATE_NORMAL;				//���
	g_player.nCntState = 100;							//��ԊǗ��J�E���^�[
	g_player.nLife = 5;									//���C�t
	g_player.nIdxShadow = 0;							//�e�̔ԍ�
	g_PlayerFade = false;								//�t�F�[�h�p�̏�����

	//���f���p�[�c���̏�����
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		g_ModelParts[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_ModelParts[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_ModelParts[nCount].nIdxModelParent = g_PlayerFile[nCount].nParent;		//�e���f���̃C���f�b�N�X�w��
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_cPlayerFileName[nCount][0],	//�t�@�C�������w��
			D3DXMESH_SYSTEMMEM,								//���b�V���̐���
			pDevice,										//�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL,											//�אڃf�[�^�̃o�b�t�@�[�ւ̃|�C���^
			&g_ModelParts[nCount].pBuffMat,					//�}�e���A���̃o�b�t�@�[�ւ̃|�C���^
			NULL,											//�C���X�^���X�̃o�b�t�@�[�ւ̃|�C���^
			&g_ModelParts[nCount].nNumMat,					//D3DXMATERIAL�̐��̃|�C���^
			&g_ModelParts[nCount].pMesh);					//�C���^�[�t�F�[�X�ւ̃|�C���^�̃A�h���X
	}

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		//���_���̎擾
		nNumVtx = g_ModelParts[nCount].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_ModelParts[nCount].pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_ModelParts[nCount].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x�l
			if (vtx.x < g_player.vtxMin.x)
			{//���_�l��������������
				g_player.vtxMin.x = vtx.x;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.x > g_player.vtxMax.x)
			{//���_�l���傫��������
				g_player.vtxMax.x = vtx.x;		//�ő咸�_�l�ɑ��
			}

			//y�l
			if (vtx.y < g_player.vtxMin.y)
			{//���_�l��������������
				g_player.vtxMin.y = vtx.y;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.y > g_player.vtxMax.y)
			{//���_�l���傫��������
				g_player.vtxMax.y = vtx.y;		//�ő咸�_�l�ɑ��
			}

			//z�l
			if (vtx.z < g_player.vtxMin.z)
			{//���_�l��������������
				g_player.vtxMin.z = vtx.z;		//�ŏ����_�l�ɑ��
			}
			else if (vtx.z > g_player.vtxMax.z)
			{//���_�l���傫��������
				g_player.vtxMax.z = vtx.z;		//�ő咸�_�l�ɑ��
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@�̃A�����b�N
		g_ModelParts[nCount].pMesh->UnlockVertexBuffer();
	}

	//�T�C�Y
	g_player.size.x = g_player.vtxMax.x - g_player.vtxMin.x;	//x�̃T�C�Y���擾
	g_player.size.y = g_player.vtxMax.y - g_player.vtxMin.y;	//y�̃T�C�Y���擾
	g_player.size.z = g_player.vtxMax.z - g_player.vtxMin.z;	//z�̃T�C�Y���擾

	//�e�p�[�c�̔z�u
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		g_ModelParts[nCount].pos = g_PlayerFile[nCount].pos;
	}

	//�e�̐ݒ�
	g_player.nIdxShadow = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.1f, g_player.pos.z), g_player.rot, D3DXVECTOR3(25.0f, 0.0f, 25.0f));

	//���C�t�Q�[�W�̐ݒ�
	SetLife(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYERLIFE_HEIGHT, g_player.pos.z), 5, g_player.nLife);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitPlayer(void)
{
	//���b�V���̔j��
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		if (g_ModelParts[nCount].pMesh != NULL)
		{
			g_ModelParts[nCount].pMesh->Release();
			g_ModelParts[nCount].pMesh = NULL;
		}
	}

	//�}�e���A���̔j��
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		if (g_ModelParts[nCount].pBuffMat != NULL)
		{
			g_ModelParts[nCount].pBuffMat->Release();
			g_ModelParts[nCount].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdatePlayer(void)
{
	//�O��̈ʒu���X�V
	g_player.posOld = g_player.pos;

	//�ړ�����
	MovePlayer();

	//�e�̔���
	if (GetKeyboardTrigger(DIK_SPACE) || GetControllerPressTrigger(0, XINPUT_GAMEPAD_A))
	{//SPACE�L�[�A�R���g���[���[��A����������
		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 20.0f, g_player.pos.z));		//�e�̔���
	}

	//����(����)
	float fDis;
	fDis = g_player.rotDest.y - g_player.rot.y;

	//����(����)
	float fPosDisX, fPosDisY, fPosDisZ;
	fPosDisX = g_player.posOld.x - g_player.pos.x;
	fPosDisY = g_player.posOld.y - g_player.pos.y;
	fPosDisZ = g_player.posOld.z - g_player.pos.z;

	//���K��
	if (fDis > D3DX_PI)
	{//3.14���傫��������
		fDis -= D3DX_PI * 2.0f;
	}
	else if (fDis < -D3DX_PI)
	{//-3.14��菬����������
		fDis += D3DX_PI * 2.0f;
	}

	//�����̐��K��
	if (g_player.rot.y > D3DX_PI)
	{//3.14���傫��������
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y < -D3DX_PI)
	{//-3.14��菬����������
		g_player.rot.y += D3DX_PI * 2.0f;
	}

	//���f���̉�]�����炩�ɂ���
	g_player.rot.y += fDis * PLAYER_ROTATION;

	//�X�^�[�g�̓����蔻��
	CollisionStart(&g_player.pos, g_player.size);

	//�S�[���̓����蔻��
	CollisionGoal(&g_player.pos, g_player.size);

	//�G�̓����蔻��
	CollisionEnemy(&g_player.pos, &g_player.posOld, g_player.size);

	//�v���C���[�̏�ԊǗ�
	StatePlayer();

	//�A�C�e���Ƃ̓����蔻��
	CollisionItem(&g_player.pos, g_player.size);

	//�ǂ̓����蔻��
	CollisionStage(&g_player.pos, &g_player.posOld, g_player.size);

	//�e�̈ʒu�X�V
	SetPositionShadow(0, D3DXVECTOR3(g_player.pos.x, PLAYERSHADOW_HEIGHT, g_player.pos.z));

	//���C�t�Q�[�W�̈ʒu�X�V
	SetPositionLife(0, D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYERLIFE_HEIGHT, g_player.pos.z));

	//�v���C���[�}�b�v�̈ʒu�X�V
	SetPositionPlayerMap(g_player.pos);

	//���[�V��������
	MotionPlayer(0);
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL * pMat;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	if (g_player.bUse)
	{//�g�p���Ă�����
		//���f���p�[�c�̐ݒ�
		for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_ModelParts[nCount].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ModelParts[nCount].rot.y, g_ModelParts[nCount].rot.x, g_ModelParts[nCount].rot.z);
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_ModelParts[nCount].pos.x, g_ModelParts[nCount].pos.y, g_ModelParts[nCount].pos.z);
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxTrans);

			//�e���f���̃}�g���b�N�X�Ƃ̊|���Z
			D3DXMATRIX mtxParent;

			if (g_ModelParts[nCount].nIdxModelParent == -1)
			{//�e���f���̃C���f�b�N�X��-1�Ȃ�
				mtxParent = g_player.mtxWorld;					//�v���C���[�̃}�g���b�N�X����
			}
			else
			{//�e�̃��f���̃C���f�b�N�X��-1�ȊO�Ȃ�
				mtxParent = g_ModelParts[g_ModelParts[nCount].nIdxModelParent].mtxWorld;	//�����̐e�̃}�g���b�N�X����
			}

			//�|���Z
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_ModelParts[nCount].mtxWorld);

			//���݂̃}�e���A����ێ�
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL *)g_ModelParts[nCount].pBuffMat->GetBufferPointer();

			//x�t�@�C���̕`��
			for (int nCntMat = 0; nCntMat < (int)g_ModelParts[nCount].nNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, NULL);

				//���f���p�[�c�̕`��
				g_ModelParts[nCount].pMesh->DrawSubset(nCntMat);
			}

			//�ێ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//�ړ�����
//-------------------------------------------
void MovePlayer(void)
{
	//�J���������擾
	Camera * pCamera = GetCamera();

	//�X�e�B�b�N�̏����擾
	D3DXVECTOR3 Stick = GetControllerStickDir(0);

	if (GetKeyboardPress(DIK_A) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_LEFT)
		|| GetControllerStickPress(0, STICK_LEFT))
	{//A�L�[�A�R���g���[���[�̏\�����A�X�e�B�b�N�����������獶�ɐi��
		if (GetKeyboardPress(DIK_W))
		{//AW����������
			g_player.pos.x += sinf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetKeyboardPress(DIK_S))
		{//AS����������
			g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetControllerStickPress(0, STICK_UP) || GetControllerStickPress(0, STICK_DOWN))
		{//�R���g���[���[�̎΂�
			g_player.pos.x += Stick.x * PLAYER_DIS;
			g_player.pos.z += Stick.y * PLAYER_DIS;
		}
		else
		{//A����������
			g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / 2.0f);		//�ړI�̊p�x
			g_player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 2.0f)) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 2.0f)) * PLAYER_DIS;
		}
	}
	else if (GetKeyboardPress(DIK_D) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_RIGHT)
		|| GetControllerStickPress(0, STICK_RIGHT))
	{//D�L�[�A�R���g���[���[�̉E�A�X�e�B�b�N�̉E����������E�ɐi��
		if (GetKeyboardPress(DIK_W))
		{//DW����������
			g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetKeyboardPress(DIK_S))
		{//DS����������
			g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetControllerStickPress(0, STICK_UP) || GetControllerStickPress(0, STICK_DOWN))
		{//�R���g���[���[�̎΂ߑΉ�
			g_player.pos.x += Stick.x * PLAYER_DIS;
			g_player.pos.z += Stick.y * PLAYER_DIS;
		}
		else
		{//D����������
			g_player.rotDest.y = pCamera->rot.y - (D3DX_PI / 2.0f);			//�ړI�̊p�x
			g_player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 2.0f)) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 2.0f)) * PLAYER_DIS;
		}
	}
	else if (GetKeyboardPress(DIK_W) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_UP)
		|| GetControllerStickPress(0, STICK_UP))
	{//W�L�[�A�R���g���[���[�̏\���L�[�̏�A�X�e�B�b�N�̏���������牜�ɐi��
		g_player.rotDest.y = pCamera->rot.y + D3DX_PI;					//�ړI�̊p�x
		g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
		g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
	}
	else if (GetKeyboardPress(DIK_S) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_DOWN)
		|| GetControllerStickPress(0, STICK_DOWN))
	{//S�L�[�A�R���g���[���[�̉��A�X�e�B�b�N�̉������������O�ɐi��
		g_player.rotDest.y = pCamera->rot.y;								//�ړI�̊p�x
		g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
		g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
	}
}

//-------------------------------------------
//
//���[�V��������
//
//int nMotion �� ���[�V�����ԍ����w��
//-------------------------------------------
void MotionPlayer(int nMotion)
{
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{//�p�[�c����
		g_ModelParts[nCount].rot += D3DXVECTOR3(((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.x - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.x) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame),
			((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.y - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.y) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame),
			((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.z - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.z) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame));
	}

	if (g_nNextKeySet >= g_Motion[nMotion].nNumKey)
	{//���ɍĐ�����KeySet��KeySet�������傫���Ȃ�����
		g_nNextKeySet = 0;		//0�ɂ���
	}

	if (g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame <= g_nNowFrame)
	{//���݂̃t���[�������ݒ�t���[�������傫���Ȃ�����
		g_nNowFrame = 0;		//���݂̃t���[������0�ɂ���
		g_nNowKeySet++;			//���݂�KeySet�𑝂₷
		g_nNextKeySet++;		//���ɍĐ�����KeySet�𑝂₷
	}

	if (g_nNowKeySet >= g_Motion[nMotion].nNumKey)
	{//����KeySet�����[�V����KeySet�����傫���Ȃ�����
		if (g_Motion[g_nNowMotion].nLoop == 1)
		{//1�������ꍇ���[�v����
			g_nNowFrame = 0;	//���ݍĐ����̃t���[������0�ɂ���
			g_nNowKeySet = 0;	//���ݍĐ�����KeySet��0�ɂ���
		}
	}

	g_nNowFrame++;		//���ݍĐ����̃t���[�����𑝂₷
}

//-------------------------------------------
//��ԏ���
//-------------------------------------------
void StatePlayer(void)
{
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:		//�ʏ���
		break;
	case PLAYERSTATE_DAMAGE:		//�_���[�W���
		g_player.nCntState--;		//�J�E���g�����炷
		BlinkPlayer(PLAYER_RATIO);	//�_�ŏ���
		break;
	case PLAYERSTATE_DATH:			//���S���
		g_player.bUse = false;
		if (!g_PlayerFade)
		{//�t�F�[�h���Ă��Ȃ�������
			SetFade(MODE_GAMEOVER);		//�Q�[���I�[�o�[�\��
			g_PlayerFade = true;		//�t�F�[�h���Ă���
		}
		break;
	default:
		assert(false);
		break;
	}
}

//-------------------------------------------
//�_�ŏ���
//
//int ratio �� �_�ł̊���
//-------------------------------------------
void BlinkPlayer(int ratio)
{
	if (0 == g_player.nCntState % ratio)
	{
		g_player.bUse = false;					//�g�p���Ȃ�
	}
	if (10 == g_player.nCntState % ratio)
	{
		g_player.bUse = true;					//�g�p����
	}
	if (g_player.nCntState == 0)
	{//�J�E���g��0�ɂȂ�����
		g_player.state = PLAYERSTATE_NORMAL;		//�ʏ��Ԃɂ���
		g_player.bUse = true;						//�g�p����
	}
}

//-------------------------------------------
//�q�b�g����
//
//int nDamage �� �_���[�W��
//-------------------------------------------
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		g_player.nLife -= nDamage;		//�v���C���[�̃��C�t�����炷
		SubLife(0,nDamage);				//���C�t�Q�[�W�����炷

		if (g_player.nLife == 0)
		{//���C�t��0�ɂȂ�����
			g_player.state = PLAYERSTATE_DATH;		//���S��Ԃɂ���
		}
		else
		{
			g_player.nCntState = PLAYER_STATE;		//�J�E���^�[�̐��l��ݒ�
			g_player.state = PLAYERSTATE_DAMAGE;	//�_���[�W��Ԃɂ���
		}
	}
}

//-------------------------------------------
//����Y���̎擾
//-------------------------------------------
float GetPlayerRot(void)
{
	return g_player.rot.y;
}

//-------------------------------------------
//�v���C���[���̎擾
//-------------------------------------------
Player * GetPlayer(void)
{
	return &g_player;
}

//-------------------------------------------
//�O���t�@�C���ǂݍ��ݏ���(���f��)
//-------------------------------------------
void LoadPlayerFile(void)
{
	FILE * pFile;		//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data/character.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		while (1)
		{
			fscanf(pFile, "%s", &g_cPlayer[0]);		//������̓ǂݍ���

			if (strcmp(&g_cPlayer[0], "NUM_MODEL") == 0)
			{//���f���̐��̓ǂݍ���
				fscanf(pFile, "%s", &g_cPlayer[0]);
				fscanf(pFile, "%d", &g_nNumPlayer);		//������̓ǂݍ���
			}

			if (strcmp(&g_cPlayer[0], "MODEL_FILENAME") == 0)
			{//���f���t�@�C���ǂݍ���
				for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
				{
					while (strcmp(&g_cPlayer[0], "MODEL_FILENAME") != 0)
					{//���f���t�@�C���l�[���ł͂Ȃ��ꍇ
						fscanf(pFile, "%s", &g_cPlayer[0]);
					}

					if (strcmp(&g_cPlayer[0], "MODEL_FILENAME") == 0)
					{//���f���t�@�C���l�[����������
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%s", &g_cPlayerFileName[nCount][0]);		//������̓ǂݍ���
					}
				}
			}

			if (strcmp(&g_cPlayer[0], "CHARACTERSET") == 0)
			{//�L�����N�^�[�Z�b�g����������
				int nCount = 0;
				do
				{//�G���h�L�����N�^�[�Z�b�g�ɂȂ�܂�
					fscanf(pFile, "%s", &g_cPlayer[0]);			//������̓ǂݍ���

					if (strcmp(&g_cPlayer[0], "PARTSSET") == 0)
					{//�p�[�c�Z�b�g����������
						fscanf(pFile, "%s", &g_cPlayer[0]);		//������̓ǂݍ���

						do
						{//�G���h�p�[�c�Z�b�g�ɂȂ�܂�
							if (strcmp(&g_cPlayer[0], "INDEX") == 0)
							{//�C���f�b�N�X�̓ǂݍ���
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_PlayerFile[nCount].nIndex);		//������̓ǂݍ���
							}
							else if (strcmp(&g_cPlayer[0], "PARENT") == 0)
							{//�e�̃C���f�b�N�X�̓ǂݍ���
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_PlayerFile[nCount].nParent);		//������̓ǂݍ���
							}
							else if (strcmp(&g_cPlayer[0], "POS") == 0)
							{//���f���̈ʒu�̓ǂݍ���
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.x);		//������̓ǂݍ���
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.y);		//������̓ǂݍ���
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.z);		//������̓ǂݍ���
							}
							else if (strcmp(&g_cPlayer[0], "ROT") == 0)
							{//���f���̌����̓ǂݍ���
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.x);		//������̓ǂݍ���
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.y);		//������̓ǂݍ���
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.z);		//������̓ǂݍ���
							}
							else
							{
								fscanf(pFile, "%s", &g_cPlayer[0]);
							}
						} while (strcmp(&g_cPlayer[0], "END_PARTSSET") != 0);
					}
					nCount++;		//�J�E���g��1���₷
				} while (strcmp(&g_cPlayer[0], "END_CHARACTERSET") != 0);
			}

			if (strcmp(&g_cPlayer[0], "END_SCRIPT") == 0)
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

//-------------------------------------------
//�O���t�@�C���ǂݍ��ݏ���(���[�V����)
//-------------------------------------------
void LoadMotionPlayerFile(void)
{
	int nCntMotion = 0;		//���[�V�����̐���
	int nCntKey = 0;		//Key��
	int nCntKeySet = 0;		//KeySet��

	FILE * pFile;		//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("data/character.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		while (1)
		{
			fscanf(pFile, "%s", &g_cPlayer[0]);		//������̓ǂݍ���

			if (strcmp(&g_cPlayer[0], "MOTIONSET") == 0)
			{//����MOTIONSET�̕�����������
				do
				{//END_MOTIONSET�������甲����
					if (strcmp(&g_cPlayer[0], "LOOP") == 0)
					{//���[�v���邩�ǂ���
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%d", &g_Motion[nCntMotion].nLoop);
					}
					else if (strcmp(&g_cPlayer[0], "NUM_KEY") == 0)
					{//�w�肳�ꂽ�L�[����ǂݍ���
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%d", &g_Motion[nCntMotion].nNumKey);
					}
					else if (strcmp(&g_cPlayer[0], "KEYSET") == 0)
					{//����KEYSET�̕�����������
						do
						{//END_KEYSET�������甲����
							if (strcmp(&g_cPlayer[0], "FRAME") == 0)
							{//�t���[������ǂݍ���
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].nFrame);
							}
							else if (strcmp(&g_cPlayer[0], "KEY") == 0)
							{//����KEY�̕�����������
								do
								{//END_KEY�������甲����
									if (strcmp(&g_cPlayer[0], "POS") == 0)
									{//�ʒu�̓ǂݍ���
										fscanf(pFile, "%s", &g_cPlayer[0]);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.x);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.y);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.z);
									}
									else if (strcmp(&g_cPlayer[0], "ROT") == 0)
									{//�����̓ǂݍ���
										fscanf(pFile, "%s", &g_cPlayer[0]);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.x);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.y);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.z);
									}
									else
									{//����ȊO
										fscanf(pFile, "%s", &g_cPlayer[0]);
									}
								} while (strcmp(&g_cPlayer[0], "END_KEY") != 0);
								nCntKey++;		//Key�̃J�E���g�𑝂₷
							}
							else
							{
								fscanf(pFile, "%s", &g_cPlayer[0]);
							}
						} while (strcmp(&g_cPlayer[0], "END_KEYSET") != 0);
						nCntKeySet++;		//KeySet�̃J�E���g�𑝂₷
						nCntKey = 0;		//Key�J�E���g��0�ɖ߂�
					}
					else
					{
						fscanf(pFile, "%s", &g_cPlayer[0]);
					}
				} while (strcmp(&g_cPlayer[0], "END_MOTIONSET") != 0);
				nCntMotion++;		//Motion�̃J�E���g�𑝂₷
				nCntKeySet = 0;		//KeySet�̃J�E���g��0�ɖ߂�
			}
			if (strcmp(&g_cPlayer[0], "END_SCRIPT") == 0)
			{//���̕����������甲����
				break;
			}
		}
		fclose(pFile);					//�t�@�C�������
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł���");
	}
}