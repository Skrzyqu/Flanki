#include "pocisk.h"
#include "odbicie.cpp"
#include <cmath>
#include <iostream>

// Konstruktor - ustawia wartoœci startowe
pocisk::pocisk(float x, float y, sf::Color kolor)
{
    pozycja = { x, y };
    pozycjaStartowa = pozycja;
    predkosc = { 0.0f, 0.0f };
    lotka.setFillColor(kolor);
	lotka.setOrigin(sf::Vector2f(lotka.getRadius(), lotka.getRadius())); // Ustawienie œrodka jako  punktu odniesienia
    lotka.setPosition(pozycja);
}

// Funkcja obs³uguj¹ca myszkê
void pocisk::obsluzWejscie(sf::Event event, const sf::RenderWindow& okno)
{
	// Rozpoczêcie celowania (naci¹gniêcie procy)
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (!czyLeci)
            {
                sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));
                // Sprawdzenie czy klikniêto w obrêb pi³ki
                float dx = mysz.x - pozycja.x;
                float dy = mysz.y - pozycja.y;
                float dystansKwadrat = dx * dx + dy * dy;
                float promien = lotka.getRadius();
                if (dystansKwadrat <= promien * promien)
                {
                    czyCeluje = true;
                }
            }
        }
    }

	// Wystrzelenie pocisku (puszczenie procy)
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
        if (mouseEvent->button == sf::Mouse::Button::Left)
        {
            if (czyCeluje)
            {
                czyCeluje = false;
                czyLeci = true;

                sf::Vector2f naciag;
                naciag.x = pozycjaStartowa.x - pozycja.x;
                naciag.y = pozycjaStartowa.y - pozycja.y;

                predkosc = naciag * 0.15f;
                pozycja = pozycjaStartowa;
                lotka.setPosition(pozycja);
            }
        }
    }
}
void pocisk::aktualizujPozycjeCelowania(const sf::RenderWindow& okno)
{
    if (czyCeluje)
    {
        sf::Vector2f mysz = okno.mapPixelToCoords(sf::Mouse::getPosition(okno));

        // Wektor od œrodka do myszki
        sf::Vector2f wektor;
        wektor.x = mysz.x - pozycjaStartowa.x;
        wektor.y = mysz.y - pozycjaStartowa.y;

        // Obliczamy d³ugoœæ tego wektora
        float dlugosc = std::sqrt(wektor.x * wektor.x + wektor.y * wektor.y);

        // Przycinanie wektora do maksymalnego zasiêgu (MAX_NACIAG)
        if (dlugosc > MAX_NACIAG)
        {
            float skala = MAX_NACIAG / dlugosc;
            wektor.x *= skala;
            wektor.y *= skala;
        }

        // Aktualizacja wizualna pozycji "na gumce"
        pozycja.x = pozycjaStartowa.x + wektor.x;
        pozycja.y = pozycjaStartowa.y + wektor.y;
    }
}

// G³ówna pêtla fizyki dla pocisku
void pocisk::aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja)
{
    // Obs³uga logiki przed strza³em
    if (czyCeluje)
    {
        aktualizujPozycjeCelowania(okno);
    }
    // Obs³uga logiki w trakcie lotu
    if (czyLeci)
    {
		// Sprawdzenie kolizji z przeszkod¹
        odbicie_przeszkoda(this, cel);

		// Aktualizacja pozycji i prêdkoœci
        zmiana_pozycji();
        zmiana_predkosci(grawitacja);

		// Odbicie od pod³o¿a
        odbicie_podloze(&okno);

		// Reset jeœli pocisk wyleci poza ekran
        if (pozycja.x > 3000 || pozycja.x < -500)
        {
            resetuj();
        }
    }
}

void pocisk::resetuj()
{
    czyLeci = false;
    predkosc = { 0, 0 };
    pozycja = pozycjaStartowa;
    lotka.setPosition(pozycja);
}
// Rysowanie bia³ej linii naci¹gu
void pocisk::rysujCelowanie(sf::RenderWindow& okno)
{
    if (czyCeluje)
    {
		// Rysowanie linii
        sf::Vector2i mousePos = sf::Mouse::getPosition(okno);
        sf::Vertex line[] =
        {
            sf::Vertex(pozycjaStartowa, sf::Color::White),
            sf::Vertex(pozycja, sf::Color::Magenta) // Rysujemy do 'pozycja' (tam gdzie naci¹gnêliœmy myszk¹)
        };
        okno.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

// --- Funkcje pomocnicze (Private) ---

void pocisk::zmiana_predkosci(sf::Vector2f grawitacja)
{
    predkosc.x += grawitacja.x;
    predkosc.y += grawitacja.y;
}

void pocisk::zmiana_pozycji()
{
    pozycja.x += predkosc.x;
    pozycja.y += predkosc.y;
    lotka.setPosition(pozycja); 
}

void pocisk::odbicie_podloze(sf::RenderWindow* okno)
{
    // Sprawdzenie dolnej krawêdzi ekranu
    if (pozycja.y + lotka.getRadius() >= POZIOM_PODLOGI)
    {
        predkosc.y = predkosc.y * -0.7f;

        // ZMIANA: Ustawiamy pi³kê na powierzchni naszej nowej pod³ogi
        pozycja.y = POZIOM_PODLOGI - lotka.getRadius();

        if ((fabs(predkosc.y) < precyzja))
        {
            predkosc.y = 0.0f;
            // ZMIANA: Tu te¿ poprawka pozycji
            pozycja.y = POZIOM_PODLOGI - lotka.getRadius();
        }
        lotka.setPosition(pozycja);
    }
}