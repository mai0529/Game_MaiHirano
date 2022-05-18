//-------------------------------------------
//
//パーティクル情報[particle.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//インクルードファイル
#include "main.h"

//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXCOLOR col;			//カラー
	float fSize;			//サイズ
	float fGravity;			//重力
	int nLife;				//ライフ
	int nType;				//種類
	bool bUse;				//使用しているかどうか
}Particle;

//プロトタイプ宣言
void InitParticle(void);		//初期化処理
void UninitParticle(void);		//終了処理
void UpdateParticle(void);		//更新処理
void DrawParticle(void);		//描画処理
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,int nLife,int nType, float fSize,float fGravity);	//エフェクトの設定処理

#endif