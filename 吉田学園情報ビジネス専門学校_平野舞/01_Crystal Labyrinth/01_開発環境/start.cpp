//-------------------------------------------
//
//円柱処理[cylinder.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "start.h"
#include "particle.h"
#include "effect.h"

//マクロ定義
#define START_X_BLOCK			(15)		//X方向のブロック数
#define START_Z_BLOCK			(15)		//X方向のブロック数

#define START_VERTEX_NUM			((START_X_BLOCK + 1) * (START_Z_BLOCK + 1))									//頂点数
#define START_INDEX_NUM			((((START_X_BLOCK + 1) * 2) * START_Z_BLOCK) + (2 * (START_Z_BLOCK - 1)))		//インデックス数
#define START_PRIMITIVE_NUM		((2 * START_X_BLOCK * START_Z_BLOCK) + (4 * (START_Z_BLOCK - 1)))				//ポリゴン数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureStart = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pldxBuffClinderField;		//インデックスバッファへのポインタ
Start g_aStart;										//スタートの情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitStart(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aStart.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);	//位置
	g_aStart.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_aStart.bUse = true;								//使用する

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.png",
		&g_pTextureStart);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * START_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * START_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffClinderField,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < START_Z_BLOCK + 1; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < START_X_BLOCK + 1; nCntX++, nCount++)
		{//X座標

			g_aStart.rot.y = ((D3DX_PI * 2.0f) / START_X_BLOCK) * (float) nCntX;			//角度設定

			//角度が範囲から超えた場合(角度の正規化)
			if (g_aStart.rot.y  < -D3DX_PI)
			{//角度が-3.14よりも小さくなった場合
				g_aStart.rot.y += D3DX_PI * 2;
			}
			else if (g_aStart.rot.y  > D3DX_PI)
			{//角度が3.14よりも大きくなった場合
				g_aStart.rot.y -= D3DX_PI * 2;
			}
			
			//頂点座標の設定
			pVtx[0].pos.x = sinf(g_aStart.rot.y) * 40.0f;			//X座標
			pVtx[0].pos.y = (100.0f / START_Z_BLOCK) * nCntZ;	//Y座標
			pVtx[0].pos.z = cosf(g_aStart.rot.y) * 40.0f;			//Z座標

			//各頂点の法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, g_aStart.rot.y, 0.0f);

			//正規化する
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.1f, 0.4f, 0.7f, 1.0f);

			////テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / START_X_BLOCK) * nCntX , 0.0f - (1.0f / START_Z_BLOCK) * nCntZ);

			pVtx += 1;			//頂点データを1つ分進める
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffStart->Unlock();

	//インデックスバッファをロック
	WORD * pldx;
	g_pldxBuffClinderField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <START_Z_BLOCK; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < START_X_BLOCK + 1; nCntX++, nData++)
		{//X座標
		 //インデックスの設定
			pldx[0] = (START_X_BLOCK + 1) + nData;		//下
			pldx[1] = nData;							//上

			pldx += 2;		//インデックスデータを2つ分進める
		}
		if (nCntZ != (START_Z_BLOCK - 1))
		{//折り返し
			pldx[0] = nData - 1;						//右上
			pldx[1] = nData + (START_X_BLOCK + 1);		//左下

			pldx += 2;			//インデックスデータを2つ分進める
		}
	}

	//インデックスバッファのアンロック
	g_pldxBuffClinderField->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitStart(void)
{
	//テクスチャの破棄
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

	//インデックスバッファの開放
	if (g_pldxBuffClinderField != NULL)
	{
		g_pldxBuffClinderField->Release();
		g_pldxBuffClinderField = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateStart(void)
{
	g_aStart.rot.y -= 0.05;	//回転

	if (g_aStart.bUse)
	{
			//ランダム
			int nRandX, nRandZ;
			nRandX = rand() % 40;		//X軸のランダム
			nRandZ = rand() % 40;		//Z軸のランダム

				//パーティクルを設定 
			SetParticle(D3DXVECTOR3(g_aStart.pos.x + nRandX, 0.1f, g_aStart.pos.z + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x - nRandX, 0.1f, g_aStart.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x + 10 + nRandX, 0.1f, g_aStart.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 8.0f, -1.0f);
			SetParticle(D3DXVECTOR3(g_aStart.pos.x - 10 - nRandX, 0.1f, g_aStart.pos.z + 10 + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.4f, 0.8f), 12, EFFECT_TWINKLE, 8.0f, -1.0f);
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawStart(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	if (g_aStart.bUse)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aStart.mtxWorld);

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pldxBuffClinderField);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStart.rot.y, g_aStart.rot.x, g_aStart.rot.z);
		D3DXMatrixMultiply(&g_aStart.mtxWorld, &g_aStart.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aStart.pos.x, g_aStart.pos.y, g_aStart.pos.z);
		D3DXMatrixMultiply(&g_aStart.mtxWorld, &g_aStart.mtxWorld, &mtxTrans);

		//ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//加算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aStart.mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//Zバッファを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureStart);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, START_VERTEX_NUM, 0, START_PRIMITIVE_NUM);

		//Zバッファを無効に戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//設定を元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//-------------------------------------------
//当たり判定処理
//
//D3DXVECTOR3 * pPos → 位置
//D3DXVECTOR3 size	 → 大きさ
//-------------------------------------------
void CollisionStart(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	if (g_aStart.bUse)
	{
		float nDisX = pPos->x - g_aStart.pos.x;							//X軸の差の距離
		float nDisZ = (pPos->z + (size.z / 2.0f)) - g_aStart.pos.z;		//Z軸の差の距離
		float nData = sqrt((nDisX * nDisX) + (nDisZ * nDisZ));			//差の距離

		if (nData >= 40.0f)
		{//円から出たら
			g_aStart.bUse = false;		//使わない
		}
	}
}

//-------------------------------------------
//スタート情報の取得
//-------------------------------------------
Start * GetStart(void)
{
	return &g_aStart;
}