//-------------------------------------------
//
//エフェクト処理[effect.cpp]
//Author:平野舞
//
//------------------------------------------- 

//インクルードファイル
#include "effect.h"

//マクロ定義
#define MAX_EFFECT		(4096)			//エフェクトの最大数
#define MAX_TEXEFFECT	(7)				//エフェクト画像の最大数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureEffect[MAX_TEXEFFECT] = {};		//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];						//エフェクトの情報

//外部ファイル用のグローバル変数
char g_cReadEffect[256];				//文字読み込み用
char g_cTexName[30][256];				//画像名読み込み用
int g_nNumEffect;						//画像を読み込む数を読み込む用

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitEffect(void)
{
	//外部ファイル読み込み
	LoadEffectFile();

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//エフェクト情報の初期化
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);	//カラーの初期化
		g_aEffect[nCntEffect].fSize = 0.0f;								//サイズの初期化
		g_aEffect[nCntEffect].nLife = 0;								//ライフの初期化
		g_aEffect[nCntEffect].nType = EFFECT_CIRCLE;					//種類の初期化
		g_aEffect[nCntEffect].bUse = false;								//使用しているかどうか
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			&g_cTexName[nCount][0],
			&g_pTextureEffect[nCount]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(10.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (10.0f / 2.0f),  (10.0f / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(10.0f / 2.0f), -(10.0f / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (10.0f / 2.0f), -(10.0f / 2.0f), 0.0f);

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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitEffect(void)
{
	for (int nCount = 0; nCount < g_nNumEffect; nCount++)
	{//テクスチャの破棄
		if (g_pTextureEffect[nCount] != NULL)
		{
			g_pTextureEffect[nCount]->Release();
			g_pTextureEffect[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateEffect(void)
{
	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//エフェクトが使用されている
			//寿命(表示時間)
			g_aEffect[nCntEffect].nLife -= 1;				//ライフを減らしていく
			if (g_aEffect[nCntEffect].nLife == 0)
			{//ライフが0になったら
				g_aEffect[nCntEffect].bUse = false;		//使用していない状態にする
			}
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse)
		{//使用していたら
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			D3DXMATRIX mtxView;

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//加算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//Zバッファを有効に戻す
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

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
}

//-------------------------------------------
//エフェクトの設定処理
//
//D3DXVECTOR3 pos → 位置
//D3DXCOLOR col	  → 色
//int nLife		  → 寿命
//float fSize	  → 大きさ
//int nType		  → 種類(effect.hにenum型で指定)
//-------------------------------------------
void SetEffect(D3DXVECTOR3 pos,D3DXCOLOR col, int nLife,float fSize, int nType)
{
	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (!g_aEffect[nCntEffect].bUse)
		{//使用していなかったら
			g_aEffect[nCntEffect].pos = pos;		//位置
			g_aEffect[nCntEffect].col = col;		//カラー
			g_aEffect[nCntEffect].nLife = nLife;	//ライフ
			g_aEffect[nCntEffect].fSize = fSize;	//サイズ
			g_aEffect[nCntEffect].nType = nType;	//サイズ
			g_aEffect[nCntEffect].bUse = true;		//使用する

			//サイズの更新
			pVtx[0].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fSize / 2.0f),  (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3( (g_aEffect[nCntEffect].fSize / 2.0f),  (g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(g_aEffect[nCntEffect].fSize / 2.0f), -(g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3( (g_aEffect[nCntEffect].fSize / 2.0f), -(g_aEffect[nCntEffect].fSize / 2.0f), 0.0f);

			//頂点カラーの更新
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			break;
		}
		pVtx += 4;		//データ情報を進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}

//-------------------------------------------
//外部ファイル読み込み処理
//-------------------------------------------
void LoadEffectFile(void)
{
	FILE * pFile;		//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data/effect.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			fscanf(pFile, "%s", &g_cReadEffect[0]);		//文字列の読み込み

			if (strcmp(&g_cReadEffect[0], "NUM_EFFECT") == 0)
			{//モデルの数読み込み
				fscanf(pFile, "%s", &g_cReadEffect[0]);
				fscanf(pFile, "%d", &g_nNumEffect);		//文字列の読み込み
			}

			if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
			{//モデルファイル読み込み
				for (int nCount = 0; nCount < g_nNumEffect; nCount++)
				{
					while (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") != 0)
					{//モデルファイルネームではない場合
						fscanf(pFile, "%s", &g_cReadEffect[0]);
					}

					if (strcmp(&g_cReadEffect[0], "EFFECT_FILENAME") == 0)
					{//モデルファイルネームだったら
						fscanf(pFile, "%s", &g_cReadEffect[0]);
						fscanf(pFile, "%s", &g_cTexName[nCount][0]);		//文字列の読み込み
					}
				}
			}
			if (strcmp(&g_cReadEffect[0], "END_SCRIPT") == 0)
			{//この文があったら抜ける
				break;
			}
		}
		fclose(pFile);									//ファイルを閉じる
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした");
	}
}
