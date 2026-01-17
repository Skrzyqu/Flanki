#include "przeszkoda.h"
#include <iostream>
#include <cmath> // Potrzebne do funkcji abs()

// Konstruktor
przeszkoda::przeszkoda()
{
    // 1. £adowanie tekstury
    if (!tekstura.loadFromFile("beer.png"))
    {
        std::cerr << "Blad ladowania tekstury puszki (beer.png)!" << std::endl;
    }

    // 2. Konfiguracja duszka (wygl¹d)
    duszek.setTexture(tekstura, true);
    duszek.setScale({ 1.5f, 1.5f }); // Dopasuj skalê jeœli puszka jest za ma³a/du¿a

    // 3. Pobieramy wymiary po przeskalowaniu
    sf::FloatRect bounds = duszek.getGlobalBounds();
    wymiaryStojace = { bounds.size.x, bounds.size.y };

    // 4. Konfiguracja hitboxa (fizyka)
    blok.setSize(wymiaryStojace);
    blok.setFillColor(sf::Color::Transparent); // Hitbox jest niewidzialny (widaæ duszka)
    // Opcjonalnie do testów w³¹cz kolor: blok.setFillColor(sf::Color(255, 0, 0, 100));

    // 5. Ustawienie punktu zaczepienia (Origin) na ŒRODKU PODSTAWY
    // Dziêki temu puszka obraca siê wokó³ denka, a nie lewego górnego rogu
    duszek.setOrigin({ duszek.getLocalBounds().size.x / 2.0f, duszek.getLocalBounds().size.y });
    blok.setOrigin({ wymiaryStojace.x / 2.0f, wymiaryStojace.y });

    // Domyœlna pozycja (mo¿na nadpisaæ w gra.cpp)
    pozycja = { 960.0f, 980.0f }; // Œrodek ekranu, na pod³odze
    duszek.setPosition(pozycja);
    blok.setPosition(pozycja);

    // Inicjalizacja zmiennych
    czyTrafiona = false;
    wTrakcieUpadku = false;
    aktualnyKat = 0.0f;
    krokObrotu = 0.0f;
}

// Funkcja wywo³ywana w momencie trafienia
void przeszkoda::przewroc(float xPocisku)
{
    // Jeœli ju¿ oberwa³a, ignorujemy kolejne trafienia
    if (czyTrafiona) return;

    czyTrafiona = true;
    wTrakcieUpadku = true;

    // Pobieramy œrodek puszki
    float xPuszki = pozycja.x;

    // --- FIZYKA UDERZENIA ---
    // Decydujemy w któr¹ stronê przewróciæ na podstawie tego, z której strony przylecia³a pi³ka.

    if (xPocisku < xPuszki)
    {
        // Pi³ka z lewej -> Puszka leci w prawo (+)
        krokObrotu = 5.0f; // Szybkoœæ obrotu (stopnie na klatkê)
    }
    else
    {
        // Pi³ka z prawej -> Puszka leci w lewo (-)
        krokObrotu = -5.0f;
    }
}

// Funkcja wywo³ywana w ka¿dej klatce gry (napêdza animacjê)
void przeszkoda::aktualizuj()
{
    // Animujemy tylko jeœli puszka w³aœnie upada
    if (wTrakcieUpadku)
    {
        // 1. Zmieniamy k¹t
        aktualnyKat += krokObrotu;
        duszek.setRotation(aktualnyKat);

        // 2. Sprawdzamy czy osi¹gnêliœmy poziom (90 stopni lub -90 stopni)
        if (std::abs(aktualnyKat) >= 90.0f)
        {
            // --- KONIEC UPADKU ---
            wTrakcieUpadku = false;

            // Doklejamy do idealnych 90/-90 (¿eby nie by³o np. 92 stopni)
            if (aktualnyKat > 0) aktualnyKat = 90.0f;
            else                 aktualnyKat = -90.0f;

            duszek.setRotation(aktualnyKat);

            // 3. Zmieniamy wymiary hitboxa na le¿¹ce
            // (Zamiana szerokoœci z wysokoœci¹)
            blok.setSize({ wymiaryStojace.y, wymiaryStojace.x });

            // Nowy origin dla le¿¹cego hitboxa (¿eby le¿a³ p³asko na ziemi)
            blok.setOrigin({ wymiaryStojace.y / 2.0f, wymiaryStojace.x });
        }
    }
}

// Resetowanie puszki (nowa runda)
void przeszkoda::postaw()
{
    czyTrafiona = false;
    wTrakcieUpadku = false;

    // Reset obrotu
    aktualnyKat = 0.0f;
    duszek.setRotation(0.0f);

    // Reset wymiarów hitboxa do pionu
    blok.setSize(wymiaryStojace);
    blok.setOrigin({ wymiaryStojace.x / 2.0f, wymiaryStojace.y });
}