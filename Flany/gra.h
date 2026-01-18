#pragma once
#include <SFML/Graphics.hpp>
#include "pocisk.h"
#include "przeszkoda.h"
#include "bohater.h"

class Gra
{
public:
    // Aktorzy
    pocisk graczLewy;
    Bohater skinLewy;
    pocisk graczPrawy;
    Bohater skinPrawy;

    // Otoczenie
    przeszkoda puszka;
    przeszkoda podloga;

    // UI
    sf::RectangleShape tloPaskaLewego, piwoLewe;
    sf::RectangleShape tloPaskaPrawego, piwoPrawe;

    float poziomLewego = 100.0f;
    float poziomPrawego = 100.0f;
    const float LYK_PIWA = 1.0f;

    // Render
    sf::Font czcionka;
    sf::Text napisTury;
    bool czyKoniecGry = false;
    sf::Text napisWygranej;
    sf::Texture teksturaTla;
    sf::Texture teksturaLotki; // Dodane, bo u¿ywasz w konstruktorze
    sf::Sprite tlo;

    // State Machine
    bool turaLewego;
    bool strzalWTok;
    bool fazaBiegania = false;
    bool biegWStronePuszki = true;
    float szybkoscBiegu = 50.0f;

    // AI
    bool graZBotem = false;
    int licznikBota = 0;

    Gra(bool trybBot);

    void obsluzWejscie(sf::RenderWindow& okno);
    void aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja);
    void rysuj(sf::RenderWindow& okno);

    // --- NOWE POZIOMY (KOLEGA) ---
    void tutorialE2(sf::RenderWindow& okno);
    void tutorialE3(sf::RenderWindow& okno);
	void tutorialE4(sf::RenderWindow& okno);


private:
    void zmienTure(sf::RenderWindow& okno);
    void aktualizujNapis();
    void obsluzBieganie();
    void obsluzPicie();
    void sprawdzWygrana();
    void logikaBota(sf::RenderWindow& okno);
};