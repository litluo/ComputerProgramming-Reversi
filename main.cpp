/*
本作业为黑白棋对弈，主要是帮助学生更加熟练的掌握程序设计。掌握怎么去分析和设计程序的功能。
完成作业需要五个阶段：分析设计，绘制棋盘。检测有效走法。人类玩家走法。人类玩家与计算机主体循环。计算机走法。
*/

#include<bits/stdc++.h>
#include<time.h>
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
        COLORREF color = BLUE;
};

class BOX{
    public:
        int x1, y1, x2, y2;
        int used = 0;
        COLORREF color = RGB(255, 205, 150);
    public:
        void draw();
};

void copy(CHESS **a, CHESS b[N][N]);

class TreeNode{
    public:
        TreeNode(int parent, int col):parent(parent), col(col){
            w = 0;
            n = 0;
            memset(child, -1, sizeof(child));
        }
    public:
        int parent;
        int w, n, col;
        int child[64];
};

class HumanPlayer{
    public:
        int col = 0;
    public:
        void play(int *cnt);
};

class RoxannePlayer{
    public:
        RoxannePlayer(int col):col(col){
            ;
        }
    public:
        int roxanne_table[N][N];
        int col;
    public:
        void roxanne_select();
        void get_move();
};

class AIPlayer{
    public:
        int col = 0;
        int timelimit = 2;
        CHESS board[N][N];
        RoxannePlayer sim_black = RoxannePlayer(0);
        RoxannePlayer sim_white = RoxannePlayer(1);
    public:
        void mcts();
        TreeNode select(TreeNode node, CHESS board[N][N]);
        void expand(TreeNode node, CHESS board[N][N]);
        void simulate(TreeNode node, CHESS board[N][N]);
        void back_prop(TreeNode node, int score);
        void get_move(CHESS board[N][N]);
};

void init();
void game();
void drawChessboard();

CHESS chess[8][8];
BOX box[8][8];
const int upLenth = 100, downLenth = 100, leftLenth = 80, rightLenth = 80, step = 60, N = 8;


int main(){
    initgraph(leftLenth+N*step+rightLenth+20, upLenth+N*step+downLenth+20, NOMINIMIZE);
    drawChessboard();
    game();
    return 0;
}

