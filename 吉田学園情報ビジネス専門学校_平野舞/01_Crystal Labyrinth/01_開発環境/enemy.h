// ------------------------------------------ -
//
//敵表示情報[enemy.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define MAX_ENEMY		(128)		//敵の最大数

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常状態
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_DATH,			//死亡状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

//敵の構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posDis;			//距離の差
	D3DXVECTOR3 posOld;			//前回の位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 size;			//サイズ
	D3DXVECTOR3 vtxMin;			//最小頂点値
	D3DXVECTOR3 vtxMax;			//最大頂点値
	LPD3DXMESH pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;		//マテリアル情報へのポインタ
	DWORD nNumMat;				//マテリアル情報の数
	ENEMYSTATE state;			//状態管理
	int nCounter;				//カウント
	int nCntState;				//状態管理カウンター
	int nLife;					//ライフ
	bool bUse;					//使用するかどうか
	bool bDis;					//表示するかどうか
	bool bRot;					//回転するかどうか
	bool bAttack;				
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);				//初期化処理
void UninitEnemy(void);				//終了処理
void UpdateEnemy(void);				//更新処理
void DrawEnemy(void);				//描画処理
void SetEnemy(D3DXVECTOR3 pos);		//設定処理
void StateEnemy(int nCntEnemy);		//状態管理
void BlinkEnemy(int nCntEnemy,int ratio);		//点滅処理
void HitEnemy(int nCntEnemy,int nDamage);		//ヒット処理
void FollowEnemy(int nCntEnemy);	//追従処理
void CollisionEnemy(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 size);		//当たり判定
Enemy * GetEnemy(void);				//情報の取得

#endif
