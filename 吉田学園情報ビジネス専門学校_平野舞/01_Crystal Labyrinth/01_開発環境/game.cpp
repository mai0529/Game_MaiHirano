//-------------------------------------------
//
//ゲーム処理[game.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "game.h"
#include "map.h"
#include "fade.h"
#include "time.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "shadow.h"
#include "bullet.h"
#include "floor.h"
#include "start.h"
#include "effect.h"
#include "particle.h"
#include "item.h"
#include "goal.h"
#include "time.h"
#include "stage.h"
#include "enemy.h"
#include "life.h"
#include "sound.h"

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitGame(void)
{
	//タイムの初期化
	InitTime();

	//影の初期化
	InitShadow();

	//ライフの初期化
	InitLife();

	//マップの初期化
	InitMap();

	//アイテムの初期化
	InitItem();

	//敵の初期化
	InitEnemy();

	//プレイヤーの初期化
	InitPlayer();

	//弾の初期化
	InitBullet();

	//壁の初期化
	InitStage();

	//エフェクトの初期化
	InitEffect();

	//パーティクルの初期化
	InitParticle();

	//床の初期化
	InitFloor();

	//スタートの初期化
	InitStart();

	//ゴールの初期化
	InitGoal();

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();
	
	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM001);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//タイムの終了
	UninitTime();

	//床の終了
	UninitFloor();

	//スタートの終了
	UninitStart();

	//ゴールの終了
	UninitGoal();

	//プレイヤーの終了
	UninitPlayer();

	//弾の終了
	UninitBullet();

	//敵の終了
	UninitEnemy();

	//ライフの終了
	UninitLife();

	//アイテムの終了
	UninitItem();

	//エフェクトの終了
	UninitEffect();

	//パーティクルの終了
	UninitParticle();

	//影の終了
	UninitShadow();

	//マップの終了
	UninitMap();

	//壁の終了
	UninitStage();

	//カメラの終了
	UninitCamera();

	//ライトの終了
	UninitLight();
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateGame(void)
{
	//タイムの更新
	UpdateTime();

	//床の更新
	UpdateFloor();

	//プレイヤーの更新
	UpdatePlayer();

	//弾の更新
	UpdateBullet();

	//敵の更新
	UpdateEnemy();

	//ライフの更新
	UpdateLife();

	//アイテムの更新
	UpdateItem();

	//スタートの更新
	UpdateStart();

	//ゴールの更新
	UpdateGoal();

	//エフェクトの更新
	UpdateEffect();

	//パーティクルの更新
	UpdateParticle();

	//影の更新
	UpdateShadow();

	//壁の更新
	UpdateStage();

	//マップの更新
	UpdateMap();

	//カメラの更新
	UpdateCamera();

	//ライトの更新
	UpdateLight();
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawGame(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューボート
	D3DVIEWPORT9 viewport;

	//ビューボートの設定
	pDevice->GetViewport(&viewport);

	//カメラ情報の取得
	Camera * pCamera = GetCamera();

	for (int nCount = 0; nCount < MAX_CAMERA; nCount++, pCamera++)
	{
		//ビューポートの設定
		pDevice->SetViewport(&pCamera->viewport);

		//画面クリア(バックバッファ&Zバッファのクリア)
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0),
			1.0f,
			0);

		//カメラの設定
		SetCamera(nCount);

		switch (nCount)
		{
		case 0:
			//タイムの描画
			DrawTime();

			//アイテムの描画
			DrawItem();

			//敵の描画
			DrawEnemy();

			//床の描画
			DrawFloor();

			//壁の描画
			DrawStage();

			//プレイヤーの描画
			DrawPlayer();

			//スタートの描画
			DrawStart();

			//ゴールの描画
			DrawGoal();

			//影の描画
			DrawShadow();

			//弾の描画
			DrawBullet();

			//ライフの描画
			DrawLife();

			//エフェクトの描画
			DrawEffect();
			break;

		case 1:
			//床の描画
			DrawFloor();

			//マップの描画
			DrawMap();
			break;

		default:
			break;
		}
	}

	//ビューボートの設定
	pDevice->SetViewport(&viewport);
}