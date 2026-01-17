#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "odbicie.h"






//Funkcje sprawdzaj¹ od któryej strony dosz³o do kolizji


bool CzyBok(pocisk* lotka, przeszkoda& puszka)
{
	//Wymiary lotki
	float promienLotki = lotka->lotka.getRadius();
	float srodeklotkiX = lotka->pozycja.x;
	//Wymiary puszki
	float srodekblokuX = puszka.pozycja.x;
	float polowkaSzerokosciBloku = puszka.rozmiar.x / 2.0f;

	//Sprawdzamy czy zderzy³y siê z bokiem puszki

	bool lewaObicie = (srodeklotkiX + promienLotki >= srodekblokuX - polowkaSzerokosciBloku);
	bool prawaObicie = (srodeklotkiX - promienLotki <= srodekblokuX + polowkaSzerokosciBloku);

	return lewaObicie || prawaObicie;

}

bool CzyGoraDol(pocisk* lotka, przeszkoda& puszka)
{
	//Wymiary lotki
	float promienLotki = lotka->lotka.getRadius();
	float srodeklotkiY = lotka->pozycja.y;
	//Wymiary puszki
	float srodekblokuY = puszka.pozycja.y;
	float polowkaWysokosciBloku = puszka.rozmiar.y / 2.0f;
	//Sprawdzamy czy zderzy³y siê z gór¹ puszki
	bool gornaKolizja = (srodeklotkiY + promienLotki >= srodekblokuY - polowkaWysokosciBloku);
	bool dolnaKolizja = (srodeklotkiY - promienLotki <= srodekblokuY + polowkaWysokosciBloku);


	return gornaKolizja || dolnaKolizja;
}



/**
 * @brief Obs³uguje logikê kolizji miêdzy lotk¹ (Okr¹g) a przeszkod¹ (AABB).
 * @param lotka WskaŸnik na obiekt dynamiczny.
 * @param puszka Referencja na obiekt statyczny.
 */
void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka)
{
	// Pobieramy granice (Bounding Boxes)
	sf::FloatRect granicePuszki = puszka.blok.getGlobalBounds();
	sf::FloatRect graniceLotki = lotka->lotka.getGlobalBounds();

	// [COLLISION] U¿ywamy prostego testu AABB (Axis-Aligned Bounding Box).
	// Dla wiêkszej precyzji nale¿a³oby u¿yæ testu Circle-Box, 
	// ale AABB jest wystarczaj¹co szybkie i dobre dla tej gry.
	auto czyNachodza = granicePuszki.findIntersection(graniceLotki);


	if (czyNachodza)
	{
		if (CzyBok(lotka, puszka))
		{
			// Odbicie w osi X
			lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka.sprezystosc; // T³umienie prêdkoœci po odbiciu
		}
		else if (CzyGoraDol(lotka, puszka))
		{
			// Odbicie w osi Y
			lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka.sprezystosc; // T³umienie prêdkoœci po odbiciu
		}
		else 
		{
			// odbicie pod k¹tem (np. róg puszki)
			lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka.sprezystosc;
			lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka.sprezystosc;
		}

		if (puszka.czyPuszka)
		{



			// Logowanie zdarzenia (Debug)
			std::cout << "TRAFIENIE! Resetuje lotke." << std::endl;

			// Zmiana stanu gry (Game Logic wewn¹trz systemu fizyki - trochê brudne, ale dzia³a)
			puszka.czyTrafiona = true;

			// Opcjonalnie: Tutaj mo¿na dodaæ logikê fizycznego odbicia (zmiana wektora prêdkoœci),
			// ale w tej implementacji tylko zaliczamy trafienie.
		}
	}
}




