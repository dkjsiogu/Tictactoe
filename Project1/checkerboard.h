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
struct SmallCheckerboard {//С���̽ṹ��
	int state;
	int location_x;
	int location_y;
};

class checkerboard{
public:
	checkerboard();
	SmallCheckerboard board[3][3];//���̸�����״̬
	bool allow;//�����Ƿ����
	int start_x, start_y;
	bool checksuccess();//�������ʤ��;
	int state;//��¼���̹���

	
private:
	void change(int);//ʤ���ж���ı�ĳһ���̵�״̬
};
#endif // !1
