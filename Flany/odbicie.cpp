#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>
#include <iostream>

//funkcja odbicia pocisku od przeszkody

/*static bool czyOdbicie(przeszkoda blok, pocisk lotka)
{
	if (lotka.pozycja.x + lotka.lotka.getRadius() *2 >= blok.pozycja.x && //sprawdzenie czy pocisk dotknal lewej krawedzi przeszkody
		lotka.pozycja.x <= blok.pozycja.x + blok.rozmiar.x && //sprawdzenie czy pocisk dotknal prawej krawedzi przeszkody
		lotka.pozycja.y + lotka.lotka.getRadius() *2 >= blok.pozycja.y && //sprawdzenie czy pocisk dotknal górnej krawedzi przeszkody
		lotka.pozycja.y <= blok.pozycja.y + blok.rozmiar.y) //sprawdzenie czy pocisk dotknal dolnej krawedzi przeszkody
	{
		
		printf("kolizja %f \t %f\n", lotka.pozycja.x, lotka.pozycja.y);
		return true; //doszlo do odbicia
	}
	else
	{
		return false; //nie doszlo do odbicia
	}
}

static bool czyOdbicie(const przeszkoda& blok, const pocisk& lotka)
{
	sf::FloatRect boundsPuszki = blok.blok.getGlobalBounds();
	sf::FloatRect boundsLotki = lotka.lotka.getGlobalBounds();

	return boundsPuszki.findIntersection(boundsLotki).has_value();
}

//funkcja odbicia pocisku od przeszkody
static void odbicie_przeszkoda(pocisk* lotka, przeszkoda puszka)
{
	if (czyOdbicie(puszka, *lotka))
	{
		lotka->predkosc.x *= -0.6f;
		lotka->predkosc.y *= -0.6f;

		lotka->pozycja.y -= 2.0f;
	}
}*/
static void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka)
{
	// Pobieramy granice
	sf::FloatRect granicePuszki = puszka.blok.getGlobalBounds();
	sf::FloatRect graniceLotki = lotka->lotka.getGlobalBounds();

	// Sprawdzamy czy na siebie nachodz¹
	auto czyNachodza = granicePuszki.findIntersection(graniceLotki);

	if (czyNachodza) // Jeœli jest kolizja
	{
		// 1. Wypisz w terminalu
		std::cout << "TRAFIENIE! Resetuje lotke." << std::endl;

		// 2. Zmieñ kolor puszki (opcjonalne, dla efektu)
		puszka.czyTrafiona = true;

		// 3. ZRESETUJ LOTKÊ (wraca na start i przestaje lecieæ)
		//lotka->resetuj();
	}
}