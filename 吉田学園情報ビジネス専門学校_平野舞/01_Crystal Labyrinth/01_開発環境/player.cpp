//-------------------------------------------
//
//プレイヤー表示処理[player.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "player.h"
#include "effect.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "item.h"
#include "start.h"
#include "stage.h"
#include "goal.h"
#include "enemy.h"
#include "fade.h"
#include "life.h"
#include "map.h"
#include "controller.h"

//マクロ定義
#define MAX_MOTION			(10)		//最大Motion数
#define PLAYER_RATIO		(20)		//点滅の割合
#define PLAYERLIFE_HEIGHT	(60.0f)		//ライフゲージの高さ
#define PLAYERSHADOW_HEIGHT	(0.1f)		//影の高さ
#define PLAYER_ROTATION		(0.2f)		//回転を滑らかにする
#define PLAYER_STATE		(100)		//状態のカウンター

//グローバル変数
Player g_player;							//プレイヤー情報
ModelParts g_ModelParts[MAX_MODELPARTS];	//モデルパーツの情報
bool g_PlayerFade;							//フェードしているかどうか

//外部ファイルを読み込む用のグローバル変数宣言
PlayerFile g_PlayerFile[MAX_MODELPARTS];	//外部ファイルの情報
int g_nNumPlayer;							//パーツ数の読み込み用
char g_cPlayerFileName[30][256];			//プレイヤーファイル名の読み込み用
char g_cPlayer[256];						//文字列を読み込む用

