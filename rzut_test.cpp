#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode({1920, 1000}), "My window");
    window.setFramerateLimit(60);

    sf::Vector2f anchor{ 150.f, 900.f };

    float maxDragDistance = 100.f;
    float gravity = 0.1f;

    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(anchor);
    ball.setOrigin({ 10.f,10.f });

    sf::Vector2f velocity{ 0.f, 0.f };
    bool isDragging = false;
    bool isFlying = false;

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    isDragging = true;
                    isFlying = false;       // Zatrzymujemy lot
                    velocity = { 0.f, 0.f }; // Zerujemy prêdkoœæ
                }
            }
            if (event->is<sf::Event::MouseButtonReleased>())
            {
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                {
                    isDragging = false;
                    isFlying = true;

                    // Obliczamy wektor: Odleg³oœæ od œrodka do miejsca gdzie puœci³eœ myszkê
                    sf::Vector2f dragVector = anchor - ball.getPosition();

                    // Mno¿nik si³y (0.1f). Im mniejszy, tym wolniej poleci.
                    velocity = dragVector * 0.1f;
                }
            }
        }

        if (isDragging)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f currentMousePos = sf::Vector2f(mousePos);

            // 1. Obliczamy wektor od œrodka do myszki
            sf::Vector2f diff = currentMousePos - anchor;

            // 2. Obliczamy d³ugoœæ tego wektora (Pitagoras: a^2 + b^2 = c^2)
            // sqrt to pierwiastek, diff.x * diff.x to kwadrat
            float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
            // 3. Sprawdzamy czy przekroczyliœmy limit
            if (distance > maxDragDistance)
            {
                // Jeœli tak, musimy "skróciæ" wektor
                // Dzielimy limit przez obecn¹ d³ugoœæ, ¿eby dostaæ skalê (np. 0.5 jeœli jesteœmy 2x za daleko)
                float scale = maxDragDistance / distance;

                // Mno¿ymy wektor przez skalê i dodajemy do œrodka
                sf::Vector2f limitedPos = anchor + (diff * scale);
                ball.setPosition(limitedPos);
            }
            else
            {
                // Jeœli mieœcimy siê w limicie, pi³ka idzie dok³adnie za myszk¹
                ball.setPosition(currentMousePos);
            }
        }
        else if (isFlying)
        {
            // Swobodny lot w nieskoñczonoœæ
            velocity.y += gravity;
            ball.move(velocity);
            
            if (ball.getPosition().y > 1080 + ball.getRadius()) {
                isFlying = false;
                ball.setPosition(anchor);
                velocity = { 0.f, 0.f };
            }
        }

        // --- RYSOWANIE ---
        window.clear();
        if (isDragging) {
            sf::Vertex line[] = { sf::Vertex(anchor, sf::Color::Red), sf::Vertex(ball.getPosition(), sf::Color::Red) };
            window.draw(line, 5, sf::PrimitiveType::Lines);
        }

        // Rysujemy TYLKO pi³kê. ¯adnych linii, ¿adnych kropek pomocniczych.
        window.draw(ball);

        window.display();
    }
}