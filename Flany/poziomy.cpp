#include "Gra.h" // WA¯NE: To pozwala widzieæ klasê Gra
#include <cmath>
#include <iostream>
#include "przeszkoda.h"
#include "odbicie.h"
#include "pocisk.h"

#define srodekX 960.0f
#define srodekY 540.0f

void Gra::tutorialE2(sf::RenderWindow& okno)
{
    float przesuniecieS1 = 200;

    przeszkoda sciana1;
    sciana1.rozmiar = { 30.0f, 280.0f };
    sciana1.pozycja = { srodekX - przesuniecieS1, srodekY + 340 };
    sciana1.blok.setSize(sciana1.rozmiar);
    sciana1.blok.setOrigin(sf::Vector2f(sciana1.rozmiar.x / 2.0f, sciana1.rozmiar.y / 2.0f));
    sciana1.blok.setPosition(sciana1.pozycja);
    sciana1.blok.setFillColor(sf::Color(255, 0, 0));
    sciana1.blok.setOutlineColor(sf::Color::Black);
    sciana1.blok.setOutlineThickness(2.0f);

    przeszkoda sciana2;
    sciana2.rozmiar = sciana1.rozmiar;
    sciana2.pozycja = { srodekX + przesuniecieS1, sciana1.pozycja.y };
    sciana2.blok.setSize(sciana2.rozmiar);
    sciana2.blok.setOrigin(sf::Vector2f(sciana2.rozmiar.x / 2.0f, sciana2.rozmiar.y / 2.0f));
    sciana2.blok.setPosition(sciana2.pozycja);
    sciana2.blok.setFillColor(sf::Color(255, 0, 0));
    sciana2.blok.setOutlineColor(sf::Color::Black);
    sciana2.blok.setOutlineThickness(2.0f);

    // ... (mo¿esz dodaæ resztê œcian jeœli chcesz)

    okno.draw(sciana2.blok);
    okno.draw(sciana1.blok);
}

void Gra::tutorialE3(sf::RenderWindow& okno)
{
    float przesuniecie1 = 560;

    // trampolina L
    przeszkoda trampolinaL;
    trampolinaL.rozmiar = { 100.0f, 10.0f };
    trampolinaL.pozycja = { srodekX - przesuniecie1, srodekY + 480 };
    trampolinaL.blok.setSize(trampolinaL.rozmiar);
    trampolinaL.blok.setOrigin(sf::Vector2f(trampolinaL.rozmiar.x / 2.0f, trampolinaL.rozmiar.y / 2.0f));
    trampolinaL.blok.setPosition(trampolinaL.pozycja);
    trampolinaL.blok.setFillColor(sf::Color(0, 0, 0));
    trampolinaL.blok.setOutlineColor(sf::Color::Yellow);
    trampolinaL.blok.setOutlineThickness(2.0f);
    trampolinaL.sprezystosc = 2.0f; // BARDZO SPRÊ¯YSTE!

    // trampolina P
    przeszkoda trampolinaP;
    trampolinaP.rozmiar = trampolinaL.rozmiar;
    trampolinaP.pozycja = { srodekX + przesuniecie1, trampolinaL.pozycja.y };
    trampolinaP.blok.setSize(trampolinaP.rozmiar);
    trampolinaP.blok.setOrigin(sf::Vector2f(trampolinaP.rozmiar.x / 2.0f, trampolinaP.rozmiar.y / 2.0f));
    trampolinaP.blok.setPosition(trampolinaP.pozycja);
    trampolinaP.blok.setFillColor(sf::Color(0, 0, 0));
    trampolinaP.blok.setOutlineColor(sf::Color::Yellow);
    trampolinaP.blok.setOutlineThickness(2.0f);
    trampolinaP.sprezystosc = 2.0f;

    // Musimy wywo³aæ kolizjê dla tych przeszkód, jeœli pociski lec¹!
    // Uwaga: W tym prostym kodzie tutorialE3 tylko RYSUJE przeszkody. 
    // ¯eby dzia³a³a fizyka, te przeszkody powinny byæ sk³adowymi klasy Gra 
    // lub przekazywane do `aktualizujFizyke`. 
    // Na ten moment kod kolegi tylko je rysuje, ale kolizje dodamy tutaj "na szybko" sprawdzaj¹c globalnie:

    if (graczLewy.czyLeci) odbicie_przeszkoda(&graczLewy, trampolinaL);
    if (graczLewy.czyLeci) odbicie_przeszkoda(&graczLewy, trampolinaP);
    if (graczPrawy.czyLeci) odbicie_przeszkoda(&graczPrawy, trampolinaL);
    if (graczPrawy.czyLeci) odbicie_przeszkoda(&graczPrawy, trampolinaP);

    okno.draw(trampolinaL.blok);
    okno.draw(trampolinaP.blok);
}