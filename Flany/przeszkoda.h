#pragma once
#include <SFML/Graphics.hpp>
//kalsa odpowaiadajaca za przeszkody
class przeszkoda
{
public:
	sf::Vector2f pozycja;
	sf::Vector2f rozmiar;
	sf::RectangleShape blok;
	//bool sprezyste; na razie nieuzywane
	//int hp;	na razie nieuzywane
	//float massa; na razie nieuzywane

};

