#pragma once
#include "Character.h"
#include "Define.h"
#include <string>
#include <vector>
using namespace std;

enum Element { NONE = 0, FIRE = 1, WATER = 2, EARTH = 3, WIND = 4 };

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
        bonusDamage = 0; // Character 클래스의 변수 사용
    }

    int fightCount[5] = { 0, 0, 0, 0, 0 };  

    string applyElementBonus(string elem) {
        if (elem == "불") {
            bonusDamage += 5; maxHp += 10; hp += 10;
            return "[불] 원소를 흡수했습니다. (공격력 +5, 체력 +10)";
        } else if (elem == "물") {
            lifeSteal += 5; maxHp += 5; hp += 5;
            return "[물] 원소를 흡수했습니다. (피흡 +5%, 체력 +5)";
        } else if (elem == "흙") {
            defense += 2; maxHp += 15; hp += 15;
            return "[흙] 원소를 흡수했습니다. (방어력 +2, 체력 +15)";
        } else if (elem == "바람") {
            evadeChance += 10; maxHp += 5; hp += 5;
            return "[바람] 원소를 흡수했습니다. (회피율 +10%, 체력 +5)";
        }
        return "";
    }
};

// ▼ 다른 파일들이 이 함수를 인식할 수 있도록 선언부 추가!
void drawSlime(Player& player);