//-------------------------------------------
//
//タイマー情報[time.h]
//Author:平野舞
//
//-------------------------------------------

#ifndef _TIME_H_
#define _TIME_H_

//インクルードファイル
#include "main.h"

//プロトタイプ宣言
void InitTime(void);		//初期化処理
void UninitTime(void);		//終了処理
void UpdateTime(void);		//更新処理
void DrawTime(void);		//描画処理
int GetTime(void);			//タイマーの取得

#endif
