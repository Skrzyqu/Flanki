#include "pocisk.h"
#include "odbicie.h" // U¿ywamy nag³ówka!
#include <cmath>
#include <iostream>

pocisk::pocisk(float x, float y, sf::Color kolor)
{
    pozycja = { x, y };
    pozycjaStartowa = pozycja;
    predkosc = { 0.0f, 0.0f };
    lotka.setFillColor(kolor);
    lotka.setOrigin(sf::Vector2f(lotka.getRadius(), lotka.getRadius()));
    lotka.setPosition(pozycja);
}

void pocisk::obsluzWejscie(sf::Event event, const sf::RenderWindow& okno)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left && !czyLeci)
        {
            sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
            float dx = mysz.x - pozycja.x;
            float dy = mysz.y - pozycja.y;
            if ((dx * dx + dy * dy) <= (lotka.getRadius() * lotka.getRadius()))
            {
                czyCeluje = true;
            }
        }
    }

    if (event.is<sf::Event::MouseButtonReleased>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseEvent->button == sf::Mouse::Button::Left && czyCeluje)
        {
            czyCeluje = false;
            czyLeci = true;
            sf::Vector2f naciag = pozycjaStartowa - pozycja;
            predkosc = naciag * 0.15f;
            pozycja = pozycjaStartowa;
            lotka.setPosition(pozycja);
        }
    }
}

void pocisk::aktualizujPozycjeCelowania(const sf::RenderWindow& okno)
{
    if (czyCeluje)
    {
        sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
        sf::Vector2f wektor = mysz - pozycjaStartowa;
        float dlugosc = std::sqrt(wektor.x * wektor.x + wektor.y * wektor.y);

        if (dlugosc > MAX_NACIAG)
        {
            float skala = MAX_NACIAG / dlugosc;
            wektor *= skala;
        }
        pozycja = pozycjaStartowa + wektor;
    }
}

void pocisk::aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja)
{
    if (czyCeluje) aktualizujPozycjeCelowania(okno);

    if (czyLeci)
    {
        // Wywo³ujemy funkcjê z odbicie.cpp
        odbicie_przeszkoda(this, &cel);

        zmiana_pozycji();
        zmiana_predkosci(grawitacja);
        odbicie_podloze(&okno);

        // Kill plane
        if (pozycja.x > 3000 || pozycja.x < -500) resetuj();
    }
}

void pocisk::resetuj()
{
    czyLeci = false;
    predkosc = { 0, 0 };
    pozycja = pozycjaStartowa;
    lotka.setPosition(pozycja);
}

void pocisk::rysujCelowanie(sf::RenderWindow& okno)
{
    if (czyCeluje)
    {
        sf::Vertex line[] = {
            sf::Vertex(pozycjaStartowa, sf::Color::White),
            sf::Vertex(pozycja, sf::Color::Magenta)
        };
        okno.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void pocisk::zmiana_predkosci(sf::Vector2f grawitacja)
{
    predkosc += grawitacja;
}

void pocisk::zmiana_pozycji()
{
    pozycja += predkosc;
    lotka.setPosition(pozycja);
}

void pocisk::odbicie_podloze(sf::RenderWindow* okno)
{
    // --- LOGIKA FIZYCZNA KOLEGI (Tarcie i toczenie) ---
    if (pozycja.y + lotka.getRadius() >= POZIOM_PODLOGI)
    {
        // Odbicie z t³umieniem
        predkosc.y = predkosc.y * -0.3f;
        pozycja.y = POZIOM_PODLOGI - lotka.getRadius();

        // Jeœli prêdkoœæ pionowa jest bardzo ma³a -> toczenie
        if (std::abs(predkosc.y) < precyzja)
        {
            predkosc.y = 0.0f;
            pozycja.y = POZIOM_PODLOGI - lotka.getRadius();

            // TARCIE (Zwalnianie w poziomie)
            if (std::abs(predkosc.x) > 0.0f)
            {
                if (predkosc.x > 0)
                {
                    predkosc.x -= 0.3f;
                    if (predkosc.x <= 0) predkosc.x = 0.0f;
                }
                else
                {
                    predkosc.x += 0.3f;
                    if (predkosc.x >= 0) predkosc.x = 0.0f;
                }
            }
            else
            {
                // ZATRZYMANIE (Zamiast Sleep() po prostu wy³¹czamy fizykê)
                czyLeci = false;
            }
        }
        lotka.setPosition(pozycja);
    }
}