#include<bits/stdc++.h>
#include<time.h>
#include <graphics.h>
using namespace std;

const int N = 8;

typedef pair<int, int> pii;

class BOX{
    public:
        int x1, y1, x2, y2;
        int used = 0;
        COLORREF color = RGB(255, 205, 150);
    public:
        void draw();
};

class BOARD{
    public:
        BOARD(){
            memset(chess, -1, sizeof(chess));
            cnt[0] = cnt[1] = 2;
            chess[3][3] = chess[4][4] = 0;
            chess[3][4] = chess[4][3] = 1;
    }
    public:
        int chess[N][N]; // -1 for empty, 0 for black, 1 for white
        int cnt[2];
    public:
        void draw(int x, int y);
        bool check(int x, int y, int col);
        int checkLine(int x, int y, int dx, int dy, int col);
        void reversi(int x, int y, int col, int d);
        void reversiLine(int x, int y, int dx, int dy, int tot, int col, int d);
        bool checkAvilable(int col);
        void copy(BOARD selboard);
        void play(int x, int y, int col, int d);
};

class TreeNode{
    public:
        TreeNode(TreeNode *parent, int col):parent(parent), col(col){
            for (int i = 0; i < 64; i++)
                child[i] = NULL;
        }
    public:
        TreeNode *parent;
        TreeNode *child[64];
        pair<int, int> pos;
        int w = 0;
        int n = 0;
        int col = 0;
};

class HumanPlayer{
    public:
        HumanPlayer(int col):col(col){
            ;
        }
    private:
        int col;
    public:
        void play(BOARD *selfboard);
};

class RoxannePlayer{
    public:
        RoxannePlayer(int col):col(col){
        }
    private:
        int roxanne_table[N][N] = {
                {1, 5, 3, 3, 3, 3, 5, 1},
                {5, 5, 4, 4, 4, 4, 5, 5},
                {3, 4, 2, 2, 2, 2, 4, 3},
                {3, 4, 2, 9, 9, 2, 4, 3},
                {3, 4, 2, 9, 9, 2, 4, 3},
                {3, 4, 2, 2, 2, 2, 4, 3},
                {5, 5, 4, 4, 4, 4, 5, 5},
                {1, 5, 3, 3, 3, 3, 5, 1}
            };
        int col;
    public:
        void play(BOARD *selfboard, int d);
};

class MobilityPlayer{
    public:
        MobilityPlayer(int col):col(col){
        }
    private:
        int mobility_table[N][N] = {
                {1, 8, 2, 4, 4, 2, 8, 1},
                {8, 9, 7, 6, 6, 7, 9, 8},
                {2, 7, 3, 5, 5, 3, 7, 2},
                {4, 6, 5, 10, 10, 5, 6, 4},
                {4, 6, 5, 10, 10, 5, 6, 4},
                {2, 7, 3, 5, 5, 3, 7, 2},
                {8, 9, 7, 6, 6, 7, 9, 8},
                {1, 8, 2, 4, 4, 2, 8, 1}
            };
        int col;
    public:
        void play(BOARD *selfboard, int d);
};

class MCTSPlayer{
    public:
        MCTSPlayer(int col):col(col){
            ;
        }
    private:
        int col;
        int tick;
    public:
        void play(BOARD *selfboard);
    private:
        pii mcts(BOARD selfboard);
        TreeNode* select(TreeNode *node, BOARD *selfboard);
        void expand(TreeNode *node, BOARD *selfboard);
        int simulate(TreeNode *node, BOARD selfboard);
        void back_prop(TreeNode *node, int score);
};

void init();
void game();
void drawChessboard();

BOX box[N][N];
pii las;
const int upLenth = 100, downLenth = 100, leftLenth = 80, rightLenth = 80, step = 60;

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
    setlinestyle(PS_SOLID,2);
	setcolor(RGB(0,0,0));
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
        for (int j = 0; j < N; j++){
            box[i][j].x1 = leftLenth+20+i*step+3;
            box[i][j].y1 = upLenth+20+j*step+3;
            box[i][j].x2 = leftLenth+20+i*step+step-4;
            box[i][j].y2 = upLenth+20+j*step+step-4;
        }
    }
}

