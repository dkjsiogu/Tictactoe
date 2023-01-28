#include "tictactoe.h"
#include "checkerboard.h"
#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WinMM.Lib")

tictactoe::tictactoe()//存储只需加载一次的数据
{	
	this->game = START;
	loadimage(&back, _T("resources/back.bmp"));//加载背景
	loadimage(&piece[VOIDPIECE], _T("resources/block0.bmp"));//加载旗子图片
	loadimage(&piece[FORK], _T("resources/block1.bmp"));
	loadimage(&piece[CIRCLE], _T("resources/block2.bmp"));
	this->PBoard.location_x = 0;
	this->PBoard.location_y = 0;
}

void tictactoe::init()
{
	initgraph(920, 710);
	putimage(0, 0, &back);
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);						// 获取当前字体设置
	f.lfHeight = 160;						// 设置字体高度为 160
	_tcscpy_s(f.lfFaceName, _T("微软雅黑"));
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	settextstyle(&f);						// 设置字体样式
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4);
	this->delay = 0;//第一次创建的时候让他一个一个出来
	this->PBoard.state = FREE;//第一次可以选择任意一个棋子
	checkerboard init;
	for (int i = 0; i < 3; i++) {
		for (int f = 0; f < 3; f++) {
			BigBoard[i][f] = init;//初始化棋盘状态
			//初始化大棋盘的起始位置
			BigBoard[i][f].start_x = f * 3 * BLOCKSIZE + f * 2 * 20; BigBoard[i][f].start_y = i * 3 * BLOCKSIZE + i * 2 * 20;
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					BigBoard[i][f].board[j][k].location_x = k * BLOCKSIZE + BigBoard[i][f].start_x;//k=(location_x-start_x)/BLOCKSIZE
					BigBoard[i][f].board[j][k].location_y = j * BLOCKSIZE + BigBoard[i][f].start_y;//j=(location_y-start_y)/BLOCKSIZE
				}
			}
		}
	}
	updateWindows(BigBoard, delay);
}

void tictactoe::play()
{
	init();
	if (game == START) {
		start();
	}
	mciSendString(TEXT("play resources/playing_MP3.mp3 repeat"), nullptr, 0, nullptr);//播放背景音乐
	this->game = PLAYING;
	this->delay = 0;
	int side = CIRCLE;//圆圈先手
	while (this->game!=END) {
		mouseEvent(BigBoard, side);
		BoardCheck();
	}
	return;
}

void tictactoe::start()
{
	settextstyle(180, 0, _T("Consolas"));
	setbkcolor(RGB(100, 255, 100));
	setbkmode(OPAQUE);
	outtextxy(50, 250, _T("开始游戏"));
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 20);
	setlinecolor(RGB(255, 255, 0));
	rectangle(50, 250, 730, 430);
	while (1) {
		ExMessage mouseEvent;
		mouseEvent = getmessage(EX_MOUSE);
		if (mouseEvent.message == WM_LBUTTONDOWN) {
			if (mouseEvent.x >= 50 && mouseEvent.x <= 730 && mouseEvent.y >= 250 && mouseEvent.y <= 430) {
				setlinecolor(RGB(0, 0, 0));
				rectangle(50, 250, 730, 430);
				Sleep(300);
				cleardevice();
				init();
				setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4);
				return;
			}
		}
	}
}

