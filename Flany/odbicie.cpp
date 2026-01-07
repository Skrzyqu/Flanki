#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>

//funkcja odbicia pocisku od przeszkody

static bool czyOdbicie(przeszkoda blok, pocisk lotka)
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

//funkcja odbicia pocisku od przeszkody
static void odbicie_przeszkoda(pocisk* lotka, przeszkoda blok)
{
	if (czyOdbicie(blok, *lotka))
	{
		lotka->predkosc.x = lotka->predkosc.x * -0.85f;
		lotka->predkosc.y = lotka->predkosc.y * -0.85f;
	}
}