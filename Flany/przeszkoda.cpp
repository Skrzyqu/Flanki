#include "przeszkoda.h"
#include <cmath>
#include <algorithm> // Do std::clamp (opcjonalnie) lub if-ów
#include <SFML/Graphics.hpp>

przeszkoda::przeszkoda() : duszek(tekstura)
{
    czyTrafiona = false;
    wTrakcieUpadku = false;
    aktualnyKat = 0.0f;
    predkoscObrotu = 0.0f;
}

void przeszkoda::przewroc(float predkoscPociskuX)
{
    // Jeœli ju¿ le¿y, ignorujemy
    if (wTrakcieUpadku || std::abs(aktualnyKat) >= 89.0f) return;

    wTrakcieUpadku = true;
    czyTrafiona = true;

    // 1. Obliczamy si³ê uderzenia (wartoœæ bezwzglêdna prêdkoœci)
    float silaUderzenia = std::abs(predkoscPociskuX);

    // 2. Skalujemy si³ê na prêdkoœæ obrotu
    // Wspó³czynnik 0.6 oznacza, ¿e np. przy prêdkoœci 15.0f, obrót wyniesie 9 stopni/klatkê.
    float bazowaPredkoscObrotu = silaUderzenia * 0.6f;

    // 3. Zak³adamy "kaganiec" (Limity prêdkoœci)
    // Minimum 2.0 stopnie (¿eby nie przewraca³a siê wieki)
    // Maksimum 15.0 stopni (¿eby animacja by³a czytelna)
    if (bazowaPredkoscObrotu < 2.0f) bazowaPredkoscObrotu = 2.0f;
    if (bazowaPredkoscObrotu > 15.0f) bazowaPredkoscObrotu = 15.0f;

    // 4. Ustalamy kierunek zgodnie z uderzeniem
    if (predkoscPociskuX > 0)
    {
        // Uderzenie w prawo -> obrót dodatni
        predkoscObrotu = bazowaPredkoscObrotu;
    }
    else
    {
        // Uderzenie w lewo -> obrót ujemny
        predkoscObrotu = -bazowaPredkoscObrotu;
    }
}

void przeszkoda::aktualizuj()
{
    if (wTrakcieUpadku)
    {
        aktualnyKat += predkoscObrotu;

        // SFML 3.0: Konwersja float -> sf::Angle
        duszek.setRotation(sf::degrees(aktualnyKat));
        blok.setRotation(sf::degrees(aktualnyKat));

        // Sprawdzamy czy puszka le¿y (90 stopni)
        if (std::abs(aktualnyKat) >= 90.0f)
        {
            wTrakcieUpadku = false;

            // "Dopinamy" do idealnego poziomu
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

    // Reset do pionu
    duszek.setRotation(sf::degrees(0.0f));
    blok.setRotation(sf::degrees(0.0f));
}