//モーション用のグローバル変数宣言
int g_nNowFrame;							//現在のフレーム数
int g_nNowKeySet;							//現在再生中のKeySet番号
int g_nNextKeySet;							//次に再生するKeySet番号
int g_nNowMotion;							//現在再生中のMotion番号
MotionSet g_Motion[MAX_MOTION];				//MotionSetの情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitPlayer(void)
{
	//モーションの初期化
	g_nNowFrame = 0;							//現在のフレーム数の初期化
	g_nNowKeySet = 0;							//現在のキー数の初期化
	g_nNowMotion = 0;							//現在のモーション番号の初期化
	g_nNextKeySet = g_nNowKeySet + 1;			//次に再生するKeySet番号の初期化

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モーション分
		for (int nCntKeySet = 0; nCntKeySet < MAX_KEYSET; nCntKeySet++)
		{//KeySet分
			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{//Key分
				g_Motion[nCntMotion].nLoop = 0;			//ループ判定
				g_Motion[nCntMotion].nNumKey = 0;		//キー数
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].nFrame = 0;		//フレーム数
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
				g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
			}
		}
	}

	//外部ファイル読み込み処理
	LoadPlayerFile();
	LoadMotionPlayerFile();

	//プレイヤー情報の初期化
	g_player.pos = D3DXVECTOR3(375.0f, 0.0f, 125.0f);	//位置
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前位置
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	g_player.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目的の角度
	g_player.size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//サイズ
	g_player.vtxMin = D3DXVECTOR3(500.0f, 500.0f, 500.0f);		//最小頂点値
	g_player.vtxMax = D3DXVECTOR3(-500.0f, -500.0f, -500.0f);	//最大頂点値
	g_player.bUse = true;								//使用する
	g_player.state = PLAYERSTATE_NORMAL;				//状態
	g_player.nCntState = 100;							//状態管理カウンター
	g_player.nLife = 5;									//ライフ
	g_player.nIdxShadow = 0;							//影の番号
	g_PlayerFade = false;								//フェード用の初期化

	//モデルパーツ情報の初期化
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		g_ModelParts[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_ModelParts[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_ModelParts[nCount].nIdxModelParent = g_PlayerFile[nCount].nParent;		//親モデルのインデックス指定
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(&g_cPlayerFileName[nCount][0],	//ファイル名を指定
			D3DXMESH_SYSTEMMEM,								//メッシュの生成
			pDevice,										//インターフェースへのポインタ
			NULL,											//隣接データのバッファーへのポインタ
			&g_ModelParts[nCount].pBuffMat,					//マテリアルのバッファーへのポインタ
			NULL,											//インスタンスのバッファーへのポインタ
			&g_ModelParts[nCount].nNumMat,					//D3DXMATERIALの数のポインタ
			&g_ModelParts[nCount].pMesh);					//インターフェースへのポインタのアドレス
	}

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		//頂点数の取得
		nNumVtx = g_ModelParts[nCount].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_ModelParts[nCount].pMesh->GetFVF());

		//頂点バッファのロック
		g_ModelParts[nCount].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x値
			if (vtx.x < g_player.vtxMin.x)
			{//頂点値が小さかったら
				g_player.vtxMin.x = vtx.x;		//最小頂点値に代入
			}
			else if (vtx.x > g_player.vtxMax.x)
			{//頂点値が大きかったら
				g_player.vtxMax.x = vtx.x;		//最大頂点値に代入
			}

			//y値
			if (vtx.y < g_player.vtxMin.y)
			{//頂点値が小さかったら
				g_player.vtxMin.y = vtx.y;		//最小頂点値に代入
			}
			else if (vtx.y > g_player.vtxMax.y)
			{//頂点値が大きかったら
				g_player.vtxMax.y = vtx.y;		//最大頂点値に代入
			}

			//z値
			if (vtx.z < g_player.vtxMin.z)
			{//頂点値が小さかったら
				g_player.vtxMin.z = vtx.z;		//最小頂点値に代入
			}
			else if (vtx.z > g_player.vtxMax.z)
			{//頂点値が大きかったら
				g_player.vtxMax.z = vtx.z;		//最大頂点値に代入
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファのアンロック
		g_ModelParts[nCount].pMesh->UnlockVertexBuffer();
	}

	//サイズ
	g_player.size.x = g_player.vtxMax.x - g_player.vtxMin.x;	//xのサイズを取得
	g_player.size.y = g_player.vtxMax.y - g_player.vtxMin.y;	//yのサイズを取得
	g_player.size.z = g_player.vtxMax.z - g_player.vtxMin.z;	//zのサイズを取得

	//各パーツの配置
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		g_ModelParts[nCount].pos = g_PlayerFile[nCount].pos;
	}

	//影の設定
	g_player.nIdxShadow = SetShadow(D3DXVECTOR3(g_player.pos.x, 0.1f, g_player.pos.z), g_player.rot, D3DXVECTOR3(25.0f, 0.0f, 25.0f));

	//ライフゲージの設定
	SetLife(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYERLIFE_HEIGHT, g_player.pos.z), 5, g_player.nLife);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitPlayer(void)
{
	//メッシュの破棄
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		if (g_ModelParts[nCount].pMesh != NULL)
		{
			g_ModelParts[nCount].pMesh->Release();
			g_ModelParts[nCount].pMesh = NULL;
		}
	}

	//マテリアルの破棄
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{
		if (g_ModelParts[nCount].pBuffMat != NULL)
		{
			g_ModelParts[nCount].pBuffMat->Release();
			g_ModelParts[nCount].pBuffMat = NULL;
		}
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdatePlayer(void)
{
	//前回の位置を更新
	g_player.posOld = g_player.pos;

	//移動処理
	MovePlayer();

	//弾の発射
	if (GetKeyboardTrigger(DIK_SPACE) || GetControllerPressTrigger(0, XINPUT_GAMEPAD_A))
	{//SPACEキー、コントローラーのAを押したら
		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 20.0f, g_player.pos.z));		//弾の発射
	}

	//距離(向き)
	float fDis;
	fDis = g_player.rotDest.y - g_player.rot.y;

	//距離(距離)
	float fPosDisX, fPosDisY, fPosDisZ;
	fPosDisX = g_player.posOld.x - g_player.pos.x;
	fPosDisY = g_player.posOld.y - g_player.pos.y;
	fPosDisZ = g_player.posOld.z - g_player.pos.z;

	//正規化
	if (fDis > D3DX_PI)
	{//3.14より大きかったら
		fDis -= D3DX_PI * 2.0f;
	}
	else if (fDis < -D3DX_PI)
	{//-3.14より小さかったら
		fDis += D3DX_PI * 2.0f;
	}

	//向きの正規化
	if (g_player.rot.y > D3DX_PI)
	{//3.14より大きかったら
		g_player.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_player.rot.y < -D3DX_PI)
	{//-3.14より小さかったら
		g_player.rot.y += D3DX_PI * 2.0f;
	}

	//モデルの回転を滑らかにする
	g_player.rot.y += fDis * PLAYER_ROTATION;

	//スタートの当たり判定
	CollisionStart(&g_player.pos, g_player.size);

	//ゴールの当たり判定
	CollisionGoal(&g_player.pos, g_player.size);

	//敵の当たり判定
	CollisionEnemy(&g_player.pos, &g_player.posOld, g_player.size);

	//プレイヤーの状態管理
	StatePlayer();

	//アイテムとの当たり判定
	CollisionItem(&g_player.pos, g_player.size);

	//壁の当たり判定
	CollisionStage(&g_player.pos, &g_player.posOld, g_player.size);

	//影の位置更新
	SetPositionShadow(0, D3DXVECTOR3(g_player.pos.x, PLAYERSHADOW_HEIGHT, g_player.pos.z));

	//ライフゲージの位置更新
	SetPositionLife(0, D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYERLIFE_HEIGHT, g_player.pos.z));

	//プレイヤーマップの位置更新
	SetPositionPlayerMap(g_player.pos);

	//モーション処理
	MotionPlayer(0);
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL * pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	if (g_player.bUse)
	{//使用していたら
		//モデルパーツの設定
		for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_ModelParts[nCount].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ModelParts[nCount].rot.y, g_ModelParts[nCount].rot.x, g_ModelParts[nCount].rot.z);
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_ModelParts[nCount].pos.x, g_ModelParts[nCount].pos.y, g_ModelParts[nCount].pos.z);
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxTrans);

			//親モデルのマトリックスとの掛け算
			D3DXMATRIX mtxParent;

			if (g_ModelParts[nCount].nIdxModelParent == -1)
			{//親モデルのインデックスが-1なら
				mtxParent = g_player.mtxWorld;					//プレイヤーのマトリックスを代入
			}
			else
			{//親のモデルのインデックスが-1以外なら
				mtxParent = g_ModelParts[g_ModelParts[nCount].nIdxModelParent].mtxWorld;	//自分の親のマトリックスを代入
			}

			//掛け算
			D3DXMatrixMultiply(&g_ModelParts[nCount].mtxWorld, &g_ModelParts[nCount].mtxWorld, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_ModelParts[nCount].mtxWorld);

			//現在のマテリアルを保持
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL *)g_ModelParts[nCount].pBuffMat->GetBufferPointer();

			//xファイルの描画
			for (int nCntMat = 0; nCntMat < (int)g_ModelParts[nCount].nNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, NULL);

				//モデルパーツの描画
				g_ModelParts[nCount].pMesh->DrawSubset(nCntMat);
			}

			//保持していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//-------------------------------------------
