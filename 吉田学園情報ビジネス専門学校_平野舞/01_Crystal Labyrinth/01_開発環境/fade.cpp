//-------------------------------------------
//
//フェード処理[fade.cpp](未完成)
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "fade.h"

//マクロ定義
#define FADE_WIDTH		(640.0f)		//幅
#define FADE_HEIGHT		(360.0f)		//高さ

//グローバル変数
//LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
FADE g_fade;										//フェードの状態
MODE g_modeNext;									//次の画面(モード)
D3DXCOLOR g_colorFade;								//ポリゴン(フェード)の色

//-------------------------------------------
//初期化処理
//
//MODE modeNext → 次の画面モードを指定
//-------------------------------------------
void InitFade(MODE modeNext)
{
	g_fade = FADE_IN;									//フェードイン状態に
	g_modeNext = modeNext;								//次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(640.0f - FADE_WIDTH, 360.0f - FADE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + FADE_WIDTH, 360.0f - FADE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - FADE_WIDTH, 360.0f + FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + FADE_WIDTH, 360.0f + FADE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateFade(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{//フェードイン状態
			g_colorFade.a -= 0.05f;				//ポリゴンを透明にしていく
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;				//何もしていない状態に
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFade.a += 0.05f;				//ポリゴンを不透明にしていく
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;				//フェードイン状態に
												//モード設定
				SetMode(g_modeNext);			//画面遷移の設定
			}
		}

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawFade(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------
//フェードの設定
//
//MODE modeNext → 次の画面を指定(main.hにenum型で指定)
//-------------------------------------------
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;									//フェードアウト状態に
	g_modeNext = modeNext;								//次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴン(透明)にしておく
}

//-------------------------------------------
//フェードの取得
//-------------------------------------------
FADE GetFade(void)
{
	return g_fade;
}