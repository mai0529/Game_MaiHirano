//-------------------------------------------
//
//カメラ情報[camera.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

//インクルードファイル
#include "main.h"
#include "input.h"
#include "player.h"

//マクロ定義
#define MAX_CAMERA		(2)			//カメラの最大数

//構造体
typedef struct
{
	D3DXVECTOR3 posV;				//視点
	D3DXVECTOR3 posR;				//注視点
	D3DXVECTOR3 posVDest;			//目的の視点
	D3DXVECTOR3 posRDest;			//目的の注視点
	D3DXVECTOR3 vecU;				//上方向ベクトル
	D3DXVECTOR3 rot;				//向き(角度)
	D3DXVECTOR3 rotDest;			//目的の向き(角度)
	float fDistance;				//視点から注視点の距離
	D3DXMATRIX mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス
	int nCounter;					//カウンター
	D3DVIEWPORT9 viewport;			//ビューポート
}Camera;

//プロトタイプ宣言
void InitCamera(void);				//初期化処理
void UninitCamera(void);			//終了処理
void UpdateCamera(void);			//更新処理
void SetCamera(int nCntCamere);		//セット処理
Camera * GetCamera(void);			//ポインタの取得

#endif