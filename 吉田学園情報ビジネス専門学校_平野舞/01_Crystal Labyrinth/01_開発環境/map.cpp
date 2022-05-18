//-------------------------------------------
//
//マップ処理[map.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "map.h"
#include "maze.h"
#include "enemy.h"
#include "item.h"

//マクロ定義
#define MAX_MAP		(MAX_STAGEX * MAX_STAGEX)				//壁の最大数
#define NUM_MAP		(5)										//最大テクスチャ数
#define MAX_BUFFMAP	(4)										//最大バッファ数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureMap[NUM_MAP] = {};			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMap[MAX_BUFFMAP] = {};	//頂点バッファへのポインタ
Map g_aMap[MAX_MAP];										//マップ情報
PlayerMap g_playerMap;										//プレイヤー情報
EnemyMap g_aEnemyMap[MAX_ENEMY];							//敵情報
ItemMap g_aItemMap[MAX_ITEM];								//アイテム情報
int g_nCntZeroMap;											//0の数を数える初期化

//外部ファイルを読み込む用のグローバル変数宣言
int g_nMapDate[MAX_STAGEZ][MAX_STAGEX];				//位置情報を読み込む用
char g_cMap[256];									//その他用

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitMap(void)
{
	//外部ファイル読み込み
	LoadMap();

	//マップ情報の初期化
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		g_aMap[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期化
		g_aMap[nCntMap].nType = 0;								//位置の初期化
		g_aMap[nCntMap].bUse = false;							//使用しない
	}

	//プレイヤーマップ情報の初期化
	g_playerMap.pos = D3DXVECTOR3(-980.0f, 0.0f, 20.0f);	//位置の初期化
	g_playerMap.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化

	//敵マップ情報の初期化
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		g_aEnemyMap[nCntEnemyMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期化
		g_aEnemyMap[nCntEnemyMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きの初期化
		g_aEnemyMap[nCntEnemyMap].bUse = false;							//使用しない
	}

	//アイテムマップ情報の初期化
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		g_aItemMap[nCntItemMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の初期化
		g_aItemMap[nCntItemMap].bUse = false;							//使用しない
	}

	g_nCntZeroMap = 0;		//カウントの初期化

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み(壁)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wall000.jpg",
		&g_apTextureMap[0]);

	//テクスチャの読み込み(アイテム)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_apTextureMap[1]);

	//テクスチャの読み込み(プレイヤー)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player000.png",
		&g_apTextureMap[2]);

	//テクスチャの読み込み(敵)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureMap[3]);

	//テクスチャの読み込み(ゴール)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/goal000.png",
		&g_apTextureMap[4]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MAP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[0],
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffMap[0]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[0]->Unlock();

	//------------------------Player用-------------------------------------------
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[1],
		NULL);

	//頂点バッファをロック
	g_pVtxBuffMap[1]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
	pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
	pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
	pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  -(75.0f / 2.0f));

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

	//頂点バッファをアンロックする
	g_pVtxBuffMap[1]->Unlock();

	//------------------------Enemy用-------------------------------------------
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[2],
		NULL);

	//頂点バッファをロック
	g_pVtxBuffMap[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_ENEMY; nCntMap++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[2]->Unlock();

	//------------------------Enemy用-------------------------------------------
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap[3],
		NULL);

	//頂点バッファをロック
	g_pVtxBuffMap[3]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMap = 0; nCntMap < MAX_ITEM; nCntMap++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[1].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f,  (75.0f / 2.0f));
		pVtx[2].pos = D3DXVECTOR3(-(75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));
		pVtx[3].pos = D3DXVECTOR3( (75.0f / 2.0f), 0.0f, -(75.0f / 2.0f));

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
	g_pVtxBuffMap[3]->Unlock();

	//マップの設置
	for (int nCountZ = 0; nCountZ < MAX_STAGEZ; nCountZ++)
	{//Z分回す
		for (int nCountX = 0; nCountX < MAX_STAGEX; nCountX++)
		{//X分回す
			switch (g_nMapDate[nCountZ][nCountX])
			{
			case 0:		//0だった場合
				g_nCntZeroMap++;
				if (7 == g_nCntZeroMap || 23 == g_nCntZeroMap || 37 == g_nCntZeroMap || 53 == g_nCntZeroMap)
				{//敵を配置

					SetEnemyMap(D3DXVECTOR3(300.0f + (75.0f * nCountX), 10.0f, 50.0f + (75.0f * nCountZ)));
				}
				break;
			case 1:		//1だった場合ブロックを置く
				SetMap(D3DXVECTOR3(300.0f + (75.0f * nCountX), 0.0f, 50.0f + (75.0f * nCountZ)),0);
				break;
			default:
				break;
			}
		}
	}

	//プレイヤーマップの設置
	g_playerMap.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitMap(void)
{
	for (int nCntMap = 0; nCntMap < NUM_MAP; nCntMap++)
	{//テクスチャの破棄
		if (g_apTextureMap[nCntMap] != NULL)
		{
			g_apTextureMap[nCntMap]->Release();
			g_apTextureMap[nCntMap] = NULL;
		}
	}
	
	for (int nCntMap = 0; nCntMap < MAX_BUFFMAP; nCntMap++)
	{//頂点バッファの破棄
		if (g_pVtxBuffMap[nCntMap] != NULL)
		{
			g_pVtxBuffMap[nCntMap]->Release();
			g_pVtxBuffMap[nCntMap] = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateMap(void)
{
	//敵情報を取得
	Enemy * pEnemy = GetEnemy();

	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++, pEnemy++)
	{
		if (pEnemy->nLife == 0)
		{//もし敵のライフがなくなったら
			g_aEnemyMap[nCntEnemyMap].bUse = false;		//使用しない
		}
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawMap(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		if (g_aMap[nCntMap].bUse)
		{//使用していたら
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMap[nCntMap].mtxWorld);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMap[nCntMap].pos.x, g_aMap[nCntMap].pos.y, g_aMap[nCntMap].pos.z);
			D3DXMatrixMultiply(&g_aMap[nCntMap].mtxWorld, &g_aMap[nCntMap].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMap[nCntMap].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMap[0], 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_apTextureMap[g_aMap[nCntMap].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMap * 4, 2);

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	//----------------------------------Player用--------------------------------------------------------------
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_playerMap.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_playerMap.rot.y, g_playerMap.rot.x, g_playerMap.rot.z);
	D3DXMatrixMultiply(&g_playerMap.mtxWorld, &g_playerMap.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_playerMap.pos.x, g_playerMap.pos.y, g_playerMap.pos.z);
	D3DXMatrixMultiply(&g_playerMap.mtxWorld, &g_playerMap.mtxWorld, &mtxTrans);

	//ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_playerMap.mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMap[1], 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_apTextureMap[2]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//----------------------------------Enemy用--------------------------------------------------------------
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		if (g_aEnemyMap[nCntEnemyMap].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemyMap[nCntEnemyMap].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemyMap[nCntEnemyMap].rot.y, g_aEnemyMap[nCntEnemyMap].rot.x, g_aEnemyMap[nCntEnemyMap].rot.z);
			D3DXMatrixMultiply(&g_aEnemyMap[nCntEnemyMap].mtxWorld, &g_aEnemyMap[nCntEnemyMap].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemyMap[nCntEnemyMap].pos.x, g_aEnemyMap[nCntEnemyMap].pos.y, g_aEnemyMap[nCntEnemyMap].pos.z);
			D3DXMatrixMultiply(&g_aEnemyMap[nCntEnemyMap].mtxWorld, &g_aEnemyMap[nCntEnemyMap].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyMap[nCntEnemyMap].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMap[2], 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャ設定
			pDevice->SetTexture(0, g_apTextureMap[3]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyMap * 4, 2);

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			//----------------------------------Item用--------------------------------------------------------------
			for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
			{
				if (g_aItemMap[nCntItemMap].bUse)
				{
					//ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_aItemMap[nCntItemMap].mtxWorld);

					//位置を反映
					D3DXMatrixTranslation(&mtxTrans, g_aItemMap[nCntItemMap].pos.x, g_aItemMap[nCntItemMap].pos.y, g_aItemMap[nCntItemMap].pos.z);
					D3DXMatrixMultiply(&g_aItemMap[nCntItemMap].mtxWorld, &g_aItemMap[nCntItemMap].mtxWorld, &mtxTrans);

					//ライトを無効にする
					pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

					//ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_aItemMap[nCntItemMap].mtxWorld);

					//頂点バッファをデバイスのデータストリームに設定
					pDevice->SetStreamSource(0, g_pVtxBuffMap[3], 0, sizeof(VERTEX_3D));

					//頂点フォーマットの設定
					pDevice->SetFVF(FVF_VERTEX_3D);

					//テクスチャ設定
					pDevice->SetTexture(0, g_apTextureMap[1]);

					//ポリゴンの描画
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItemMap * 4, 2);

					//ライトを有効に戻す
					pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
				}
			}
		}
	}
}

//-------------------------------------------
//マップの設定処理
//
//D3DXVECTOR3 pos → 位置
//int nType		  → 種類
//-------------------------------------------
void SetMap(D3DXVECTOR3 pos, int nType)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{
		if (!g_aMap[nCntMap].bUse)
		{
			g_aMap[nCntMap].pos = pos;			//位置
			g_aMap[nCntMap].nType = nType;		//種類
			g_aMap[nCntMap].bUse = true;		//使用する

			break;
		}
	}
}