void drawChessboard(){
    TCHAR strnum[8][3] = { _T("1"),_T("2") ,_T("3") ,_T("4"),_T("5") ,_T("6"), _T("7"), _T("8")};
    TCHAR strabc[8][3] = { _T("A"),_T("B") ,_T("C") ,_T("D"),_T("E") ,_T("F"), _T("G"), _T("H")};
    
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

void drawScore(int x, int y, int col, int *cnt, int stu = 0){
    settextstyle(30, 0, _T("宋体"));
    char str[20];
    RECT r;
    clearrectangle(leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+90);
    r = {leftLenth-20, upLenth+N*step+50, leftLenth+20+2*step, upLenth+N*step+90};
    sprintf(str, "当前:%s", col ? "白色" : "黑色");
    drawtext(_T(str), &r, DT_SINGLELINE);
    r = {leftLenth+20+2*step, upLenth+N*step+50, leftLenth+20+4*step, upLenth+N*step+90};
    sprintf(str, "白色:%d", cnt[1]);
    drawtext(_T(str), &r, DT_SINGLELINE);
    r = {leftLenth+20+4*step, upLenth+N*step+50, leftLenth+20+6*step, upLenth+N*step+90};
    sprintf(str, "黑色:%d", cnt[0]);
    drawtext(_T(str), &r, DT_SINGLELINE);
    r = {leftLenth+20+6*step, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+90};
    switch (stu){
        case 0:
            drawtext(_T(""), &r, DT_SINGLELINE);
            break;
        case 1:
            drawtext(_T("校验中"), &r, DT_SINGLELINE);
            break;
    }
}

void drawWin(int *cnt){
    settextstyle(60, 0, _T("宋体"));
    settextcolor(RED);
    clearrectangle(leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+90);
    RECT r = {leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+downLenth+10};
    if (cnt[0] > cnt[1]){
        drawtext(_T("黑色胜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else if (cnt[0] < cnt[1]){
        drawtext(_T("白色胜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else{
        drawtext(_T("平局"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

int checkLine(int x, int y, int dx, int dy, int col){
    int i, j;
    for (i = x+dx, j = y+dy; i >= 0 && i < 8 && j >= 0 && j < 8; i += dx, j += dy){
        if (!chess[i][j].used) break;
        if (chess[i][j].col == col && i == x+dx && j == y+dy) break;
        if (chess[i][j].col == col && (i != x+dx || j != y+dy)){
            if(dx != 0)
                return (i-x)/dx - 1;
            return (j-y)/dy - 1;
        }
    }
    return 0;
}

void reversiLine(int x, int y, int dx, int dy, int cnt, int col){
    for (int i = x+dx, j = y+dy; cnt; i += dx, j += dy, cnt--){
        chess[i][j].col = col;
        chess[i][j].color = col ? WHITE : BLACK;
        chess[i][j].draw();
    }
}

bool check(int x, int y, int col){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        int cnt = checkLine(x, y, dx[k], dy[k], col);
        if (cnt){
            return true;
        }
    }
    return false;
}

void reversi(int x, int y, int col, int *cnt){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        int res = checkLine(x, y, dx[k], dy[k], col);
        if (res){
            reversiLine(x, y, dx[k], dy[k], res, col);
            cnt[col] += res;
            cnt[!col] -= res;
        }
    }
    cnt[col] += 1;
    drawScore(x, y, col, cnt);
}

bool checkAvilable(int col){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (!chess[i][j].used && check(i, j, col))
                return true;
    return false;
}

void HumanPlayer::play(int *cnt){
    int oldi, oldj;
    while(1){
        MOUSEMSG mouse = GetMouseMsg();
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (mouse.x >= box[i][j].x1 && mouse.x <= box[i][j].x2 && mouse.y >= box[i][j].y1 && mouse.y <= box[i][j].y2){
                    if (chess[i][j].used)
                        continue;
                    if (mouse.mkLButton){
                        if (!chess[i][j].used){
                            if (check(i, j, col)){
                                chess[i][j].col = col;
                                chess[i][j].color = col ? WHITE : BLACK;
                                chess[i][j].draw();
                                chess[i][j].used = 1;
                                reversi(i, j, col, cnt);
                                box[oldi][oldj].color = RGB(255, 205, 150);
                                box[oldi][oldj].draw();
                                return;
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
}

TreeNode AIPlayer::select(TreeNode node, CHESS board[N][N]){
    if (node.child[0] == -1){
        return node;
    }
    for (int i = 0; node.child[i] != -1; i++){
        ;
    }
}

void AIPlayer::mcts(){
    TreeNode root(-1, col);
    int tick = time(NULL);
    while(time(NULL) - tick < timelimit){
        memcpy(board, chess, sizeof(chess));
        TreeNode choice(-1, col);
        choice = select(root, board);
        expand(root, board);
    }
}

void game(){
    int cnt[2];
    cnt[0] = 2, cnt[1] = 2;
    int i, j;
    bool player = 0;
    HumanPlayer HuPlayer[2];
    HuPlayer[0].col = 0, HuPlayer[1].col = 1;
    chess[3][3].col = 0, chess[3][3].color = BLACK, chess[3][3].draw(), chess[3][3].used = 1;
    chess[3][4].col = 1, chess[3][4].color = WHITE, chess[3][4].draw(), chess[3][4].used = 1;
    chess[4][3].col = 1, chess[4][3].color = WHITE, chess[4][3].draw(), chess[4][3].used = 1;
    chess[4][4].col = 0, chess[4][4].color = BLACK, chess[4][4].draw(), chess[4][4].used = 1;
    drawScore(-1, -1, player, cnt);
    while(true){
        if(HuPlayer[1].col = player)
            HuPlayer[1].play(cnt);
        else
            HuPlayer[0].play(cnt);
        player = !player;
        drawScore(i, j, player, cnt);
        if (!checkAvilable(player)){
            break;
        }
    }
    drawWin(cnt);
    while(true){
        MOUSEMSG mouse = GetMouseMsg();
        if (mouse.mkLButton)
            ;//break;
    }
    return;
}
