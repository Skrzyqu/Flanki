#include "przeszkoda.h"
#include "pocisk.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib> 

static void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka)
{
    sf::FloatRect granicePuszki = puszka.blok.getGlobalBounds();
    sf::FloatRect graniceLotki = lotka->lotka.getGlobalBounds();

    // Pobieramy informacje o obszarze przeciêcia (overlap)
    auto intersection = granicePuszki.findIntersection(graniceLotki);

    if (intersection && !puszka.czyTrafiona)
    {
        puszka.czyTrafiona = true;

        float vX = lotka->predkosc.x;
        float vY = lotka->predkosc.y;
        float silaUderzenia = std::abs(vX);
        float chaos = 0.9f + static_cast<float>(rand() % 20) / 100.0f;

        // --- DETEKCJA STRONY KOLIZJI ---
        // Jeœli obszar przeciêcia jest szerszy ni¿ wy¿szy -> uderzenie z góry (lub do³u)
        // Jeœli jest wy¿szy ni¿ szerszy -> uderzenie z boku
        bool uderzenieZGory = intersection->size.x > intersection->size.y;

        if (uderzenieZGory)
        {
            // --- UDERZENIE W WIECZKO / GÓRÊ PUSZKI ---
            std::cout << "TRAFIENIE W WIECZKO" << std::endl;

            // Odbicie w pionie (jak pi³ka od pod³ogi)
            // Odwracamy Y i trochê t³umimy
            lotka->predkosc.y = -vY * 0.5f * chaos;

            // W poziomie: NIE ODWRACAMY! Lotka zeœlizguje siê i leci dalej w t¹ sam¹ stronê.
            // Jedynie lekkie tarcie (zostaje 90% prêdkoœci).
            lotka->predkosc.x = vX * 0.9f;

            // Korekta pozycji: wyci¹gamy lotkê NAD puszkê
            lotka->pozycja.y = granicePuszki.position.y - graniceLotki.size.y - 2.0f;
        }
        else
        {
            // --- UDERZENIE W BOK (STARA LOGIKA) ---
            const float PROG_PRZEBICIA = 13.5f;

            if (silaUderzenia > PROG_PRZEBICIA)
            {
                // MOCNY RZUT (PRZEBICIE)
                lotka->predkosc.x = vX * 0.3f * chaos;
                lotka->predkosc.y = vY * 0.5f;

                // Przy przebiciu NIE cofamy lotki przed puszkê. 
                // Pozwalamy jej "przejœæ" przez obiekt w tej klatce.
            }
            else
            {
                // S£ABY RZUT (ODBICIE)
                lotka->predkosc.x = -vX * 0.4f * chaos; // Tu odwracamy (odbicie w ty³)
                lotka->predkosc.y = -2.0f * chaos;

                // Korekta pozycji: cofamy lotkê PRZED puszkê (tylko przy odbiciu)
                if (vX > 0)
                    lotka->pozycja.x = granicePuszki.position.x - graniceLotki.size.x - 2.0f;
                else
                    lotka->pozycja.x = granicePuszki.position.x + granicePuszki.size.x + 2.0f;
            }
        }
    }
}