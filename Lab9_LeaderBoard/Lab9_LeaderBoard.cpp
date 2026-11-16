#define NOMINMAX
#include <windows.h>   // для SetConsoleOutputCP

#include <algorithm>
#include <cassert>
#include <clocale>
#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ===================== Учасники =====================
// Звичайний водій. Ім'я формує сам водій (раніше це робив клас Race).
class Driver {
public:
    Driver(std::string name, std::string country)
        : name_(std::move(name)), country_(std::move(country)) {
    }
    virtual ~Driver() = default;

    virtual std::string getName() const { return name_; }
    const std::string& getCountry() const { return country_; }

private:
    std::string name_;
    std::string country_;
};

// Самокерований автомобіль: власний спосіб формування імені.
// Перевірка типу в класі Race більше не потрібна (принципи OCP та LSP).
class SelfDrivingCar : public Driver {
public:
    SelfDrivingCar(std::string algorithmVersion, std::string country)
        : Driver("", std::move(country)),
        algorithmVersion_(std::move(algorithmVersion)) {
    }

    std::string getName() const override {
        return "Self Driving Car - " + getCountry() + " (" + algorithmVersion_ + ")";
    }

private:
    std::string algorithmVersion_;
};

// ===================== Системи нарахування балів =====================
// Абстракція: як нараховуються бали за місце (позиція рахується з 0).
class IScoringSystem {
public:
    virtual int pointsFor(std::size_t position) const = 0;
    virtual ~IScoringSystem() = default;
};

// Система з оригінального коду: 25, 18, 15.
class ClassicScoring : public IScoringSystem {
public:
    int pointsFor(std::size_t position) const override {
        static const int table[] = { 25, 18, 15 };
        return position < 3 ? table[position] : 0;
    }
};

// РОЗШИРЕННЯ: розширена система балів для перших 10 місць.
class ExtendedScoring : public IScoringSystem {
public:
    int pointsFor(std::size_t position) const override {
        static const int table[] = { 25, 18, 15, 12, 10, 8, 6, 4, 2, 1 };
        return position < 10 ? table[position] : 0;
    }
};

// ===================== Перегони =====================
// Результат одного учасника в одних перегонах.
struct RaceResult {
    std::string driverName;
    int points;
};

// Інтерфейс перегонів. LeaderBoard залежить від нього, а не від конкретного Race (DIP).
class IRace {
public:
    virtual std::vector<RaceResult> results() const = 0;
    virtual ~IRace() = default;
};

// Перегони: порядок фінішу + система нарахування балів.
class Race : public IRace {
public:
    Race(std::vector<std::shared_ptr<Driver>> finishOrder,
        std::shared_ptr<IScoringSystem> scoring)
        : finishOrder_(std::move(finishOrder)), scoring_(std::move(scoring)) {
    }

    std::vector<RaceResult> results() const override {
        std::vector<RaceResult> out;
        for (std::size_t i = 0; i < finishOrder_.size(); ++i) {
            out.push_back({ finishOrder_[i]->getName(), scoring_->pointsFor(i) });
        }
        return out;
    }

private:
    std::vector<std::shared_ptr<Driver>> finishOrder_;
    std::shared_ptr<IScoringSystem> scoring_;
};

// ===================== Таблиця лідерів =====================
class LeaderBoard {
public:
    explicit LeaderBoard(std::vector<const IRace*> races)
        : races_(std::move(races)) {
    }

    // Сума балів кожного учасника за всі перегони.
    std::map<std::string, int> driverResults() const {
        std::map<std::string, int> totals;
        for (const IRace* race : races_) {
            for (const auto& r : race->results()) {
                totals[r.driverName] += r.points;
            }
        }
        return totals;
    }

    // Рейтинг учасників: від більшої кількості балів до меншої.
    std::vector<std::string> driverRankings() const {
        auto totals = driverResults();
        std::vector<std::pair<std::string, int>> v(totals.begin(), totals.end());

        std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;  // більше балів вище
            return a.first < b.first;  // РОЗШИРЕННЯ: за рівності балів за іменем
            });

        std::vector<std::string> names;
        for (const auto& p : v) names.push_back(p.first);
        return names;
    }