void BOARD::draw(int x, int y){
    int rx = leftLenth+20+x*step+step/2;
    int ry = upLenth+20+y*step+step/2;
    COLORREF color = chess[x][y] ? WHITE : BLACK;
    setfillcolor(color);
    setlinecolor(color);
    setlinestyle(PS_SOLID, 2);
    fillcircle(rx-0.5, ry-0.5, step/2-3);
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

void drawScore(int col, BOARD selfboard, int stu = 0){
    settextstyle(30, 0, _T("宋体"));
    char str[20];
    RECT r;
    clearrectangle(leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+90);
    r = {leftLenth-20, upLenth+N*step+50, leftLenth+20+2*step, upLenth+N*step+90};
    sprintf(str, "当前:%s", col ? "白色" : "黑色");
    drawtext(_T(str), &r, DT_SINGLELINE);
    r = {leftLenth+20+2*step, upLenth+N*step+50, leftLenth+20+4*step, upLenth+N*step+90};
    sprintf(str, "白色:%d", selfboard.cnt[1]);
    drawtext(_T(str), &r, DT_SINGLELINE);
    r = {leftLenth+20+4*step, upLenth+N*step+50, leftLenth+20+6*step, upLenth+N*step+90};
    sprintf(str, "黑色:%d", selfboard.cnt[0]);
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

void drawWin(BOARD selfboard){
    settextstyle(60, 0, _T("宋体"));
    settextcolor(RED);
    clearrectangle(leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+90);
    RECT r = {leftLenth-20, upLenth+N*step+50, leftLenth+20+8*step, upLenth+N*step+downLenth+10};
    if (selfboard.cnt[0] > selfboard.cnt[1]){
        drawtext(_T("黑色胜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else if (selfboard.cnt[0] < selfboard.cnt[1]){
        drawtext(_T("白色胜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    else{
        drawtext(_T("平局"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

int BOARD::checkLine(int x, int y, int dx, int dy, int col){
    int i, j;
    for (i = x+dx, j = y+dy; i >= 0 && i < 8 && j >= 0 && j < 8; i += dx, j += dy){
        if (chess[i][j] == -1) break;
        if (chess[i][j] == col && i == x+dx && j == y+dy) break;
        if (chess[i][j] == col && (i != x+dx || j != y+dy)){
            if(dx != 0)
                return (i-x)/dx - 1;
            return (j-y)/dy - 1;
        }
    }
    return 0;
}

void BOARD::reversiLine(int x, int y, int dx, int dy, int tot, int col, int d){
    for (int i = x+dx, j = y+dy; tot; i += dx, j += dy, tot--){
        chess[i][j] = col;
        if (d)
            draw(i, j);
    }
}

bool BOARD::check(int x, int y, int col){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        int tot = checkLine(x, y, dx[k], dy[k], col);
        if (tot){
            return true;
        }
    }
    return false;
}

void BOARD::reversi(int x, int y, int col, int d){
    int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    for (int k = 0; k < 8; k++){
        int res = checkLine(x, y, dx[k], dy[k], col);
        if (res){
            reversiLine(x, y, dx[k], dy[k], res, col, d);
            cnt[col] += res;
            cnt[!col] -= res;
        }
    }
    cnt[col] += 1;
}

bool BOARD::checkAvilable(int col){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (chess[i][j] == -1 && check(i, j, col))
                return true;
    return false;
}

void BOARD::copy(BOARD selfboard){
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            (*this).chess[i][j] = selfboard.chess[i][j];
    (*this).cnt[0] = selfboard.cnt[0];
    (*this).cnt[1] = selfboard.cnt[1];
}

void BOARD::play(int x, int y, int col, int d){
    chess[x][y] = col;
    if (d)
        draw(x, y);
    reversi(x, y, col, d);
    if (d){
        box[x][y].color = LIGHTCYAN;
        box[x][y].draw();
        int lx = las.first, ly = las.second;
        if (lx != -1 && ly != -1){
            box[lx][ly].color = RGB(255, 205, 150);
            box[lx][ly].draw();
        }
        las = make_pair(x, y);
        printf("%d %d\n", x, y);
    }
}

void HumanPlayer::play(BOARD *selfboard){
    int oldi, oldj;
    while(1){
        MOUSEMSG mouse = GetMouseMsg();
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (mouse.x >= box[i][j].x1 && mouse.x <= box[i][j].x2 && mouse.y >= box[i][j].y1 && mouse.y <= box[i][j].y2){
                    if ((*selfboard).chess[i][j] != -1)
                        continue;
                    if (mouse.mkLButton){
                        if ((*selfboard).chess[i][j] == -1){
                            if ((*selfboard).check(i, j, col)){
                                box[oldi][oldj].color = RGB(255, 205, 150);
                                box[oldi][oldj].draw();
                                (*selfboard).play(i, j, col, 1);
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

void RoxannePlayer::play(BOARD *selfboard, int d){
    int mminx, mminy, mmin = 8;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            if((*selfboard).chess[i][j] == -1 && roxanne_table[i][j] < mmin && (*selfboard).check(i, j, col))
                mminx = i, mminy = j, mmin = roxanne_table[i][j];
            else if ((*selfboard).chess[i][j] == -1 && roxanne_table[i][j] == mmin && rand()%2 && (*selfboard).check(i, j, col))
                mminx = i, mminy = j, mmin = roxanne_table[i][j];
        }
    (*selfboard).play(mminx, mminy, col, d);
}

void MobilityPlayer::play(BOARD *selfboard, int d){
    int mminx, mminy, mmin = 21;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            if((*selfboard).chess[i][j] == -1 && mobility_table[i][j] < mmin && (*selfboard).check(i, j, col))
                mminx = i, mminy = j, mmin = mobility_table[i][j];
            else if((*selfboard).chess[i][j] == -1 && mobility_table[i][j] == mmin && rand()%2 && (*selfboard).check(i, j, col))
                mminx = i, mminy = j, mmin = mobility_table[i][j];
            }
    (*selfboard).play(mminx, mminy, col, d);
}

pii MCTSPlayer::mcts(BOARD selfboard){
    TreeNode root = TreeNode(NULL, col);
    while (time(NULL) - tick < 3){
        BOARD silentboard;
        silentboard.copy(selfboard);
        TreeNode *choice;
        choice = select(&root, &silentboard);
        expand(choice, &silentboard);
        int score = simulate(choice, silentboard);
        /*
        if ((*choice).col != col)
            score = 1 - score;
        */
        back_prop(choice, score);
    }
    int mmax = -1;
    TreeNode *mmove;
    int i = 0;
    while(root.child[i] != NULL){
        printf("%d %d %d %d\n", (*root.child[i]).pos.first, (*root.child[i]).pos.second, (*root.child[i]).w, (*root.child[i]).n);
        if ((*root.child[i]).n > mmax){
            mmax = (*root.child[i]).n;
            mmove = root.child[i];
        }
        i++;
    }
    return (*mmove).pos;
}

TreeNode* MCTSPlayer::select(TreeNode *node, BOARD *selfboard){
    if ((*node).child[0] == NULL)
        return node;
    TreeNode *mmove;
    double mmax = -1;
    int i = 0;
    while((*node).child[i] != NULL){
        if ((*(*node).child[i]).n == 0){
            mmove = (*node).child[i];
            break;
        }
        int N = (*node).n;
        int n = (*(*node).child[i]).n;
        int w = (*(*node).child[i]).w;
        double score = (double)w / n + sqrt(2.0 * log(N) / n);
        if (score > mmax){
            mmax = score;
            mmove = (*node).child[i];
        }
        i++;
    }
    if (node->parent != NULL){
        int x = (*mmove).pos.first, y = (*mmove).pos.second;
        (*selfboard).chess[x][y] = !(*node).col;
        (*selfboard).reversi(x, y, !(*node).col, 0);
    }
    return select(mmove, selfboard);
}

void MCTSPlayer::expand(TreeNode *node, BOARD *selfboard){
    int tot = 0;
    while((*node).child[tot] != NULL)
        tot++;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            if ((*selfboard).chess[i][j] == -1 && (*selfboard).check(i, j, (*node).col)){
                (*node).child[tot] = new TreeNode(node, !((*node).col));
                (*(*node).child[tot]).pos = make_pair(i, j);
                tot++;
            }
        }
}

int MCTSPlayer::simulate(TreeNode *node, BOARD selfboard){
    int colnow = (*node).col;
    BOARD silentboard;
    silentboard.copy(selfboard);
    while(true){
        if (!silentboard.checkAvilable(colnow)){
            break;
        }
        RoxannePlayer roxanneplayer = RoxannePlayer(colnow);
        roxanneplayer.play(&silentboard, 0);
        colnow = !colnow;
    }
    return silentboard.cnt[col] > silentboard.cnt[!col] ? 1 : 0;
}

void MCTSPlayer::back_prop(TreeNode *node, int score){
    (*node).n++;
    (*node).w += score;
    if ((*node).parent != NULL)
        back_prop((*node).parent, score);
}

void MCTSPlayer::play(BOARD *selfboard){
    tick = time(NULL);
    pii p;
    p = mcts(*selfboard);
    int x = p.first, y = p.second;
    (*selfboard).play(x, y, col, 1);
}

void game(){
    bool player = 0;
    las = make_pair(-1, -1);
    BOARD showboard;
    showboard.draw(3, 3);
    showboard.draw(4, 4);
    showboard.draw(3, 4);
    showboard.draw(4, 3);
    drawScore(player, showboard);
    while(true){
        if (player){
            MCTSPlayer mctsplayer = MCTSPlayer(player);
            mctsplayer.play(&showboard);
        }
        else{
            HumanPlayer humanplayer = HumanPlayer(player);
            humanplayer.play(&showboard);
        }
        player = !player;
        drawScore(player, showboard, 1);
        if (!showboard.checkAvilable(player)){
            break;
        }
        drawScore(player, showboard, 0);
        //Sleep(1000);
    }
    drawWin(showboard);
    while(true){
        MOUSEMSG mouse = GetMouseMsg();
        if (mouse.mkLButton)
            ;//break;
    }
    return;
}

/*
MCTSPlayer mctsplayer = MCTSPlayer(player);
mctsplayer.play(&showboard);

HumanPlayer humanplayer = HumanPlayer(player);
humanplayer.play(&showboard);

RoxannePlayer roxanneplayer = RoxannePlayer(player);
roxanneplayer.play(&showboard, 1);

MobilityPlayer mobilityplayer = MobilityPlayer(player);
mobilityplayer.play(&showboard, 1);
*/