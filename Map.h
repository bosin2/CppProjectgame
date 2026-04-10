#pragma once
#include "Define.h"
#include "Player.h"

extern Tile gameMap[ROWS][COLS];      // 타일 종류 배열
extern int  elementMap[ROWS][COLS];   // 타일별 속성값 배열

// 맵 초기화 함수
void initMap();

// 게임 시작 시 시작 위치 선택 화면 출력
void drawStartMap();

// 현재 플레이어 위치를 포함하여 맵을 그리는 함수
void drawMap(Player& player);
