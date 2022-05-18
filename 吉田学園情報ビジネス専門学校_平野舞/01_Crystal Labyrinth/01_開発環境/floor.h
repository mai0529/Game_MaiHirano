//-------------------------------------------
//
//床表示情報[floor.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_

//インクルードファイル
#include "main.h"

//構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fWidth;			//幅
	float fHeigth;			//高さ
	bool bUse;				//使用しているかどうか
}Floor;

//プロトタイプ宣言
void InitFloor(void);			//初期化処理
void UninitFloor(void);			//終了処理
void UpdateFloor(void);			//更新処理
void DrawFloor(void);			//描画処理
void SetFloor(D3DXVECTOR3 pos, float fWidth, float fHeigth);		//設定処理

#endif