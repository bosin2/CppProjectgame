#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX 
#include <windows.h>
#include <iostream>

// 화면 전체를 지우고 커서를 홈 위치로 이동시키는 ANSI 이스케이프 시퀀스
#define CLEAR "\x1b[2J\x1b[H"

using namespace std;

// 콘솔 커서를 특정 위치(행, 열)로 이동시키는 함수
void gotoxy(int row, int col);

// 체력 바를 화면에 그리는 함수
void drawHpBar(int current, int max);

// extern : 외부에있는 변수를 선언함
extern const char* CLR;
extern const char* RED;
extern const char* BLUE;
extern const char* GREEN;
extern const char* YELLOW;
extern const char* CYAN;
extern const char* PINK;
extern const char* WHITE;
extern const char* BLACK;
extern const char* BOLD;

// 배경색 포함 코드
extern const char* WB;
extern const char* BB;
extern const char* RB;
extern const char* GB;
extern const char* YB;
extern const char* CB;

// 맵 크기
const int ROWS = 5;
const int COLS = 12;

// 맵 타일 종류
enum Tile {
    MONSTER,
    WELL,
    BOSS,
    VISIT
};