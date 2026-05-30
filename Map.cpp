// Map.cpp
#include "Map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int  elementMap[ROWS][COLS]; // 속성 정보
Tile gameMap[ROWS][COLS];    // 타일 종류

void initMap() {
    srand((unsigned int)time(NULL));

    // 1. 전체 몬스터 타일 + 랜덤 속성 배정
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            gameMap[r][c] = MONSTER;
            elementMap[r][c] = (rand() % 4) + 1;
        }
    }

    // 2. 우물 랜덤 배치
    int wellCount = 0;
    while (wellCount < 15) {
        int r = rand() % ROWS;
        int c = rand() % (COLS - 2) + 1; // 첫열, 보스열 제외
        if (gameMap[r][c] != WELL) {
            gameMap[r][c] = WELL;
            elementMap[r][c] = 0; // 우물은 속성 없음
            wellCount++;
        }
    }

    // 3. 마지막 열 보스 배치
    for (int r = 0; r < ROWS; r++) {
        gameMap[r][COLS - 1] = BOSS;
        elementMap[r][COLS - 1] = 0;
    }

}

// 시작 위치 선택 화면
void drawStartMap() {
    cout << CLEAR;

    for (int r = 0; r < ROWS; r++) {
        if (r == 0) {
            cout << "┏━━";
            for (int c = 1; c < COLS; c++) cout << "┳━━";
            cout << "┓\n";
        }
        else {
            cout << "┣━━";
            for (int c = 1; c < COLS; c++) cout << "╋━━";
            cout << "┫\n";
        }

        for (int c = 0; c < COLS; c++) {
            cout << "┃";
            if (c == 0) {
                cout << (r + 1) << " "; 
            }
            else {
                switch (gameMap[r][c]) {
                case MONSTER:
                    switch (elementMap[r][c]) {
                    case FIRE:  cout << RB << "  " << CLR; break;
                    case WATER: cout << BB << "  " << CLR; break;
                    case EARTH: cout << GB << "  " << CLR; break;
                    case WIND:  cout << YB << "  " << CLR; break;
                    }
                    break;
                case WELL:  cout << WB << "  " << CLR; break;
                case BOSS:  cout << BOLD << PINK << "★ " << CLR; break;
                case VISIT: cout << "  ";                 break;
                }
            }
        }
        cout << "┃\n";
    }

    cout << "┗━━";
    for (int c = 1; c < COLS; c++) cout << "┻━━";
    cout << "┛\n";

    cout << "\n  시작 위치 선택(번호가 적힌 타일 중에서 고르세요) ";
}

// 메인 맵 화면
void drawMap(Player& player) {
    cout << CLEAR;

    for (int r = 0; r < ROWS; r++) {
        if (r == 0) {
            cout << "┏━━";
            for (int c = 1; c < COLS; c++) cout << "┳━━";
            cout << "┓";
        }
        else {
            cout << "┣━━";
            for (int c = 1; c < COLS; c++) cout << "╋━━";
            cout << "┫";
        }

        // 스탯 패널
        if (r == 0) { cout << "     HP : "; drawHpBar(player.hp, player.maxHp); }
        if (r == 1) { cout << "     MP : "; drawHpBar(player.mp, player.maxMp); }
        if (r == 2) {
            cout << "    [보유 원소]   1: " << (player.slots[0] == "" ? "비어있음" : player.slots[0]);
        }
        if (r == 3) {
            cout << "                  2: " << (player.slots[1] == "" ? "비어있음" : player.slots[1]);
        }
        if (r == 4) {
            cout << "                  3: " << (player.slots[2] == "" ? "비어있음" : player.slots[2]);
        }

        cout << "\n";

        // 타일 내용
        for (int c = 0; c < COLS; c++) {
            cout << "┃";
            if (r == player.row && c == player.col)
                cout << BOLD << WHITE << "▣" << CLR << " "; // 플레이어
            else if (c == 0)
                cout << "  "; // 첫 열 공백
            else {
                switch (gameMap[r][c]) {
                case MONSTER:
                    switch (elementMap[r][c]) {
                    case FIRE:  cout << RB << "  " << CLR; break;
                    case WATER: cout << BB << "  " << CLR; break;
                    case EARTH: cout << GB << "  " << CLR; break;
                    case WIND:  cout << YB << "  " << CLR; break;
                    }
                    break;
                case WELL:  cout << WB << "  " << CLR; break;
                case BOSS:  cout << BOLD << PINK << "★ " << CLR; break;
                case VISIT: cout << "  ";                 break; // 지나온 타일 공백
                }
            }
        }
        cout << "┃\n";
    }

    // 맨 아래 테두리
    cout << "┗━━";
    for (int c = 1; c < COLS; c++) cout << "┻━━";
    cout << "┛\n\n";

    cout << RB << "  " << CLR << " 불  ";
    cout << BB << "  " << CLR << " 물  ";
    cout << GB << "  " << CLR << " 흙  ";
    cout << YB << "  " << CLR << " 바람  ";
    cout << WB << "  " << CLR << " 우물  ";
    cout << BOLD << PINK << "★ " << CLR << " 보스\n";

    cout << "\n  이동:  1(↗)  2(→)  3(↘)\n";
}