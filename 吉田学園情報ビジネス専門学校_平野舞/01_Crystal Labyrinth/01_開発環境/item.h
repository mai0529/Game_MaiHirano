//-------------------------------------------
//
//アイテム情報[item.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

//インクルードファイル
#include "main.h"
#include "maze.h"

//マクロ定義
#define MAX_ITEM		(128)		//アイテムの最大数

//アイテムの構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	bool bUse;						//使用しているかどうか
}Item;

//プロトタイプ宣言
void InitItem(void);				//初期化処理
void UninitItem(void);				//終了処理
void UpdateItem(void);				//更新処理
void DrawItem(void);				//描画処理
void SetItem(D3DXVECTOR3 pos);		//アイテムの設定処理
void CollisionItem(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//アイテムの当たり判定
int GetCntItem(void);				//カウンターの取得
Item * GetItem(void);				//アイテムの情報を取得

#endif