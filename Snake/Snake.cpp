// Snake.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <string>

using namespace std;

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


int Canvas[Height][Width] = { 0 };
int PointX[Infinity] = { 0 };
int PointY[Infinity] = { 0 };
bool hasFood = false;
bool isDeath = false;
int length = InitLength;


void Gotoxy(int x, int y);
void HideCursor();
void InitMap();
void Show();
void ProduceFood();
void Move();
bool IsEat();
bool IsDeath();
void ReStart();
void Operate();
void TurnTo(int orien);

int main()
{
	char ch = ' ';
	int i = 0;

	// 初始化地图
	InitMap();

	// 随机种子
	srand((unsigned)time(NULL));

	while (1)
	{
		Operate();
		Move();
		Show();
		Sleep(200);
	}

    return 0;
}

void Gotoxy(int x, int y)		//类似于清屏函数，光标移动到原点位置进行重画
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);

	HideCursor();
}

void HideCursor()				//清除光标的函数
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);//隐藏输入符号
}

void Show()
{
	int i, j;

	Gotoxy(0, 0);
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
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
}

void InitMap()
{
	int i, j;
	int x = length;
	int y = length;

	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1)
			{
				Canvas[i][j] = Bound;
			}
			else if (i == Height / 2 && j >= 5 && j < 5 + InitLength)
			{
				Canvas[i][j] = Snake;
				PointX[--x] = i;
				PointY[--y] = j;
			}
			else
			{
				Canvas[i][j] = Empty;
			}
		}
	}
	ProduceFood();
}

// 随机产生食物位置
void ProduceFood()
{
	if (!hasFood)
	{
		int i = 0;
		int j = 0;

		while (Canvas[i][j] != Empty)
		{
			i = rand() % Height;
			j = rand() % Width;
		}
		Canvas[i][j] = Food;
		hasFood = true;
	}
}

void Move()
{
	int i = length - 1;

	int distX = PointX[0] - PointX[1];
	int distY = PointY[0] - PointY[1];

	// 坐标值变换
	for (; i >= 0; i--)
	{
		if (i != 0)
		{
			if (i == length - 1)	// 原蛇尾部位
			{
				Canvas[PointX[i]][PointY[i]] = Empty; 
			}

			PointX[i] = PointX[i - 1];
			PointY[i] = PointY[i - 1];
		}
		else						// 原蛇头部位
		{
			PointX[0] += distX;
			PointY[0] += distY;
			// 判断是否正常移动
			if (IsDeath())
			{
				isDeath = true;
				ReStart();
			}
			else if (IsEat())
			{
				hasFood = false;
				ProduceFood();
			}
			Canvas[PointX[0]][PointY[0]] = Snake;
		}
	}
}

bool IsEat()
{
	int distX = PointX[length - 2] - PointX[length - 1];
	int distY = PointY[length - 2] - PointY[length - 1];
	if (Canvas[PointX[0]][PointY[0]] == Food)
	{
		length++;
		PointX[length - 1] = PointX[length - 2] - distX;
		PointY[length - 1] = PointY[length - 2] - distY;
		Canvas[PointX[length - 1]][PointY[length - 1]] = Snake;
		
		return true;
	}
	return false;
}


bool IsDeath()
{
	if (Canvas[PointX[0]][PointY[0]] == Snake || Canvas[PointX[0]][PointY[0]] == Bound)
	{
		return true;
	}
	return false;
}

void ReStart()
{
	if (isDeath)
	{
		printf("游戏失败,");
		system("pause");
		if (_getch())
		{
			system("cls");
			InitMap();
			isDeath = false;
		}
	}
}

void Operate()
{
	char ch = ' ';
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'w': 
			Move();
			TurnTo(Up);
			break;
		case 's':
			Move();
			TurnTo(Down);
			break;
		case 'a':
			Move();
			TurnTo(Left);
			break;
		case 'd':
			Move();
			TurnTo(Right);
			break;
		case 'p':
			system("pause");
			break;
		}
	}
}

void TurnTo(int orien)
{
	int distX = PointX[0] - PointX[1];
	int distY = PointY[0] - PointY[1];

	if (distX == 0)	// 蛇头水平运动
	{
		switch (orien)
		{
		case Up:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1] - 1;
			PointY[0] = PointY[1]; 
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		case Down:			
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1] + 1;
			PointY[0] = PointY[1];
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		}
	}

	if (distY == 0)	// 蛇头竖直运动
	{
		switch (orien)
		{
		case Left:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1];
			PointY[0] = PointY[1] - 1;
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		case Right:
			Canvas[PointX[0]][PointY[0]] = Empty;
			PointX[0] = PointX[1];
			PointY[0] = PointY[1] + 1;
			Canvas[PointX[0]][PointY[0]] = Snake;
			break;
		}
	}
}
