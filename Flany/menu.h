#pragma once
#include <SFML/Graphics.hpp>

/**
 * =========================================================================================
 * HIGH LEVEL OVERVIEW
 * =========================================================================================
 * Prosty system UI obs³uguj¹cy scenê startow¹ (Main Menu).
 * * Odpowiedzialnoœæ:
 * 1. Wyœwietlanie opcji wyboru trybu gry.
 * 2. Przechwytywanie inputu w fazie przed uruchomieniem g³ównej pêtli gry.
 * 3. Zwracanie kodu steruj¹cego (Exit Code / Game Mode) do funkcji main().
 *
 * [ARCHITECTURAL NOTE]
 * W wiêkszym silniku klasa ta dziedziczy³aby po `IScene` lub `IGameState`
 * i by³a zarz¹dzana przez `SceneManager`.
 * =========================================================================================
 */
class Menu
{
public:
    // [RESOURCE MANAGEMENT]
    // Zasób Font musi byæ za³adowany przed u¿yciem go w sf::Text.
    // Kolejnoœæ deklaracji w klasie C++ determinuje kolejnoœæ destrukcji (odwrotn¹ do deklaracji).
    // Text zale¿y od Font, wiêc Font powinien byæ zniszczony ostatni -> zadeklarowany pierwszy.
    sf::Font czcionka;
    sf::Text tekstMenu;

    /**
     * @brief Inicjalizuje widok menu i centruje elementy.
     * @param szerokosc Szerokoœæ okna renderowania (do pozycjonowania UI).
     * @param wysokosc Wysokoœæ okna renderowania.
     */
    Menu(float szerokosc, float wysokosc);

    /**
     * @brief [RENDER] Rysuje klatkê menu (T³o + Tekst).
     */
    void rysuj(sf::RenderWindow& okno);

    /**
     * @brief Przetwarza kolejkê zdarzeñ dla fazy menu.
     * * Implementuje prost¹ logikê wyboru klawiszami 1/2.
     * * @return Kod wyboru:
     * 0 - Brak decyzji (u¿ytkownik myœli / idle).
     * 1 - Wybrano tryb PvP (Gracz vs Gracz).
     * 2 - Wybrano tryb PvE (Gracz vs Bot).
     * -1 - ¯¹danie zamkniêcia aplikacji.
     */
    int sprawdzWybor(sf::RenderWindow& okno);
};