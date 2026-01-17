#pragma once
#include <SFML/Graphics.hpp>
#include "przeszkoda.h"

constexpr auto precyzja = 36.32f; //wartosc przy której pocisk nie odbija sie juz od podloza
constexpr float MAX_NACIAG = 100.0f; //maksymalny naciag luku
constexpr float POZIOM_PODLOGI = 1030.0f; //poziom podlogi w oknie gry

//klasa zarzadzajaca pociskiem
class pocisk
	{
	public:
		// --- FIZYKA POCISKU ---
		sf::Vector2f pozycja;
		sf::Vector2f predkosc;
		sf::CircleShape lotka{ 10.0f };

		// --- STANY POCISKU ---
		bool czyLeci = false;
		bool czyCeluje = false;
		sf::Vector2f pozycjaStartowa;

		//konstruktor
		pocisk(float x, float y, sf::Color kolor);

		//funkcja obslugujaca myszke
		void obsluzWejscie(sf::Event event, const sf::RenderWindow& okno);
		//glowna petla fizyki dla pocisku
		void aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja);
		//rysowanie bialej linii naciagu
		void rysujCelowanie(sf::RenderWindow& okno);

		void resetuj();
	private:
		//funkcje pomocnicze
		void zmiana_pozycji();
		void zmiana_predkosci(sf::Vector2f grawitacja);
		void odbicie_podloze(sf::RenderWindow* okno);
		void aktualizujPozycjeCelowania(const sf::RenderWindow& okno);
		
		/*
		// =============================================================
		//funkcja zmieniajaca predkosc pocisku
		sf::Vector2f zmiana_predkosci(sf::Vector2f* predkosc, sf::Vector2f* sila)
		{
			predkosc->x += sila->x; //zmianana predkosci w osi x
			predkosc->y += sila->y; //zmianana predkosci w osi y
			return *predkosc;
		}

		// =============================================================
		//funkcja zmieniajaca pozycje pocisku
		sf::Vector2f zmiana_pozycji(sf::Vector2f* pozycja, sf::Vector2f* predkosc)
		{
			pozycja->x += predkosc->x; //zmianana pozycji w osi x
			pozycja->y += predkosc->y; //zmianana pozycji w osi y

			return *pozycja;
		}


		// =============================================================
		//funkcja obrazujaca rzut pocisku
		void rzut(sf::Vector2f* pozycja, sf::Vector2f* predkosc, sf::RenderWindow* okno, sf::Vector2f grawitacja, sf::CircleShape lotka)
		{

			lotka.setPosition(*pozycja);
			okno->draw(lotka);
			lotka.move(zmiana_pozycji(pozycja, predkosc));
			okno->display();
			zmiana_predkosci(predkosc, &grawitacja);
			okno->clear(sf::Color::Black);


		}
		// =============================================================
		//odbice od pod³o¿a
		void odbicie_podloze(sf::Vector2f* pozycja, sf::Vector2f* predkosc, sf::RenderWindow* okno, sf::CircleShape pocisk)
		{

			if (pozycja->y + pocisk.getRadius() * 2 >= okno->getSize().y) //sprawdzenie czy pocisk dotknal dolnej krawedzi okna
			{
				predkosc->y = predkosc->y * -0.7f; //odbijanie sie od dolnej krawedzi okna z utrata predkosci
				pozycja->y = okno->getSize().y - pocisk.getRadius() * 2; //ustawienie pocisku na samej krawedzi okna
			//	std::cout << predkosc->y << std::endl;

				if ((fabs(predkosc->y) < precyzja))
				{
					predkosc->y = 0.0f; //zerowanie predkosci w osi y gdy jest bardzo mala
					pozycja->y = okno->getSize().y - pocisk.getRadius() * 2; //ustawienie pocisku na samej krawedzi okna

				}

			}
		} */
	}; 