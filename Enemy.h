#pragma once
#include "Define.h"
#include "include/json.hpp"
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

//몬스터 패턴
struct Pattern {
    string name;
    int damage;
    string type;
};
//몬스터 구조체
struct Enemy {
    string name;
    string element;
    string grade;
    int hp;
    int maxHp;
    vector<string> ascii;
    vector<Pattern> patterns;
    int nextPattern = 0;
    bool isDefending = false;
    bool isMiss = false;
};
//등급, 원소
Enemy loadEnemy(string element, string grade);