#pragma once
#include "Define.h"
#include "include/json.hpp"
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace std;

struct Pattern {
    string name;
    int damage;
    string type;
};

struct Enemy {
    string name;
    string element;
    string grade;
    int hp;
    int maxHp;
    vector<string> ascii;
    vector<Pattern> patterns;
    int currentPatternIdx = 0;
};

Enemy loadEnemy(string element, string grade);