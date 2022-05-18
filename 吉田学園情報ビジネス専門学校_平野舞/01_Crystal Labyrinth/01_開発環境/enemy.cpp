//-------------------------------------------
//
//敵の処理[enemy.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "enemy.h"
#include "player.h"
#include "shadow.h"
#include "stage.h"
#include "life.h"
#include "start.h"
#include "map.h"
#include "item.h"

//マクロ定義
#define ENEMY_STATERATIO		(20)		//点滅させる割合
#define ENEMY_CNTSTATE			(100)		//状態遷移カウンター
#define ENEMY_FOLLOWSTATE		(180)		//追従用のカウンター
#define ENEMYLIFE_HEIGTH		(70.0f)		//ライフゲージの高さ

//グローバル変数
Enemy g_aEnemy[MAX_ENEMY];		//敵の情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitEnemy(void)
{
	//敵情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aEnemy[nCntEnemy].posDis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//距離の差
		g_aEnemy[nCntEnemy].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//サイズ
		g_aEnemy[nCntEnemy].vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);		//最小頂点値
		g_aEnemy[nCntEnemy].vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);	//最大頂点値
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					//状態
		g_aEnemy[nCntEnemy].nCounter = 180;								//カウンター
		g_aEnemy[nCntEnemy].nCntState = 100;							//状態管理カウンター
		g_aEnemy[nCntEnemy].nLife = 5;									//ライフ
		g_aEnemy[nCntEnemy].bUse = false;								//使用しない
		g_aEnemy[nCntEnemy].bDis = false;								//使用しない
		g_aEnemy[nCntEnemy].bRot = true;								//回転する
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX("data/MODEL/enemy.x",		
			D3DXMESH_SYSTEMMEM,						
			pDevice,								
			NULL,									
			&g_aEnemy[nCntEnemy].pBuffMat,			
			NULL,									
			&g_aEnemy[nCntEnemy].nNumMat,			
			&g_aEnemy[nCntEnemy].pMesh);			
	}

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点数の取得
		nNumVtx = g_aEnemy[nCntEnemy].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMesh->GetFVF());

		//頂点バッファのロック
		g_aEnemy[nCntEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x値
			if (vtx.x < g_aEnemy[nCntEnemy].vtxMin.x)
			{//頂点値が小さかったら
				g_aEnemy[nCntEnemy].vtxMin.x = vtx.x;		//最小頂点値に代入
			}
			else if (vtx.x > g_aEnemy[nCntEnemy].vtxMax.x)
			{//頂点値が大きかったら
				g_aEnemy[nCntEnemy].vtxMax.x = vtx.x;		//最大頂点値に代入
			}

			//y値
			if (vtx.y < g_aEnemy[nCntEnemy].vtxMin.y)
			{//頂点値が小さかったら
				g_aEnemy[nCntEnemy].vtxMin.y = vtx.y;		//最小頂点値に代入
			}
			else if (vtx.y > g_aEnemy[nCntEnemy].vtxMax.y)
			{//頂点値が大きかったら
				g_aEnemy[nCntEnemy].vtxMax.y = vtx.y;		//最大頂点値に代入
			}

			//z値
			if (vtx.z < g_aEnemy[nCntEnemy].vtxMin.z)
			{//頂点値が小さかったら
				g_aEnemy[nCntEnemy].vtxMin.z = vtx.z;		//最小頂点値に代入
			}
			else if (vtx.z > g_aEnemy[nCntEnemy].vtxMax.z)
			{//頂点値が大きかったら
				g_aEnemy[nCntEnemy].vtxMax.z = vtx.z;		//最大頂点値に代入
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			g_aEnemy[nCntEnemy].size.x = g_aEnemy[nCntEnemy].vtxMax.x - g_aEnemy[nCntEnemy].vtxMin.x;		//xのサイズを取得
			g_aEnemy[nCntEnemy].size.y = g_aEnemy[nCntEnemy].vtxMax.y - g_aEnemy[nCntEnemy].vtxMin.y;		//yのサイズを取得
			g_aEnemy[nCntEnemy].size.z = g_aEnemy[nCntEnemy].vtxMax.z - g_aEnemy[nCntEnemy].vtxMin.z;		//zのサイズを取得
		}

		//頂点バッファのアンロック
		g_aEnemy[nCntEnemy].pMesh->UnlockVertexBuffer();
	}
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//メッシュの破棄
		if (g_aEnemy[nCntEnemy].pMesh != NULL)
		{
			g_aEnemy[nCntEnemy].pMesh->Release();
			g_aEnemy[nCntEnemy].pMesh = NULL;
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//マテリアルの破棄
		if (g_aEnemy[nCntEnemy].pBuffMat != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMat->Release();
			g_aEnemy[nCntEnemy].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateEnemy(void)
{
	Start * pStart = GetStart();		//ゴールの情報を取得

	if (!pStart->bUse)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			if (g_aEnemy[nCntEnemy].bUse)
			{//使用していたら
				//前回の位置を保存
				g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

				//プレイヤーに追従する
				FollowEnemy(nCntEnemy);

				//壁との当たり判定
				CollisionStage(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, g_aEnemy[nCntEnemy].size);

				//ライフゲージの位置更新
				SetPositionLife(1 + nCntEnemy, D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMYLIFE_HEIGTH, g_aEnemy[nCntEnemy].pos.z));

				//敵マップの位置更新
				SetPositionEnemyMap(nCntEnemy, g_aEnemy[nCntEnemy].pos);

				//状態管理
				StateEnemy(nCntEnemy);
			}
		}
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL * pMat;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse && g_aEnemy[nCntEnemy].bDis)
		{//敵を使用していたら
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_aEnemy[nCntEnemy].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_aEnemy[nCntEnemy].pMesh->DrawSubset(nCntMat);
			}

			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//設定処理
