#pragma once
#include "Enemy.h"
#include "Player.h"

// ================================================
// Boss : Enemy 상속
// - 3페이즈 (페이즈당 HP 300, json에서 로드)
// - 3턴마다 원소 패턴 순환 (불→물→흙→바람)
// - 페이즈 클리어시 플레이어 HP/MP 전부 회복
// ================================================
class Boss : public Enemy {
public:
    int phase     = 1;   // 현재 페이즈 (1~3)
    int turnCount = 0;   // 현재 원소 패턴 사용 턴 수
    int elemIndex = 0;   // 원소 인덱스 (0=불,1=물,2=흙,3=바람)

    // 원소별 패턴 4세트 (loadBoss()에서 json 로드)
    vector<Pattern> firePatterns;
    vector<Pattern> waterPatterns;
    vector<Pattern> earthPatterns;
    vector<Pattern> windPatterns;

    // 페이즈별 보스 아트 (페이즈가 넘어갈 때 머리가 하나씩 사라짐)
    vector<vector<string>> phaseAscii;

    Boss() {
        attack  = 20;
        defense = 5;
    }

    // 3턴마다 다음 원소 패턴으로 전환, 전환된 경우 true 반환
    bool advanceTurn() {
        turnCount++;
        if (turnCount >= 3) {
            turnCount = 0;
            elemIndex = (elemIndex + 1) % 4;
            switch (elemIndex) {
            case 0: patterns = firePatterns;  element = "fire";  break;
            case 1: patterns = waterPatterns; element = "water"; break;
            case 2: patterns = earthPatterns; element = "earth"; break;
            case 3: patterns = windPatterns;  element = "wind";  break;
            }
            return true;
        }
        return false;
    }

    // 페이즈 클리어시 HP 리셋 + 원소 초기화
    void nextPhase() {
        phase++;
        hp          = maxHp;
        turnCount   = 0;
        elemIndex   = 0;
        isDefending = false;
        isMiss      = false;
        patterns    = firePatterns;
        element     = "fire";
    }
};

// json에서 보스 데이터 로드 (Enemy.cpp에 구현)
Boss loadBoss();

bool startBossBattle(Player& player, Boss& boss);



