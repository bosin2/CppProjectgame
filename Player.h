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
    int row = 2;
    int col = 0;

    string slots[3] = { "", "", "" };

    Player() {
        name = "슬라임";
        hp = 100;        
        maxHp = 100;     
        attack = 15;
        defense = 0;
    }

    int fightCount[5] = { 0, 0, 0, 0, 0 };
};

void drawSlime(Player& player);