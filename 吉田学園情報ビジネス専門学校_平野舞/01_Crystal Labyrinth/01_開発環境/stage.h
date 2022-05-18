//-------------------------------------------
//
//ステージ情報[stage.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _STAGE_H_
#define _STAGE_H_

//インクルードファイル
#include "main.h"
#include "maze.h"

//マクロ定義
#define MAX_STAGE		(MAX_STAGEX * MAX_STAGEX)				//壁の最大数

//ステージの構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 size;			//サイズ
	D3DXVECTOR3 vtxMin;			//最小頂点値
	D3DXVECTOR3 vtxMax;			//最大頂点値
	bool bUse;					//使用しているかどうか
	LPD3DXMESH pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;		//マテリアル情報へのポインタ
	DWORD nNumMat;				//マテリアル情報の数
}Stage;

//プロトタイプ宣言
void InitStage(void);			//初期化処理
void UninitStage(void);			//終了処理
void UpdateStage(void);			//更新処理
void DrawStage(void);			//描画処理
void SetStage(D3DXVECTOR3 pos);	//設定処理
void CollisionStage(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);		//当たり判定処理
void LoadStage(void);			//外部ファイルの読み込み
Stage * GetStage(void);			//情報を取得

#endif