//-------------------------------------------
//敵の設定処理
//
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetEnemyMap(D3DXVECTOR3 pos)
{
	for (int nCntEnemyMap = 0; nCntEnemyMap < MAX_ENEMY; nCntEnemyMap++)
	{
		if (!g_aEnemyMap[nCntEnemyMap].bUse)
		{
			g_aEnemyMap[nCntEnemyMap].pos = pos;		//位置
			g_aEnemyMap[nCntEnemyMap].bUse = true;		//使用する

			break;
		}
	}
}

//-------------------------------------------
//アイテムの設定処理
//
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetItemMap(D3DXVECTOR3 pos)
{
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		if (!g_aItemMap[nCntItemMap].bUse)
		{
			g_aItemMap[nCntItemMap].pos = pos;		//位置
			g_aItemMap[nCntItemMap].bUse = true;	//使用する

			break;
		}
	}
}

//-------------------------------------------
//プレイヤーマップの位置更新処理
//
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetPositionPlayerMap(D3DXVECTOR3 pos)
{
	g_playerMap.pos = pos;		//位置の更新
}

//-------------------------------------------
//敵マップの位置更新処理
//
//int nIdxEnemy	  → 何番目の敵か指定
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetPositionEnemyMap(int nIdxEnemy, D3DXVECTOR3 pos)
{
	g_aEnemyMap[nIdxEnemy].pos = pos;		//位置の更新
}

