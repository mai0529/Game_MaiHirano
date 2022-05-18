//-------------------------------------------
//
//ランキング処理[ranking.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "ranking.h"
#include "fade.h"
#include "input.h"
#include "time.h"
#include "sound.h"
#include "gameclear.h"
#include "controller.h"

//マクロ定義
#define MAX_RANKY				(5)			//Yの数
#define MAX_RANKX				(3)			//Xの数
#define RANKING_WIDTH			(50)		//順位の幅
#define RANKING_HEIGHT			(100)		//順位の高さ
#define MAX_TEX					(2)			//画像の枚数

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//頂点バッファへのポインタ
D3DXVECTOR3 g_posRank[MAX_RANKY][MAX_RANKX];				//スコアの位置
Rank g_RankTime[MAX_RANKY];									//ランキングスコア情報
bool g_bRankFade;											//フェードしているかどうか

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitRanking(void)
{
	//スコア設置場所の初期化
	for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
	{
		//スコアの設置場所
		g_posRank[0][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 210.0f, 0.0f);
		g_posRank[1][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 330.0f, 0.0f);
		g_posRank[2][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 460.0f, 0.0f);
		g_posRank[3][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 560.0f, 0.0f);
		g_posRank[4][nCntX] = D3DXVECTOR3(450.0f + (70.0f * nCntX), 655.0f, 0.0f);
	}
	g_bRankFade = false;		//フェードしていない

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking001.jpg",
		&g_pTextureRank[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&g_pTextureRank[1]);

	//頂点バッファの生成(背景)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

	//頂点バッファの生成(スコア)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * 5,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

	//---------------------------背景用---------------------------------------------------
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRank[0]->Unlock();

	//---------------------------数字用---------------------------------------------------
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;		//頂点データを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();

	//ランキングの設置
	SetRanking();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitRanking(void)
{
	//サウンドの停止
	StopSound();

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{//テクスチャの破棄
		if (g_pTextureRank[nCount] != NULL)
		{				 
			g_pTextureRank[nCount]->Release();
			g_pTextureRank[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{	//頂点バッファの破棄
		if (g_pVtxBuffRank[nCount] != NULL)
		{				 
			g_pVtxBuffRank[nCount]->Release();
			g_pVtxBuffRank[nCount] = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateRanking(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bRankFade
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bRankFade)
	{
			PlaySound(SOUND_LABEL_SE000);	//SEの再生
			SetFade(MODE_TITLE);		//タイトル画面に移行
			g_bRankFade = true;			//フェードしている
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawRanking(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < 5; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//-------------------------------------------
//ランキングのリセット
//-------------------------------------------
void ResetRanking(void)
{
	FILE * pFile;		//ファイルポインタ

	//ファイルを開く
	pFile = fopen("data/Ranking.txt", "r");
	if(pFile != NULL)
	{//ファイルが開けた場合
		//ファイルからランキングを読み込む
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			fscanf(pFile, "%d\n", &g_RankTime[nCount].nTime);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}

//-------------------------------------------
//ランキングの設定
//-------------------------------------------
void SetRanking(void)
{
	//クリア判定の情報を獲得
	bool pClear = GetClear();

		int aPosTexU[5][MAX_RANKX];			//各桁の数字を格納

		if (pClear)
		{
			int nTime;				//時間を取得する用変数
			nTime = GetTime();		//時間を取得
			g_RankTime[5].nTime = 300 - nTime;		//300秒から取得した時間を引く

			//バブルソート
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

			FILE * pFile;			//ファイルポインタを宣言

			//ランキング結果をファイルに書き出す
			pFile = fopen("data/Ranking.txt", "w");
			if (pFile != NULL)
			{
				for (int nCount = 0; nCount < MAX_RANKY; nCount++)
				{
					//ファイルにランキングを書き出す
					fprintf(pFile, "%d\n", g_RankTime[nCount].nTime);
				}
				fclose(pFile);
			}
			else
			{//ファイルが開けなかった場合
				printf("ファイルが開けませんでした。");
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

		//頂点情報へのポインタ
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の更新
		for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
			{
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 1.0f);

				pVtx += 4;		//データを4つ分進める
			}
		}

		//頂点バッファをアンロックする
		g_pVtxBuffRank[1]->Unlock();
}
