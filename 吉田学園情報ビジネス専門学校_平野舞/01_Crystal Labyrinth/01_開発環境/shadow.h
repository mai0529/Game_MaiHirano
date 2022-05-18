//-------------------------------------------
//
//影情報[Shadow.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_

//インクルードファイル
#include "main.h"

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 size;		//サイズ
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているかどうか
}Shadow;

//プロトタイプ宣言
void InitShadow(void);			//初期化処理
void UninitShadow(void);		//終了処理
void UpdateShadow(void);		//更新処理
void DrawShadow(void);			//描画処理
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);		//影の設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);	//影の位置更新処理

#endif