private:
    std::vector<const IRace*> races_;
};

// ===================== Тести =====================
// Підроблені перегони: дозволяють перевіряти LeaderBoard без Driver та Race.
class FakeRace : public IRace {
public:
    explicit FakeRace(std::vector<RaceResult> r) : r_(std::move(r)) {}
    std::vector<RaceResult> results() const override { return r_; }
private:
    std::vector<RaceResult> r_;
};

// Виводить назву тесту, який пройшов.
void report(const char* name) {
    std::cout << "[OK] " << name << std::endl;
}

// Перевірка підсумовування балів за кілька перегонів.
void testSumsPointsAcrossRaces() {
    FakeRace r1({ {"A", 25}, {"B", 18} });
    FakeRace r2({ {"B", 25}, {"A", 18} });
    LeaderBoard lb({ &r1, &r2 });
    assert(lb.driverResults().at("A") == 43);
    assert(lb.driverResults().at("B") == 43);
}

// Перевірка: переможець іде першим у рейтингу.
void testWinnerIsFirstInRanking() {
    FakeRace r({ {"Slow", 1}, {"Fast", 25} });
    LeaderBoard lb({ &r });
    assert(lb.driverRankings()[0] == "Fast");
}

// Перевірка імені самокерованого автомобіля.
void testSelfDrivingCarName() {
    auto scoring = std::make_shared<ClassicScoring>();
    Race race({ std::make_shared<SelfDrivingCar>("1.2", "Acme") }, scoring);
    LeaderBoard lb({ &race });
    assert(lb.driverResults().count("Self Driving Car - Acme (1.2)") == 1);
}

// Перевірка: людина та самокерований автомобіль в одних перегонах.
void testHumanAndRobotInOneRace() {
    auto scoring = std::make_shared<ClassicScoring>();
    Race race({ std::make_shared<Driver>("Lewis", "UK"),
               std::make_shared<SelfDrivingCar>("1.2", "Acme") }, scoring);
    LeaderBoard lb({ &race });
    auto res = lb.driverResults();
    assert(res.at("Lewis") == 25);
    assert(res.at("Self Driving Car - Acme (1.2)") == 18);
}

// Перевірка класичної системи балів.
void testClassicScoring() {
    ClassicScoring s;
    assert(s.pointsFor(0) == 25);
    assert(s.pointsFor(2) == 15);
    assert(s.pointsFor(3) == 0);
}

// Тест розширення: розширена система балів.
void testExtendedScoring() {
    ExtendedScoring s;
    assert(s.pointsFor(3) == 12);
    assert(s.pointsFor(9) == 1);
    assert(s.pointsFor(10) == 0);
}

// Тест розширення: за рівності балів порядок визначається за іменем.
void testTieBreakByName() {
    FakeRace r({ {"Zed", 10}, {"Abe", 10} });
    LeaderBoard lb({ &r });
    assert(lb.driverRankings()[0] == "Abe");
}

int main() {
    // Налаштування української мови та кодування консолі.
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF-8");

    std::cout << "=== Запуск тестів LeaderBoard ===" << std::endl;

    testSumsPointsAcrossRaces();
    report("Підсумовування балів за кілька перегонів");
    testWinnerIsFirstInRanking();
    report("Переможець першим у рейтингу");
    testSelfDrivingCarName();
    report("Ім'я самокерованого автомобіля");
    testHumanAndRobotInOneRace();
    report("Людина та самокерований автомобіль в одних перегонах");
    testClassicScoring();
    report("Класична система балів");
    testExtendedScoring();
    report("Розширена система балів (розширення)");
    testTieBreakByName();
    report("Рівність балів: порядок за іменем (розширення)");

    std::cout << "Усі тести пройдено" << std::endl;
    return 0;
}