#pragma once
#ifndef CHECKERBOARD_H_
#define CHECKERBOARD_H_
#include <graphics.h>
#define VOIDPIECE  0
#define CIRCLE  1
#define  FORK  2
#define ERROR_PLACE  3
#define  FREE  0
#define  USABLE  1

#define BLOCKSIZE 69
struct SmallCheckerboard {//小棋盘结构体
	int state;
	int location_x;
	int location_y;
};

class checkerboard{
public:
	checkerboard();
	SmallCheckerboard board[3][3];//棋盘各格子状态
	bool allow;//棋盘是否可用
	int start_x, start_y;
	bool checksuccess();//检查棋盘胜利;
	int state;//记录棋盘归属

	
private:
	void change(int);//胜利判定后改变某一棋盘的状态
};
#endif // !1
