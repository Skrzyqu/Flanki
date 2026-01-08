#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "pocisk.h"
#include "przeszkoda.h"
#include "odbicie.cpp"


int main()
{

	//sf::Vector2f gravity{ 0.0f, 9.8f };
	sf::Vector2f gravity{ 0.0f, 0.0f };
	pocisk rzutka;
	
	/*

	rzutka.pozycja = sf::Vector2f{ 100.0f, 300.0f };
	rzutka.predkosc = sf::Vector2f{ 10.0f, 10.0f };
	rzutka.lotka.setFillColor(sf::Color::Red);
	*/

	rzutka.pozycja = sf::Vector2f{ 1100.0f, 600.0f };
	rzutka.predkosc = sf::Vector2f{ -7.0f, -15.0f };
	rzutka.lotka.setFillColor(sf::Color::Red);



	
	przeszkoda kamien;
	kamien.pozycja = sf::Vector2f{ 
		400.0f, 200.0f };
	kamien.rozmiar = sf::Vector2f{ 5000.0f, 16.0f };
	kamien.blok.setSize(kamien.rozmiar);
	kamien.blok.setPosition(kamien.pozycja);
	kamien.blok.setFillColor(sf::Color::Green);


	
	getchar();// uruchomienie dopiero po enter
	sf::RenderWindow okno(sf::VideoMode({ 1200, 800 }), "Lotka");
	okno.setFramerateLimit(60);
	okno.clear();




    while (okno.isOpen())
    {
        while (const std::optional event = okno.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                okno.close();
        }
			okno.draw(kamien.blok);

		
			odbicie_przeszkoda(&rzutka, kamien);
			rzutka.rzut(&rzutka.pozycja, &rzutka.predkosc, &okno, gravity, rzutka.lotka);
			rzutka.odbicie_podloze(&rzutka.pozycja, &rzutka.predkosc, &okno, rzutka.lotka);
		
			
	
	}

	return 0;
}

