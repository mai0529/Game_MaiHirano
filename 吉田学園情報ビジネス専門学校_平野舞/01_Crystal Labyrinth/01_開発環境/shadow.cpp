//-------------------------------------------
//
//影処理[shadow.cpp]
//Author:平野舞
//
//------------------------------------------- 

//インクルードファイル
#include "shadow.h"

//マクロ定義
#define MAX_SHADOW		(124)				//影の最大数
#define SHADOW_WIDTH	(16.0f)				//影の幅
#define SHADOW_HEIGHT	(16.0f)				//影の高さ

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
Shadow g_aShadow[MAX_SHADOW];						//影の情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//影情報の初期化
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
		g_aShadow[nCntShadow].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//サイズの初期化
		g_aShadow[nCntShadow].bUse = false;								//使用しているかどうか
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(SHADOW_WIDTH / 2.0f), 0.0f,  (SHADOW_HEIGHT / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (SHADOW_WIDTH / 2.0f), 0.0f,  (SHADOW_HEIGHT / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(SHADOW_WIDTH / 2.0f), 0.0f, -(SHADOW_HEIGHT / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (SHADOW_WIDTH / 2.0f), 0.0f, -(SHADOW_HEIGHT / 2.0f));

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

	//頂点バッファのアンロック
	g_pVtxBuffShadow->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateShadow(void)
{
	//今はなし
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse)
		{//使用していたら
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);

			//設定を元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//-------------------------------------------
//影の設定処理
//
//D3DXVECTOR3 pos  → 位置
//D3DXVECTOR3 rot  → 向き
//D3DXVECTOR3 size → 大きさ
//-------------------------------------------
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	int nCntShadow;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (!g_aShadow[nCntShadow].bUse)
		{//使用していなかったら
			g_aShadow[nCntShadow].pos = pos;		//位置
			g_aShadow[nCntShadow].rot = rot;		//向き
			g_aShadow[nCntShadow].size = size;		//サイズ
			g_aShadow[nCntShadow].bUse = true;		//使用する

			//サイズの更新
			pVtx[0].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].size.x / 2.0f),0.0f, (g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[1].pos = D3DXVECTOR3((g_aShadow[nCntShadow].size.x / 2.0f), 0.0f, (g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[2].pos = D3DXVECTOR3(-(g_aShadow[nCntShadow].size.x / 2.0f),0.0f, -(g_aShadow[nCntShadow].size.z / 2.0f));
			pVtx[3].pos = D3DXVECTOR3((g_aShadow[nCntShadow].size.x / 2.0f), 0.0f, -(g_aShadow[nCntShadow].size.z / 2.0f));

			break;
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;				//影の番号を返す
}

//-------------------------------------------
//影の位置の更新処理
//
//int nIdxShadow  → 何番目の影か指定
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos = pos;		//位置の更新
}