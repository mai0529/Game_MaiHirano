//-------------------------------------------
//
//タイマー処理[time.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "time.h"
#include "fade.h"
#include "start.h"

//マクロ定義
#define TIME_WIDTH		(20.0f)		//タイムの幅
#define TIME_HEIGHT		(45.0f)		//タイムの高さ
#define TIME_MAX		(3)			//タイムの最大桁数

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posTime[TIME_MAX];					//タイムの位置
int g_nTime;										//タイムの値
int g_nCntTime;										//何フレームか
bool g_bTimeFade;									//フェードしているかどうか

//-------------------------------------------
//タイマーの初期化処理
//-------------------------------------------
void InitTime(void)
{
	for (int nCount = 0; nCount < TIME_MAX; nCount++)
	{//位置の初期化
		g_posTime[nCount] = D3DXVECTOR3(600.0f + (25.0f * nCount), 30.0f, 0.0f);
	}
	g_nTime = 300;						//何秒間か
	int g_nCntTime = 60;				//60フレーム
	g_bTimeFade = false;				//フェードしていない

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&g_pTextureTime);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < TIME_MAX; nCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime[nCount].x - (TIME_WIDTH / 2.0f), g_posTime[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[1].pos = D3DXVECTOR3(g_posTime[nCount].x + (TIME_WIDTH / 2.0f), g_posTime[nCount].y - (TIME_HEIGHT / 2.0f), 0);
		pVtx[2].pos = D3DXVECTOR3(g_posTime[nCount].x - (TIME_WIDTH / 2.0f), g_posTime[nCount].y + (TIME_HEIGHT / 2.0f), 0);
		pVtx[3].pos = D3DXVECTOR3(g_posTime[nCount].x + (TIME_WIDTH / 2.0f), g_posTime[nCount].y + (TIME_HEIGHT / 2.0f), 0);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;		//データを4つ分進める
	}
		//頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
}

//-------------------------------------------
//タイマーの終了処理
//-------------------------------------------
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//-------------------------------------------
//タイマーの更新処理
//-------------------------------------------
void UpdateTime(void)
{
	int nTimePos[3];		//テクスチャ設定用

	//スタート情報の取得
	Start * pStart = GetStart();

	if (!pStart->bUse)
	{//スタートがfalseになったらタイマー開始
		if (g_nTime != 0)
		{//タイマが0ではなかったらフレーム数を減らす
			g_nCntTime--;
		}

		if (g_nCntTime <= 0)
		{//フレームが0になったらタイマを減らす
			g_nTime--;
			g_nCntTime = 60;
		}
	}

	nTimePos[0] = g_nTime % 1000 / 100;
	nTimePos[1] = g_nTime % 100 / 10;
	nTimePos[2] = g_nTime % 10;

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * nTimePos[nCntTime] + 0.1f, 1.0f);

		pVtx += 4;		//データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

	if (g_nTime == 0 && !g_bTimeFade)
	{
			SetFade(MODE_GAMEOVER);		//ゲームオーバー画面に移行
			g_bTimeFade = true;			//フェードしている
	}
}

//-------------------------------------------
//タイマーの描画処理
//-------------------------------------------
void DrawTime(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCntTime = 0; nCntTime < TIME_MAX; nCntTime++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTime);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//-------------------------------------------
//タイマーの取得
//-------------------------------------------
int GetTime(void)
{
	return g_nTime;
}