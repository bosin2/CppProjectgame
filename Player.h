#pragma once
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

struct Player {
    int hp = 100;
    int maxHp = 100;
    int mp = 50;
    int maxMp = 50;
    int row = 2;
    int col = 0;

    string slots[3] = { "", "", "" };
    bool isDefending = false;

    int fightCount[5] = { 0, 0, 0, 0, 0 };
};

void drawSlime(Player& player);