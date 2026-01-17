#include "przeszkoda.h"
#include <cmath>
#include <SFML/Graphics.hpp>

przeszkoda::przeszkoda() : duszek(tekstura)
{
    czyTrafiona = false;
    wTrakcieUpadku = false;
    aktualnyKat = 0.0f;
    predkoscObrotu = 0.0f;
    czyPuszka = false; // Domyœlnie to przeszkoda, nie cel
}

void przeszkoda::przewroc(float predkoscPociskuX)
{
    if (wTrakcieUpadku || std::abs(aktualnyKat) >= 89.0f) return;

    wTrakcieUpadku = true;
    czyTrafiona = true;

    float silaUderzenia = std::abs(predkoscPociskuX);
    float bazowaPredkoscObrotu = silaUderzenia * 0.6f;

    if (bazowaPredkoscObrotu < 2.0f) bazowaPredkoscObrotu = 2.0f;
    if (bazowaPredkoscObrotu > 15.0f) bazowaPredkoscObrotu = 15.0f;

    if (predkoscPociskuX > 0) predkoscObrotu = bazowaPredkoscObrotu;
    else predkoscObrotu = -bazowaPredkoscObrotu;
}

void przeszkoda::aktualizuj()
{
    if (wTrakcieUpadku)
    {
        aktualnyKat += predkoscObrotu;
        duszek.setRotation(sf::degrees(aktualnyKat));
        blok.setRotation(sf::degrees(aktualnyKat));

        if (std::abs(aktualnyKat) >= 90.0f)
        {
            wTrakcieUpadku = false;
            if (aktualnyKat > 0) aktualnyKat = 90.0f;
            else aktualnyKat = -90.0f;

            duszek.setRotation(sf::degrees(aktualnyKat));
            blok.setRotation(sf::degrees(aktualnyKat));
        }
    }
}

void przeszkoda::postaw()
{
    czyTrafiona = false;
    wTrakcieUpadku = false;
    aktualnyKat = 0.0f;
    predkoscObrotu = 0.0f;

    duszek.setRotation(sf::degrees(0.0f));
    blok.setRotation(sf::degrees(0.0f));
}