#pragma once
#include "Enemy.h"
#include "Player.h"

// ========================================
// Boss : Enemy를 상속받아 보스 전용 기능 추가
// 데이터는 Enemy.json의 "boss" 키에서 로드됨
// ========================================
class Boss : public Enemy {
public:
    Boss() {
        // json에서 로드할거니까 비워둠
        // 단, json에 없는 수치는 여기서 기본값 지정
        attack = 20;
        defense = 5;
    }
};

// json에서 보스 데이터 로드
Boss loadBoss();

bool startBossBattle(Player& player, Boss& boss);