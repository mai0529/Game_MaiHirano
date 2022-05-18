//-------------------------------------------
//
//エフェクト情報[effect.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

//インクルードファイル
#include "main.h"

//エフェクトの種類
typedef enum
{
	EFFECT_CIRCLE = 0,		//まる
	EFFECT_SUMMON,			//召喚出来そうなやつ
	EFFECT_RING,			//黄色の輪
	EFFECT_BLUE_RING,		//青のぼわぼわ
	EFFECT_BLUE_CIRCLE,		//青のまる
	EFFECT_TWINKLE,			//キラキラ
	EFFECT_FIREWORK,		//花火
	EFFECT_MAX
}EFFECT;

//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXCOLOR col;			//カラー
	float fSize;			//サイズ
	int nLife;				//ライフ
	int nType;				//種類
	bool bUse;				//使用しているかどうか
}Effect;

//プロトタイプ宣言
void InitEffect(void);			//初期化処理
void UninitEffect(void);		//終了処理
void UpdateEffect(void);		//更新処理
void DrawEffect(void);			//描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife,float fSize,int nType);	//エフェクトの設定処理
void LoadEffectFile(void);		//外部ファイル情報の読み込み

#endif