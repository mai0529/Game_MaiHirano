//-------------------------------------------
//
//円柱情報[start.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _START_H_
#define _START_H_

//インクルードファイル
#include "main.h"

//スタートの構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	bool bUse;						//使用しているかどうか
}Start;

//プロトタイプ宣言
void InitStart(void);		//初期化処理
void UninitStart(void);		//終了処理
void UpdateStart(void);		//更新処理
void DrawStart(void);		//描画処理
void CollisionStart(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//当たり判定
Start * GetStart(void);		//情報を取得

#endif
