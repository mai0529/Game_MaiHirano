//-------------------------------------------
//
//マップ情報[map.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _MAP_H_
#define _MAP_H_

//インクルードファイル
#include "main.h"

//ブロックマップの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nType;					//種類
	bool bUse;					//使用しているかどうか
}Map;

//プレイヤーマップの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
}PlayerMap;

//敵マップの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
}EnemyMap;

//アイテムマップの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
}ItemMap;

//プロトタイプ宣言
void InitMap(void);					//初期化処理
void UninitMap(void);				//終了処理
void UpdateMap(void);				//更新処理
void DrawMap(void);					//描画処理
void SetMap(D3DXVECTOR3 pos, int nType);	//設定処理
void SetEnemyMap(D3DXVECTOR3 pos);	//敵の設定処理
void SetItemMap(D3DXVECTOR3 pos);	//アイテムの設定処理
void SetPositionPlayerMap(D3DXVECTOR3 pos);						//プレイヤーマップの位置更新処理
void SetPositionEnemyMap(int nIdxEnemy, D3DXVECTOR3 pos);		//敵マップの位置更新処理
void UseItemMap(int nCntItemMap);	//アイテムの獲得処理
void LoadMap(void);					//外部ファイルの読み込み					
	
#endif