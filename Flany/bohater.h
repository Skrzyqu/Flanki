#pragma once
#include <SFML/Graphics.hpp>
#include <string>

/**
 * =========================================================================================
 * HIGH LEVEL OVERVIEW
 * =========================================================================================
 * Klasa reprezentuj¹ca wizualn¹ stronê gracza (Skin/Character).
 * Odpowiada wy³¹cznie za:
 * 1. Wyœwietlanie Sprite'a.
 * 2. Zarz¹dzanie klatkami animacji (Idle, Throw, Run).
 * 3. Proste poruszanie siê (Interpolacja pozycji).
 *
 * Klasa nie zawiera logiki gry (nie wie o wyniku, pi³ce, turach).
 * =========================================================================================
 */
class Bohater {
public:
    sf::Texture tekstura;

    // [ALLOCATION] Przechowywanie ka¿dej klatki jako osobnej sf::Texture jest ma³o wydajne
    // przy du¿ej liczbie assetów (du¿o prze³¹czeñ kontekstu GPU).
    // TODO: W przysz³oœci u¿yæ Texture Atlas (Sprite Sheet) i zmieniaæ TextureRect.
    std::vector<sf::Texture> klatkiAnimacji; // Rzut / Idle
    std::vector<sf::Texture> klatkiBiegania; // Bieg
    sf::Sprite duszek;

    sf::Vector2f pozycjaStartowa;
    bool twarzaWPrawo;

    // --- STANY ANIMACJI (State Flags) ---
    bool czyAnimujeRzut;
    bool czyJestGotowy; // Czy czeka na swoj¹ turê (pozycja bojowa)
    bool czyBiegnie;

    int obecnaKlatka;
    sf::Clock zegarAnimacji;
    float czasMiedzyKlatkami = 0.1f; // Prêdkoœæ odtwarzania (seconds)

    /**
     * @brief Tworzy bohatera w zadanej pozycji.
     * @param x Wspó³rzêdna X ekranu.
     * @param y Wspó³rzêdna Y ekranu.
     */
    Bohater(float x, float y);

    void rysuj(sf::RenderWindow& okno);

    /**
     * @brief Przesuwa bohatera w kierunku celu z zadan¹ prêdkoœci¹.
     * U¿ywa normalizacji wektora do uzyskania sta³ej prêdkoœci.
     */
    void podejdzDo(sf::Vector2f cel, float szybkosc);

    /** Resetuje pozycjê i stan animacji do domyœlnego (Idle). */
    void zresetujPozycje();

    /** Odwraca sprite'a w osi X (Flip). */
    void ustawZwrot(bool wPrawo);

    /**
     * @brief [UPDATE] G³ówna logika animacji.
     * Prze³¹cza tekstury w zale¿noœci od up³ywu czasu i stanu (Rzut > Bieg > Idle).
     */
    void aktualizujAnimacje();

    /** Uruchamia sekwencjê animacji rzutu (One-shot). */
    void wykonajRzut();

    /** Ustawia sprite'a w pozycji "Gotowy do rzutu" lub "Luz". */
    void ustawGotowosc(bool gotowy);

    /** Przerywa animacjê biegu i wraca do Idle. */
    void zatrzymajSie();
};