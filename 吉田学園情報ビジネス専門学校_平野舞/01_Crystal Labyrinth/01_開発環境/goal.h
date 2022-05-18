//-------------------------------------------
//
//ゴール情報[goal.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _GOAL_H_
#define _GOAL_H_

//インクルードファイル
#include "main.h"

//ゴールの構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	bool bUse;						//使用しているかどうか
}Goal;

//プロトタイプ宣言
void InitGoal(void);		//初期化処理
void UninitGoal(void);		//終了処理
void UpdateGoal(void);		//更新処理
void DrawGoal(void);		//描画処理
void SetGoal(D3DXVECTOR3 pos);			//ゴールの設定処理
void CollisionGoal(D3DXVECTOR3 * pPos, D3DXVECTOR3 size);	//ゴールの当たり判定	

#endif