//移動処理
//-------------------------------------------
void MovePlayer(void)
{
	//カメラ情報を取得
	Camera * pCamera = GetCamera();

	//スティックの情報を取得
	D3DXVECTOR3 Stick = GetControllerStickDir(0);

	if (GetKeyboardPress(DIK_A) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_LEFT)
		|| GetControllerStickPress(0, STICK_LEFT))
	{//Aキー、コントローラーの十字左、スティック左を押したら左に進む
		if (GetKeyboardPress(DIK_W))
		{//AWを押したら
			g_player.pos.x += sinf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetKeyboardPress(DIK_S))
		{//ASを押したら
			g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetControllerStickPress(0, STICK_UP) || GetControllerStickPress(0, STICK_DOWN))
		{//コントローラーの斜め
			g_player.pos.x += Stick.x * PLAYER_DIS;
			g_player.pos.z += Stick.y * PLAYER_DIS;
		}
		else
		{//Aを押したら
			g_player.rotDest.y = pCamera->rot.y + (D3DX_PI / 2.0f);		//目的の角度
			g_player.pos.x += sinf(pCamera->rot.y - (D3DX_PI / 2.0f)) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y - (D3DX_PI / 2.0f)) * PLAYER_DIS;
		}
	}
	else if (GetKeyboardPress(DIK_D) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_RIGHT)
		|| GetControllerStickPress(0, STICK_RIGHT))
	{//Dキー、コントローラーの右、スティックの右を押したら右に進む
		if (GetKeyboardPress(DIK_W))
		{//DWを押したら
			g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetKeyboardPress(DIK_S))
		{//DSを押したら
			g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
			g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI / 4.0f) * PLAYER_DIS;
		}
		else if (GetControllerStickPress(0, STICK_UP) || GetControllerStickPress(0, STICK_DOWN))
		{//コントローラーの斜め対応
			g_player.pos.x += Stick.x * PLAYER_DIS;
			g_player.pos.z += Stick.y * PLAYER_DIS;
		}
		else
		{//Dを押したら
			g_player.rotDest.y = pCamera->rot.y - (D3DX_PI / 2.0f);			//目的の角度
			g_player.pos.x += sinf(pCamera->rot.y + (D3DX_PI / 2.0f)) * PLAYER_DIS;
			g_player.pos.z += cosf(pCamera->rot.y + (D3DX_PI / 2.0f)) * PLAYER_DIS;
		}
	}
	else if (GetKeyboardPress(DIK_W) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_UP)
		|| GetControllerStickPress(0, STICK_UP))
	{//Wキー、コントローラーの十字キーの上、スティックの上を押したら奥に進む
		g_player.rotDest.y = pCamera->rot.y + D3DX_PI;					//目的の角度
		g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
		g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
	}
	else if (GetKeyboardPress(DIK_S) || GetControllerPress(0, XINPUT_GAMEPAD_DPAD_DOWN)
		|| GetControllerStickPress(0, STICK_DOWN))
	{//Sキー、コントローラーの下、スティックの下を押したら手前に進む
		g_player.rotDest.y = pCamera->rot.y;								//目的の角度
		g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
		g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI) * PLAYER_DIS;
	}
}

