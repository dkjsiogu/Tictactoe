#include "checkerboard.h"
#include<iostream>
#include<graphics.h>
checkerboard::checkerboard() {
	int i, f;
	this->state = 0;
	this->allow = true;
	for (i = 0; i < 3; i++) {
		for (f = 0; f < 3; f++) {
			this->board[i][f].state = VOIDPIECE;
		}
	}
}

bool checkerboard::checksuccess()//小棋盘检测
{
	if ((this->board[0][0].state == FORK && this->board[1][1].state == FORK && this->board[2][2].state == FORK)||(this->board[2][0].state == FORK && this->board[1][1].state == FORK && this->board[0][2].state == FORK)) {
		change(FORK);
		return true;
	}
	if ((this->board[0][0].state == CIRCLE && this->board[1][1].state == CIRCLE && this->board[2][2].state == CIRCLE)||(this->board[2][0].state == CIRCLE && this->board[1][1].state == CIRCLE && this->board[0][2].state == CIRCLE)) {
		change(CIRCLE);
		return true;
	}
	
	for (int i = 0; i < 3; i++) {
		int Horizontal[2] = { 0,0 }; int Vertical[2] = { 0,0 };//横竖
		for (int f = 0; f < 3; f++) {
			if (this->board[i][f].state == FORK) {
				Horizontal[FORK-1]++;
			}
			else if (this->board[i][f].state == CIRCLE) {
				Horizontal[CIRCLE-1]++;
			}
			if (this->board[f][i].state == FORK) {
				Vertical[FORK-1]++;
			}
			else if (this->board[f][i].state == CIRCLE) {
				Vertical[CIRCLE-1]++;
			}
		}
		if (Horizontal[CIRCLE-1] == 3 || Vertical[CIRCLE-1] == 3) {
			change(CIRCLE);
			return true;
		}
		if (Horizontal[FORK-1] == 3 || Vertical[FORK-1] == 3) {
			change(FORK);
			return true;
		}
	}
	bool draw = true;
	for (int i = 0; i < 3; i++) {
		for (int f = 0; f < 3; f++) {
			if (this->board[i][f].state == VOIDPIECE) {
				draw = false;
				break;
			}
		}
	}
	if (draw == true) {
		change(VOIDPIECE);
		return true;
	}
	return false;
}

void checkerboard::change(int side)
{	
	if (side != VOIDPIECE) {
		this->state = side;
		this->allow = false;
		for (int i = 0; i < 3; i++) {
			for (int f = 0; f < 3; f++) {
				this->board[i][f].state = side;
			}
		}
	}
	else {
		int X = 0, O = 0;//如果一个棋盘占满，根据双方旗子数决定归属
		for (int i = 0; i < 3; i++) {
			for (int f = 0; f < 3; f++) {
				if (board[i][f].state == FORK) {
					X++;
				}
				else {
					O++;
				}
			}
		}
		if (X > O) {
			change(FORK);
		}
		else {
			change(CIRCLE);
		}
	}
}
