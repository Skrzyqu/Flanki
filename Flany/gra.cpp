#include "Gra.h"

Gra::Gra() :
    // Lista inicjalizacyjna - szybsza ni¿ przypisywanie w ciele konstruktora
    graczLewy(150.0f, 900.0f, sf::Color::Red),
    graczPrawy(1800.0f, 900.0f, sf::Color::Blue),
    turaLewego(true),
    strzalWTok(false),
    napisTury(czcionka)
{
    // Konfiguracja przeszkody
    kamien.pozycja = sf::Vector2f{ 1200.0f, 800.0f };
    kamien.rozmiar = sf::Vector2f{ 50.0f, 600.0f };
    kamien.blok.setSize(kamien.rozmiar);
    kamien.blok.setPosition(kamien.pozycja);
    kamien.blok.setFillColor(sf::Color::Green);

    // £adowanie zasobów
    (void)czcionka.openFromFile("arial.ttf");

    // Konfiguracja napisu
    napisTury.setCharacterSize(60);
    napisTury.setStyle(sf::Text::Bold);

	// Inicjalizacja napisu tury
    aktualizujNapis();
}

void Gra::obsluzWejscie(sf::RenderWindow& okno)
{
    while (const std::optional event = okno.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            okno.close();

		// Obs³uga wejœcia tylko jeœli nie ma strza³u w toku
        if (!strzalWTok)
        {
            if (turaLewego)
            {
                graczLewy.obsluzWejscie(*event, okno);
            }
            else
            {
                graczPrawy.obsluzWejscie(*event, okno);
            }
        }
    }
}

void Gra::aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja)
{
	// Aktualizacja fizyki obu graczy
    graczLewy.aktualizujFizyke(okno, kamien, grawitacja);
    graczPrawy.aktualizujFizyke(okno, kamien, grawitacja);

	// Sprawdzenie czy jest strza³ w toku
    if (turaLewego && graczLewy.czyLeci) strzalWTok = true;
    if (!turaLewego && graczPrawy.czyLeci) strzalWTok = true;

	// Zmiana tury jeœli strza³ siê zakoñczy³ (Trzeba dodaæ logike zatrzymania tury gdy prêdkoœæ bêdzie = 0)
    if (strzalWTok)
    {
        bool czyKoniecLotu = false;

        if (turaLewego && !graczLewy.czyLeci) czyKoniecLotu = true;
        if (!turaLewego && !graczPrawy.czyLeci) czyKoniecLotu = true;

        if (czyKoniecLotu)
        {
            zmienTure(okno);
        }
    }
}

void Gra::zmienTure(sf::RenderWindow& okno)
{
    strzalWTok = false;
    turaLewego = !turaLewego;
    aktualizujNapis();
}

void Gra::aktualizujNapis()
{
    if (turaLewego)
    {
        napisTury.setString("Tura Czerwonego");
        napisTury.setFillColor(sf::Color::Red);
    }
    else
    {
        napisTury.setString("Tura Niebieskiego");
        napisTury.setFillColor(sf::Color::Blue);
    }

    // Centrowanie napisu
    sf::FloatRect tekstBounds = napisTury.getLocalBounds();
    napisTury.setOrigin(sf::Vector2f(tekstBounds.size.x / 2.0f, tekstBounds.size.y / 2.0f));
    napisTury.setPosition(sf::Vector2f(1200.0f, 50.0f));
}

void Gra::rysuj(sf::RenderWindow& okno)
{
    okno.clear(sf::Color::Black);

    okno.draw(kamien.blok);
    okno.draw(graczLewy.lotka);
    okno.draw(graczPrawy.lotka);

    if (turaLewego) graczLewy.rysujCelowanie(okno);
    else            graczPrawy.rysujCelowanie(okno);

    okno.draw(napisTury);

    okno.display();
}