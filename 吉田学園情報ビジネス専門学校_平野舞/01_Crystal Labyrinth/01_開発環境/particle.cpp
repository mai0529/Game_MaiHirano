//-------------------------------------------
//
//パーティクル処理[particle.cpp]
//Author:平野舞
//
//------------------------------------------- 

//インクルードファイル
#include "particle.h"
#include "effect.h"
#include <stdlib.h>

//マクロ定義
#define MAX_PARTICLE		(256)				//パーティクルの最大数

//グローバル変数宣言
//LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;		//頂点バッファへのポインタ
Particle g_aParticle[MAX_PARTICLE];						//パーティクルの情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//エフェクト情報の初期化
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//カラーの初期化
		g_aParticle[nCntParticle].fSize = 0.0f;								//サイズの初期化
		g_aParticle[nCntParticle].fGravity = 0.0f;							//重力の初期化
		g_aParticle[nCntParticle].nLife = 0;								//ライフの初期化
		g_aParticle[nCntParticle].nType = EFFECT_CIRCLE;					//種類の初期化
		g_aParticle[nCntParticle].bUse = false;								//使用しているかどうか
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(1.0f / 2.0f),  (1.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (1.0f / 2.0f),  (1.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(1.0f / 2.0f), -(1.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (1.0f / 2.0f), -(1.0f / 2.0f), 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffParticle->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitParticle(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			g_aParticle[nCntParticle].move.y -= g_aParticle[nCntParticle].fGravity;

			//位置情報を更新
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//エフェクトを設定
			SetEffect(g_aParticle[nCntParticle].pos, g_aParticle[nCntParticle].col, g_aParticle[nCntParticle].nLife, g_aParticle[nCntParticle].fSize, g_aParticle[nCntParticle].nType);

			//a値を減らしていく
			g_aParticle[nCntParticle].col.a -= 0.1;

			//寿命(表示時間)
			g_aParticle[nCntParticle].nLife -= 1;			//ライフを減らしていく
			if (g_aParticle[nCntParticle].nLife == 0)
			{//ライフが0になったら
				g_aParticle[nCntParticle].bUse = false;		//使用していない状態にする
				g_aParticle[nCntParticle].move.y = 0;
			}
		}
	}
}

//-------------------------------------------
//描画処理処理
//-------------------------------------------
void DrawParticle(void)
{
	//なし
}

//-------------------------------------------
//設定処理
//
//D3DXVECTOR3 pos  → 位置
//D3DXVECTOR3 move → 移動量
//D3DXCOLOR col	   → 色
//int nLife		   → 寿命
//int nType		   → 種類(effect.hにenum型で記入あり)
//float fSize	   → 大きさ
//float fGravity   → 重力
//-------------------------------------------
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, int nType, float fSize, float fGravity)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].bUse)
		{//使用していなかったら
			g_aParticle[nCntParticle].pos = pos;				//位置
			g_aParticle[nCntParticle].move = move;				//位置
			g_aParticle[nCntParticle].col = col;				//カラー
			g_aParticle[nCntParticle].nLife = nLife;			//ライフ
			g_aParticle[nCntParticle].nType = nType;			//種類
			g_aParticle[nCntParticle].fSize = fSize;			//サイズ
			g_aParticle[nCntParticle].fGravity = fGravity;		//重力
			g_aParticle[nCntParticle].bUse = true;				//使用する

			break;
		}
	}
}