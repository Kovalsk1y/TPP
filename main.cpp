#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

const float PI = 3.14159265f;

// Переведення градусів у радіани
float toRad(float deg) { return deg * PI / 180.f; }

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 700}), "Spaceship");
    window.setFramerateLimit(60);

    // Шрифт беремо зі стандартної папки Windows
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    // ---------- Фізичний стан корабля ----------
    sf::Vector2f pos;          // положення (світові координати)
    sf::Vector2f vel;          // швидкість
    float angle = 0.f;         // кут повороту корабля (0 = носом вгору), градуси
    float gimbal = 0.f;        // відхилення тяги відносно корабля, градуси
    float thrust = 200.f;      // сила тяги, Н
    const float mass = 1.f;    // маса, кг
    bool engineOn = false;     // двигун увімкнений?
    bool paused = false;       // пауза?
    float engineTime = 0.f;    // сумарний час роботи двигуна
    float simTime = 0.f;       // час моделювання
    float distance = 0.f;      // пройдений шлях
    std::vector<sf::Vector2f> trajectory;

    // Повернення до початкового стану
    auto resetSim = [&]()
    {
        pos = {0.f, 0.f};
        vel = {0.f, 0.f};
        angle = 0.f;
        gimbal = 0.f;
        engineOn = false;
        engineTime = 0.f;
        simTime = 0.f;
        distance = 0.f;
        trajectory.clear();
    };
    resetSim();

    // ---------- Графічні об'єкти ----------
    // Корабель - трикутник з центром у точці (0,0)
    sf::ConvexShape ship;
    ship.setPointCount(3);
    ship.setPoint(0, {0.f, -20.f});
    ship.setPoint(1, {14.f, 16.f});
    ship.setPoint(2, {-14.f, 16.f});
    ship.setFillColor(sf::Color::White);

    // Полум'я двигуна - трикутник позаду корабля
    sf::ConvexShape flame;
    flame.setPointCount(3);
    flame.setPoint(0, {-6.f, 16.f});
    flame.setPoint(1, {6.f, 16.f});
    flame.setPoint(2, {0.f, 45.f});
    flame.setFillColor(sf::Color(255, 140, 0));

    // Маркер старту
    sf::CircleShape startMark(8.f);
    startMark.setOrigin({8.f, 8.f});
    startMark.setFillColor(sf::Color::Yellow);

    // Точки-орієнтири, щоб було видно рух
    sf::CircleShape dot(2.f);
    dot.setOrigin({2.f, 2.f});
    dot.setFillColor(sf::Color(90, 90, 90));

    sf::VertexArray trajectoryLine(sf::PrimitiveType::LineStrip);

    sf::Text infoText(font, "", 18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({15.f, 10.f});

    // Камера, яка слідує за кораблем
    sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 700.f}));

    sf::Clock clock;

    while (window.isOpen())
    {
        //1. Оброблення подій 
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Space)
                    engineOn = !engineOn;
                if (key->code == sf::Keyboard::Key::P)
                    paused = !paused;
                if (key->code == sf::Keyboard::Key::R)
                    resetSim();
                if (key->code == sf::Keyboard::Key::Up)
                    thrust += 50.f;
                if (key->code == sf::Keyboard::Key::Down)
                    thrust -= 50.f;
                if (key->code == sf::Keyboard::Key::E)
                    gimbal += 5.f;
                if (key->code == sf::Keyboard::Key::Q)
                    gimbal -= 5.f;

                // Обмеження параметрів
                thrust = std::clamp(thrust, 0.f, 1000.f);
                gimbal = std::clamp(gimbal, -45.f, 45.f);
            }
        }

        // Крок часу (обмежуємо, щоб розрахунки були стабільними)
        float dt = std::min(clock.restart().asSeconds(), 0.05f);

        //  2. Оновлення фізичної моделі 
        if (!paused)
        {
            // Поворот корабля (поки натиснута клавіша)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                angle -= 120.f * dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                angle += 120.f * dt;

            // Прискорення: a = F / m. Опору середовища немає.
            sf::Vector2f acc(0.f, 0.f);
            if (engineOn)
            {
                float phi = toRad(angle + gimbal);        // напрямок тяги
                sf::Vector2f dir(std::sin(phi), -std::cos(phi));
                acc = dir * (thrust / mass);
                engineTime += dt;
            }

            // Чисельне інтегрування: v += a*dt, x += v*dt
            vel += acc * dt;
            sf::Vector2f step = vel * dt;
            pos += step;

            distance += std::sqrt(step.x * step.x + step.y * step.y);
            simTime += dt;

            // Запам'ятовуємо траєкторію
            trajectory.push_back(pos);
            if (trajectory.size() > 3000)
                trajectory.erase(trajectory.begin());
        }

        //  3. Відображення 
        window.clear(sf::Color::Black);

        // Світ: камера в центрі корабля
        view.setCenter(pos);
        window.setView(view);

        // Сітка з точок навколо корабля
        float baseX = std::floor(pos.x / 100.f) * 100.f;
        float baseY = std::floor(pos.y / 100.f) * 100.f;
        for (int i = -6; i <= 6; i++)
            for (int j = -5; j <= 5; j++)
            {
                dot.setPosition({baseX + i * 100.f, baseY + j * 100.f});
                window.draw(dot);
            }

        startMark.setPosition({0.f, 0.f});
        window.draw(startMark);

        trajectoryLine.clear();
        for (const auto& p : trajectory)
            trajectoryLine.append(sf::Vertex{p, sf::Color(0, 200, 255)});
        window.draw(trajectoryLine);

        ship.setPosition(pos);
        ship.setRotation(sf::degrees(angle));
        if (engineOn)
        {
            flame.setPosition(pos);
            flame.setRotation(sf::degrees(angle + gimbal));
            window.draw(flame);
        }
        window.draw(ship);

        // Інтерфейс: повертаємо стандартну камеру
        window.setView(window.getDefaultView());

        float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        char buf[512];
        std::snprintf(buf, sizeof(buf),
            "Engine: %s%s\n"
            "Thrust: %.0f N   Gimbal: %.0f deg   Angle: %.0f deg\n"
            "Speed: %.1f   Vx: %.1f   Vy: %.1f\n"
            "Position: (%.0f; %.0f)   Distance: %.0f\n"
            "Engine time: %.2f s   Sim time: %.2f s\n"
            "[Space] engine  [A/D] rotate  [Q/E] thrust dir  [Up/Down] thrust  [P] pause  [R] restart",
            engineOn ? "ON" : "OFF", paused ? "   (PAUSED)" : "",
            thrust, gimbal, angle,
            speed, vel.x, vel.y,
            pos.x, pos.y, distance,
            engineTime, simTime);
        infoText.setString(buf);
        window.draw(infoText);

        window.display();
    }
    return 0;
}
