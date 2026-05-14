#include "Enemy.h" //jsonㅠ파싱함수
#include "Boss.h"

Enemy loadEnemy(string element, string grade) {
    ifstream f("Enemy.json");
    json data = json::parse(f);

    Enemy e;

    for (auto& em : data[element]) {
        if (em["grade"] == grade) {
            e.name = em["name"];
            e.hp = em["hp"];
            e.maxHp = em["hp"];
            e.grade = em["grade"];
            e.element = element;

            for (auto& line : em["ascii"])
                e.ascii.push_back(line);

            for (auto& p : em["patterns"]) {
                Pattern pat;
                pat.name = p["name"];
                pat.damage = p["damage"];
                pat.type = p["type"];
                e.patterns.push_back(pat);
            }
            break;
        }
    }
    return e;
}

Boss loadBoss() {
    ifstream f("Enemy.json");
    json data = json::parse(f);

    Boss b;

    auto& bossData = data["boss"][0];  // 첫 번째 보스 데이터

    b.name = bossData["name"];
    b.grade = bossData["grade"];
    b.hp = bossData["hp"];
    b.maxHp = bossData["hp"];
    b.element = "all";

    for (auto& line : bossData["ascii"])
        b.ascii.push_back(line);

    for (auto& p : bossData["patterns"]) {
        Pattern pat;
        pat.name = p["name"];
        pat.damage = p["damage"];
        pat.type = p["type"];
        b.patterns.push_back(pat);
    }

    return b;
}