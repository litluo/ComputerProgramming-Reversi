/*
本作业为黑白棋对弈，主要是帮助学生更加熟练的掌握程序设计。掌握怎么去分析和设计程序的功能。
完成作业需要五个阶段：分析设计，绘制棋盘。检测有效走法。人类玩家走法。人类玩家与计算机主体循环。计算机走法。
*/

#include<bits/stdc++.h>
#include <graphics.h>
using namespace std;

class chess{
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

int main(){
    drawChessboard();
    int a;
    scanf("%d", &a);
    return 0;
}

chess chesses[8][8];

void drawChessboard(){
    int step = 80;
    int n = 8;
    
    initgraph(20+(n+2)*step, 100+(n+2)*step, NOMINIMIZE);
    setbkcolor(WHITE);
    cleardevice();
    setbkmode(TRANSPARENT);

    TCHAR strnum[8][3] = { _T("1"),_T("2") ,_T("3") ,_T("4"),_T("5") ,_T("6"), _T("7"), _T("8")};
    TCHAR strabc[8][3] = { _T("A"),_T("B") ,_T("C") ,_T("D"),_T("E") ,_T("F"), _T("G"), _T("H")};
    settextstyle(20, 0, _T("宋体"));

    setfillcolor(RGB(255, 205, 150));
    solidrectangle(step-20, step-20, (n+1)*step+40, (n+1)*step+40);
    setlinestyle(PS_SOLID,2);//画实线，宽度为两个像素
	setcolor(RGB(0,0,0));//设置为黑色
    for(int i = 1; i <= n+1; i++){
        line(i*step+20,1*step+20,i*step+20,(n+1)*step+20);
		line(1*step+20,i*step+20,(n+1)*step+20,i*step+20);
    }
    LOGFONT nowstyle;
	gettextstyle(&nowstyle);
	settextstyle(0, 0, NULL);
	for (int i = 0; i < n; i++)
	{
		outtextxy(step*(i+1)+step/2+15, step-5, strnum[i]);
		outtextxy(step, step*(i+1)+step/2+12, strabc[i]);
	}
	settextstyle(&nowstyle);
}