#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    // Czcionka musi byæ pierwsza (przed tekstem)
    sf::Font czcionka;
    sf::Text tekstMenu;

    Menu(float szerokosc, float wysokosc);

    void rysuj(sf::RenderWindow& okno);

    // Zwraca: 0 (nic nie wybrano), 1 (PvP), 2 (PvE)
    int sprawdzWybor(sf::RenderWindow& okno);
};