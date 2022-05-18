//-------------------------------------------------
//
//フェード情報[fade.h]
//Author:平野舞
//
//-------------------------------------------------
#ifndef _FADE_H_
#define _FADE_H_

//インクルードファイル
#include "main.h"

//フィードの状態
typedef enum
{
	FADE_NONE = 0,		//何もしていない状態
	FADE_IN,			//フェードイン状態
	FADE_OUT,			//フェードアウト状態
	FADE_MAX
}FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);		//初期化処理
void UninitFade(void);				//終了処理
void UpdateFade(void);				//更新処理
void DrawFade(void);				//描画処理
void SetFade(MODE modeNext);		//設定処理
FADE GetFade(void);					//情報の取得処理

#endif