void tictactoe::end(int a)
{
	setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 142, 1));
	updateWindows(BigBoard, 0);
	if (a == FORK) {
		outtextxy(200, 100, _T("X赢了!"));
	}
	else {
		outtextxy(200, 100, _T("O赢了!"));
	}
	mciSendString(TEXT("close resources/playing_MP3.mp3"), 0, 0, 0);
	mciSendString(TEXT("play resources/WIN.mp3 repeat"), 0, 0, 0);
	settextstyle(60, 0, _T("Consolas"));
	settextcolor(RGB(255,255,255));
	setbkcolor(RGB(108, 105, 133));
	setlinecolor(RGB(0, 0, 0));
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 10);
	setbkmode(OPAQUE);
	rectangle(100, 300, 323, 360);
	rectangle(400, 300, 623, 360);
	outtextxy(100, 300, _T("重新开始"));
	outtextxy(400, 300, _T("退出游戏"));
	while (1) {
		ExMessage mouseEvent;
		mouseEvent = getmessage(EX_MOUSE);
		if (mouseEvent.message == WM_LBUTTONDOWN) {
			if (mouseEvent.x >= 100 && mouseEvent.x <= 323 && mouseEvent.y >= 300 && mouseEvent.y <= 360) {
				setlinecolor(RGB(204, 166, 75));
				rectangle(100, 300, 323, 360);
				Sleep(300);
				cleardevice();
				mciSendString(TEXT("close resources/WIN.mp3"), 0, 0, 0);
				play();
			}
			if (mouseEvent.x >= 400 && mouseEvent.x <= 623 && mouseEvent.y >= 300 && mouseEvent.y <= 360) {
				setlinecolor(RGB(204, 166, 75));
				rectangle(400, 300, 623, 360);
				Sleep(300);
				game = END;
				return;
			}
		}
		
	}
}

void tictactoe::BoardCheck()
{
	for (int i=0; i < 3; i++) {//检查小棋盘胜利
		for (int f = 0; f < 3; f++) {
			if (BigBoard[i][f].checksuccess()) {
				updateWindows(BigBoard, 0);
			}
		}
	}
	if ((BigBoard[0][0].state == FORK && BigBoard[1][1].state == FORK && BigBoard[2][2].state == FORK)|| (BigBoard[2][0].state == FORK && BigBoard[1][1].state == FORK && BigBoard[0][2].state == FORK)) {
		end(FORK);
	}
	if ((BigBoard[0][0].state == CIRCLE && BigBoard[1][1].state == CIRCLE && BigBoard[2][2].state == CIRCLE)|| (BigBoard[2][0].state == CIRCLE && BigBoard[1][1].state == CIRCLE && BigBoard[0][2].state == CIRCLE)) {
		end(CIRCLE);
	}
	for (int i = 0; i < 3; i++) {
		int Horizontal[2] = { 0,0 }; int Vertical[2] = { 0,0 };//横竖
		for (int f = 0; f < 3; f++) {
			if (BigBoard[i][f].state == FORK) {
				Horizontal[FORK - 1]++;
			}
			else if (BigBoard[i][f].state == CIRCLE) {
				Horizontal[CIRCLE - 1]++;
			}
			if (BigBoard[f][i].state == FORK) {
				Vertical[FORK - 1]++;
			}
			else if (BigBoard[f][i].state == CIRCLE) {
				Vertical[CIRCLE - 1]++;
			}
		}
		if (Horizontal[CIRCLE - 1] == 3 || Vertical[CIRCLE - 1] == 3) {
			end(CIRCLE);
		}
		if (Horizontal[FORK-1] == 3 || Vertical[FORK-1] == 3) {
			end(FORK);
		}
	}
}

