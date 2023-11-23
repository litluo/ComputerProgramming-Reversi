/*
本作业为黑白棋对弈，主要是帮助学生更加熟练的掌握程序设计。掌握怎么去分析和设计程序的功能。
完成作业需要五个阶段：分析设计，绘制棋盘。检测有效走法。人类玩家走法。人类玩家与计算机主体循环。计算机走法。
*/

#include<bits/stdc++.h>
#include <graphics.h>
using namespace std;

class CHESS{
    public:
        void draw();
    public:
        int x = 0;
        int y = 0;
        bool used = 0;
        bool col = 0; // 0 for black, 1 for white
        COLORREF color = BLACK;
};

class BOX{
    public:
        int x1, y1, x2, y2;
        int used = 0;
        COLORREF color = RGB(255, 205, 150);
    public:
        void draw();
};

void init();
void game();
void drawChessboard();

CHESS chess[8][8];
BOX box[8][8];

int main(){
    drawChessboard();
    game();
    int a;
    scanf("%d", &a);
    scanf("%d", &a);
    return 0;
}

const int upLenth = 100, downLenth = 100, leftLenth = 80, rightLenth = 80, step = 60, N = 8;
bool player;


void drawChessboard(){
    TCHAR strnum[8][3] = { _T("1"),_T("2") ,_T("3") ,_T("4"),_T("5") ,_T("6"), _T("7"), _T("8")};
    TCHAR strabc[8][3] = { _T("A"),_T("B") ,_T("C") ,_T("D"),_T("E") ,_T("F"), _T("G"), _T("H")};
    
    initgraph(leftLenth+N*step+rightLenth+20, upLenth+N*step+downLenth+20, NOMINIMIZE);
    setbkcolor(WHITE);
    cleardevice();
    setbkmode(TRANSPARENT);

    setfillcolor(RGB(255, 205, 150));
    solidrectangle(leftLenth-20, upLenth-20, leftLenth+N*step+40, upLenth+N*step+40);
    setlinestyle(PS_SOLID,2);//画实线，宽度为两个像素
	setcolor(RGB(0,0,0));//设置为黑色
    for(int i = 0; i <= N; i++){
        line(i*step+leftLenth+20,upLenth+20,i*step+leftLenth+20,N*step+upLenth+20);
		line(leftLenth+20,i*step+upLenth+20,N*step+leftLenth+20,i*step+upLenth+20);
    }

    settextstyle(20, 0, _T("宋体"));
	for (int i = 0; i < N; i++)
	{
		outtextxy(leftLenth+step*i+step/2+15, upLenth-5, strnum[i]);
		outtextxy(leftLenth-5, upLenth+step*i+step/2+15, strabc[i]);
	}

    settextstyle(50, 0, _T("宋体"));
    RECT r = {0, 0, leftLenth+N*step+rightLenth+20, upLenth-20};
    drawtext(_T("黑白棋"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    for (int i = 0; i < N; i++){
        for (int j =0; j < N; j++){
            chess[i][j].x = leftLenth+20+i*step+step/2;
            chess[i][j].y = upLenth+20+j*step+step/2;
        }
    }
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            box[i][j].x1 = leftLenth+20+i*step+3;
            box[i][j].y1 = upLenth+20+j*step+3;
            box[i][j].x2 = leftLenth+20+i*step+step-4;
            box[i][j].y2 = upLenth+20+j*step+step-4;
        }
    }
}

void CHESS::draw(){
    setfillcolor(color);
    setlinecolor(color);
    setlinestyle(PS_SOLID, 2);
    fillcircle(x-0.5, y-0.5, step/2-3);
    return;
}

void BOX::draw(){
    if (used) return;
    setlinecolor(color);
    setlinestyle(PS_SOLID, 3);
    line(x1, y1, x1+10, y1);
    line(x1, y1, x1, y1+10);
    line(x2, y1, x2-10, y1);
    line(x2, y1, x2, y1+10);
    line(x1, y2, x1+10, y2);
    line(x1, y2, x1, y2-10);
    line(x2, y2, x2-10, y2);
    line(x2, y2, x2, y2-10);
    return;
}

bool check(int x, int y, int col){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        for (int i = x+dx[k], j = y+dy[k]; i >= 0 && i < 8 && i >= 0 && i < 8; i += dx[k], j += dy[x]){
            if (!chess[i][j].used) break;
            if (chess[i][j].col == col && i != x+dx[k] && j != y+dy[k]) return true;
        }
    }
    return false;
}

void reversi(int x, int y, int col){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        for (int i = x+dx[k], j = y+dy[k]; i >= 0 && i < 8 && i >= 0 && i < 8; i += dx[k], j += dy[x]){
            if (!chess[i][j].used) break;
            if (chess[x+dx[k]][y+dy[k]].col == col) break;
            if (chess[i][j].col == col){
                for (int ii = x+dx[k], jj = y+dy[k]; ii != i && jj != j; ii += dx[k], jj += dy[k]){
                    chess[ii][jj].col = col;
                    chess[ii][jj].color = col ? WHITE : BLACK;
                    chess[ii][jj].draw();
                }
                break;
            }
        }
    }
}

void game(){
    int oldi, oldj;
    int i, j;
    player = 0;
    chess[3][3].col = 0, chess[3][3].color = BLACK, chess[3][3].draw(), chess[3][3].used = 1;
    chess[3][4].col = 1, chess[3][4].color = WHITE, chess[3][4].draw(), chess[3][4].used = 1;
    chess[4][3].col = 1, chess[4][3].color = WHITE, chess[4][3].draw(), chess[4][3].used = 1;
    chess[4][4].col = 0, chess[4][4].color = BLACK, chess[4][4].draw(), chess[4][4].used = 1;
    while(true){
        NEXTPLAYER:
        MOUSEMSG mouse = GetMouseMsg();
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (mouse.x >= box[i][j].x1 && mouse.x <= box[i][j].x2 && mouse.y >= box[i][j].y1 && mouse.y <= box[i][j].y2){
                    if (chess[i][j].used)
                        continue;
                    if (mouse.mkLButton){
                        if (!chess[i][j].used){
                            if (check(i, j, player)){
                                chess[i][j].color = player ? WHITE : BLACK;
                                chess[i][j].draw();
                                chess[i][j].used = 1;
                                reversi(i, j, player);
                                player = !player;
                                goto NEXTPLAYER;
                            }
                        }
                    }
                    if (i != oldi || j != oldj){
                        box[i][j].color = LIGHTGRAY;
                        box[i][j].draw();
                        box[oldi][oldj].color = RGB(255, 205, 150);
                        box[oldi][oldj].draw();
                        oldi = i, oldj = j;
                    }
                }
            }
        }
    }
    return;
}
