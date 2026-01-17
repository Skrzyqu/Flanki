#include "Gra.h"
#include <cmath>
#include <iostream>
#include "przeszkoda.h"
#include "odbicie.h"
#include "pocisk.h"
#define srodekX 960.0f
#define srodekY 540.0f
// Poziomy do gry











void Gra::tutorialE2(sf::RenderWindow& okno)
{
	float przesuniecieS1 = 200;

		//sciana 1
		przeszkoda sciana1;
		sciana1.rozmiar = { 30.0f, 280.0f };
		sciana1.pozycja = { srodekX-przesuniecieS1, srodekY +340 };
		sciana1.blok.setSize(sciana1.rozmiar);
		sciana1.blok.setOrigin(sf::Vector2f(sciana1.rozmiar.x / 2.0f, sciana1.rozmiar.y / 2.0f)); // centrowanie
		sciana1.blok.setPosition(sciana1.pozycja);
		sciana1.blok.setFillColor(sf::Color(255, 0, 0));
		sciana1.blok.setOutlineColor(sf::Color::Black);
		sciana1.blok.setOutlineThickness(2.0f);

		//sciana 2

		przeszkoda sciana2;
		sciana2.rozmiar = sciana1.rozmiar;
		sciana2.pozycja = { srodekX + przesuniecieS1, sciana1.pozycja.y };
		sciana2.blok.setSize(sciana2.rozmiar);
		sciana2.blok.setOrigin(sf::Vector2f(sciana2.rozmiar.x / 2.0f, sciana2.rozmiar.y / 2.0f)); // centrowanie
		sciana2.blok.setPosition(sciana2.pozycja);
		sciana2.blok.setFillColor(sf::Color(255, 0, 0));
		sciana2.blok.setOutlineColor(sf::Color::Black);
		sciana2.blok.setOutlineThickness(2.0f);


		//sciana 3
		
		przeszkoda sciana3;
		sciana3.rozmiar = { 30.0f, 500.0f };
		sciana3.pozycja = { srodekX - przesuniecieS1, srodekY - 100 };
		sciana3.blok.setSize(sciana3.rozmiar);
		sciana3.blok.setOrigin(sf::Vector2f(sciana3.rozmiar.x / 2.0f, sciana3.rozmiar.y / 2.0f)); // centrowanie
		sciana3.blok.setPosition(sciana3.pozycja);
		sciana3.blok.setFillColor(sf::Color(255, 0, 0));
		sciana3.blok.setOutlineColor(sf::Color::Black);
		sciana3.blok.setOutlineThickness(2.0f);
		
		przeszkoda sciana4;
		sciana4.rozmiar = sciana3.rozmiar;
		sciana4.pozycja = { srodekX + przesuniecieS1, sciana3.pozycja.y};
		sciana4.blok.setSize(sciana4.rozmiar);
		sciana4.blok.setOrigin(sf::Vector2f(sciana4.rozmiar.x / 2.0f, sciana4.rozmiar.y / 2.0f)); // centrowanie
		sciana4.blok.setPosition(sciana4.pozycja);
		sciana4.blok.setFillColor(sf::Color(255, 0, 0));
		sciana4.blok.setOutlineColor(sf::Color::Black);
		sciana4.blok.setOutlineThickness(2.0f);
		





		okno.draw(sciana2.blok);
		okno.draw(sciana1.blok);
		okno.draw(sciana3.blok);
		okno.draw(sciana4.blok);
		
	

}

void Gra::tutorialE3(sf::RenderWindow& okno)
{
	float przesuniecie1 = 560;//przesuniecie przeszkod od srodka ekranu //trampolinty
	//trampolina 
	przeszkoda trampolinaL;
	trampolinaL.rozmiar = { 100.0f, 10.0f };
	trampolinaL.pozycja = { srodekX - przesuniecie1, srodekY + 480 };
	trampolinaL.blok.setSize(trampolinaL.rozmiar);
	trampolinaL.blok.setOrigin(sf::Vector2f(trampolinaL.rozmiar.x / 2.0f, trampolinaL.rozmiar.y / 2.0f)); // centrowanie
	trampolinaL.blok.setPosition(trampolinaL.pozycja);
	trampolinaL.blok.setFillColor(sf::Color(0, 0, 0)); //czarna
	trampolinaL.blok.setOutlineColor(sf::Color::Yellow);
	trampolinaL.blok.setOutlineThickness(2.0f);
	trampolinaL.sprezystosc = 2.0f; //wspolczynnik odbicia

	przeszkoda trampolinaP;
	trampolinaP.rozmiar = trampolinaL.rozmiar;
	trampolinaP.pozycja = { srodekX + przesuniecie1, trampolinaL.pozycja.y };
	trampolinaP.blok.setSize(trampolinaP.rozmiar);
	trampolinaP.blok.setOrigin(sf::Vector2f(trampolinaP.rozmiar.x / 2.0f, trampolinaP.rozmiar.y / 2.0f)); // centrowanie
	trampolinaP.blok.setPosition(trampolinaP.pozycja);
	trampolinaP.blok.setFillColor(sf::Color(0, 0, 0)); //czarna
	trampolinaP.blok.setOutlineColor(sf::Color::Yellow);
	trampolinaP.blok.setOutlineThickness(2.0f);
	trampolinaP.sprezystosc = 2.0f; //wspolczynnik odbicia


	okno.draw(trampolinaL.blok);
	okno.draw(trampolinaP.blok);

}