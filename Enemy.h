// Enemy.h
#pragma once
#include "Define.h"
#include "include/json.hpp" // JSON 처리 라이브러리 (외부 라이브러리)
#include <fstream>
#include <vector>

using json = nlohmann::json;

// 몬스터 공격 패턴 구조체
struct Pattern {
    string name;   // 패턴 이름
    int damage;    // 공격 데미지
    string type;   // 패턴 타입

    // type에 따라 전투 처리 분기 필요

};

// 몬스터 정보 구조체
struct Enemy {
    string name;              // 이름
    string element;           // 속성 (fire / water / earth / wind)
    string grade;             // 등급 (하급 / 중급 / 상급)
    int hp;                   // 현재 체력
    int maxHp;                // 최대 체력
    vector<string> ascii;     // 아스키 아트
    vector<Pattern> patterns; // 공격 패턴 목록

    // 아래 필드 추가
    // 현재 예고된 패턴 인덱스
    // 방어력
};

// 속성과 등급으로 Enemy.json에서 몬스터 데이터를 불러오는 함수
Enemy loadEnemy(string element, string grade);
