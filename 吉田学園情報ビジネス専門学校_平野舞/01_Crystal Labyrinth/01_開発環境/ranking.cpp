//-------------------------------------------
//
//�����L���O����[ranking.cpp]
//Author:���앑
//
//-------------------------------------------

//�C���N���[�h�t�@�C��
#include "ranking.h"
#include "fade.h"
#include "input.h"
#include "time.h"
#include "sound.h"
#include "gameclear.h"
#include "controller.h"

//�}�N����`
#define MAX_RANKY				(5)			//Y�̐�
#define MAX_RANKX				(3)			//X�̐�
#define RANKING_WIDTH			(50)		//���ʂ̕�
#define RANKING_HEIGHT			(100)		//���ʂ̍���
#define MAX_TEX					(2)			//�摜�̖���

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posRank[MAX_RANKY][MAX_RANKX];				//�X�R�A�̈ʒu
Rank g_RankTime[MAX_RANKY];									//�����L���O�X�R�A���
bool g_bRankFade;											//�t�F�[�h���Ă��邩�ǂ���

//-------------------------------------------
//����������
//-------------------------------------------
void InitRanking(void)
{
	//�X�R�A�ݒu�ꏊ�̏�����
	for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
	{
		//�X�R�A�̐ݒu�ꏊ
		g_posRank[0][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 210.0f, 0.0f);
		g_posRank[1][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 330.0f, 0.0f);
		g_posRank[2][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 460.0f, 0.0f);
		g_posRank[3][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 560.0f, 0.0f);
		g_posRank[4][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 655.0f, 0.0f);
	}
	g_bRankFade = false;		//�t�F�[�h���Ă��Ȃ�

	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking001.jpg",
		&g_pTextureRank[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&g_pTextureRank[1]);

	//���_�o�b�t�@�̐���(�w�i)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

	//���_�o�b�t�@�̐���(�X�R�A)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * 5,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

	//---------------------------�w�i�p---------------------------------------------------
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffRank[0]->Unlock();

	//---------------------------�����p---------------------------------------------------
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;		//���_�f�[�^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[1]->Unlock();

	//�����L���O�̐ݒu
	SetRanking();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitRanking(void)
{
	//�T�E���h�̒�~
	StopSound();

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{//�e�N�X�`���̔j��
		if (g_pTextureRank[nCount] != NULL)
		{				 
			g_pTextureRank[nCount]->Release();
			g_pTextureRank[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{	//���_�o�b�t�@�̔j��
		if (g_pVtxBuffRank[nCount] != NULL)
		{				 
			g_pVtxBuffRank[nCount]->Release();
			g_pVtxBuffRank[nCount] = NULL;
		}
	}
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateRanking(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bRankFade
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bRankFade)
	{
			PlaySound(SOUND_LABEL_SE000);	//SE�̍Đ�
			SetFade(MODE_TITLE);		//�^�C�g����ʂɈڍs
			g_bRankFade = true;			//�t�F�[�h���Ă���
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawRanking(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < 5; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//-------------------------------------------
//�����L���O�̃��Z�b�g
//-------------------------------------------
void ResetRanking(void)
{
	FILE * pFile;		//�t�@�C���|�C���^

	//�t�@�C�����J��
	pFile = fopen("data/Ranking.txt", "r");
	if(pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C�����烉���L���O��ǂݍ���
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			fscanf(pFile, "%d\n", &g_RankTime[nCount].nTime);
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}
}

//-------------------------------------------
//�����L���O�̐ݒ�
//-------------------------------------------
void SetRanking(void)
{
	//�N���A����̏����l��
	bool pClear = GetClear();

		int aPosTexU[5][MAX_RANKX];			//�e���̐������i�[

		if (pClear)
		{
			int nTime;				//���Ԃ��擾����p�ϐ�
			nTime = GetTime();		//���Ԃ��擾
			g_RankTime[5].nTime = 300 - nTime;		//300�b����擾�������Ԃ�����

			//�o�u���\�[�g
			for (int nCnt = 0; nCnt < 5; nCnt++)
			{
				for (int nCnt1 = nCnt + 1; nCnt1 < 6; nCnt1++)
				{
					if (g_RankTime[nCnt].nTime > g_RankTime[nCnt1].nTime)
					{
						int nData = g_RankTime[nCnt].nTime;
						g_RankTime[nCnt].nTime = g_RankTime[nCnt1].nTime;
						g_RankTime[nCnt1].nTime = nData;
					}
				}
			}

			FILE * pFile;			//�t�@�C���|�C���^��錾

			//�����L���O���ʂ��t�@�C���ɏ����o��
			pFile = fopen("data/Ranking.txt", "w");
			if (pFile != NULL)
			{
				for (int nCount = 0; nCount < MAX_RANKY; nCount++)
				{
					//�t�@�C���Ƀ����L���O�������o��
					fprintf(pFile, "%d\n", g_RankTime[nCount].nTime);
				}
				fclose(pFile);
			}
			else
			{//�t�@�C�����J���Ȃ������ꍇ
				printf("�t�@�C�����J���܂���ł����B");
			}
		}

		for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
			{
				aPosTexU[nCntY][0] = g_RankTime[nCntY].nTime % 1000 / 100;
				aPosTexU[nCntY][1] = g_RankTime[nCntY].nTime % 100 / 10;
				aPosTexU[nCntY][2] = g_RankTime[nCntY].nTime % 10 / 1;
			}
		}

		//���_���ւ̃|�C���^
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W�̍X�V
		for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
			{
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 1.0f);

				pVtx += 4;		//�f�[�^��4���i�߂�
			}
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffRank[1]->Unlock();
}
