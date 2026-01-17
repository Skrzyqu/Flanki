#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>
#include <iostream>

/**
 * @brief Obs³uguje logikê kolizji miêdzy lotk¹ (Okr¹g) a przeszkod¹ (AABB).
 * @param lotka WskaŸnik na obiekt dynamiczny.
 * @param puszka Referencja na obiekt statyczny.
 */
static void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka)
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
		// Logowanie zdarzenia (Debug)
		std::cout << "TRAFIENIE! Resetuje lotke." << std::endl;

		// Zmiana stanu gry (Game Logic wewn¹trz systemu fizyki - trochê brudne, ale dzia³a)
		puszka.czyTrafiona = true;

		// Opcjonalnie: Tutaj mo¿na dodaæ logikê fizycznego odbicia (zmiana wektora prêdkoœci),
		// ale w tej implementacji tylko zaliczamy trafienie.
	}
}