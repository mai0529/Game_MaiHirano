//-------------------------------------------
//
//ステージ処理[stage.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "stage.h"
#include "item.h"
#include "enemy.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureStage = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;		//頂点バッファへのポインタ
Stage g_aStage[MAX_STAGE];							//壁の情報
int g_nCntMapItem;									//アイテム設置数
int g_nCntZero;										//0の数を取得

//外部ファイル用
int g_nStageDate[MAX_STAGEZ][MAX_STAGEX];			//迷路マップを読み込む用
char g_cStage[256];									//その他

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitStage(void)
{
	//外部ファイル読み込み
	LoadStage();

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		//モデル情報の初期設定
		g_aStage[nCount].pos = D3DXVECTOR3(70.0f + (70.0f * nCount), 0.0f, 30.0f);		//位置
		g_aStage[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//向き
		g_aStage[nCount].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//サイズ
		g_aStage[nCount].vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);					//最小頂点値
		g_aStage[nCount].vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);				//最大頂点値
		g_aStage[nCount].bUse = false;													//使用しない
	}
	g_nCntMapItem = 0;		//アイテムカウントを初期化
	g_nCntZero = 0;			//0の数を初期化

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{//Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/wall.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aStage[nCount].pBuffMat,
			NULL,
			&g_aStage[nCount].nNumMat,
			&g_aStage[nCount].pMesh);
	}

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//頂点数の取得
		nNumVtx = g_aStage[nCntStage].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aStage[nCntStage].pMesh->GetFVF());

		//頂点バッファのロック
		g_aStage[nCntStage].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x値
			if (vtx.x < g_aStage[nCntStage].vtxMin.x)
			{//頂点値が小さかったら
				g_aStage[nCntStage].vtxMin.x = vtx.x;		//最小頂点値に代入
			}
			else if (vtx.x > g_aStage[nCntStage].vtxMax.x)
			{//頂点値が大きかったら
				g_aStage[nCntStage].vtxMax.x = vtx.x;		//最大頂点値に代入
			}

			//y値
			if (vtx.y < g_aStage[nCntStage].vtxMin.y)
			{//頂点値が小さかったら
				g_aStage[nCntStage].vtxMin.y = vtx.y;		//最小頂点値に代入
			}
			else if (vtx.y > g_aStage[nCntStage].vtxMax.y)
			{//頂点値が大きかったら
				g_aStage[nCntStage].vtxMax.y = vtx.y;		//最大頂点値に代入
			}

			//z値
			if (vtx.z < g_aStage[nCntStage].vtxMin.z)
			{//頂点値が小さかったら
				g_aStage[nCntStage].vtxMin.z = vtx.z;		//最小頂点値に代入
			}
			else if (vtx.z > g_aStage[nCntStage].vtxMax.z)
			{//頂点値が大きかったら
				g_aStage[nCntStage].vtxMax.z = vtx.z;		//最大頂点値に代入
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		g_aStage[nCntStage].pMesh->UnlockVertexBuffer();
	}

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		g_aStage[nCntStage].size.x = g_aStage[nCntStage].vtxMax.x - g_aStage[nCntStage].vtxMin.x;		//Xのサイズの取得
		g_aStage[nCntStage].size.y = g_aStage[nCntStage].vtxMax.y - g_aStage[nCntStage].vtxMin.y;		//Yのサイズの取得
		g_aStage[nCntStage].size.z = g_aStage[nCntStage].vtxMax.z - g_aStage[nCntStage].vtxMin.z;		//Zのサイズの取得
	}

	//モデルブロックとアイテムの設置
	for (int nCountZ = 0; nCountZ < MAX_STAGEZ; nCountZ++)
	{//Z分回す
		for (int nCountX = 0; nCountX < MAX_STAGEX; nCountX++)
		{//X分回す
			switch (g_nStageDate[nCountZ][nCountX])
			{
			case 0:		//0だった場合
				g_nCntZero++;		//数をカウントする
				if (7 == g_nCntZero || 23 == g_nCntZero || 37 == g_nCntZero || 53 == g_nCntZero)
				{//敵を設置
					SetEnemy(D3DXVECTOR3(300.0f + (75.0f * nCountX), 10.0f, 50.0f + (75.0f * nCountZ)));
				}
				break;
			case 1:		//1だった場合モデルブロックを置く
				SetStage(D3DXVECTOR3(300.0f + (75.0f * nCountX), 0.0f, 50.0f + (75.0f * nCountZ)));
				break;
			default:
				break;
			}
		}
	}
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitStage(void)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//メッシュの破棄
		if (g_aStage[nCntStage].pMesh != NULL)
		{
			g_aStage[nCntStage].pMesh->Release();
			g_aStage[nCntStage].pMesh = NULL;
		}
	}

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		//マテリアルの破棄
		if (g_aStage[nCntStage].pBuffMat != NULL)
		{
			g_aStage[nCntStage].pBuffMat->Release();
			g_aStage[nCntStage].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateStage(void)
{
	//今はなし
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawStage(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL * pMat;

	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (g_aStage[nCntStage].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aStage[nCntStage].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aStage[nCntStage].rot.y, g_aStage[nCntStage].rot.x, g_aStage[nCntStage].rot.z);
			D3DXMatrixMultiply(&g_aStage[nCntStage].mtxWorld, &g_aStage[nCntStage].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aStage[nCntStage].pos.x, g_aStage[nCntStage].pos.y, g_aStage[nCntStage].pos.z);
			D3DXMatrixMultiply(&g_aStage[nCntStage].mtxWorld, &g_aStage[nCntStage].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aStage[nCntStage].mtxWorld);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_aStage[nCntStage].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aStage[nCntStage].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_aStage[nCntStage].pMesh->DrawSubset(nCntMat);
			}

			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//設定処理
//
//D3DXVECTOR3 pos → 位置
//-------------------------------------------
void SetStage(D3DXVECTOR3 pos)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (!g_aStage[nCntStage].bUse)
		{
			g_aStage[nCntStage].pos = pos;			//位置
			g_aStage[nCntStage].bUse = true;		//使用する

			break;
		}
	}
}