//
//D3DXVECTOR3 pos → 位置の指定
//-------------------------------------------
void SetEnemy(D3DXVECTOR3 pos)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_aEnemy[nCntEnemy].bUse)
		{//敵を使用していなかったら
			g_aEnemy[nCntEnemy].pos = pos;		//位置
			g_aEnemy[nCntEnemy].bUse = true;	//使用する
			g_aEnemy[nCntEnemy].bDis = true;	//使用する

			//ライフゲージの設定
			SetLife(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + ENEMYLIFE_HEIGTH, g_aEnemy[nCntEnemy].pos.z), 5, g_aEnemy[nCntEnemy].nLife);
			break;
		}
	}
}

//-------------------------------------------
//状態管理処理
//
//int nCntEnemy → 何番目の敵か指定
//-------------------------------------------
void StateEnemy(int nCntEnemy)
{
	switch (g_aEnemy[nCntEnemy].state)
	{
	case ENEMYSTATE_NORMAL:		//通常状態
		break;
	case ENEMYSTATE_DAMAGE:		//ダメージ状態
		g_aEnemy[nCntEnemy].nCntState--;					//カウンターを減らす
		//点滅してダメージが当たったように見せる
		BlinkEnemy(nCntEnemy, ENEMY_STATERATIO);
		break;
	case ENEMYSTATE_DATH:		//死亡状態
		g_aEnemy[nCntEnemy].bUse = false;						//使用しない
		break;
	default:
		break;
	}
}

//-------------------------------------------
//状態管理処理
//
//int nCntEnemy → 何番目の敵か指定
//int ratio     → 点滅の割合
//-------------------------------------------
void BlinkEnemy(int nCntEnemy,int ratio)
{
	if (0 == g_aEnemy[nCntEnemy].nCntState % ratio)
	{
		g_aEnemy[nCntEnemy].bDis = true;				//表示する
	}
	if (10 == g_aEnemy[nCntEnemy].nCntState % ratio)
	{
		g_aEnemy[nCntEnemy].bDis = false;				//表示しない
	}
	if (g_aEnemy[nCntEnemy].nCntState == 0)
	{//状態管理カウンターが0になったら
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;		//通常状態にする
		g_aEnemy[nCntEnemy].bDis = true;					//使用する
	}
}

//-------------------------------------------
//ヒット処理
//
//int nCntEnemy → 何番目の敵か指定
//int nDamage	→ ダメージ数
//-------------------------------------------
void HitEnemy(int nCntEnemy, int nDamage)
{
	if (g_aEnemy[nCntEnemy].bUse)
	{//もし使用していたら
  		g_aEnemy[nCntEnemy].nLife -= nDamage;		//敵のライフを減らす
		SubLife(1 + nCntEnemy,nDamage);				//ライフゲージを減らす

		if (g_aEnemy[nCntEnemy].nLife == 0)
		{//ライフが0になったら
			SetItem(g_aEnemy[nCntEnemy].pos);					//アイテムを設置
			SetItemMap(g_aEnemy[nCntEnemy].pos);				//マップにアイテムを表示
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DATH;		//死亡状態にする
		}
		else
		{
			g_aEnemy[nCntEnemy].nCntState = ENEMY_CNTSTATE;		//カウンターを設定する
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		//ダメージ状態にする
		}
	}
}

