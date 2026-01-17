#pragma once
#include <SFML/Graphics.hpp>
#include "przeszkoda.h"

constexpr auto precyzja = 1.0f;       // Próg zatrzymania
constexpr float MAX_NACIAG = 100.0f;
constexpr float POZIOM_PODLOGI = 1030.0f;

class pocisk
{
public:
    sf::Vector2f pozycja;
    sf::Vector2f predkosc;
    sf::CircleShape lotka{ 10.0f };

    bool czyLeci = false;
    bool czyCeluje = false;
    sf::Vector2f pozycjaStartowa;

    pocisk(float x, float y, sf::Color kolor);

    void obsluzWejscie(sf::Event event, const sf::RenderWindow& okno);
    void aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja);
    void rysujCelowanie(sf::RenderWindow& okno);
    void resetuj();

private:
    void zmiana_pozycji();
    void zmiana_predkosci(sf::Vector2f grawitacja);
    void odbicie_podloze(sf::RenderWindow* okno);
    void aktualizujPozycjeCelowania(const sf::RenderWindow& okno);
};