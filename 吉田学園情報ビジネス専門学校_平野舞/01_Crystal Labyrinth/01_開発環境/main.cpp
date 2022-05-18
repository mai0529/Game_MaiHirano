//-------------------------------------------
//
//3Dゲーム処理[main.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include <windows.h>
#include "main.h"
#include "input.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "gameclear.h"
#include "fade.h"
#include "gameover.h"
#include "ranking.h"
#include "maze.h"
#include "camera.h"
#include "result.h"
#include "sound.h"
#include "controller.h"

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		//Direst3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;					//現在のモード
bool g_MeshField;							//メッシュフィールド
bool g_Billboard;							//ワイヤーフレーム表示するかどうか

//-------------------------------------------
//メイン関数
//-------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウプロシージャ
		0,
		0,
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		//ファイルのアイコン
	};

	HWND hWnd;			//ウインドウハンドル(識別子)
	MSG msg;			//メッセージを格納する変数

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,			//拡張ウインドウスタイル
		CLASS_NAME,						//ウインドウクラスの名前
		WINDOW_NAME,					//ウインドウの名前
		WS_OVERLAPPEDWINDOW,			//ウインドウスタイル
		CW_USEDEFAULT,					//ウインドウの左上X座標
		CW_USEDEFAULT,					//ウインドウの左上Y座標
		(rect.right - rect.left),		//ウインドウの幅
		(rect.bottom - rect.top),		//ウインドウの高さ
		NULL,							//親ウインドウのハンドル
		NULL,							//メニューハンドルまたはウインドウID
		hInstance,						//インスタンスハンドル
		NULL);							//ウインドウ作成データ

	DWORD dwCurrentTime;		//現在時刻
	DWORD dwExecLastTime;		//最後に処理した時刻

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);		//ウインドウの表示状態を設定
	UpdateWindow(hWnd);				//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{//メッセージの設定
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);		//ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();		//現在時刻を取得	
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存

				//更新処理
				Update();

				//描画処理
				Draw();
			}
		}
	}

	//終了処理
	Uninit();

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//-------------------------------------------
//ウインドウプロシージャ
//-------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;		//返り値を格納
	//HDC hDC;

	switch (uMsg)
	{
	case WM_DESTROY:		//ウインドウ破棄のメッセージ
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		//キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:		//ESCキーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_TOPMOST);
			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:		//閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_TOPMOST);
		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//-------------------------------------------
//初期設定
//-------------------------------------------
HRESULT Init(HINSTANCE hInstence, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメーター

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									//パラメーターのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;								//ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;								//ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;								//バックバッファの形式
	d3dpp.BackBufferCount = 1;											//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;							//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;								//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;							//デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;											//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			//インターバル

	//Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{//Direct3Dデバイスの生成(描画処理はハードウェア、頂点はCPU)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{//Direct3Dデバイスの生成(描画処理と頂点処理をCPUで行う)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化設定
	if (FAILED(InitKeyboard(hInstence, hWnd)))
	{
		return E_FAIL;
	}

	//コントローラーの初期化処理
	if (FAILED(InitController()))
	{
		return E_FAIL;
	}

	//迷路の初期化
	InitMaze();

	//リザルトの初期化
	ResetResult();

	//ランキングの初期化
	ResetRanking();

	//サウンドの初期化処理
	InitSound(hWnd);

	//モード設定
	InitFade(g_mode);

	return S_OK;
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void Uninit(void)
{
	//分解能を戻す
	timeEndPeriod(1);

	//サウンドの終了処理
	UninitSound();

	//タイトル画面の終了処理
	UninitTitle();

	//チュートリアル画面の終了処理
	UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//ゲームクリア画面の終了処理
	UninitGameclear();

	//ゲームオーバー画面の終了処理
	UninitGameover();

	//リザルト画面の終了処理
	UninitResult();

	//ランキング画面の終了処理
	UninitRanking();

	//フェード画面の終了処理
	UninitFade();

	//コントローラーの終了処理
	UninitController();

	////キーボードの終了処理
	UninitKeyboard();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//コントローラーの更新処理
	UpdateController();

	switch (g_mode)
	{
	case MODE_TITLE:			//タイトル画面
		UpdateTitle();
		break;
	case MODE_TUTORIAL:			//チュートリアル画面
		UpdateTutorial();
		break;
	case MODE_GAME:				//ゲーム画面
			UpdateGame();
		break;
	case MODE_GAMECLEAR:		//ゲームクリア画面
		UpdateGameclear();
		break;
	case MODE_GAMEOVER:			//ゲームオーバー画面
		UpdateGameover();
		break;
	case MODE_RESULT:			//リザルト画面
		UpdateResult();
		break;
	case MODE_RANKING:			//ランキング画面
		UpdateRanking();
		break;
	}

	//フェードの更新処理
	UpdateFade();
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		switch (g_mode)
		{
		case MODE_TITLE:			//タイトル画面
			DrawTitle();
			break;
		case MODE_TUTORIAL:			//チュートリアル画面
			DrawTutorial();
			break;
		case MODE_GAME:				//ゲーム画面
			DrawGame();
			break;
		case MODE_GAMECLEAR:		//ゲームクリア画面
			DrawGameclear();
			break;
		case MODE_GAMEOVER:			//ゲームオーバー画面
			DrawGameover();
			break;
		case MODE_RESULT:			//リザルト画面
			DrawResult();
			break;
		case MODE_RANKING:			//ランキング画面
			DrawRanking();
			break;
		}

		MakeMaze();
		CreateMaze();

		//フェードの描画処理
		DrawFade();

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//-------------------------------------------
//デバイスの取得
//-------------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//-------------------------------------------
//モードの設定
//-------------------------------------------
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:			//タイトル画面
		UninitTitle();
		break;
	case MODE_TUTORIAL:			//チュートリアル画面
		UninitTutorial();
		break;
	case MODE_GAME:				//ゲーム画面
		UninitGame();	
		break;
	case MODE_GAMECLEAR:		//ゲームクリア画面
		UninitGameclear();
		break;
	case MODE_GAMEOVER:			//ゲームオーバー画面
		UninitGameover();
		break;
	case MODE_RESULT:			//リザルト画面
		UninitResult();
		break;
	case MODE_RANKING:			//ランキング画面
		UninitRanking();
		break;
	}

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_TITLE:			//タイトル画面
		InitTitle();
		break;
	case MODE_TUTORIAL:			//チュートリアル画面
		InitTutorial();
		break;
	case MODE_GAME:				//ゲーム画面
		InitGame(); 
		break;
	case MODE_GAMECLEAR:		//ゲームクリア画面
		InitGameclear();
		break;
	case MODE_GAMEOVER:			//ゲームオーバー画面
		InitGameover();
		break;
	case MODE_RESULT:			//リザルト画面
		InitResult();
		break;
	case MODE_RANKING:			//ランキング画面
		InitRanking();
		break;
	}

	g_mode = mode;		//現在の画面(モード)を切り替える
}

//-------------------------------------------
//モードの取得
//-------------------------------------------
MODE GetMode(void)
{
	return g_mode;
}