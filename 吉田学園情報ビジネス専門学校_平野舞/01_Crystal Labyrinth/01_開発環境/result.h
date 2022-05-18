//-------------------------------------------
//
//リザルト情報[result.h]
//Author:平野舞
//
//-------------------------------------------

#ifndef _RESULT_H_
#define _RESULT_H_

//インクルードファイル
#include "main.h"

//リザルト構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nTime;			//スコア
}Result;

//プロトタイプ宣言
void InitResult(void);		//初期化処理
void UninitResult(void);	//終了処理
void UpdateResult(void);	//更新処理
void DrawResult(void);		//描画処理
void ResetResult(void);		//リセット処理
void SetResult(void);		//設定処理

#endif