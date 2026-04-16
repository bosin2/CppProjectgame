#include <iostream>
using namespace std;

//콘솔커서이동시키는함수
void gotoxy(int row, int col) {
    printf("\x1b[%d;%dH", row, col);
}

void drawHpBar(int current, int max) {
    int filled = (current * 10) / max;
    cout << "[";
    for (int i = 0; i < 10; i++)
        cout << (i < filled ? "█" : "░");
    cout << "] " << current << "/" << max;
}

// 글자색
const char* CLR    = "\x1b[0m";
const char* RED    = "\x1b[31m";
const char* BLUE   = "\x1b[34m";
const char* GREEN  = "\x1b[32m";
const char* YELLOW = "\x1b[33m";
const char* CYAN   = "\x1b[36m";
const char* PINK   = "\x1b[35m";
const char* WHITE  = "\x1b[37m";
const char* BLACK  = "\x1b[30m";

// 볼드체
const char* BOLD = "\x1b[1m";

// 배경색
const char* WB = "\x1b[47m";
const char* BB = "\x1b[44m";
const char* RB = "\x1b[41m";
const char* GB = "\x1b[42m";
const char* YB = "\x1b[43m";
const char* CB = "\x1b[46m";