//-------------------------------------------
//
//モーション処理
//
//int nMotion → モーション番号を指定
//-------------------------------------------
void MotionPlayer(int nMotion)
{
	for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
	{//パーツ分回す
		g_ModelParts[nCount].rot += D3DXVECTOR3(((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.x - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.x) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame),
			((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.y - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.y) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame),
			((g_Motion[nMotion].g_KeySet[g_nNextKeySet].g_Key[nCount].rot.z - g_Motion[nMotion].g_KeySet[g_nNowKeySet].g_Key[nCount].rot.z) / g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame));
	}

	if (g_nNextKeySet >= g_Motion[nMotion].nNumKey)
	{//次に再生するKeySetがKeySet数よりも大きくなったら
		g_nNextKeySet = 0;		//0にする
	}

	if (g_Motion[nMotion].g_KeySet[g_nNowKeySet].nFrame <= g_nNowFrame)
	{//現在のフレーム数が設定フレーム数より大きくなったら
		g_nNowFrame = 0;		//現在のフレーム数を0にする
		g_nNowKeySet++;			//現在のKeySetを増やす
		g_nNextKeySet++;		//次に再生するKeySetを増やす
	}

	if (g_nNowKeySet >= g_Motion[nMotion].nNumKey)
	{//今のKeySetがモーションKeySetよりも大きくなったら
		if (g_Motion[g_nNowMotion].nLoop == 1)
		{//1だった場合ループする
			g_nNowFrame = 0;	//現在再生中のフレーム数を0にする
			g_nNowKeySet = 0;	//現在再生中のKeySetを0にする
		}
	}

	g_nNowFrame++;		//現在再生中のフレーム数を増やす
}

//-------------------------------------------
//状態処理
//-------------------------------------------
void StatePlayer(void)
{
	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:		//通常状態
		break;
	case PLAYERSTATE_DAMAGE:		//ダメージ状態
		g_player.nCntState--;		//カウントを減らす
		BlinkPlayer(PLAYER_RATIO);	//点滅処理
		break;
	case PLAYERSTATE_DATH:			//死亡状態
		g_player.bUse = false;
		if (!g_PlayerFade)
		{//フェードしていなかったら
			SetFade(MODE_GAMEOVER);		//ゲームオーバー表示
			g_PlayerFade = true;		//フェードしている
		}
		break;
	default:
		assert(false);
		break;
	}
}

