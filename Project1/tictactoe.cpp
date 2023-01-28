#include "tictactoe.h"
#include "checkerboard.h"
#include <graphics.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WinMM.Lib")

tictactoe::tictactoe()//�洢ֻ�����һ�ε�����
{	
	this->game = START;
	loadimage(&back, _T("resources/back.bmp"));//���ر���
	loadimage(&piece[VOIDPIECE], _T("resources/block0.bmp"));//��������ͼƬ
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
	gettextstyle(&f);						// ��ȡ��ǰ��������
	f.lfHeight = 160;						// ��������߶�Ϊ 160
	_tcscpy_s(f.lfFaceName, _T("΢���ź�"));
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
	settextstyle(&f);						// ����������ʽ
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 4);
	this->delay = 0;//��һ�δ�����ʱ������һ��һ������
	this->PBoard.state = FREE;//��һ�ο���ѡ������һ������
	checkerboard init;
	for (int i = 0; i < 3; i++) {
		for (int f = 0; f < 3; f++) {
			BigBoard[i][f] = init;//��ʼ������״̬
			//��ʼ�������̵���ʼλ��
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
	mciSendString(TEXT("play resources/playing_MP3.mp3 repeat"), nullptr, 0, nullptr);//���ű�������
	this->game = PLAYING;
	this->delay = 0;
	int side = CIRCLE;//ԲȦ����
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
	outtextxy(50, 250, _T("��ʼ��Ϸ"));
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
		outtextxy(200, 100, _T("XӮ��!"));
	}
	else {
		outtextxy(200, 100, _T("OӮ��!"));
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
	outtextxy(100, 300, _T("���¿�ʼ"));
	outtextxy(400, 300, _T("�˳���Ϸ"));
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
	for (int i=0; i < 3; i++) {//���С����ʤ��
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
		int Horizontal[2] = { 0,0 }; int Vertical[2] = { 0,0 };//����
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
	if (side == CIRCLE) {//˳�����һ���ֵ��ķ��ж�
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
					for (int j = 0; j < 3; j++) {//�����������Ƿ�㵽����
						if (mouseEvent.x >= array[i][f].board[k][j].location_x && mouseEvent.x <= array[i][f].board[k][j].location_x + BLOCKSIZE && mouseEvent.y >= array[i][f].board[k][j].location_y && mouseEvent.y <= array[i][f].board[k][j].location_y + BLOCKSIZE) {
							if (
								((array[i][f].board[k][j].state == VOIDPIECE) &&//������,�ж��ǲ��ǿ����,�ж��Ƿ���������������ӣ��ж���ͨ�����Ƿ�����λ����Ҫ
									((this->PBoard.state == FREE) || (this->PBoard.state == USABLE && this->PBoard.location_x == f && this->PBoard.location_y == i))) &&
								(array[i][f].allow == true)
								) {

								this->PBoard.state = USABLE;
								this->PBoard.location_x = j; this->PBoard.location_y = k;//��С���̵�λ��������̵�λ�������
								if (array[this->PBoard.location_y][this->PBoard.location_x].allow == false) {
									this->PBoard.state = FREE;
								}

								setlinecolor(RGB(100, 255, 42));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								Sleep(300);//��ʾ���
								setlinecolor(RGB(0, 0, 0));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								array[i][f].board[k][j].state = side;
								mciSendString(TEXT("play resources/TrueClick.mp3"), nullptr, 0, nullptr);
								updateWindows(BigBoard, 0);
								if (PBoard.state == USABLE) {
									setlinecolor(RGB(100, 255, 42));//��ʾ��һ������
									rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
									Sleep(500);
									setlinecolor(RGB(0, 0, 0));
									rectangle(array[PBoard.location_y][PBoard.location_x].start_x, array[PBoard.location_y][PBoard.location_x].start_y, array[PBoard.location_y][PBoard.location_x].start_x + BLOCKSIZE * 3, array[PBoard.location_y][PBoard.location_x].start_y + BLOCKSIZE * 3);
								}
								else {
									for (int i = 0; i < 3; i++) {
										for (int f = 0; f < 3; f++) {
											if (array[i][f].allow) {
												setlinecolor(RGB(100, 255, 42));//��ʾ��һ������
												rectangle(array[i][f].start_x, array[i][f].start_y, array[i][f].start_x + BLOCKSIZE * 3, array[i][f].start_y + BLOCKSIZE * 3);
												Sleep(500);
												setlinecolor(RGB(0, 0, 0));
												rectangle(array[i][f].start_x, array[i][f].start_y, array[i][f].start_x + BLOCKSIZE * 3, array[i][f].start_y + BLOCKSIZE * 3);
											}
										}
									}
								}
								switch (side)//���ⲽ����Բ,��һ�����ǲ�
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
								mciSendString(TEXT("play resources/FalseClick.mp3"), nullptr, 0, nullptr);//���ű�������
								setlinecolor(RGB(200, 19, 26));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								Sleep(300);//��ʾ���
								setlinecolor(RGB(0, 0, 0));
								rectangle(array[i][f].board[k][j].location_x, array[i][f].board[k][j].location_y, array[i][f].board[k][j].location_x + BLOCKSIZE, array[i][f].board[k][j].location_y + 69);
								setlinecolor(RGB(100, 255, 42));//�ٴ���ʾ��һ������
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
					switch (array[i][f].board[j][k].state) {//����˳����������ͼ
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