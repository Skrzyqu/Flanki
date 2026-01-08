#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>

//funkcja sprawdza czy dosz³o do odbuicia od boku

static bool czyOdbicieBok(przeszkoda blok, pocisk lotka)
{
	if (lotka.pozycja.x + lotka.lotka.getRadius() *2 <= blok.pozycja.x || //sprawdzenie czy pocisk dotknal lewej krawedzi przeszkody (puntk x = 0 lotki jest na jej lewym krañcu )
		lotka.pozycja.x <= blok.pozycja.x + blok.rozmiar.x) //&& //sprawdzenie czy pocisk dotknal prawej krawedzi przeszkody
	{
		if (lotka.pozycja.y <= blok.pozycja.y + blok.rozmiar.y && 
			lotka.pozycja.y + lotka.lotka.getRadius() * 2 >= blok.pozycja.y) //sprawdzenie czy pocisk jest w wysokosci przeszkody
		{
			printf("kolizja %f \t %f\n", lotka.pozycja.x, lotka.pozycja.y);
			return true;
		}
	}
	else
	{
		return false; 
	}
}



//funkcja sprawdzajaca czy doszlo do odbicia od gory lub dolu przeszkody
static bool czyOdbicieGoraDol(przeszkoda blok, pocisk lotka)
{
	if (lotka.pozycja.y + lotka.lotka.getRadius() *2 <= blok.pozycja.y || //sprawdzenie czy pocisk dotknal górnej krawedzi przeszkody
		lotka.pozycja.y <= blok.pozycja.y + blok.rozmiar.y) //sprawdzenie czy pocisk dotknal dolnej krawedzi przeszkody
	{
		if (lotka.pozycja.x <= blok.pozycja.x + blok.rozmiar.x &&
			lotka.pozycja.x + lotka.lotka.getRadius() * 2 >= blok.pozycja.x)
		{
			printf("kolizja %f \t %f\n", lotka.pozycja.x, lotka.pozycja.y);
			return true;
		}
	}
	else
	{
		return false; 
	}
}


//funkcja odbicia pocisku od przeszkody
static void odbicie_przeszkoda(pocisk* lotka, przeszkoda blok)
{
	if (czyOdbicieBok(blok, *lotka))
	{
		lotka->predkosc.x = lotka->predkosc.x * -0.85f;
		
	}
	if (czyOdbicieGoraDol(blok, *lotka))
	{
		lotka->predkosc.y = lotka->predkosc.y * -0.85f;

	}
}

