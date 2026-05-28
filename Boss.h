#pragma once
#include "Enemy.h"
#include "Player.h"

// 보스 (Enemy 상속, Enemy.json "boss" 키에서 로드)
class Boss : public Enemy {
public:
    Boss() {
        attack = 20;
        defense = 5;
    }
};

// json에서 보스 데이터 로드
Boss loadBoss();

bool startBossBattle(Player& player, Boss& boss);