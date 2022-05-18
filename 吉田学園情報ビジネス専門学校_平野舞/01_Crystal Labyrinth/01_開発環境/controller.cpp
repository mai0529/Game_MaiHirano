//-------------------------------------------
//
//コントローラーの情報[controller.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "main.h"
#include "controller.h"

//マクロ定義
#define MAX_CONTROLLER		(2)		//コントローラーの最大数

//グローバル宣言
XINPUT_STATE g_JoyKeyState[2];				//ジョイパッドのプレス情報
XINPUT_STATE g_JoyKeyStateTrigger[2];		//ジョイパッドのトリガー情報

//-------------------------------------------
//コントローラーの初期化処理
//-------------------------------------------
HRESULT InitController(void)
{
	for (int nCount = 0; nCount < MAX_CONTROLLER; nCount++)
	{
		//メモリのクリア
		ZeroMemory(&g_JoyKeyState[nCount], sizeof(XINPUT_STATE));

		//メモリのクリア
		ZeroMemory(&g_JoyKeyStateTrigger[nCount], sizeof(XINPUT_STATE));
	}

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//-------------------------------------------
//コントローラーの終了処理
//-------------------------------------------
void UninitController(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//-------------------------------------------
//コントローラーの更新処理
//-------------------------------------------
void UpdateController(void)
{
	for (int nCount = 0; nCount < MAX_CONTROLLER; nCount++)
	{
		//ジョイパッドの入力情報
		XINPUT_STATE JoyKeyState[2];

		//ジョイパッドの情報を取得
		if (XInputGetState(nCount, &JoyKeyState[nCount]) == ERROR_SUCCESS)
		{
			g_JoyKeyStateTrigger[nCount].Gamepad.wButtons = (g_JoyKeyState[nCount].Gamepad.wButtons ^ JoyKeyState[nCount].Gamepad.wButtons) & JoyKeyState[nCount].Gamepad.wButtons;		//ボタンのトリガー情報を保存
			g_JoyKeyState[nCount] = JoyKeyState[nCount];	//ボタンのプレス情報を保存
		}
	}
}

//-------------------------------------------------------------------
//
//コントローラーのボタンプレス情報を取得
//int nPlayer →コントローラーの1Pの場合0を指定、2Pの場合1を指定
//int nKey →ボタンを指定、controller.hにマクロの記載あり
//
//--------------------------------------------------------------------
bool GetControllerPress(int nPlayer,int nKey)
{
	return (g_JoyKeyState[nPlayer].Gamepad.wButtons & nKey) ? true : false;
}

//---------------------------------------------------------------
//
//コントローラーのボタントリガー情報を取得
//int nPlayer →コントローラーの1Pの場合0を指定、2Pの場合1を指定
//int nKey →ボタンを指定、controller.hにマクロの記載あり
//
//--------------------------------------------------------------
bool GetControllerPressTrigger(int nPlayer,int nKey)
{
	return (g_JoyKeyStateTrigger[nPlayer].Gamepad.wButtons & nKey) ? true : false;
}

//-----------------------------------------------------------------
//
//コントローラーのスティックプレス情報を取得
//int nPlayer →コントローラーの1Pの場合0を指定、2Pの場合1を指定
//int nKey →スティックの傾ける向きを指定、
//			 controller.hにenum型で記載あり
//
//-----------------------------------------------------------------
bool GetControllerStickPress(int nPlayer, STICK nKey)
{
	//初期化
	bool Ret = false;	

	//スティックのデッドゾーンの処理
	if ((g_JoyKeyState[nPlayer].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
		&& g_JoyKeyState[nPlayer].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		&& (g_JoyKeyState[nPlayer].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
			&& g_JoyKeyState[nPlayer].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		g_JoyKeyState[nPlayer].Gamepad.sThumbLX = 0;		//値を0にする
		g_JoyKeyState[nPlayer].Gamepad.sThumbLY = 0;		//値を0にする
	}

	if (nKey == STICK_LEFT && g_JoyKeyState[nPlayer].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// スティックの左が押された
		Ret = true;
	}

	if (nKey == STICK_RIGHT && g_JoyKeyState[nPlayer].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// スティックの右が押された
		Ret = true;
	}

	if (nKey == STICK_DOWN && g_JoyKeyState[nPlayer].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// スティックの下が押された
		Ret = true;
	}

	if (nKey == STICK_UP && g_JoyKeyState[nPlayer].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{// スティックの上が押された
		Ret = true;
	}

	return Ret;		//bool型を返す
}

//-----------------------------------------------------------------
//
//コントローラーのスティックの斜め入力を取得
//int nPlayer →コントローラーの1Pの場合0を指定、2Pの場合1を指定
//
//D3DXVECTOR3 □□ = GetControllerStickDir(int nPlayer)　で呼び出し
//g_player.pos.x += □□.x * プレイヤーの速度  で斜め対応化
//g_player.pos.z += □□.y * プレイヤーの速度  で斜め対応化
//
//-----------------------------------------------------------------
D3DXVECTOR3 GetControllerStickDir(int nPlayer)
{
	D3DXVECTOR3 StickAngle(g_JoyKeyState[nPlayer].Gamepad.sThumbLX, g_JoyKeyState[nPlayer].Gamepad.sThumbLY,0.0f);

	//角度を1にする
	D3DXVec3Normalize(&StickAngle,&StickAngle);

	return StickAngle;		//値を返す
}