//-------------------------------------------
//
//弾の処理[bullet.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "bullet.h"
#include "effect.h"
#include "enemy.h"
#include "stage.h"
#include "sound.h"

//マクロ定義
#define BULLET_WIDTH	(30.0f)		//幅
#define BULLET_HEIGTH	(30.0f)		//高さ
#define BULLET_DIS		(5.0f)		//弾の速度
#define BULLET_MAXLIFE	(15)		//ライフ数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];						//ビルボードの情報
int g_nIdxBulletShadow;								//影の番号

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//弾情報の初期化
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期化
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量の初期化
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
		g_aBullet[nCntBullet].nLife = 0;									//ライフの初期化
		g_aBullet[nCntBullet].bUse = false;									//使用しているかどうか
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet001.png",
		&g_pTextureBullet);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D * pVtx = NULL;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2.0f),  (BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3( (BULLET_WIDTH / 2.0f),  (BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2.0f), -(BULLET_HEIGTH / 2.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3( (BULLET_WIDTH / 2.0f), -(BULLET_HEIGTH / 2.0f), 0.0f);

		//各頂点の法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;				//頂点データを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet!= NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet= NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet!= NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet= NULL;
	}
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateBullet(void)
{
	Enemy * pEnemy = GetEnemy();		//敵情報の取得
	Stage * pStage = GetStage();		//壁情報の取得

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//使用していたら
			//位置情報の更新
			g_aBullet[nCntBullet].pos.x -= sinf(g_aBullet[nCntBullet].rot.x) * BULLET_DIS;
			g_aBullet[nCntBullet].pos.z -= cosf(g_aBullet[nCntBullet].rot.z) * BULLET_DIS;

			//エフェクトを追加
			SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.7f,0.5f,0.0f,1.0f),20,25.0f, EFFECT_FIREWORK);

			for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse)
				{//敵が使用されている
					if (g_aBullet[nCntBullet].pos.x >= pEnemy->pos.x - (pEnemy->size.x)
						&& g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + (pEnemy->size.x)
						&& g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + (pEnemy->size.y)
						&& g_aBullet[nCntBullet].pos.y >= pEnemy->pos.y - (pEnemy->size.y)
						&& g_aBullet[nCntBullet].pos.z <= pEnemy->pos.z + (pEnemy->size.z)
						&& g_aBullet[nCntBullet].pos.z >= pEnemy->pos.z - (pEnemy->size.z))
					{//敵の当たり判定
						HitEnemy(nCntEnemy, 1);		//ヒット処理
						g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
					}
				}
			}

			g_aBullet[nCntBullet].nLife -= 1;			//ライフを減らしていく
			if (g_aBullet[nCntBullet].nLife == 0)
			{//ライフが0になったら
				g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
			}
		}
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse)
		{//使用していたら
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			D3DXMATRIX mtxView;

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
			g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
			g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
			g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
			g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
			g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
			g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
			g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
			g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//ライトを有効に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//-------------------------------------------
//弾の設定処理
//
//D3DXVECTOR3 pos → 位置の設定
//-------------------------------------------
void SetBullet(D3DXVECTOR3 pos)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	float Model = GetPlayerRot();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_aBullet[nCntBullet].bUse)
		{//使用していなかったら
			g_aBullet[nCntBullet].pos = pos;		//位置

			g_aBullet[nCntBullet].rot.x = Model;				//弾のx軸の向き
			g_aBullet[nCntBullet].rot.z = Model;				//弾のz軸の向き
			g_aBullet[nCntBullet].nLife = BULLET_MAXLIFE;		//ライフの設定

			PlaySound(SOUND_LABEL_SE_SHOT);		//SEの再生

			g_aBullet[nCntBullet].bUse = true;		//使用する
			break;
		}
		pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}