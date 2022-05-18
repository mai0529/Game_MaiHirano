// ------------------------------------------
//
//プレイヤー表示情報[player.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define PLAYER_DIS		(3.5f)		//モデルの移動距離
#define MAX_MODELPARTS	(30)		//最大モデルのパーツ数
#define MAX_KEY			(20)		//最大Key数分
#define MAX_KEYSET		(10)		//最大KeySet数分

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NORMAL = 0,		//通常状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_DATH,			//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//-----------------------------------------------------
//外部ファイル読み込む用
//-----------------------------------------------------
//外部ファイルの構造体
typedef struct
{
	int nIndex;			//インデックス
	int nParent;		//親インデックス
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
}PlayerFile;

//-----------------------------------------------------
//プレイヤーのモデルパーツ
//-----------------------------------------------------
//モデルの構造体
typedef struct
{
	LPD3DXMESH pMesh;				//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;			//マテリアル情報へのポインタ
	DWORD nNumMat;					//マテリアル情報の数
	LPDIRECT3DTEXTURE9 pTexture;	//テクスチャの読み込み
	D3DXMATRIX mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	int nIdxModelParent;			//親モデルのインデックス
}ModelParts;

//-----------------------------------------------------
//プレイヤー
//-----------------------------------------------------
//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 posOld;			//前の位置
	D3DXVECTOR3 rot;			//向き
	D3DXVECTOR3 rotDest;		//目的の角度
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 size;			//サイズ
	D3DXVECTOR3 vtxMin;			//最小頂点値
	D3DXVECTOR3 vtxMax;			//最大頂点値
	ModelParts aModel[MAX_MODELPARTS];		//モデルのパーツ数
	bool bUse;					//使用するかどうか
	PLAYERSTATE state;			//状態管理
	int nCntState;				//状態管理カウンター
	int nLife;					//ライフ
	int nIdxShadow;				//影の番号
}Player;

//-----------------------------------------------------
//モーション
//-----------------------------------------------------
//Key構造体　
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
}Key;

//KeySetの構造体
typedef struct
{
	int nFrame;				//フレーム数
	Key g_Key[MAX_KEY];		//Keyの情報
}KeySet;

//Motion構造体
typedef struct
{
	int nLoop;		//ループ判定
	int nNumKey;	//キー数
	KeySet g_KeySet[MAX_KEYSET];		//KeySetの情報
}MotionSet;


//プロトタイプ宣言
void InitPlayer(void);				//初期化処理
void UninitPlayer(void);			//終了処理
void UpdatePlayer(void);			//更新処理
void DrawPlayer(void);				//描画処理
void MovePlayer(void);				//プレイヤーの移動処理
void MotionPlayer(int nMotion);		//モーション処理
void StatePlayer(void);				//状態処理
void BlinkPlayer(int ratio);		//点滅処理
void HitPlayer(int nDamage);		//ヒット処理
float GetPlayerRot(void);			//プレイヤーの角度を取得
Player * GetPlayer(void);			//プレイヤー情報の取得
void LoadPlayerFile(void);			//プレイヤーモデルの外部ファイル情報の読み込み
void LoadMotionPlayerFile(void);	//プレイヤーモーションの外部ファイル情報の読み込み

#endif