//-------------------------------------------
//追従処理
//
//int nCntEnemy → 何番目の敵か指定
//-------------------------------------------
void FollowEnemy(int nCntEnemy)
{
	Player * pPlayer = GetPlayer();		//プレイヤーの情報を取得

	g_aEnemy[nCntEnemy].nCounter--;		//カウントを減らしていく

	if (g_aEnemy[nCntEnemy].bRot && 0 == g_aEnemy[nCntEnemy].nCounter)
	{//回転していたら、カウンターが0になったら
	 //一定時間で向きを変える
			g_aEnemy[nCntEnemy].rot.y += D3DX_PI / 2.0f;		//向きを変える
			g_aEnemy[nCntEnemy].nCounter = ENEMY_FOLLOWSTATE;	//カウントを戻す
	}

	//向きの正規化
	if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{//3.14より大きかったら
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//プレイヤーを追尾
	//プレイヤーと敵の距離の差
	g_aEnemy[nCntEnemy].posDis.x = pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x;
	g_aEnemy[nCntEnemy].posDis.z = pPlayer->pos.z - g_aEnemy[nCntEnemy].pos.z;

	float fLength = D3DXVec3Length(&g_aEnemy[nCntEnemy].posDis);

	D3DXVec3Normalize(&g_aEnemy[nCntEnemy].posDis, &g_aEnemy[nCntEnemy].posDis);

	//角度の差
	float fAngle = atan2(g_aEnemy[nCntEnemy].posDis.z, g_aEnemy[nCntEnemy].posDis.x);

	//左右
	if (g_aEnemy[nCntEnemy].rot.y - ((D3DX_PI / 180.0f) * 30.0f) < fAngle - (D3DX_PI / 2.0f) &&
		g_aEnemy[nCntEnemy].rot.y + ((D3DX_PI / 180.0f) * 30.0f) > fAngle - (D3DX_PI / 2.0f))
	{//視界に入ったら
		if (fLength < 150.0f)
		{//範囲内だったら
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].posDis * 1.0f;		//追尾する
			g_aEnemy[nCntEnemy].bRot = false;									//回転しない
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].bRot = true;		//回転する
	}

	//手前奥
	if (g_aEnemy[nCntEnemy].rot.y - ((D3DX_PI / 180.0f) * 30.0f) < fAngle + (D3DX_PI / 2.0f) &&
		g_aEnemy[nCntEnemy].rot.y + ((D3DX_PI / 180.0f) * 30.0f) > fAngle + (D3DX_PI / 2.0f))
	{//視界に入ったら
		if (fLength < 150.0f)
		{//範囲内だったら
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].posDis * 1.0f;		//追尾する
			g_aEnemy[nCntEnemy].bRot = false;									//回転しない
		}
	}
	else
	{
		g_aEnemy[nCntEnemy].bRot = true;		//回転する
	}
}

//-------------------------------------------
//敵の当たり判定
//-------------------------------------------
void CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse)
		{
			if ((pPos->y - (size.y / 2.0f)) <= ((g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].vtxMax.y) / 2.0) &&	//上
				(pPos->y + (size.y / 2.0f)) >= ((g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].vtxMin.y) / 2.0))		//下
			{//y軸の当たり判定
			 //左右の当たり判定
				if ((pPos->z - (size.z / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z) &&	//手前
					(pPos->z + (size.z / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z))		//奥
				{//z範囲の当たり判定
					if ((pPosOld->x + (size.x / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) &&		//前回の位置
						(pPos->x + (size.x / 2.0f)) > (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x))			//現在の位置
					{//左側の当たり判定
						pPos->x = (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) - (size.x / 2.0f);		//押し戻す
						HitPlayer(1);		//ダメージ処理
					}
					else if ((pPosOld->x - (size.x / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x) &&	//前回の位置
						(pPos->x - (size.x / 2.0f)) < (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x))				//現在の位置
					{//右側の当たり判定
						pPos->x = (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x) + (size.x / 2.0f);		//押し戻す
						HitPlayer(1);		//ダメージ処理
					}
				}

				//手前奥の当たり判定
				else if ((pPos->x + (size.x / 2.0f)) >= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMin.x) &&		//左
					(pPos->x - (size.x / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].vtxMax.x))		//右
				{//x軸の当たり判定
					if ((pPosOld->z + (size.z / 2.0f)) <= (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z) &&		//前回の位置
						(pPos->z + (size.z / 2.0f)) > (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z))				//現在の位置
					{//手前の当たり判定
						pPos->z = (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMin.z) - (size.z / 2.0f);		//押し戻す
							HitPlayer(1);		//ダメージ処理
					}
					else if ((pPosOld->z - (size.z / 2.0f)) >= g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z &&	//前回の位置
						(pPos->z - (size.z / 2.0f)) < (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z))				//現在の位置
					{//奥の当たり判定
						pPos->z = (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].vtxMax.z) + (size.z / 2.0f);		//押し戻す
						HitPlayer(1);		//ダメージ処理
					}
				}
			}
		}
	}
}

//-------------------------------------------
//情報の取得処理
//-------------------------------------------
Enemy * GetEnemy(void)
{
	return &g_aEnemy[0];
}