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
	void play();//��ʼ��Ϸ�ĺ���
private:
	int game;//��Ϸ״̬
	IMAGE piece[3];//����ͼƬ
	IMAGE back;//����
	int delay;//�ӳ�ʱ��
	int side;//���嵽ĳһ��
	SmallCheckerboard PBoard;//�洢�²����̸���״̬
	checkerboard BigBoard[3][3];//������

	void start();//��Ϸ��ʼ
	void end(int);//��Ϸ����
	void init();//��ʼ��
	void BoardCheck();//�������״̬
	void mouseEvent(checkerboard arrray[3][3],int& side);//����¼�
	void updateWindows(checkerboard a[3][3],int);//���´���
};
#endif
