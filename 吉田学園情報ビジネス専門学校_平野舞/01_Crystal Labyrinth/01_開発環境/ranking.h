//-------------------------------------------
//
//ランキング情報[ranking.h]
//Author:平野舞
//
//-------------------------------------------

#ifndef _RANKING_H_
#define _RANKING_H_

//インクルードファイル
#include "main.h"

//ランキング構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nTime;			//時間
}Rank;

//プロトタイプ宣言
void InitRanking(void);		//初期化処理
void UninitRanking(void);	//終了処理
void UpdateRanking(void);	//更新処理
void DrawRanking(void);		//描画処理
void ResetRanking(void);	//ランキングのリセット処理
void SetRanking(void);		//設定処理

#endif