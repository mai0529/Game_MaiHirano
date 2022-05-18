//-------------------------------------------
//
//迷路情報[maze.h]
//Author:平野舞
//
//-------------------------------------------
#ifndef _MAZE_H_
#define _MAZE_H_

//インクルードファイル
#include "main.h"

//マクロ定義
#define MAX_STAGEX		(13)		//Xサイズ
#define MAX_STAGEZ		(13)		//Zサイズ

void InitMaze(void);			//初期化処理
void MakeMaze(void);			//迷路表示処理
void DigMaze(int X, int Z);		//穴を掘る処理
void CreateMaze(void);			//迷路生成処理

#endif
