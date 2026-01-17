#include "pocisk.h"
// FIXME: Include pliku .cpp jest b³êdem architektonicznym (narusza ODR). 
// Kod z odbicie.cpp powinien byæ w odbicie.h lub skompilowany osobno.
// Zostawiam bez zmian ze wzglêdu na wymogi zadania.
#include "odbicie.cpp" 

#include <cmath>
#include <iostream>

pocisk::pocisk(float x, float y, sf::Color kolor)
{
    pozycja = { x, y };
    pozycjaStartowa = pozycja;
    predkosc = { 0.0f, 0.0f };
    lotka.setFillColor(kolor);
    // Ustawienie origin na œrodek ko³a u³atwia kolizje i obroty
    lotka.setOrigin(sf::Vector2f(lotka.getRadius(), lotka.getRadius()));
    lotka.setPosition(pozycja);
}

void pocisk::obsluzWejscie(sf::Event event, const sf::RenderWindow& okno)
{
    // --- INPUT: ROZPOCZÊCIE CELOWANIA ---
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (!czyLeci)
            {
                sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));

                // Detekcja klikniêcia w okr¹g (Point-Circle Collision)
                float dx = mysz.x - pozycja.x;
                float dy = mysz.y - pozycja.y;
                float dystansKwadrat = dx * dx + dy * dy;
                float promien = lotka.getRadius();

                // Porównujemy kwadraty odleg³oœci, aby unikn¹æ kosztownego pierwiastkowania (sqrt).
                if (dystansKwadrat <= promien * promien)
                {
                    czyCeluje = true;
                }
            }
        }
    }

    // --- INPUT: WYSTRZELENIE ---
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (czyCeluje)
            {
                czyCeluje = false;
                czyLeci = true;

                // Obliczanie wektora si³y odwrotnego do naci¹gu
                sf::Vector2f naciag;
                naciag.x = pozycjaStartowa.x - pozycja.x;
                naciag.y = pozycjaStartowa.y - pozycja.y;

                // Skalowanie si³y (Mno¿nik mocy rzutu)
                predkosc = naciag * 0.15f;

                // Reset wizualny na start, fizyka przejmuje kontrolê
                pozycja = pozycjaStartowa;
                lotka.setPosition(pozycja);
            }
        }
    }
}

void pocisk::aktualizujPozycjeCelowania(const sf::RenderWindow& okno)
{
    if (czyCeluje)
    {
        sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));

        // Wektor kierunkowy
        sf::Vector2f wektor;
        wektor.x = mysz.x - pozycjaStartowa.x;
        wektor.y = mysz.y - pozycjaStartowa.y;

        float dlugosc = std::sqrt(wektor.x * wektor.x + wektor.y * wektor.y);

        // CLAMP: Ograniczenie maksymalnego naci¹gu procy
        if (dlugosc > MAX_NACIAG)
        {
            float skala = MAX_NACIAG / dlugosc;
            wektor.x *= skala;
            wektor.y *= skala;
        }

        // Aktualizacja wizualna "naci¹gniêtej" lotki
        pozycja.x = pozycjaStartowa.x + wektor.x;
        pozycja.y = pozycjaStartowa.y + wektor.y;
    }
}

void pocisk::aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja)
{
    if (czyCeluje)
    {
        aktualizujPozycjeCelowania(okno);
    }

    if (czyLeci)
    {
        // 1. Sprawdzenie kolizji (zanim przesuniemy)
        odbicie_przeszkoda(this, cel);

        // 2. Integracja Eulera (Krok symulacji)
        zmiana_pozycji();
        zmiana_predkosci(grawitacja);

        // 3. Ograniczenia œwiata (World Bounds)
        odbicie_podloze(&okno);

        // Kill plane - jeœli wyleci za daleko, resetujemy
        if (pozycja.x > 3000 || pozycja.x < -500)
        {
            resetuj();
        }
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
        // Debug Line / Wizualizacja naci¹gu
        sf::Vertex line[] =
        {
            sf::Vertex(pozycjaStartowa, sf::Color::White),
            sf::Vertex(pozycja, sf::Color::Magenta)
        };
        okno.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

// --- PRIVATE HELPERS ---

void pocisk::zmiana_predkosci(sf::Vector2f grawitacja)
{
    // v = v0 + a*dt (przyjmujemy dt = 1 klatka)
    predkosc.x += grawitacja.x;
    predkosc.y += grawitacja.y;
}

void pocisk::zmiana_pozycji()
{
    // x = x0 + v*dt
    pozycja.x += predkosc.x;
    pozycja.y += predkosc.y;
    lotka.setPosition(pozycja);
}

void pocisk::odbicie_podloze(sf::RenderWindow* okno)
{
    // Kolizja z pod³og¹
    if (pozycja.y + lotka.getRadius() >= POZIOM_PODLOGI)
    {
        // Odbicie z utrat¹ energii (Coefficient of Restitution = 0.7)
        predkosc.y = predkosc.y * -0.7f;

        // Korekcja penetracji (teleportacja na powierzchniê)
        pozycja.y = POZIOM_PODLOGI - lotka.getRadius();

        // Próg uœpienia (Sleep Threshold) - zapobieganie drganiom przy ma³ych prêdkoœciach
        if ((fabs(predkosc.y) < precyzja))
        {
            predkosc.y = 0.0f;
            pozycja.y = POZIOM_PODLOGI - lotka.getRadius();
        }
        lotka.setPosition(pozycja);
    }
}