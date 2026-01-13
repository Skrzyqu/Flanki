#pragma once
#include <SFML/Graphics.hpp>
#include "pocisk.h"
#include "przeszkoda.h"
// Klasa zarz¹dzaj¹ca logik¹ gry
class Gra
{
public:
	// --- Obiekty gry ---
    pocisk graczLewy;
    pocisk graczPrawy;
    przeszkoda kamien;

	// --- UI / HUD ---
    sf::Font czcionka;
    sf::Text napisTury;

	// --- Stany gry ---
    bool turaLewego;
    bool strzalWTok;

    // Konstruktor
    Gra();

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
};