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
    pat.name   = p["name"];
    pat.damage = p["damage"];
    pat.type   = p["type"];
    return pat;
}

// json에서 원소·등급으로 몬스터 로드
Enemy loadEnemy(string element, string grade) {
    json data = loadEnemyData();

    Enemy e;
    e.element = element;
    e.grade   = grade;

    if (!data.contains(element) || !data[element].is_array()) {
        throw runtime_error("Enemy.json에 원소 데이터가 없습니다: " + element);
    }

    for (auto& em : data[element]) {
        if (em["grade"] == grade) {
            e.name  = em["name"];
            e.hp    = em["hp"];
            e.maxHp = em["hp"];

            for (auto& line : em["ascii"])
                e.ascii.push_back(line);

            if (em.contains("ascii_hit")) {
                for (auto& line : em["ascii_hit"])
                    e.ascii_hit.push_back(line);
            }

            for (auto& p : em["patterns"])
                e.patterns.push_back(makePattern(p));

            break;
        }
    }
    if (e.patterns.empty()) {
        throw runtime_error("Enemy.json에서 몬스터를 찾지 못했습니다: " + element + " / " + grade);
    }
    return e;
}

// json에서 보스 데이터 로드 (원소별 패턴 4세트 포함)
Boss loadBoss() {
    json data = loadEnemyData();

    if (!data.contains("boss") || !data["boss"].is_array() || data["boss"].empty()) {
        throw runtime_error("Enemy.json에 보스 데이터가 없습니다.");
    }

    auto& bd = data["boss"][0];

    Boss b;
    b.name  = bd["name"];
    b.grade = bd["grade"];
    b.hp    = bd["hp"];
    b.maxHp = bd["hp"];
    b.element = "fire";   // 시작 원소

    for (auto& line : bd["ascii"])
        b.ascii.push_back(line);
    if (bd.contains("phaseAscii") && bd["phaseAscii"].is_array()) {
        for (auto& phase : bd["phaseAscii"]) {
            vector<string> art;
            for (auto& line : phase)
                art.push_back(line);
            if (!art.empty())
                b.phaseAscii.push_back(art);
        }

        if (!b.phaseAscii.empty())
            b.ascii = b.phaseAscii[0];
    }

    // 원소별 패턴 4세트 로드
    for (auto& p : bd["firePatterns"])  b.firePatterns.push_back(makePattern(p));
    for (auto& p : bd["waterPatterns"]) b.waterPatterns.push_back(makePattern(p));
    for (auto& p : bd["earthPatterns"]) b.earthPatterns.push_back(makePattern(p));
    for (auto& p : bd["windPatterns"])  b.windPatterns.push_back(makePattern(p));

    if (b.firePatterns.empty() || b.waterPatterns.empty() ||
        b.earthPatterns.empty() || b.windPatterns.empty()) {
        throw runtime_error("Enemy.json 보스의 원소별 패턴이 비어 있습니다.");
    }

    // 시작 패턴은 불 패턴
    b.patterns = b.firePatterns;

    return b;
}



