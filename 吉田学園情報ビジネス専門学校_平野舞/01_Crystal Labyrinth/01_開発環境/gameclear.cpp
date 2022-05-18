//------------------------------------------ -
//
//ゲームクリア処理[gameclear.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "gameclear.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "controller.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureGameclear = NULL;				//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameclear = NULL;			//頂点バッファへのポインタ
bool g_bClear;												//ゲームクリア判定
bool g_bGameClearFade;										//フェードしているかどうか

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitGameclear(void)
{
	g_bClear = false;			//クリアしていない
	g_bGameClearFade = false;	//フェードしていない

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result001.jpg",
		&g_pTextureGameclear);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameclear,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameclear->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffGameclear->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitGameclear(void)
{
	//サウンドの停止
	StopSound();

	if (g_pTextureGameclear != NULL)
	{//テクスチャの破棄
		g_pTextureGameclear->Release();
		g_pTextureGameclear = NULL;
	}

	if (g_pVtxBuffGameclear != NULL)
	{//頂点バッファの破棄
		g_pVtxBuffGameclear->Release();
		g_pVtxBuffGameclear = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateGameclear(void)
{
	//決定キー(ENTERキー)が押されたかどうか
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bGameClearFade 
		|| GetControllerPressTrigger(0, XINPUT_GAMEPAD_A) && !g_bGameClearFade)
	{
			PlaySound(SOUND_LABEL_SE000);
			SetFade(MODE_RESULT);			//ランキング画面に移行
			g_bClear = true;				//ゲームクリア
			g_bGameClearFade = false;		//カウントを1にする
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawGameclear(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameclear, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureGameclear);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------
//クリアしたかどうかの情報を獲得する処理
//-------------------------------------------
bool GetClear(void)
{
	return g_bClear;
}