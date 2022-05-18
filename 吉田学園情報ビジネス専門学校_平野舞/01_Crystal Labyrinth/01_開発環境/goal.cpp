//-------------------------------------------
//
//ゴール処理[goal.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "goal.h"
#include "item.h"
#include "particle.h"
#include "effect.h"
#include "stage.h"
#include "fade.h"
#include "map.h"

//マクロ定義
#define GOAL_X_BLOCK			(15)		//X方向のブロック数
#define GOAL_Z_BLOCK			(15)		//X方向のブロック数

#define GOAL_VERTEX_NUM			((GOAL_X_BLOCK + 1) * (GOAL_Z_BLOCK + 1))									//頂点数
#define GOAL_INDEX_NUM			((((GOAL_X_BLOCK + 1) * 2) * GOAL_Z_BLOCK) + (2 * (GOAL_Z_BLOCK - 1)))		//インデックス数
#define GOAL_PRIMITIVE_NUM		((2 * GOAL_X_BLOCK * GOAL_Z_BLOCK) + (4 * (GOAL_Z_BLOCK - 1)))				//ポリゴン数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureGoal = NULL;				//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal = NULL;			//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pldxBuffGoalField;				//インデックスバッファへのポインタ
Goal g_aGoal;											//ゴールの情報
bool g_bAttackGoal;										//ゴールに当たったかどうか

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitGoal(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//初期化
	g_aGoal.pos = D3DXVECTOR3(1570.0f, 0.0f, 1320.0f);	//位置
	g_aGoal.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_aGoal.bUse = false;								//使用しない
	g_bAttackGoal = false;								//ゴールに当たっていない

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/effect000.png",
		&g_pTextureGoal);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GOAL_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * GOAL_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pldxBuffGoalField,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0;

	for (int nCntZ = 0; nCntZ < GOAL_Z_BLOCK + 1; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < GOAL_X_BLOCK + 1; nCntX++, nCount++)
		{//X座標

			g_aGoal.rot.y = ((D3DX_PI * 2.0f) / GOAL_X_BLOCK) * (float) nCntX;			//角度設定

			//角度が範囲から超えた場合(角度の正規化)
			if (g_aGoal.rot.y  < -D3DX_PI)
			{//角度が-3.14よりも小さくなった場合
				g_aGoal.rot.y += D3DX_PI * 2;
			}
			else if (g_aGoal.rot.y  > D3DX_PI)
			{//角度が3.14よりも大きくなった場合
				g_aGoal.rot.y -= D3DX_PI * 2;
			}
			
			//頂点座標の設定
			pVtx[0].pos.x = sinf(g_aGoal.rot.y) * 40.0f;		//X座標
			pVtx[0].pos.y = (100.0f / GOAL_Z_BLOCK) * nCntZ;	//Y座標
			pVtx[0].pos.z = cosf(g_aGoal.rot.y) * 40.0f;		//Z座標

			//各頂点の法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, g_aGoal.rot.y, 0.0f);

			//正規化する
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.9f, 0.2f, 0.1f, 1.0f);

			////テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / GOAL_X_BLOCK) * nCntX , 0.0f - (1.0f / GOAL_Z_BLOCK) * nCntZ);

			pVtx += 1;			//頂点データを1つ分進める
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffGoal->Unlock();

	//インデックスバッファをロック
	WORD * pldx;
	g_pldxBuffGoalField->Lock(0, 0, (void**)&pldx, 0);

	int nData = 0;

	for (int nCntZ = 0; nCntZ <GOAL_Z_BLOCK; nCntZ++)
	{//Z座標
		for (int nCntX = 0; nCntX < GOAL_X_BLOCK + 1; nCntX++, nData++)
		{//X座標
		 //インデックスの設定
			pldx[0] = (GOAL_X_BLOCK + 1) + nData;	//下
			pldx[1] = nData;						//上

			pldx += 2;		//インデックスデータを2つ分進める
		}
		if (nCntZ != (GOAL_Z_BLOCK - 1))
		{//折り返し
			pldx[0] = nData - 1;					//右上
			pldx[1] = nData + (GOAL_X_BLOCK + 1);	//左下

			pldx += 2;			//インデックスデータを2つ分進める
		}
	}

	//インデックスバッファのアンロック
	g_pldxBuffGoalField->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitGoal(void)
{
	//テクスチャの破棄
	if (g_pTextureGoal != NULL)
	{
		g_pTextureGoal->Release();
		g_pTextureGoal = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGoal != NULL)
	{
		g_pVtxBuffGoal->Release();
		g_pVtxBuffGoal = NULL;
	}

	//インデックスバッファの開放
	if (g_pldxBuffGoalField != NULL)
	{
		g_pldxBuffGoalField->Release();
		g_pldxBuffGoalField = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateGoal(void)
{
	//獲得したアイテム数の取得
	int nGoalItem = GetCntItem();

	if (4 == nGoalItem)
	{//アイテムを全部取ったら
		SetGoal(D3DXVECTOR3(1130.0f, 0.0f, 880.0f));		//ゴールの設定
		SetMap(D3DXVECTOR3(1130.0f, 0.0f, 880.0f), 4);

		g_aGoal.rot.y -= 0.05f;	//回転

		//ランダム
		int nRandX, nRandZ;
		nRandX = rand() % 40;		//X軸のランダム
		nRandZ = rand() % 40;		//Z軸のランダム

		//パーティクルを設定
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x + nRandX, 0.1f, g_aGoal.pos.z + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x - nRandX, 0.1f, g_aGoal.pos.z - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x + 10 + nRandX, 0.1f, g_aGoal.pos.z - 10 - nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 5.0f, -1.0f);
		SetParticle(D3DXVECTOR3(g_aGoal.pos.x - 10 - nRandX, 0.1f, g_aGoal.pos.z + 10 + nRandZ), D3DXVECTOR3(0.1f, 0.0f, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.0f, 0.8f), 12, EFFECT_TWINKLE, 4.0f, -1.0f);
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawGoal(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	if (g_aGoal.bUse)
	{//使用していたら
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aGoal.mtxWorld);

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pldxBuffGoalField);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoal.rot.y, g_aGoal.rot.x, g_aGoal.rot.z);
		D3DXMatrixMultiply(&g_aGoal.mtxWorld, &g_aGoal.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aGoal.pos.x, g_aGoal.pos.y, g_aGoal.pos.z);
		D3DXMatrixMultiply(&g_aGoal.mtxWorld, &g_aGoal.mtxWorld, &mtxTrans);

		//ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//加算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aGoal.mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffGoal, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//Zバッファを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureGoal);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, GOAL_VERTEX_NUM, 0, GOAL_PRIMITIVE_NUM);

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
//設定処理
//
//D3DXVECTOR3 pos → 位置の設定
//-------------------------------------------
void SetGoal(D3DXVECTOR3 pos)
{
	if (!g_aGoal.bUse)
	{//使用していなかったら
		g_aGoal.pos = pos;		//位置
		g_aGoal.bUse = true;	//使用する
	}
}

//-------------------------------------------
//当たり判定処理
//
//D3DXVECTOR3 * pPos →	 位置
//D3DXVECTOR3 size	 →	 大きさ
//-------------------------------------------
void CollisionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	if (g_aGoal.bUse)
	{//使用していたら
			float nDisX = pPos->x - g_aGoal.pos.x;							//X軸の差の距離
			float nDisZ = (pPos->z + (size.z / 2.0f)) - g_aGoal.pos.z;		//Z軸の差の距離
			float nData = sqrt((nDisX * nDisX) + (nDisZ * nDisZ));			//差の距離

			if (nData < 30.0f && !g_bAttackGoal)
			{//円に入ったら、円に当たってなかったら
					SetFade(MODE_GAMECLEAR);		//クリア表示
					g_bAttackGoal = true;			//当たった
			}
	}
}