//------------------------------------------ -
//
//ゲームオーバー処理[gameover.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "gameover.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "gameclear.h"
#include "controller.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureGameover = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameover = NULL;		//頂点バッファへのポインタ
D3DXVECTOR3 g_posGameover;								//タイトルの位置
bool g_bGameOverFade;									//フェードしているかどうか

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitGameover(void)
{
	bool pClear = GetClear();

	g_posGameover = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//座標の初期化
	pClear = false;										//クリアしていない
	g_bGameOverFade = false;							//フェードしていない

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/result002.jpg",
		&g_pTextureGameover);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameover,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGameover->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffGameover->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM003);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitGameover(void)
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	if (g_pTextureGameover != NULL)
	{
		g_pTextureGameover->Release();
		g_pTextureGameover = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGameover != NULL)
	{
		g_pVtxBuffGameover->Release();
		g_pVtxBuffGameover = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateGameover(void)
{
	//決定キー(ENTERキー)が押されたかどうか
	if (GetKeyboardTrigger(DIK_RETURN) && !g_bGameOverFade
		|| GetControllerPress(0, XINPUT_GAMEPAD_A) && !g_bGameOverFade)
	{
			PlaySound(SOUND_LABEL_SE000);	//SEの再生
			SetFade(MODE_RESULT);			//ランキング画面に移行
			g_bGameOverFade = true;			//フェードしている
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawGameover(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameover, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureGameover);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}