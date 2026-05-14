#pragma once
#define JSON_HAS_CPP_17 0
#include "character.h"
#include "Define.h"

#include "include/json.hpp"
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

//몬스터 패턴
class Pattern {
public:
    string name;
    int damage;
    string type;
};
// character 에서 몬스터 상속 
class Enemy : public Character {
public:
    string element;
    string grade;

    vector<string>  ascii;
    vector<Pattern> patterns;

    int  nextPattern = 0;
    bool isMiss = false;   // 회피 버프 여부

    // 생성자
    Enemy() {
        attack = 10;
        defense = 0;
    }
};
//등급, 원소
Enemy loadEnemy(string element, string grade);