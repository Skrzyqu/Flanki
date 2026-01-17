#pragma once
#include <SFML/Graphics.hpp>
#include "pocisk.h"
#include "przeszkoda.h"
#include "bohater.h"

// Klasa zarz¹dzaj¹ca logik¹ gry
class Gra
{
public:
	// --- Obiekty gry ---
    pocisk graczLewy;
    Bohater skinLewy;

    pocisk graczPrawy;
    Bohater skinPrawy; 

    przeszkoda puszka;
    przeszkoda podloga;

    // --- PASKI PIWA ---
    // T³o (pusta szklanka) i Wype³nienie (z³ocisty trunek)
    sf::RectangleShape tloPaskaLewego, piwoLewe;
    sf::RectangleShape tloPaskaPrawego, piwoPrawe;
    // Poziom piwa (0 - 100)
    float poziomLewego = 100.0f;
    float poziomPrawego = 100.0f;
    const float LYK_PIWA = 1.0f; // Ile piwa znika po jednym klikniêciu Entera

	// --- UI / HUD ---
    sf::Font czcionka;
    sf::Text napisTury;
    bool czyKoniecGry = false;
    sf::Text napisWygranej;
    sf::Texture teksturaTla;
    sf::Sprite tlo;

	// --- Stany gry ---
    bool turaLewego;
    bool strzalWTok;
    bool fazaBiegania = false;
    bool biegWStronePuszki = true;
	float szybkoscBiegu = 50.0f;

    // --- NOWE ZMIENNE DLA BOTA ---
    bool graZBotem = false; // Czy gramy z komputerem?
    int licznikBota = 0;    // Do symulowania "klikania" przez bota

    // Konstruktor przyjmuje teraz tryb gry
    Gra(bool trybBot);

	// G³ówne funkcje gry
	//funkcja obslugujaca wejscie
    void obsluzWejscie(sf::RenderWindow& okno);
	//funkcja aktualizujaca logike gry
    void aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja);
	//funkcja rysujaca wszystko na ekran
    void rysuj(sf::RenderWindow& okno);

private:
	//funkcje pomocnicze
    void zmienTure(sf::RenderWindow& okno);
    void aktualizujNapis();
	void obsluzBieganie();
    void obsluzPicie();
    void sprawdzWygrana();
    void logikaBota(sf::RenderWindow& okno);
};