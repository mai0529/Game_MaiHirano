//-------------------------------------------
//
//コントローラー情報[controller.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

//インクルードファイル
#include "main.h"

//スティックの状態
typedef enum
{
	STICK_LEFT = 0,		//スティックを左に傾けた時
	STICK_RIGHT,		//スティックを右に傾けた時
	STICK_UP,			//スティックを上に傾けた時
	STICK_DOWN,			//スティックを下に傾けた時
	STICK_MAX
}STICK;

//プロトタイプ宣言
HRESULT InitController(void);				//初期化処理
void UninitController(void);				//終了処理
void UpdateController(void);				//更新処理
bool GetControllerPress(int nPlayer,int nKey);			//プレス情報の取得
bool GetControllerPressTrigger(int nPlayer,int nKey);	//トリガー情報の取得
bool GetControllerStickPress(int nPlayer, STICK nKey);	//スティックプレス情報の取得
D3DXVECTOR3 GetControllerStickDir(int nPlayer);			//スティック斜め入力の情報を取得

#endif

//Keyボタンのマクロ----------------------------------------------------------
	// XINPUT_GAMEPAD_DPAD_UP		→ 十字キーの上ボタン
	// XINPUT_GAMEPAD_DPAD_DOWN		→ 十字キーの下ボタン
	// XINPUT_GAMEPAD_DPAD_LEFT		→ 十字キーの左ボタン
	// XINPUT_GAMEPAD_DPAD_RIGHT	→ 十字キーの右ボタン
	// XINPUT_GAMEPAD_A				→ Aボタン
	// XINPUT_GAMEPAD_B				→ Bボタン
	// XINPUT_GAMEPAD_X				→ Xボタン
	// XINPUT_GAMEPAD_Y				→ Yボタン
	// XINPUT_GAMEPAD_START			→ STARTボタン
	// XINPUT_GAMEPAD_BACK			→ BACKボタン