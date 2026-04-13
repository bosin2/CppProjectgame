#include "Enemy.h" //jsonㅠ파싱함수

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