//-------------------------------------------
//点滅処理
//
//int ratio → 点滅の割合
//-------------------------------------------
void BlinkPlayer(int ratio)
{
	if (0 == g_player.nCntState % ratio)
	{
		g_player.bUse = false;					//使用しない
	}
	if (10 == g_player.nCntState % ratio)
	{
		g_player.bUse = true;					//使用する
	}
	if (g_player.nCntState == 0)
	{//カウントが0になったら
		g_player.state = PLAYERSTATE_NORMAL;		//通常状態にする
		g_player.bUse = true;						//使用する
	}
}

//-------------------------------------------
//ヒット処理
//
//int nDamage → ダメージ数
//-------------------------------------------
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		g_player.nLife -= nDamage;		//プレイヤーのライフを減らす
		SubLife(0,nDamage);				//ライフゲージを減らす

		if (g_player.nLife == 0)
		{//ライフが0になったら
			g_player.state = PLAYERSTATE_DATH;		//死亡状態にする
		}
		else
		{
			g_player.nCntState = PLAYER_STATE;		//カウンターの数値を設定
			g_player.state = PLAYERSTATE_DAMAGE;	//ダメージ状態にする
		}
	}
}

//-------------------------------------------
//向きY情報の取得
//-------------------------------------------
float GetPlayerRot(void)
{
	return g_player.rot.y;
}

//-------------------------------------------
//プレイヤー情報の取得
//-------------------------------------------
Player * GetPlayer(void)
{
	return &g_player;
}

//-------------------------------------------
//外部ファイル読み込み処理(モデル)
//-------------------------------------------
void LoadPlayerFile(void)
{
	FILE * pFile;		//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data/character.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			fscanf(pFile, "%s", &g_cPlayer[0]);		//文字列の読み込み

			if (strcmp(&g_cPlayer[0], "NUM_MODEL") == 0)
			{//モデルの数の読み込み
				fscanf(pFile, "%s", &g_cPlayer[0]);
				fscanf(pFile, "%d", &g_nNumPlayer);		//文字列の読み込み
			}

			if (strcmp(&g_cPlayer[0], "MODEL_FILENAME") == 0)
			{//モデルファイル読み込み
				for (int nCount = 0; nCount < g_nNumPlayer; nCount++)
				{
					while (strcmp(&g_cPlayer[0], "MODEL_FILENAME") != 0)
					{//モデルファイルネームではない場合
						fscanf(pFile, "%s", &g_cPlayer[0]);
					}

					if (strcmp(&g_cPlayer[0], "MODEL_FILENAME") == 0)
					{//モデルファイルネームだったら
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%s", &g_cPlayerFileName[nCount][0]);		//文字列の読み込み
					}
				}
			}

			if (strcmp(&g_cPlayer[0], "CHARACTERSET") == 0)
			{//キャラクターセットがあったら
				int nCount = 0;
				do
				{//エンドキャラクターセットになるまで
					fscanf(pFile, "%s", &g_cPlayer[0]);			//文字列の読み込み

					if (strcmp(&g_cPlayer[0], "PARTSSET") == 0)
					{//パーツセットがあったら
						fscanf(pFile, "%s", &g_cPlayer[0]);		//文字列の読み込み

						do
						{//エンドパーツセットになるまで
							if (strcmp(&g_cPlayer[0], "INDEX") == 0)
							{//インデックスの読み込み
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_PlayerFile[nCount].nIndex);		//文字列の読み込み
							}
							else if (strcmp(&g_cPlayer[0], "PARENT") == 0)
							{//親のインデックスの読み込み
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_PlayerFile[nCount].nParent);		//文字列の読み込み
							}
							else if (strcmp(&g_cPlayer[0], "POS") == 0)
							{//モデルの位置の読み込み
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.x);		//文字列の読み込み
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.y);		//文字列の読み込み
								fscanf(pFile, "%f", &g_PlayerFile[nCount].pos.z);		//文字列の読み込み
							}
							else if (strcmp(&g_cPlayer[0], "ROT") == 0)
							{//モデルの向きの読み込み
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.x);		//文字列の読み込み
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.y);		//文字列の読み込み
								fscanf(pFile, "%f", &g_PlayerFile[nCount].rot.z);		//文字列の読み込み
							}
							else
							{
								fscanf(pFile, "%s", &g_cPlayer[0]);
							}
						} while (strcmp(&g_cPlayer[0], "END_PARTSSET") != 0);
					}
					nCount++;		//カウントを1増やす
				} while (strcmp(&g_cPlayer[0], "END_CHARACTERSET") != 0);
			}

			if (strcmp(&g_cPlayer[0], "END_SCRIPT") == 0)
			{//この文があったら抜ける
				break;
			}
		}
		fclose(pFile);									//ファイルを閉じる
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした");
	}
}

