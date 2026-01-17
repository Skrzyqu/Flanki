#pragma once
#include <SFML/Graphics.hpp>

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

