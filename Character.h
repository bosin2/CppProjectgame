#pragma once
#include <string>
using namespace std;

class Character {
public:
    string name;

    int hp = 0;
    int maxHp = 0;
    int attack = 10;
    
    // [통합된 공통 스탯]
    int defense = 0;
    int lifeSteal = 0;
    int evadeChance = 0;
    int bonusDamage = 0; 

    bool isDefending = false;

    void defend() { isDefending = true; }

    void takeDamage(int dmg) {
        int actualDmg = dmg - defense;
        if (actualDmg < 1) actualDmg = 1;
        if (isDefending) actualDmg /= 2;
        hp -= actualDmg;
        if (hp < 0) hp = 0;
    }

    bool isAlive() const { return hp > 0; }
};