//-------------------------------------------
//外部ファイル読み込み処理(モーション)
//-------------------------------------------
void LoadMotionPlayerFile(void)
{
	int nCntMotion = 0;		//モーションの数分
	int nCntKey = 0;		//Key分
	int nCntKeySet = 0;		//KeySet分

	FILE * pFile;		//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data/character.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		while (1)
		{
			fscanf(pFile, "%s", &g_cPlayer[0]);		//文字列の読み込み

			if (strcmp(&g_cPlayer[0], "MOTIONSET") == 0)
			{//もしMOTIONSETの文があったら
				do
				{//END_MOTIONSETが来たら抜ける
					if (strcmp(&g_cPlayer[0], "LOOP") == 0)
					{//ループするかどうか
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%d", &g_Motion[nCntMotion].nLoop);
					}
					else if (strcmp(&g_cPlayer[0], "NUM_KEY") == 0)
					{//指定されたキー数を読み込む
						fscanf(pFile, "%s", &g_cPlayer[0]);
						fscanf(pFile, "%d", &g_Motion[nCntMotion].nNumKey);
					}
					else if (strcmp(&g_cPlayer[0], "KEYSET") == 0)
					{//もしKEYSETの文があったら
						do
						{//END_KEYSETが来たら抜ける
							if (strcmp(&g_cPlayer[0], "FRAME") == 0)
							{//フレーム数を読み込む
								fscanf(pFile, "%s", &g_cPlayer[0]);
								fscanf(pFile, "%d", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].nFrame);
							}
							else if (strcmp(&g_cPlayer[0], "KEY") == 0)
							{//もしKEYの文があったら
								do
								{//END_KEYが来たら抜ける
									if (strcmp(&g_cPlayer[0], "POS") == 0)
									{//位置の読み込む
										fscanf(pFile, "%s", &g_cPlayer[0]);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.x);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.y);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].pos.z);
									}
									else if (strcmp(&g_cPlayer[0], "ROT") == 0)
									{//向きの読み込む
										fscanf(pFile, "%s", &g_cPlayer[0]);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.x);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.y);
										fscanf(pFile, "%f", &g_Motion[nCntMotion].g_KeySet[nCntKeySet].g_Key[nCntKey].rot.z);
									}
									else
									{//それ以外
										fscanf(pFile, "%s", &g_cPlayer[0]);
									}
								} while (strcmp(&g_cPlayer[0], "END_KEY") != 0);
								nCntKey++;		//Keyのカウントを増やす
							}
							else
							{
								fscanf(pFile, "%s", &g_cPlayer[0]);
							}
						} while (strcmp(&g_cPlayer[0], "END_KEYSET") != 0);
						nCntKeySet++;		//KeySetのカウントを増やす
						nCntKey = 0;		//Keyカウントを0に戻す
					}
					else
					{
						fscanf(pFile, "%s", &g_cPlayer[0]);
					}
				} while (strcmp(&g_cPlayer[0], "END_MOTIONSET") != 0);
				nCntMotion++;		//Motionのカウントを増やす
				nCntKeySet = 0;		//KeySetのカウントを0に戻す
			}
			if (strcmp(&g_cPlayer[0], "END_SCRIPT") == 0)
			{//この文があったら抜ける
				break;
			}
		}
		fclose(pFile);					//ファイルを閉じる
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした");
	}
}