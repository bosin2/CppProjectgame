#pragma once
#include "character.h"
#include "Define.h"
#include <string>
#include <vector>
using namespace std;

enum Element {
    NONE = 0,
    FIRE = 1,
    WATER = 2,
    EARTH = 3,
    WIND = 4
};

class Player : public Character {
public:
    int mp = 50;
    int maxMp = 50;
    int row = 2;        // 맵 행 위치
    int col = 0;        // 맵 열 위치

    string slots[3] = { "", "", "" };   // 보유 원소 슬롯 (최대 3개)

    Player() {
        name = "슬라임";
        hp = 100;
        maxHp = 100;
        attack = 15;
        defense = 0;
    }

    int fightCount[5] = { 0, 0, 0, 0, 0 };  // 속성별 전투 횟수 (등급 결정용, 인덱스=Element 열거형)
};

// 슬롯 1번 원소 색으로 슬라임 아스키아트 출력
void drawSlime(Player& player);