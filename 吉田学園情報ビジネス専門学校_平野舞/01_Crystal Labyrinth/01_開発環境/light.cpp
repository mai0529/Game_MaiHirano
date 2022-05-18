//-------------------------------------------
//
//���C�g����[light.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "light.h"

//�}�N����`
#define MAX_LIGHT	(3)			//���C�g�̍ő吔

//�O���[�o���ϐ�
D3DLIGHT9 g_light[MAX_LIGHT];		//���C�g���

//-------------------------------------------
//����������
//-------------------------------------------
void InitLight(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���C�g�̕����x�N�g��
	D3DXVECTOR3 vecDir;

	//���C�g���N���A�ɂ���
	ZeroMemory(&g_light[0], sizeof(g_light));

	float PosX = 0.0f;			//�ʒuX
	float PosY = 0.0f;			//�ʒuY
	float PosZ = 0.0f;			//�ʒuZ

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		switch (nCntLight)
		{
		case 0:					//1�ڂ̃��C�g
			PosX = -1.0f;			//�ʒuX
			PosY = -1.0f;			//�ʒuY
			PosZ =  1.0f;			//�ʒuZ
			break;
		case 1:					//2�ڂ̃��C�g
			PosX = 1.6f;			//�ʒuX
			PosY = 1.5f;			//�ʒuY
			PosZ = 2.0f;			//�ʒuZ
			break;
		case 2:					//3�ڂ̃��C�g
			PosX = 2.0f;			//�ʒuX
			PosY = 2.0f;			//�ʒuY
			PosZ = 1.6f;			//�ʒuZ
			break;
		default:
			assert(false);
			break;
		}

		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���C�g�̕�����ݒ�
		vecDir = D3DXVECTOR3(PosX, PosY, PosZ);

		//���K������
		D3DXVec3Normalize(&vecDir, &vecDir);
		g_light[nCntLight].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitLight(void)
{
	//���ɂȂ�
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateLight(void)
{
	//���ɂȂ�
}