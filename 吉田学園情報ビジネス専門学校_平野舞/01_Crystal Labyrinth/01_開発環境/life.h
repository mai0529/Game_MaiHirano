// ------------------------------------------
//
//ライフ表示情報[life.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _LIFE_H_
#define _LIFE_H_

//インクルードファイル
#include "main.h"

//ライフの構造体　
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nMaxLife;				//最大ライフ数
	int nNowLife;				//今のライフ数
	bool bUse;					//使用するかどうか
}Life;

//プロトタイプ宣言
void InitLife(void);		//初期化処理
void UninitLife(void);		//終了処理
void UpdateLife(void);		//更新処理
void DrawLife(void);		//描画処理
void SetLife(D3DXVECTOR3 pos,int nMax,int nNow);			//設定処理
void SubLife(int nCntLife, int nReduce);					//減算処理
void SetPositionLife(int nIdxLife, D3DXVECTOR3 pos);		//位置更新の処理

#endif