void tictactoe::mouseEvent(checkerboard array[3][3], int& side)
{
	if (side == CIRCLE) {//顺便绘制一下轮到哪方行动
		putimage(770, 230, &piece[CIRCLE]);
	}
	else {
		putimage(770, 230, &piece[FORK]);
	}
	ExMessage mouseEvent;
	mouseEvent = getmessage(EX_MOUSE);
	if (array[this->PBoard.location_y][this->PBoard.location_x].allow == false) {
		this->PBoard.state = FREE;
	}
	switch (mouseEvent.message)
	{
	case WM_LBUTTONDOWN:
		for (int i = 0; i < 3; i++) {
			for (int f = 0; f < 3; f++) {
				for (int k = 0; k < 3; k++) {
					for (int j = 0; j < 3; j++) {//遍历坚持鼠标是否点到方块
						if (mouseEvent.x >= array[i][f].board[k][j].location_x && mouseEvent.x <= array[i][f].board[k][j].location_x + BLOCKSIZE && mouseEvent.y >= array[i][f].board[k][j].location_y && mouseEvent.y <= array[i][f].board[k][j].location_y + BLOCKSIZE) {
							if (
								((array[i][f].board[k][j].state == VOIDPIECE) &&//多条件,判断是不是空棋格,判断是否满足可以自由落子，判断普通旗子是否满足位置需要
									((this->PBoard.state == FREE) || (this->PBoard.state == USABLE && this->PBoard.location_x == f && this->PBoard.location_y == i))) &&
								(array[i][f].allow == true)
								) {

								this->PBoard.state = USABLE;
								this->PBoard.location_x = j; this->PBoard.location_y = k;//将小棋盘的位置与大棋盘的位置相关联
								if (array[this->PBoard.location_y][this->PBoard.location_x].allow == false) {
									this->PBoard.state = FREE;
								}

								setlinecolor(RGB(100, 255, 42));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								Sleep(300);//提示光标
								setlinecolor(RGB(0, 0, 0));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								array[i][f].board[k][j].state = side;
								mciSendString(TEXT("play resources/TrueClick.mp3"), nullptr, 0, nullptr);
								updateWindows(BigBoard, 0);
								if (PBoard.state == USABLE) {
									setlinecolor(RGB(100, 255, 42));//提示下一步区域
									rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
									Sleep(500);
									setlinecolor(RGB(0, 0, 0));
									rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
								}
								else {
									for (int i = 0; i < 3; i++) {
										for (int f = 0; f < 3; f++) {
											if (array[i][f].allow) {
												setlinecolor(RGB(100, 255, 42));//提示下一步区域
												rectangle(array[i][f].start_x, array[i][f].start_y, array[i][f].start_x + BLOCKSIZE * 3, array[i][f].start_y + BLOCKSIZE * 3);
												Sleep(500);
												setlinecolor(RGB(0, 0, 0));
												rectangle(array[i][f].start_x, array[i][f].start_y, array[i][f].start_x + BLOCKSIZE * 3, array[i][f].start_y + BLOCKSIZE * 3);
											}
										}
									}
								}
								switch (side)//下这步的是圆,下一步就是叉
								{
								case CIRCLE:side = FORK; break;
								case FORK:side = CIRCLE; break;
								}
								break;
							}
							else if (this->PBoard.location_x == f && this->PBoard.location_y == i && array[i][f].allow == false) {
								this->PBoard.state = FREE;
							}
							else {
								mciSendString(TEXT("play resources/FalseClick.mp3"), nullptr, 0, nullptr);//播放背景音乐
								setlinecolor(RGB(200, 19, 26));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								Sleep(300);//提示光标
								setlinecolor(RGB(0, 0, 0));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								setlinecolor(RGB(100, 255, 42));//再次提示下一步区域
								rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
								Sleep(500);
								setlinecolor(RGB(0, 0, 0));
								rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
							}
						}

					}
				}
			}
		}

	}
}

void tictactoe::updateWindows(checkerboard array[3][3],int a)//
{
	int time = a;
	for (int i = 0; i < 3; i++) {
		for (int f = 0; f < 3; f++) {
			//int start_x = f * 3 * BLOCKSIZE+f*2*20; int start_y = i * 3 * BLOCKSIZE+i*2*20;
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					switch (array[i][f].board[j][k].state) {//按照顺序遍历数组绘图
					case VOIDPIECE:putimage(array[i][f].board[j][k].location_x, array[i][f].board[j][k].location_y , &piece[VOIDPIECE]); break;
					case CIRCLE:putimage(array[i][f].board[j][k].location_x, array[i][f].board[j][k].location_y, &piece[CIRCLE]); break;
					case FORK:putimage(array[i][f].board[j][k].location_x, array[i][f].board[j][k].location_y, &piece[FORK]); break;
					}
				Sleep(time);
				}
			}
		}
	}
}