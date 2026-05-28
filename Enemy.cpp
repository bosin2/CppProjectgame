#include "Enemy.h"
#include "Boss.h"
#include <stdexcept>

static json loadEnemyData() {
    ifstream f("Enemy.json");
    if (!f.is_open()) {
        throw runtime_error("Enemy.json 파일을 열 수 없습니다. 실행 파일과 같은 폴더에 Enemy.json이 있는지 확인하세요.");
    }
    return json::parse(f);
}

static Pattern makePattern(const json& p) {
    Pattern pat;
    pat.name = p["name"];
    pat.damage = p["damage"];
    pat.type = p["type"];
    return pat;
}

// JSON에서 원소·등급으로 몬스터 로드
Enemy loadEnemy(string element, string grade) {
    json data = loadEnemyData();

    Enemy e;
    e.element = element;
    e.grade = grade;

    if (!data.contains(element) || !data[element].is_array()) {
        throw runtime_error("Enemy.json에 원소 데이터가 없습니다: " + element);
    }

    for (auto& em : data[element]) {
        if (em["grade"] == grade) {
            e.name = em["name"];
            e.hp = em["hp"];
            e.maxHp = em["hp"];
            e.grade = em["grade"];
            e.element = element;

            for (auto& line : em["ascii"])
                e.ascii.push_back(line);

            if (em.contains("ascii_hit")) {
                for (auto& line : em["ascii_hit"])
                    e.ascii_hit.push_back(line);
            }

            for (auto& p : em["patterns"]) {
                e.patterns.push_back(makePattern(p));
            }
            break;
        }
    }
    if (e.patterns.empty()) {
        throw runtime_error("Enemy.json에서 몬스터를 찾지 못했습니다: " + element + " / " + grade);
    }
    return e;
}

// JSON에서 보스 데이터 로드
Boss loadBoss() {
    json data = loadEnemyData();

    Boss b;
    if (!data.contains("boss") || !data["boss"].is_array() || data["boss"].empty()) {
        throw runtime_error("Enemy.json에 보스 데이터가 없습니다.");
    }

    auto& bossData = data["boss"][0];  // 첫 번째 보스 데이터

    b.name = bossData["name"];
    b.grade = bossData["grade"];
    b.hp = bossData["hp"];
    b.maxHp = bossData["hp"];
    b.element = "all";

    for (auto& line : bossData["ascii"])
        b.ascii.push_back(line);

    for (auto& p : bossData["patterns"]) {
        b.patterns.push_back(makePattern(p));
    }
    if (b.patterns.empty()) {
        throw runtime_error("Enemy.json의 보스 패턴이 비어 있습니다.");
    }

    return b;
}
