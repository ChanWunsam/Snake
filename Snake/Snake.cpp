// Snake.cpp: 定义控制台应用程序的入口点。

/* -------------------------------  说明  ---------------------------------- */
/* 单文件版贪吃蛇, 只实现了几个主要的功能:
/* 贪吃蛇的移动和吃食增长, 食物位置随机更新, 以及死亡后重新开始
/* 下一版本的贪吃蛇预期添加界面功能和关卡功能
/* ------------------------------------------------------------------------- */


/* 头文件 */
#include "stdafx.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <time.h>
#include <Windows.h>
using namespace std;

/* 宏定义 */
#define Height 30
#define Width 60
#define InitLength 5
#define Infinity 1000
#define Empty 0
#define Bound 1
#define Snake 2
#define Food 3
#define Up 0
#define Down 1
#define Left 2
#define Right 3

/* 全局变量 */
int Canvas[Height][Width];
int PointX[Infinity];
int PointY[Infinity];
bool hasFood;
bool isDeath;
int length;

/* ------------------------------------------------------------------------- */
/* ----------------------------- 函数声明 ----------------------------------- */
/* ------------------------------------------------------------------------- */

void Init();					// 初始化变量
void Show(int fq);				// 用于画面展示,frequen表示每秒更新频率
void Gotoxy(int x, int y);		// 清屏函数，光标移动到原点位置进行重画
void HideCursor();				// Gotoxy 中用于清除光标的函数
void ProduceFood();				// 随机产生食物位置
void Move();					// 控制贪吃蛇的移动,进行二维数组的逻辑变换
bool IsEat();					// 在 Move 中判断是否接触食物
bool IsDeath();					// 在 Move 中判断是否死亡
void Operate();					// 负责响应贪吃蛇的方向操作
void TurnTo(int orien);			// 在 Operate 中进行方向改变的逻辑变换
void ReStart();					// 重新启动游戏


/* ------------------------------------------------------------------------- */
/* ------------------------------ 主函数 ------------------------------------ */
/* ------------------------------------------------------------------------- */
int main()
{
	int fq = 200; 	// 用于控制更新频率
	Init();
	while (1)
	{
		if (isDeath == true)
		{
			printf("游戏失败,");
			system("pause");	// 暂停
			if (_getch())		// 按下任意键
			{
				ReStart();
				isDeath = false;
			}
		}
		if (hasFood == false)
		{
			ProduceFood();
			hasFood = true;
		}
		Move();
		Operate();
		Show(fq);
	}
    return 0;
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- 函数定义 ----------------------------------- */
/* ------------------------------------------------------------------------- */

/* 初始化变量 */
void Init()
{
	Canvas[Height][Width] = { 0 };
	PointX[Infinity] = { 0 };
	PointY[Infinity] = { 0 };
	hasFood = false;
	isDeath = false;
	length = InitLength;
	// (px, py)表示从末至首地遍历贪吃蛇的坐标
	int px = length;
	int py = length;
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1)
			{
				Canvas[i][j] = Bound;
			}
			else if (i == Height / 2 && j >= 5 && j < 5 + InitLength)
			{
				Canvas[i][j] = Snake;
				PointX[--px] = i;
				PointY[--py] = j;
			}
			else
			{
				Canvas[i][j] = Empty;
			}
		}
	}
}

/* 进行画面展示, fq表示每秒更新频率 */
void Show(int fq)
{
	Gotoxy(0, 0);
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if (Canvas[i][j] == Empty)
			{
				printf(" ");
			}
			else if (Canvas[i][j] == Bound)
			{
				printf("#");
			}
			else if (Canvas[i][j] == Snake)
			{
				printf("O");
			}
			else if (Canvas[i][j] == Food)
			{
				printf("$");
			}
		}
		printf("\n");
	}
	Sleep(fq);
}

/* 清屏函数，光标移动到原点位置进行重画 */
void Gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
	HideCursor();
}

/* Gotoxy 中用于清除光标的函数 */
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	//隐藏输入符号
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

/* 随机产生食物位置 */
void ProduceFood()
{
	int px = 0;
	int py = 0;
	while (Canvas[px][py] != Empty)
	{
		px = rand() % Height;
		py = rand() % Width;
	}
	Canvas[px][py] = Food;
}

/* 控制贪吃蛇的移动,进行二维数组的逻辑变换 */
void Move()
{
	int distX = PointX[0] - PointX[1];
	int distY = PointY[0] - PointY[1];
	// 坐标值变换, part表示贪吃蛇的身体部位
	for (int part = length - 1; part >= 0; part--)
	{
		if (part != 0)
		{
			if (part == length - 1)	// 原蛇尾部位
			{
				Canvas[PointX[part]][PointY[part]] = Empty; 
			}

			PointX[part] = PointX[part - 1];
			PointY[part] = PointY[part - 1];
		}
		else						// 原蛇头部位
		{
			PointX[0] += distX;
			PointY[0] += distY;
			// 判断是否正常移动, 若不是改变相应状态值
			if (IsDeath())
			{
				isDeath = true;
			}
			else if (IsEat())
			{
				hasFood = false;
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
		}
	}
}

/* 在 Move 中判断是否接触食物 */
bool IsEat()
{
	/* 接触食物后蛇身要增长 */
	if (Canvas[PointX[0]][PointY[0]] == Food)
	{
		int distX = PointX[length - 2] - PointX[length - 1];
		int distY = PointY[length - 2] - PointY[length - 1];
		length++;
		PointX[length - 1] = PointX[length - 2] - distX;
		PointY[length - 1] = PointY[length - 2] - distY;
		Canvas[PointX[length - 1]][PointY[length - 1]] = Snake;
		return true;
	}
	return false;
}

/* 在 Move 中判断是否死亡 */
bool IsDeath()
{
	if (Canvas[PointX[0]][PointY[0]] == Snake || Canvas[PointX[0]][PointY[0]] == Bound)
	{
		return true;
	}
	return false;
}

/* 负责响应贪吃蛇的方向操作 */
void Operate()
{
	// _kbhit() 判断是否有外界输入
	if (_kbhit())
	{
		char ch = _getch();
		switch (ch)
		{
		case 'w':
			TurnTo(Up);
			break;
		case 's':
			TurnTo(Down);
			break;
		case 'a':
			TurnTo(Left);
			break;
		case 'd':
			TurnTo(Right);
			break;
		}
	}
}

/* 在 Operate 中进行方向改变的逻辑变换 */
void TurnTo(int orien)
{
	/* 在变换蛇头坐标时,要先判断目标位置是否有食物 */
	int distX = PointX[0] - PointX[1];
	int distY = PointY[0] - PointY[1];
	// 如果蛇头水平运动
	if (distX == 0)	
	{
		switch (orien)
		{
		case Up:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1] - 1;
			PointY[0] = PointY[1]; 
			if (IsEat())
			{
				hasFood = false;
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		case Down:			
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1] + 1;
			PointY[0] = PointY[1];
			if (IsEat())
			{
				hasFood = false;
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		}
	}
	// 如果蛇头竖直运动
	if (distY == 0)
	{
		switch (orien)
		{
		case Left:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1];
			PointY[0] = PointY[1] - 1;
			if (IsEat())
			{
				hasFood = false;
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		case Right:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1];
			PointY[0] = PointY[1] + 1;
			if (IsEat())
			{
				hasFood = false;
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		}
	}
}

/* 重新启动游戏 */
void ReStart()
{
	Init();
}
