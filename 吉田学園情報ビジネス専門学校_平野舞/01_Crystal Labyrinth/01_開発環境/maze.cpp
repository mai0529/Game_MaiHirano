//-------------------------------------------
//
//迷路処理[maze.cpp]
//Author:平野舞
//
//-------------------------------------------

//インクルードファイル
#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//マクロ定義
#define PATH			(0)			//穴
#define WALL			(1)			//壁
#define CRYSTAL			(2)			//アイテム
#define MAX_ITEM		(20)		//アイテムの最大数

//掘る方向とアイテム設置場所
typedef enum
{
	UP = 0,		//奥
	DOWN,		//手前
	LEFT,		//左
	RIGHT,		//右
	ITEM,		//アイテム
	MAX
}DIRECTION;

//各マスの情報を格納する配列
static int maze[MAX_STAGEZ][MAX_STAGEX] = {};

//テキストファイルに書き込む用
int g_nMazeData;
char g_cMaze[256];

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitMaze(void)
{
	time_t randam;

	//現在の時刻を取得
	randam = time(NULL);

	//現在の時刻を乱数の種にして乱数の生成系(ベクトル)を変更する
	srand((unsigned int)randam);
}

//-------------------------------------------
//迷路表示処理
//-------------------------------------------
void MakeMaze(void)
{
	FILE * pFile;			//ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen("data/maze.txt", "w");		//外部ファイルに書き込む
	if (pFile != NULL)
	{//ファイルが開けた場合
		for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
		{//Zのマス分回す
			for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
			{//Xのマス分回す
				if (maze[nCntZ][nCntX] == PATH)
				{//穴を掘る
					fprintf(pFile, "0", g_nMazeData);		//外部ファイルに書き込む
				}

				if (maze[nCntZ][nCntX] == WALL)
				{//壁を作る
					fprintf(pFile, "1", g_nMazeData);		//外部ファイルに書き込む
				}

				if (maze[nCntZ][nCntX] == CRYSTAL)
				{//アイテムを設置する
					fprintf(pFile, "2", g_nMazeData);		//外部ファイルに書き込む
				}
			}
			fprintf(pFile, "\n", g_nMazeData);			//外部ファイルに書き込む
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}

//-------------------------------------------
//穴を掘る処理
//-------------------------------------------
void DigMaze(int X, int Z)
{
	//どの方向を掘ろうとしたか覚えておく変数
	int Up, Down, Left, Right;

	//初期化
	Up = 0;
	Down = 0;
	Left = 0;
	Right = 0;
	
	//全方向試すまでループする
	while (Up == 0 || Down == 0 || Left == 0 || Right == 0)
	{
		//0～5までをランダムで取得
		int Date = rand() % 5;

		switch (Date)
		{
		case UP:		//奥方向を掘る
			if (Z + 2 >= 0 && Z + 2 < MAX_STAGEZ)
			{
				if (maze[Z + 2][X] == WALL)
				{//もし壁なら
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z + 2][X] = PATH;		//穴を掘る
						maze[Z + 1][X] = PATH;		//穴を掘る
						DigMaze(X, Z + 2);
					}
				}
			}
			Up++;	//Upのカウントを増やす
			break;
		case DOWN:		//手前方向に穴を掘る
			if (Z - 2 >= 0 && Z - 2 < MAX_STAGEZ)
			{
				if (maze[Z - 2][X] == WALL)
				{//もし壁なら
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z - 2][X] = PATH;		//穴を掘る
						maze[Z - 1][X] = PATH;		//穴を掘る
						DigMaze(X, Z - 2);
					}
				}
			}
			Down++;		//Downのカウントを増やす
			break;
		case LEFT:		//左方向に穴を掘る
			if (X - 2 >= 0 && X - 2 < MAX_STAGEX)
			{
				if (maze[Z][X - 2] == WALL)
				{
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z][X - 2] = PATH;		//穴を掘る
						maze[Z][X - 1] = PATH;		//穴を掘る
						DigMaze(X - 2, Z);
					}
				}
			}
			Left++;		//Leftのカウントを増やす
			break;
		case RIGHT:		//右方向に穴を掘る
			if (X + 2 >= 0 && X + 2 < MAX_STAGEX)
			{
				if (maze[Z][X + 2] == WALL)
				{
					if (7 == rand() % 10)
					{
						maze[Z][X] = CRYSTAL;
					}
					else
					{
						maze[Z][X + 2] = PATH;		//穴を掘る
						maze[Z][X + 1] = PATH;		//穴を掘る
						DigMaze(X + 2, Z);
					}
				}
			}
			Right++;	//Rightのカウントを増やす
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------
//迷路生成処理
//-------------------------------------------
void CreateMaze(void)
{
	for (int nCntZ = 0; nCntZ < MAX_STAGEZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MAX_STAGEX; nCntX++)
		{
			maze[nCntZ][nCntX] = WALL;
		}
	}

	//開始地点をランダムに設定する
	int DataX = 2 * (rand() % (MAX_STAGEX / 2)) + 1;
	int DataZ = 2 * (rand() % (MAX_STAGEZ / 2)) + 1;

	//DataX,DataZを通路に設定する
	maze[DataZ][DataX] = PATH;

	//DataX,DataZを現在地として穴を掘る
	DigMaze(DataX, DataZ);
}