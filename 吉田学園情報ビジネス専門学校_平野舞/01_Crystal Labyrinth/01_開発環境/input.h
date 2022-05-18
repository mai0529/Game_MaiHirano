//-------------------------------------------
//
//キーボード情報[input.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

//インクルードファイル
#include "main.h"

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	//初期化処理
void UninitKeyboard(void);								//終了処理
void UpdateKeyboard(void);								//更新処理
bool GetKeyboardPress(int nKey);						//キーボードのプレス処理
bool GetKeyboardTrigger(int nKey);						//キーボードのトリガー処理

#endif
