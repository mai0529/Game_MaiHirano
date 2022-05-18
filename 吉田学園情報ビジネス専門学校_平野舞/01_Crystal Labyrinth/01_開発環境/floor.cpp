//-------------------------------------------
//
//床表示処理[floor.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "floor.h"

//マクロ定義
#define FLOOR_X_BLOCK			(8)		//X方向のブロック数
#define FLOOR_Z_BLOCK			(8)		//X方向のブロック数

#define FLOOR_VERTEX_NUM		((FLOOR_X_BLOCK + 1) * (FLOOR_Z_BLOCK + 1))										//頂点数
#define FLOOR_INDEX_NUM			((((FLOOR_X_BLOCK + 1) * 2) * FLOOR_Z_BLOCK) + (2 * (FLOOR_Z_BLOCK - 1)))		//インデックス数
#define FLOOR_PRIMITIVE_NUM		((2 * FLOOR_X_BLOCK * FLOOR_Z_BLOCK) + (4 * (FLOOR_Z_BLOCK - 1)))				//ポリゴン数

#define MAX_FLOOR		(128)			//メッシュフィールドの最大数

//グローバル変数宣言
//LPDIRECT3DTEXTURE9 g_pTextureFloor = NULL;		//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFloor = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pldxBuffMeshField;			//インデックスバッファへのポインタ
D3DXVECTOR3 g_posFloor;								//位置
D3DXVECTOR3 g_rotFloor;								//向き
D3DXMATRIX g_mtxWorldFloor;							//ワールドマトリックス
Floor g_aFloor[MAX_FLOOR];							//メッシュフィールドの情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitFloor(void)
{
	//初期設定
	for (int nCntFloor = 0; nCntFloor < MAX_FLOOR; nCntFloor++)
	{
		g_aFloor[nCntFloor].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aFloor[nCntFloor].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);		//向き	
		g_aFloor[nCntFloor].fWidth = 0.0f;							//幅
		g_aFloor[nCntFloor].fHeigth = 0.0f;							//高さ
		g_aFloor[nCntFloor].bUse = false;							//使用する
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * FLOOR_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFloor,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * FLOOR_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffMeshField,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < FLOOR_Z_BLOCK + 1; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < FLOOR_X_BLOCK + 1; nCntX++)
		{//X座標
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((600 / 2.0f) + ((1350.0f / FLOOR_X_BLOCK) * nCntX), 0.0f, (2850.0f / 2.0f) - ((1400.0f / FLOOR_Z_BLOCK)* nCntZ));

			//各頂点の法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 1;			//頂点データを1つ分進める
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffFloor->Unlock();

	//インデックスバッファをロック
	WORD * pldx;
	g_pldxBuffMeshField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <FLOOR_Z_BLOCK; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < FLOOR_X_BLOCK + 1; nCntX++, nData++)
		{//X座標
		//インデックスの設定
			pldx[0] = (FLOOR_X_BLOCK + 1) + nData;		//下
			pldx[1] = nData;							//上

			pldx += 2;		//インデックスデータを2つ分進める
		}
		if (nCntZ != (FLOOR_Z_BLOCK - 1))
		{//折り返し
			pldx[0] = nData - 1;						//右上
			pldx[1] = nData + (FLOOR_X_BLOCK + 1);		//左下

			pldx += 2;			//インデックスデータを2つ分進める
		}
	}

	//インデックスバッファのアンロック
	g_pldxBuffMeshField->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitFloor(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFloor != NULL)
	{
		g_pVtxBuffFloor->Release();
		g_pVtxBuffFloor = NULL;
	}

	//インデックスバッファの開放
	if (g_pldxBuffMeshField != NULL)
	{
		g_pldxBuffMeshField->Release();
		g_pldxBuffMeshField = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateFloor(void)
{
	//今はなし
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawFloor(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldFloor);

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pldxBuffMeshField);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotFloor.y, g_rotFloor.x, g_rotFloor.z);
	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posFloor.x, g_posFloor.y, g_posFloor.z);
	D3DXMatrixMultiply(&g_mtxWorldFloor, &g_mtxWorldFloor, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldFloor);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFloor, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0, 												//頂点バッファ
		0, 												//頂点バッファの開始位置
		FLOOR_VERTEX_NUM, 								//どの範囲の頂点バッファを使用するか
		0, 												//頂点インデックスバッファの開始位置
		FLOOR_PRIMITIVE_NUM);							//いくつプリミティブを描画するか
}

//-------------------------------------------
//設定処理
//
//D3DXVECTOR3 pos → 位置
//float fWidth	  → 幅
//float fHeigth	  → 高さ
//-------------------------------------------
void SetFloor(D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFloor->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFloor = 0 ; nCntFloor < MAX_FLOOR ; nCntFloor++)
	{
		if (!g_aFloor[nCntFloor].bUse)
		{
			g_aFloor[nCntFloor].pos = pos;				//位置
			g_aFloor[nCntFloor].fWidth = fWidth;		//幅
			g_aFloor[nCntFloor].fHeigth = fHeigth;		//高さ
			g_aFloor[nCntFloor].bUse = true;			//使用する

			break;
		}
		pVtx += 4;			//頂点データを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFloor->Unlock();
}