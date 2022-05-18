//-------------------------------------------
//
//アイテムの処理[item.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "shadow.h"
#include "player.h"
#include "map.h"
#include "sound.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureItem = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
Item g_aItem[MAX_ITEM];								//アイテムの情報
int g_nIdxItemShadow;								//影の番号
int g_nCntItem;										//カウンター
int g_RandmPos;										//位置のランダム
int g_RandmMove;									//移動量のランダム

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//弾情報の初期化
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期化
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
		g_aItem[nCntItem].bUse = false;									//使用しているかどうか
	}
	g_nCntItem = 0;														//カウンターの初期化

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_pTextureItem);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(20.0f / 2.0f),  (30.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (20.0f / 2.0f),  (30.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(20.0f / 2.0f), -(30.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (20.0f / 2.0f), -(30.0f / 2.0f), 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;				//頂点データを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffItem->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitItem(void)
{
	//テクスチャの破棄
	if (g_pTextureItem!= NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem= NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem!= NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem= NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateItem(void)
{
	g_RandmPos = rand() % 10;
	g_RandmMove = rand() % 5;
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawItem(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{//使用していたら
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

			D3DXMATRIX mtxView;

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aItem[nCntItem].mtxWorld._11 = mtxView._11;
			g_aItem[nCntItem].mtxWorld._12 = mtxView._21;
			g_aItem[nCntItem].mtxWorld._13 = mtxView._31;
			g_aItem[nCntItem].mtxWorld._21 = mtxView._12;
			g_aItem[nCntItem].mtxWorld._22 = mtxView._22;
			g_aItem[nCntItem].mtxWorld._23 = mtxView._32;
			g_aItem[nCntItem].mtxWorld._31 = mtxView._13;
			g_aItem[nCntItem].mtxWorld._32 = mtxView._23;
			g_aItem[nCntItem].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureItem);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//-------------------------------------------
//アイテムの設定処理
//
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetItem(D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	//float Model = GetPlayerRot();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_aItem[nCntItem].bUse)
		{//使用していなかったら
			g_aItem[nCntItem].pos = pos;		//位置
			g_aItem[nCntItem].bUse = true;		//使用する
			break;
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//-------------------------------------------
//アイテムの当たり判定
//
//D3DXVECTOR3 * pPos → 位置
//D3DXVECTOR3 size	 → 大きさ
//-------------------------------------------
void CollisionItem(D3DXVECTOR3 * pPos, D3DXVECTOR3 size)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse)
		{//アイテムを使っているとき
			if ((pPos->x - (size.x / 2.0f)) <= g_aItem[nCntItem].pos.x + (14.0f / 2.0f)
				&& (pPos->x + (size.x / 2.0f)) >= g_aItem[nCntItem].pos.x - (14.0f / 2.0f)
				&& (pPos->z - (size.z / 2.0f)) <= g_aItem[nCntItem].pos.z
				&& (pPos->z + (size.z / 2.0f)) >= g_aItem[nCntItem].pos.z)
			{
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f,-3.0f + g_RandmMove), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//エフェクトを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f,-3.0f + g_RandmMove), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z + g_RandmPos), D3DXVECTOR3(0.0f, 0.0f, 3.0f - g_RandmMove), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z - g_RandmPos), D3DXVECTOR3(0.0f, 0.0f, 3.0f - g_RandmMove), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示

				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z + g_RandmPos), D3DXVECTOR3(-3.0f - g_RandmMove, 0.0f, 0.0f),  D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z - g_RandmPos), D3DXVECTOR3(-3.0f + g_RandmMove, 0.0f, 0.0f),  D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(3.0f  - g_RandmMove, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示
				SetParticle(D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_RandmPos, g_aItem[nCntItem].pos.y + 20.0f, g_aItem[nCntItem].pos.z), D3DXVECTOR3(3.0f  + g_RandmMove, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 8, EFFECT_FIREWORK, 15.0f, 0.3f);	//パーティクルを表示

				g_nCntItem++;		//カウンターを1増やす
				g_aItem[nCntItem].bUse = false;		//使用しない
				PlaySound(SOUND_LABEL_SE_ITEM);		//SEを再生

				UseItemMap(nCntItem);		//アイテムマップを使用しない
			}
		}
	}
}

//-------------------------------------------
//アイテムを獲得した数を取得
//-------------------------------------------
int GetCntItem(void)
{
	return g_nCntItem;
}

//-------------------------------------------
//アイテムの情報を取得
//-------------------------------------------
Item * GetItem(void)
{
	return &g_aItem[0];
}