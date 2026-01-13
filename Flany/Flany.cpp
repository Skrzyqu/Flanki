#include <SFML/Graphics.hpp>
#include "Gra.h"

int main()
{
	// Ustawienia okna
    sf::Vector2f gravity{ 0.0f, 0.4f };
    sf::RenderWindow okno(sf::VideoMode({ 1920, 1080 }), "Flanki", sf::State::Fullscreen);
    okno.setFramerateLimit(60);

    // Tworzymy silnik gry
    Gra gra;

    // G³ówna pêtla
    while (okno.isOpen())
    {
		// Pobranie zdarzeñ wejœciowych
        gra.obsluzWejscie(okno);
		// Przeliczanie fizyki i logiki gry
        gra.aktualizuj(okno, gravity);
		// Narysowanie klatki gry
        gra.rysuj(okno);
    }

    return 0;
}