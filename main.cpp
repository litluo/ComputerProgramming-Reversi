/*
����ҵΪ�ڰ�����ģ���Ҫ�ǰ���ѧ���������������ճ�����ơ�������ôȥ��������Ƴ���Ĺ��ܡ�
�����ҵ��Ҫ����׶Σ�������ƣ��������̡������Ч�߷�����������߷��������������������ѭ����������߷���
*/

#include<bits/stdc++.h>
#include <graphics.h>
using namespace std;

class node{
    public:
        void draw();
    public:
        int x = 0;
        int y = 0;
        int value = -1;
        int modle;
        bool isnew = false;
        int number = 0;
        COLORREF color = WHITE;
};

void init();
void drawChessboard();

node chess[8][8];

int main(){
    drawChessboard();
    chess[2][2].draw();
    int a;
    scanf("%d", &a);
    return 0;
}

const int upLenth = 100, downLenth = 80, leftLenth = 80, rightLenth = 80, step = 60, N = 8;


void drawChessboard(){
    TCHAR strnum[8][3] = { _T("1"),_T("2") ,_T("3") ,_T("4"),_T("5") ,_T("6"), _T("7"), _T("8")};
    TCHAR strabc[8][3] = { _T("A"),_T("B") ,_T("C") ,_T("D"),_T("E") ,_T("F"), _T("G"), _T("H")};
    
    initgraph(leftLenth+N*step+rightLenth+20, upLenth+N*step+downLenth+20, NOMINIMIZE);
    setbkcolor(WHITE);
    cleardevice();
    setbkmode(TRANSPARENT);

    setfillcolor(RGB(255, 205, 150));
    solidrectangle(leftLenth-20, upLenth-20, leftLenth+N*step+40, upLenth+N*step+40);
    setlinestyle(PS_SOLID,2);//��ʵ�ߣ����Ϊ��������
	setcolor(RGB(0,0,0));//����Ϊ��ɫ
    for(int i = 0; i <= N; i++){
        line(i*step+leftLenth+20,upLenth+20,i*step+leftLenth+20,N*step+upLenth+20);
		line(leftLenth+20,i*step+upLenth+20,N*step+leftLenth+20,i*step+upLenth+20);
    }

    settextstyle(20, 0, _T("����"));
	for (int i = 0; i < N; i++)
	{
		outtextxy(leftLenth+step*i+step/2+15, upLenth-5, strnum[i]);
		outtextxy(leftLenth-5, upLenth+step*i+step/2+15, strabc[i]);
	}

    settextstyle(50, 0, _T("����"));
    RECT r = {0, 0, leftLenth+N*step+rightLenth+20, upLenth-20};
    drawtext(_T("�ڰ���"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    for (int i = 0; i < N; i++){
        for (int j =0; j < N; j++){
            chess[i][j].x = leftLenth+20+i*step+step/2;
            chess[i][j].y = upLenth+20+i*step+step/2;
        }
    }
}

void node::draw(){
    setfillcolor(BLACK);
    setlinestyle(PS_SOLID, 0);
    fillcircle(x-0.5, y-0.5, step/2-3);
    return;
}

