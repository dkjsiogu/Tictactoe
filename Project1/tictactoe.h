#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <graphics.h>
#include "checkerboard.h"
#define START 0
#define PLAYING 1
#define END 2
class tictactoe
{
public:
	tictactoe();
	void play();//开始游戏的函数
private:
	int game;//游戏状态
	IMAGE piece[3];//棋子图片
	IMAGE back;//背景
	int delay;//延迟时间
	int side;//下棋到某一方
	SmallCheckerboard PBoard;//存储下步棋盘各种状态
	checkerboard BigBoard[3][3];//大棋盘

	void start();//游戏开始
	void end(int);//游戏结束
	void init();//初始化
	void BoardCheck();//检查棋盘状态
	void mouseEvent(checkerboard arrray[3][3],int& side);//鼠标事件
	void updateWindows(checkerboard a[3][3],int);//更新窗口
};
#endif