//-------------------------------------------
//壁の当たり判定処理
//-------------------------------------------
void CollisionStage(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int nCntStage = 0; nCntStage < MAX_STAGE; nCntStage++)
	{
		if (g_aStage[nCntStage].bUse)
		{
			if ((pPos->y - (size.y / 2.0f)) <= ((g_aStage[nCntStage].pos.y + g_aStage[nCntStage].vtxMax.y) / 2.0) &&	//上
				(pPos->y + (size.y / 2.0f)) >= ((g_aStage[nCntStage].pos.y + g_aStage[nCntStage].vtxMin.y) / 2.0))		//下
			{//y軸の当たり判定
			 //左右の当たり判定
				if ((pPos->z - (size.z / 2.0f)) < (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z) &&		//手前
					(pPos->z + (size.z / 2.0f)) > (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z))		//奥
				{//z範囲の当たり判定
					if ((pPosOld->x + (size.x / 2.0f)) <= (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) &&			//前回の位置
						(pPos->x + (size.x / 2.0f)) > (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x))				//現在の位置
					{//左側の当たり判定
						pPos->x = (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) - (size.x / 2.0f);		//押し戻す
					}
					else if ((pPosOld->x - (size.x / 2.0f)) >= (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x) &&	//前回の位置
						(pPos->x - (size.x / 2.0f)) < (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x))				//現在の位置
					{//右側の当たり判定
						pPos->x = (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x) + (size.x / 2.0f);		//押し戻す
					}
				}

				//手前奥の当たり判定
				if ((pPos->x + (size.x / 2.0f)) > (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMin.x) &&	//左
					(pPos->x - (size.x / 2.0f)) < (g_aStage[nCntStage].pos.x + g_aStage[nCntStage].vtxMax.x))		//右
				{//x軸の当たり判定
					if ((pPosOld->z + (size.z / 2.0f)) <= (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z) &&			//前回の位置
						(pPos->z + (size.z / 2.0f)) > (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z))				//現在の位置
					{//手前の当たり判定
						pPos->z = (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMin.z) - (size.z / 2.0f);		//押し戻す
					}
					else if ((pPosOld->z - (size.z / 2.0f)) >= g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z &&		//前回の位置
						(pPos->z - (size.z / 2.0f)) < (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z))				//現在の位置
					{//奥の当たり判定
						pPos->z = (g_aStage[nCntStage].pos.z + g_aStage[nCntStage].vtxMax.z) + (size.z / 2.0f);		//押し戻す
					}
				}
			}
		}
	}
}

//-------------------------------------------
//外部ファイルの読み込み処理
//-------------------------------------------
void LoadStage(void)
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
				fscanf(pFile, "%1d", &g_nStageDate[nCntZ][nCntX]);		//数字の読み込み
			}
			fscanf(pFile, "%*[^\n]", &g_cStage[0]);						//改行を飛ばす
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}

//-------------------------------------------
//ステージ情報を取得する処理
//-------------------------------------------
Stage * GetStage(void)
{
	return &g_aStage[0];
}