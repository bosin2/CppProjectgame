#pragma once
#define JSON_HAS_CPP_17 0
#include "Character.h"
#include "Define.h"

#include "include/json.hpp"
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

// 몬스터 행동 패턴 1개
class Pattern {
public:
    string name;
    int damage;
    string type;
};

// 원소 몬스터 (Character 상속)
class Enemy : public Character {
public:
    string element;
    string grade;

    vector<string>  ascii;
    vector<string>  ascii_hit;  // 피격 시 표정 (눈이 x로 변함)
    vector<Pattern> patterns;

    int  nextPattern = 0;
    bool isMiss = false;   // 회피 버프 여부

    // 생성자
    Enemy() {
        attack = 10;
        defense = 0;
    }
};
// JSON에서 원소·등급으로 몬스터 로드
Enemy loadEnemy(string element, string grade);
