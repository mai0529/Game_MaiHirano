//-------------------------------------------------
//
//ゲームクリア情報[gameclear.h]
//Author:平野舞
//
//-------------------------------------------------
#ifndef _GAMECLEAR_H_
#define _GAMECLEAR_H_

//インクルードファイル
#include "main.h"

//プロトタイプ宣言
void InitGameclear(void);		//初期化処理
void UninitGameclear(void);		//終了処理
void UpdateGameclear(void);		//更新処理
void DrawGameclear(void);		//描画処理
bool GetClear(void);			//クリアしたかどうかの情報を獲得する処理

#endif