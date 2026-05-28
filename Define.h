#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX 
#include <windows.h>
#include <iostream>

// 화면 초기화 + 커서 홈 이동
#define CLEAR "\x1b[2J\x1b[H"

using namespace std;

// 커서를 (row, col)로 이동
void gotoxy(int row, int col);

// HP/MP 바 출력
void drawHpBar(int current, int max);

// ANSI 색상 코드 (Define.cpp에서 정의)
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

// 배경색 코드
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