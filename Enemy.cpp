#include "Enemy.h"

// 속성과 등급을 Enemy.json에서 읽어오는 함수
Enemy loadEnemy(string element, string grade) {
    ifstream f("Enemy.json");
    json data = json::parse(f);

    Enemy e;

    for (auto& em : data[element]) {
        if (em["grade"] == grade) {
            e.name    = em["name"];
            e.hp      = em["hp"];
            e.maxHp   = em["hp"];
            e.grade   = em["grade"];
            e.element = element;

            // 아스키 아트 로드
            for (auto& line : em["ascii"])
                e.ascii.push_back(line);

            // 패턴 로드
            for (auto& p : em["patterns"]) {
                Pattern pat;
                pat.name   = p["name"];
                pat.damage = p["damage"];
                pat.type   = p["type"];
                e.patterns.push_back(pat);
            }
            break;
        }
    }
    return e;
}

// 아래 함수들 구현 필요

// 몬스터가 다음 턴에 사용할 패턴을 랜덤으로 선택하는 함수

// 원소 상성 계산 함수
// 상성: 불→흙→바람→물→불
