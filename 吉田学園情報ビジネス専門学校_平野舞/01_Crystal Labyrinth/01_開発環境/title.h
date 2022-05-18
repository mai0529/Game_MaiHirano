//-------------------------------------------------
//
//タイトル情報[title.h]
//Author:平野舞
//
//-------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

//インクルードファイル
#include "main.h"

//プロトタイプ宣言
void InitTitle(void);			//初期化処理
void UninitTitle(void);			//終了処理
void UpdateTitle(void);			//更新処理
void DrawTitle(void);			//描画処理
void BlinkTitle(VERTEX_2D *pVtx,int ratio);		//点滅処理

#endif