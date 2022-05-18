// ------------------------------------------
//
//ライフ表示処理[life.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "life.h"

//マクロ定義
#define MAX_LIFE		(64)		//最大ライフ
#define MATLIFE_TEX		(2)			//テクスチャの最大数
#define MAXLIFE_VTX		(2)			//バッファの最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureLife[MATLIFE_TEX] = {};			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife[MAXLIFE_VTX] = {};		//頂点バッファへのポインタ
Life g_aLife[MAX_LIFE];											//ライフ情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitLife(void)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{//初期化
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aLife[nCntLife].nMaxLife = 0;								//最大ライフ数
		g_aLife[nCntLife].nNowLife = 0;								//今のライフ数
		g_aLife[nCntLife].bUse = false;								//使用しない
	}

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み(ゲージ)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/life001.png",
		&g_apTextureLife[0]);

	//テクスチャの読み込み(枠)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/life000.png",
		&g_apTextureLife[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife[0],
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife[1],
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//----------------------------------ゲージ--------------------------------------------------------
	//頂点バッファをロック
	g_pVtxBuffLife[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(28.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (28.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(28.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (28.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife[0]->Unlock();

	//----------------------------------枠--------------------------------------------------------
	//頂点バッファをロック
	g_pVtxBuffLife[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(30.0f / 2.0f), (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(( 30.0f / 2.0f), (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(30.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (30.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife[1]->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitLife(void)
{
	for (int nCntLife = 0; nCntLife < MATLIFE_TEX; nCntLife++)
	{//テクスチャの破棄
		if (g_apTextureLife[nCntLife] != NULL)
		{
			g_apTextureLife[nCntLife]->Release();
			g_apTextureLife[nCntLife] = NULL;
		}
	}

	for (int nCntLife = 0; nCntLife < MAXLIFE_VTX; nCntLife++)
	{//頂点バッファの破棄
		if (g_pVtxBuffLife[nCntLife] != NULL)
		{
			g_pVtxBuffLife[nCntLife]->Release();
			g_pVtxBuffLife[nCntLife] = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateLife(void)
{
	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffLife[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse)
		{//使用していたら
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f / g_aLife[nCntLife].nMaxLife) * (g_aLife[nCntLife].nMaxLife - g_aLife[nCntLife].nNowLife), 1.0f);

			if (g_aLife[nCntLife].nNowLife == 0)
			{//ライフが0になったら
				g_aLife[nCntLife].bUse = false;		//使用しない
			}
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife[0]->Unlock();
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawLife(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse)
		{//使用していたら
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLife[nCntLife].mtxWorld);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aLife[nCntLife].pos.x, g_aLife[nCntLife].pos.y, g_aLife[nCntLife].pos.z);
			D3DXMatrixMultiply(&g_aLife[nCntLife].mtxWorld, &g_aLife[nCntLife].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLife[nCntLife].mtxWorld);

			for (int nCount = 0; nCount < 2; nCount++)
			{
				//頂点バッファをデバイスのデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffLife[nCount], 0, sizeof(VERTEX_3D));

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャ設定
				pDevice->SetTexture(0, g_apTextureLife[nCount]);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);

				//ライトを有効に戻す
				pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			}
		}
	}
}

//-------------------------------------------
//設定処理
//
//D3DXVECTOR3 pos → 位置
//int nMax		  → ライフの最大数
//int nNow		  → 現在の最大数
//-------------------------------------------
void SetLife(D3DXVECTOR3 pos, int nMax, int nNow)
{
	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		if (!g_aLife[nCntLife].bUse)
		{//使用していなかったら
			g_aLife[nCntLife].pos = pos;		//位置
			g_aLife[nCntLife].nMaxLife = nMax;	//最大ライフ数
			g_aLife[nCntLife].nNowLife = nNow;	//今のライフ数
			g_aLife[nCntLife].bUse = true;		//使用する

			break;
		}
	}
}

//-------------------------------------------
//ライフの減算処理
//
//int nCntLife → 何番目のライフゲージか指定
//int nReduce  → ライフが減る値を指定
//-------------------------------------------
void SubLife(int nCntLife,int nReduce)
{
		if (g_aLife[nCntLife].bUse)
		{
			g_aLife[nCntLife].nNowLife -= nReduce;
		}
}

//-------------------------------------------
//ライフゲージの位置の更新処理
//
//int nIdxLife	  → 何番目のライフゲージか指定
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetPositionLife(int nIdxLife, D3DXVECTOR3 pos)
{
	g_aLife[nIdxLife].pos = pos;
}