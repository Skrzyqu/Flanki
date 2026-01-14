#include <SFML/Graphics.hpp>
#include "Gra.h"
#include "Menu.h" // <--- Dodajemy nag³ówek Menu

int main()
{
    sf::Vector2f gravity{ 0.0f, 0.2f };

    sf::RenderWindow okno(sf::VideoMode({ 1920, 1080 }), "Flanki", sf::State::Fullscreen);
    okno.setFramerateLimit(60);
    okno.setKeyRepeatEnabled(false);

    // 1. FAZA MENU
    Menu menu(1920.0f, 1080.0f);
    int wybranyTryb = 0; // 0 = nic, 1 = PvP, 2 = PvE

    while (okno.isOpen() && wybranyTryb == 0)
    {
        wybranyTryb = menu.sprawdzWybor(okno);
        menu.rysuj(okno);
    }

    // Jeœli okno zosta³o zamkniête w menu, nie uruchamiamy gry
    if (!okno.isOpen()) return 0;


    // 2. KONFIGURACJA GRY
    bool graZBotem = (wybranyTryb == 2);
    Gra gra(graZBotem);


    // 3. PÊTLA GRY
    while (okno.isOpen())
    {
        gra.obsluzWejscie(okno);
        gra.aktualizuj(okno, gravity);
        gra.rysuj(okno);
    }

    return 0;
}