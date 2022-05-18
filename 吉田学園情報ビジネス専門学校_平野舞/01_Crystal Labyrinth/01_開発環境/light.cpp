//-------------------------------------------
//
//ライト処理[light.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "light.h"

//マクロ定義
#define MAX_LIGHT	(3)			//ライトの最大数

//グローバル変数
D3DLIGHT9 g_light[MAX_LIGHT];		//ライト情報

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitLight(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライトの方向ベクトル
	D3DXVECTOR3 vecDir;

	//ライトをクリアにする
	ZeroMemory(&g_light[0], sizeof(g_light));

	float PosX = 0.0f;			//位置X
	float PosY = 0.0f;			//位置Y
	float PosZ = 0.0f;			//位置Z

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		switch (nCntLight)
		{
		case 0:					//1つ目のライト
			PosX = -1.0f;			//位置X
			PosY = -1.0f;			//位置Y
			PosZ =  1.0f;			//位置Z
			break;
		case 1:					//2つ目のライト
			PosX = 1.6f;			//位置X
			PosY = 1.5f;			//位置Y
			PosZ = 2.0f;			//位置Z
			break;
		case 2:					//3つ目のライト
			PosX = 2.0f;			//位置X
			PosY = 2.0f;			//位置Y
			PosZ = 1.6f;			//位置Z
			break;
		default:
			assert(false);
			break;
		}

		//ライトの種類を設定
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		vecDir = D3DXVECTOR3(PosX, PosY, PosZ);

		//正規化する
		D3DXVec3Normalize(&vecDir, &vecDir);
		g_light[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitLight(void)
{
	//特になし
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateLight(void)
{
	//特になし
}