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

bool checkerboard::checksuccess()//Ð¡ÆåÅÌ¼ì²â
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
		int Horizontal[2] = { 0,0 }; int Vertical[2] = { 0,0 };//ºáÊú
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
	return false;
}

void checkerboard::change(int side)
{	
	this->state = side;
	this->allow = false;
	for (int i = 0; i < 3; i++) {
		for (int f = 0; f < 3; f++) {
			this->board[i][f].state = side;
		}
	}
}
