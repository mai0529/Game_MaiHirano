//-------------------------------------------
//
//弾情報[bullet.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

//インクルードファイル
#include "main.h"
#include "shadow.h"
#include "player.h"

//マクロ定義
#define MAX_BULLET		(300)		//弾の最大数

//壁の構造体
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	int nIdxShadow;					//影
	int nLife;						//ライフ
	bool bUse;						//使用しているかどうか
}Bullet;

//プロトタイプ宣言
void InitBullet(void);					//初期化処理
void UninitBullet(void);				//終了処理
void UpdateBullet(void);				//更新処理
void DrawBullet(void);					//描画処理
void SetBullet(D3DXVECTOR3 pos);		//弾の設定処理

#endif