//-------------------------------------------
//アイテムマップの獲得処理
//
//int nCntItemMap → 何番目のアイテムか指定
//-------------------------------------------	
void UseItemMap(int nCntItemMap)
{
	if (g_aItemMap[nCntItemMap].bUse)
	{//アイテムを使っているとき
		g_aItemMap[nCntItemMap].bUse = false;		//使用しない
	}
}

//-------------------------------------------
//アイテムの当たり判定処理
//-------------------------------------------
void CollisionItemMap(void)
{
	for (int nCntItemMap = 0; nCntItemMap < MAX_ITEM; nCntItemMap++)
	{
		if (g_aItemMap[nCntItemMap].bUse)
		{//アイテムを使っているとき
			if (g_playerMap.pos.x - (60.0f / 2.0f) <= g_aItemMap[nCntItemMap].pos.x + (60.0f / 2.0f)
				&& g_playerMap.pos.x + (60.0f / 2.0f) >= g_aItemMap[nCntItemMap].pos.x - (60.0f / 2.0f)
				&& g_playerMap.pos.z - (60.0f / 2.0f) <= g_aItemMap[nCntItemMap].pos.z + (60.0f / 2.0f)
				&& g_playerMap.pos.z + (60.0f / 2.0f) >= g_aItemMap[nCntItemMap].pos.z - (60.0f / 2.0f))
			{
				g_aItemMap[nCntItemMap].bUse = false;		//使用しない
			}
		}
	}
}

//-------------------------------------------
//外部ファイルを読み込む処理
//-------------------------------------------
void LoadMap(void)
{
	FILE * pFile;		//ファイルポインタ

	//ファイルを開く
	pFile = fopen("data/maze.txt", "r");
	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
		{//Z分回す
			for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
			{//X分回す
				fscanf(pFile, "%1d", &g_nMapDate[nCntZ][nCntX]);		//数字の読み込み
			}
			fscanf(pFile, "%*[^\n]", &g_cMap[0]);						//改行を飛ばす
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}