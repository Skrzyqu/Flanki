#include "odbicie.h"
#include "pocisk.h"     
#include "przeszkoda.h" 
#include <iostream>
#include <cmath>

bool CzyBok(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiX = lotka->pozycja.x;
    float srodekblokuX = puszka.pozycja.x;
    float polowkaSzerokosciBloku = puszka.rozmiar.x / 2.0f;

    // Margines b³êdu dla detekcji boków
    bool lewaObicie = (srodeklotkiX + promienLotki >= srodekblokuX - polowkaSzerokosciBloku);
    bool prawaObicie = (srodeklotkiX - promienLotki <= srodekblokuX + polowkaSzerokosciBloku);

    return lewaObicie || prawaObicie;
}

bool CzyGoraDol(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiY = lotka->pozycja.y;
    float srodekblokuY = puszka.pozycja.y;
    float polowkaWysokosciBloku = puszka.rozmiar.y / 2.0f;

    bool gornaKolizja = (srodeklotkiY + promienLotki >= srodekblokuY - polowkaWysokosciBloku);
    bool dolnaKolizja = (srodeklotkiY - promienLotki <= srodekblokuY + polowkaWysokosciBloku);

    return gornaKolizja || dolnaKolizja;
}

void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka)
{
    sf::FloatRect granicePuszki = puszka.blok.getGlobalBounds();
    sf::FloatRect graniceLotki = lotka->lotka.getGlobalBounds();

    auto czyNachodza = granicePuszki.findIntersection(graniceLotki);

    if (czyNachodza)
    {
        // --- NAPRAWA: ZAPISUJEMY SI£Ê PRZED ODBICIEM ---
        // Musimy wiedzieæ jak mocno i w któr¹ stronê lecia³a lotka W MOMENCIE uderzenia,
        // zanim fizyka zmieni ten wektor na przeciwny.
        float silaUderzeniaX = lotka->predkosc.x;

        // --- FIZYKA ODBICIA (KOLEGA) ---
        // Tutaj zmieniamy prêdkoœæ lotki (odbicie)
        if (CzyBok(lotka, puszka))
        {
            lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka.sprezystosc;
        }
        else if (CzyGoraDol(lotka, puszka))
        {
            lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka.sprezystosc;
        }
        else
        {
            // Odbicie od rogu
            lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka.sprezystosc;
            lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka.sprezystosc;
        }

        // --- LOGIKA TRAFIENIA I ANIMACJI (TY) ---
        // Reagujemy tylko jeœli to jest cel gry (puszka), a nie œciana
        if (puszka.czyPuszka && !puszka.czyTrafiona)
        {
            std::cout << "TRAFIENIE! Przewracam puszke." << std::endl;
            puszka.czyTrafiona = true;

            // --- NAPRAWA: PRZEKAZUJEMY ORYGINALN¥ PRÊDKOŒÆ ---
            // U¿ywamy zmiennej silaUderzeniaX, a nie lotka->predkosc.x (która jest ju¿ odwrócona przez kod wy¿ej)
            puszka.przewroc(silaUderzeniaX);
        }
    }
}