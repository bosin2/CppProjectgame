#pragma once
#include <string>
using namespace std;

// ========================================
// Character : 모든 전투 캐릭터의 부모 클래스
// Player, Enemy, Boss 가 이 클래스를 상속받음
// ========================================
class Character {
public:
    string name;

    int hp = 0;
    int maxHp = 0;
    int attack = 10;   // 기본 공격력
    int defense = 0;    // 기본 방어력 (데미지 감소량)

    bool isDefending = false;

    // 방어 실행
    void defend() {
        isDefending = true;
    }

    // 데미지 입는거 (방어 중이면 절반)
    void takeDamage(int dmg) {
        if (isDefending) dmg /= 2;
        hp -= dmg;
        if (hp < 0) hp = 0;
    }

    bool isAlive() const {
        return hp > 0;
    }
};