#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "pocisk.h"
#include "przeszkoda.h"
#include "odbicie.cpp"


int main()
{

	sf::Vector2f gravity{ 0.0f, 9.8f };
	pocisk rzutka;
	rzutka.pozycja = sf::Vector2f{ 100.0f, 400.0f };
	rzutka.predkosc = sf::Vector2f{ 10.0f, -10.0f };
	rzutka.lotka.setFillColor(sf::Color::Red);
	
	
	przeszkoda kamien;
	kamien.pozycja = sf::Vector2f{ 300.0f, 500.0f };
	kamien.rozmiar = sf::Vector2f{ 20.0f, 160000.0f };
	kamien.blok.setSize(kamien.rozmiar);
	kamien.blok.setPosition(kamien.pozycja);
	kamien.blok.setFillColor(sf::Color::Green);


	
	getchar();
	sf::RenderWindow okno(sf::VideoMode({ 2400, 1600 }), "Lotka");
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

