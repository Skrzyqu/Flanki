#pragma once
#include <SFML/Graphics.hpp>
#include "pocisk.h"
#include "przeszkoda.h"
#include "bohater.h"

/**
 * =========================================================================================
 * HIGH LEVEL OVERVIEW
 * =========================================================================================
 * G³ówna klasa zarz¹dzaj¹ca cyklem ¿ycia aplikacji (Game Loop).
 * Odpowiada za:
 * 1. Zarz¹dzanie stanem gry (State Machine: Tura gracza -> Lot pocisku -> Tura przeciwnika).
 * 2. Obs³ugê wejœcia (Input Handling) i przekazywanie go do odpowiednich aktorów.
 * 3. Renderowanie wszystkich warstw (T³o -> UI -> Aktorzy).
 *
 * Zale¿noœci:
 * - Rozmawia bezpoœrednio z klasami fizycznymi (pocisk, przeszkoda) i wizualnymi (bohater).
 * =========================================================================================
 */
class Gra
{
public:
    // --- Aktorzy Sceny ---
    /** Obiekt fizyczny lotki gracza lewego (Czerwony). */
    pocisk graczLewy;
    /** Wizualna reprezentacja gracza lewego (Sprite + Animacje). */
    Bohater skinLewy;

    /** Obiekt fizyczny lotki gracza prawego (Niebieski). */
    pocisk graczPrawy;
    /** Wizualna reprezentacja gracza prawego. */
    Bohater skinPrawy;

    // --- Otoczenie ---
    /** Cel g³ówny (puszka na œrodku). S³u¿y do detekcji kolizji AABB. */
    przeszkoda puszka;
    /** Statyczna kolizja pod³o¿a. */
    przeszkoda podloga;

    // --- UI / PASKI ZYCIA (MECHANIKA PIWA) ---
    // [TODO]: W przysz³oœci przenieœæ UI do osobnej klasy `HudManager` dla czystoœci kodu.
    sf::RectangleShape tloPaskaLewego, piwoLewe;
    sf::RectangleShape tloPaskaPrawego, piwoPrawe;

    // Poziom "¿ycia" (0.0f - 100.0f). Warunek przegranej: <= 0.
    float poziomLewego = 100.0f;
    float poziomPrawego = 100.0f;
    const float LYK_PIWA = 1.0f; // Delta obra¿eñ na turê/akcjê.

    // --- RENDER & TEXT ---
    sf::Font czcionka;
    sf::Text napisTury;
    bool czyKoniecGry = false;
    sf::Text napisWygranej;
    sf::Texture teksturaTla;
    sf::Sprite tlo;

    // --- STATE MACHINE ---
    bool turaLewego;             ///< true = tura gracza Czerwonego, false = tura Niebieskiego/Bota
    bool strzalWTok;             ///< true = blokada inputu, symulacja fizyki w toku
    bool fazaBiegania = false;   ///< true = tryb "karny", gracze zmieniaj¹ pozycje
    bool biegWStronePuszki = true;
    float szybkoscBiegu = 50.0f;

    // --- LOGIKA AI (BOT) ---
    bool graZBotem = false;
    int licznikBota = 0;    ///< Licznik klatek do opóŸniania reakcji AI (symulacja czasu reakcji cz³owieka).

    /**
     * @brief Inicjalizuje zasoby gry i ustawia stan pocz¹tkowy.
     * @param trybBot Jeœli true, prawy gracz jest sterowany przez proste AI.
     */
    Gra(bool trybBot);

    // --- G£ÓWNA PÊTLA ---

    /**
     * @brief Przetwarza zdarzenia systemowe (klawiatura, mysz).
     * Decyduje, czy input trafia do mechaniki picia, biegania czy celowania.
     */
    void obsluzWejscie(sf::RenderWindow& okno);

    /**
     * @brief [CRITICAL PATH] G³ówna logika symulacji.
     * Aktualizuje fizykê, animacje i AI. Wywo³ywana co klatkê.
     * @param grawitacja Wektor si³y grawitacji aplikowany do pocisków.
     */
    void aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja);

    /**
     * @brief [RENDER] Rysuje bie¿¹cy stan gry.
     * Kolejnoœæ rysowania: T³o -> Œwiat -> UI.
     */
    void rysuj(sf::RenderWindow& okno);

    //Poziomy do gry
    void tutorialE2(sf::RenderWindow& okno);
    void tutorialE3(sf::RenderWindow& okno);
private:
    // --- METODY POMOCNICZE (INTERNAL LOGIC) ---

    /** Resetuje pozycje lotek, prze³¹cza flagê tury i resetuje liczniki AI. */
    void zmienTure(sf::RenderWindow& okno);

    /** Odœwie¿a teksty UI w zale¿noœci od stanu gry. */
    void aktualizujNapis();

    /** Logika ruchu postaci w fazie karnej (interpolacja pozycji). */
    void obsluzBieganie();

    /** Logika mechaniki "picia" (zmniejszanie pasków ¿ycia). */
    void obsluzPicie();

    /** Sprawdza warunki zwyciêstwa (hp <= 0) i ustawia flagê koñca gry. */
    void sprawdzWygrana();

    /** Prosta maszyna stanów dla przeciwnika komputerowego (Celowanie -> Strza³ -> Reakcja). */
    void logikaBota(sf::RenderWindow& okno);
};