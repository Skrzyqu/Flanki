#pragma once
#include <SFML/Graphics.hpp>

<<<<<<< HEAD
class przeszkoda
{
public:
    sf::Vector2f pozycja;
    sf::Vector2f rozmiar; // Oryginalny rozmiar (stoj¹cy)
    sf::RectangleShape blok;
    sf::Texture tekstura;
    sf::Sprite duszek;
=======
//klasa odpowaiadajaca za przeszkody
class przeszkoda
{
public:
	sf::Vector2f pozycja;
	sf::Vector2f rozmiar;
	sf::RectangleShape blok;
	bool czyTrafiona = false;
	bool czyPuszka = false;
	
	float sprezystosc = 1.0; //wspolczynnik sprezystosci przy odbiciu domyslnie 1.0 (bez utraty energii)
	//int hp;	na razie nieuzywane
	//float massa; na razie nieuzywane
	sf::Texture tekstura;
	sf::Sprite duszek;
	przeszkoda() : duszek(tekstura) {}


	

};
>>>>>>> Odbicia-+-koncept-poziomu

    bool czyTrafiona = false;

    // --- NOWE ZMIENNE DO ANIMACJI ---
    bool wTrakcieUpadku = false;
    float aktualnyKat = 0.0f;
    float predkoscObrotu = 0.0f; // Ile stopni na klatkê

    przeszkoda();

    // Wyzwala animacjê upadku (zale¿nie od kierunku uderzenia)
    void przewroc(float predkoscPociskuX);

    // Aktualizuje klatkê animacji (obrót)
    void aktualizuj();

    // Resetuje puszkê do pionu
    void postaw();
};
