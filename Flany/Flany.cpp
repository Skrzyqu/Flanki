#include <SFML/Graphics.hpp>
#include "Gra.h"
#include "Menu.h"
#include <iostream>

/**
 * =========================================================================================
 * ENTRY POINT & BOOTSTRAP
 * =========================================================================================
 * Punkt wejœcia aplikacji. Odpowiada za:
 * 1. Inicjalizacjê okna i kontekstu OpenGL/SFML.
 * 2. Zarz¹dzanie globaln¹ pêtl¹ aplikacji (Application Loop).
 * 3. Proste prze³¹czanie stanów (State Transition): Menu -> Gra.
 *
 * [GLOBAL CONFIG]
 * - Rozdzielczoœæ: 1920x1080 (Hardcoded Full HD).
 * - Limit FPS: 60 (VSync simulation).
 * =========================================================================================
 */
int main()
{
    // Globalna sta³a fizyczna. Wp³ywa na trajektoriê wszystkich obiektów dziedzicz¹cych fizykê.
    // Wektor {0, 0.2} oznacza brak wiatru i si³ê grawitacji skierowan¹ w dó³.
    sf::Vector2f gravity{ 0.0f, 0.2f };

    // [WINDOW INIT]
    // State::Fullscreen przejmuje kontrolê nad ekranem (Exclusive Mode).
    sf::RenderWindow okno(sf::VideoMode({ 1920, 1080 }), "Flanki", sf::State::Fullscreen);
    okno.setFramerateLimit(60); // Zapobiega 100% u¿ycia GPU w prostym menu.
    okno.setKeyRepeatEnabled(false); // Wa¿ne dla inputu typu "One-shot" (np. rzut).

    // --- 1. STATE: MENU G£ÓWNE ---
    Menu menu(1920.0f, 1080.0f);
    int wybranyTryb = 0; // Kody: 0 = Idle, 1 = PvP, 2 = PvE
	int wybranaMapa = 0; // Kody: 0 = Idle, 1 = Mapa1, 2 = Mapa2, 3 = Mapa3

    // Pêtla "Menu Loop" - blokuje przejœcie do gry, dopóki gracz nie wybierze trybu.
    while (okno.isOpen() && wybranyTryb == 0)
    {
        int wybor = menu.sprawdzWybor(okno);
        
        if (wybor >= 21) {
            // Rozpoczêcie gry z map¹
            wybranyTryb = (wybor / 10) % 10; // Cyfra dziesi¹tek (1=PvP, 2=PvE)
            wybranaMapa = wybor % 10;        // Cyfra jednoœci (1,2,3)
            std::cout << "Wybrano tryb: " << wybranyTryb
                << ", mapa: " << wybranaMapa << std::endl;
        }
        else if (wybor == -1) {
            return 0; // Zamkniêcie aplikacji
        }
        else {
            menu.rysuj(okno);
        }
    }

    // Graceful Shutdown w przypadku zamkniêcia okna z poziomu menu (np. Alt+F4).
    if (!okno.isOpen()) return 0;


    // --- 2. STATE: ROZGRYWKA (GAMEPLAY) ---
    // Inicjalizacja "ciê¿kiego" obiektu Gry.
    // [MEMORY] Alokacja zasobów gry nastêpuje dopiero po wyjœciu z menu.
    bool graZBotem = (wybranyTryb == 2);
    Gra gra(graZBotem);

	gra.setWybranyPoziom(wybranaMapa);


    // --- 3. G£ÓWNA PÊTLA GRY (GAME LOOP) ---
    // Standardowy wzorzec: Input -> Update -> Render
    while (okno.isOpen())
    {
        // Krok 1: Przetwarzanie wejœcia (Klawiatura/Mysz)
        gra.obsluzWejscie(okno);

        // Krok 2: Symulacja (Fizyka, AI, Logika gry)
        // [CRITICAL PATH] To miejsce determinuje determinizm rozgrywki.
        gra.aktualizuj(okno, gravity);

        // Krok 3: Rysowanie klatki
        gra.rysuj(okno);
    }
    return 0;
}