#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

/**
 * =========================================================================================
 * HIGH LEVEL OVERVIEW
 * =========================================================================================
 * Prosty system UI obsługujący scenę startową (Main Menu).
 * * Odpowiedzialność:
 * 1. Wyświetlanie opcji wyboru trybu gry.
 * 2. Przechwytywanie inputu w fazie przed uruchomieniem głównej pętli gry.
 * 3. Zwracanie kodu sterującego (Exit Code / Game Mode) do funkcji main().
 *
 * [ARCHITECTURAL NOTE]
 * W większym silniku klasa ta dziedziczyłaby po `IScene` lub `IGameState`
 * i była zarządzana przez `SceneManager`.
 * =========================================================================================
 */
class Menu
{
public:
    // [RESOURCE MANAGEMENT]
    // Zasób Font musi być załadowany przed użyciem go w sf::Text.
    // Kolejność deklaracji w klasie C++ determinuje kolejność destrukcji (odwrotną do deklaracji).
    // Text zależy od Font, więc Font powinien być zniszczony ostatni -> zadeklarowany pierwszy.
    sf::Font czcionka;
    sf::Text tekstMenu;

    sf::Texture textureTlo;
    sf::Texture texturePvP;
    sf::Texture texturePvE;
    
    sf::Sprite  spriteTlo;
    sf::Sprite  spritePvP;
    sf::Sprite  spritePvE;

    /**
     * @brief Inicjalizuje widok menu i centruje elementy.
     * @param szerokosc Szerokość okna renderowania (do pozycjonowania UI).
     * @param wysokosc Wysokość okna renderowania.
     */
    Menu(float szerokosc, float wysokosc);

    /**
     * @brief [RENDER] Rysuje klatkę menu (Tło + Tekst).
     */
    void rysuj(sf::RenderWindow& okno);

    /**
     * @brief Przetwarza kolejkę zdarzeń dla fazy menu.
     * * Implementuje prostą logikę wyboru klawiszami 1/2.
     * * @return Kod wyboru:
     * 0 - Brak decyzji (użytkownik myśli / idle).
     * 1 - Wybrano tryb PvP (Gracz vs Gracz).
     * 2 - Wybrano tryb PvE (Gracz vs Bot).
     * -1 - Żądanie zamknięcia aplikacji.
     */
    int sprawdzWybor(sf::RenderWindow& okno);
};
