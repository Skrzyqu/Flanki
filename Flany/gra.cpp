#include "Gra.h"
#include <cmath>
#include <iostream>

Gra::Gra(bool trybBot) :
    // Lista inicjalizacyjna - szybsza ni¿ przypisywanie w ciele konstruktora
    graczLewy(150.0f, 900.0f, sf::Color::Red),
    graczPrawy(1770.0f, 900.0f, sf::Color::Blue),
    turaLewego(true),
    strzalWTok(false),
    napisTury(czcionka),
	napisWygranej(czcionka),
    graZBotem(trybBot)
{
    /*
    // Konfiguracja przeszkody
    kamien.pozycja = sf::Vector2f{ 1200.0f, 800.0f };
    kamien.rozmiar = sf::Vector2f{ 50.0f, 600.0f };
    kamien.blok.setSize(kamien.rozmiar);
    kamien.blok.setPosition(kamien.pozycja);
    kamien.blok.setFillColor(sf::Color::Green);
    */

    //konfiguracja puszki
    puszka.rozmiar = sf::Vector2f{ 60.0f, 100.0f };
    puszka.blok.setSize(puszka.rozmiar);
    puszka.pozycja = sf::Vector2f{ 960.0f, 1080.0f };
    puszka.blok.setPosition(puszka.pozycja);
    puszka.blok.setFillColor(sf::Color(192, 192, 192)); // Srebrny
    puszka.blok.setOrigin({ puszka.rozmiar.x / 2.0f, puszka.rozmiar.y });

    // 2. KONFIGURACJA PASKÓW PIWA
    sf::Vector2f rozmiarPaska{ 50.0f, 400.0f }; // Szeroki na 50, wysoki na 400
    // -- LEWY GRACZ (CZERWONY) --
    tloPaskaLewego.setSize(rozmiarPaska);
    tloPaskaLewego.setFillColor(sf::Color(50, 50, 50)); // Ciemnoszary (t³o)
    tloPaskaLewego.setPosition({ 50.0f, 100.0f }); // Po lewej stronie
    piwoLewe.setSize(rozmiarPaska);
    piwoLewe.setFillColor(sf::Color(255, 200, 0)); // Z³oty (PIWO!)
    piwoLewe.setPosition({ 50.0f, 100.0f });

    // -- PRAWY GRACZ (NIEBIESKI) --
    tloPaskaPrawego.setSize(rozmiarPaska);
    tloPaskaPrawego.setFillColor(sf::Color(50, 50, 50));
    tloPaskaPrawego.setPosition({ 1820.0f, 100.0f }); // Po prawej stronie
    piwoPrawe.setSize(rozmiarPaska);
    piwoPrawe.setFillColor(sf::Color(255, 200, 0));
    piwoPrawe.setPosition({ 1820.0f, 100.0f });

    // £adowanie zasobów
    (void)czcionka.openFromFile("arial.ttf");

    // Konfiguracja napisu
    napisTury.setCharacterSize(60);
    napisTury.setStyle(sf::Text::Bold);
    // KONFIGURACJA NAPISU WYGRANEJ
    napisWygranej.setCharacterSize(100); // Wielki napis
    napisWygranej.setStyle(sf::Text::Bold);
    napisWygranej.setFillColor(sf::Color::Green);
    napisWygranej.setString(""); // Na razie pusty

	// Inicjalizacja napisu tury
    aktualizujNapis();
}

void Gra::obsluzWejscie(sf::RenderWindow& okno)
{
    while (const std::optional event = okno.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            okno.close();

        // Wyjœcie ESC
        if (event->is<sf::Event::KeyPressed>())
        {
            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                okno.close();
        }

        if (czyKoniecGry) continue;

        // --- FAZA BIEGANIA I PICIA ---
        if (fazaBiegania)
        {
            if (event->is<sf::Event::KeyPressed>())
            {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

                // --- GRACZ LEWY (CZERWONY) - ZAWSZE SPACJA ---
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    // Jeœli jest Tura Lewego (czyli on trafi³) -> To ON PIJE
                    if (turaLewego)
                    {
                        obsluzPicie();
                    }
                    // Jeœli jest Tura Prawego (przeciwnik trafi³) -> To ON BIEGA
                    else
                    {
                        obsluzBieganie();
                    }
                }

                // --- GRACZ PRAWY (NIEBIESKI) - ZAWSZE ENTER ---
                // (Dzia³a tylko w PvP, bo w trybie z Botem, bot sam "klika")
                if (!graZBotem && keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    // Jeœli jest Tura Lewego (przeciwnik trafi³) -> To NIEBIESKI BIEGA
                    if (turaLewego)
                    {
                        obsluzBieganie();
                    }
                    // Jeœli jest Tura Prawego (on trafi³) -> To NIEBIESKI PIJE
                    else
                    {
                        obsluzPicie();
                    }
                }
            }
        }
        // --- FAZA CELOWANIA ---
        else if (!strzalWTok)
        {
            if (turaLewego)
            {
                graczLewy.obsluzWejscie(*event, okno);
            }
            else
            {
                if (!graZBotem) graczPrawy.obsluzWejscie(*event, okno);
            }
        }
    }
}

void Gra::logikaBota(sf::RenderWindow& okno)
{
    // Bot to zawsze PRAWY (Niebieski)
    if (!graZBotem) return;

    // --- 1. FAZA STRZELANIA ---
    if (!fazaBiegania && !strzalWTok && !turaLewego)
    {
        licznikBota++;

        // Czeka chwilê (np. 1 sekundê = 60 klatek) zanim strzeli, ¿ebyœ zd¹¿y³ zobaczyæ co siê dzieje
        if (licznikBota > 60)
        {
            // --- MATEMATYKA CELOWANIA ---
            // Celujemy w œrodek puszki (nie w lewy górny róg)
            sf::Vector2f cel = puszka.pozycja;
            cel.x += puszka.rozmiar.x / 2.0f;
            cel.y -= puszka.rozmiar.y / 2.0f; // Celujemy w œrodek wysokoœci

            sf::Vector2f start = graczPrawy.pozycja;

            // Fizyka:
            // X = X0 + Vx * t
            // Y = Y0 + Vy * t + 0.5 * g * t^2

            float g = 0.2f; // Grawitacja (taka sama jak w main.cpp!)
            float czasLotu = 95.0f; // Ile klatek ma lecieæ pi³ka (im wiêcej, tym wy¿szy ³uk)

            // Obliczamy idealn¹ prêdkoœæ, ¿eby trafiæ w punkt
            float vx = (cel.x - start.x) / czasLotu;
            float vy = (cel.y - start.y - 0.5f * g * czasLotu * czasLotu) / czasLotu;

            // --- CZYNNIK LUDZKI (B£¥D) ---
            // Dodajemy losowoœæ, ¿eby bot nie by³ robotem idealnym
            // B³¹d w przedziale -1.5 do 1.5
            float bladX = (rand() % 30 - 15) / 10.0f;
            float bladY = (rand() % 30 - 15) / 10.0f;

            // Czasem bot rzuci za lekko, czasem za mocno
            graczPrawy.predkosc = { vx + bladX, vy + bladY };
            graczPrawy.czyLeci = true;

            licznikBota = 0;
        }
    }

    // --- 2. FAZA BIEGANIA / PICIA ---
    if (fazaBiegania)
    {
        licznikBota++;

        // Zmniejszy³em to do 5 (by³o 8). Bot teraz klika szybciej!
        // Jeœli jest za trudny, zmieñ z powrotem na 8 lub 10.
        if (licznikBota > 5)
        {
            licznikBota = 0;

            if (turaLewego)
            {
                // Tura Czerwonego (Ty trafi³eœ).
                // Ty pijesz (Enter), Bot musi BIEGAÆ.
                obsluzBieganie();
            }
            else
            {
                // Tura Niebieskiego (Bot trafi³).
                // Ty biegasz (Spacja), Bot musi PIÆ.
                obsluzPicie();
            }
        }
    }
}

void Gra::obsluzPicie()
{
	if (czyKoniecGry) return;
    // Pije ten, czyja jest tura (bo to on trafi³ w puszkê)
    if (turaLewego)
    {
        // Pije Czerwony
        if (poziomLewego > 0)
        {
            poziomLewego -= LYK_PIWA;
            if (poziomLewego <= 0.1f)
            {
                poziomLewego = 0.0f;
                sprawdzWygrana(); // <--- Wywo³ujemy sprawdzenie!
            }

            // Aktualizacja wygl¹du paska
            // Zmieniamy wysokoœæ prostok¹ta (procent z 400px)
            float nowaWysokosc = (poziomLewego / 100.0f) * 400.0f;
            piwoLewe.setSize({ 50.0f, nowaWysokosc });

            // Poniewa¿ origin jest w lewym górnym rogu, zmniejszenie wysokoœci sprawi,
            // ¿e pasek bêdzie "mala³ do góry". Jeœli chcesz, ¿eby mala³ w dó³, 
            // trzeba by przesuwaæ te¿ pozycjê Y, ale tak te¿ wygl¹da jak wypijanie ze szklanki!
        }
    }
    else
    {
        // Pije Niebieski
        if (poziomPrawego > 0)
        {
            poziomPrawego -= LYK_PIWA;
            if (poziomPrawego <= 0.1f)
            {
                poziomPrawego = 0.0f;
                sprawdzWygrana(); // <--- Wywo³ujemy sprawdzenie!
            }

            float nowaWysokosc = (poziomPrawego / 100.0f) * 400.0f;
            piwoPrawe.setSize({ 50.0f, nowaWysokosc });
        }
    }
}

void Gra::obsluzBieganie()
{
    // Jeœli gra siê skoñczy³a w trakcie biegania, przerywamy
    if (czyKoniecGry) return;

    pocisk* biegacz = turaLewego ? &graczPrawy : &graczLewy;
    sf::Vector2f cel;

    if (biegWStronePuszki)
    {
        cel = puszka.pozycja;
        cel.y -= 20.0f;
    }
    else
    {
        cel = biegacz->pozycjaStartowa;
    }

    sf::Vector2f kierunek = cel - biegacz->pozycja;
    float dystans = std::sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);

    if (dystans <= szybkoscBiegu)
    {
        biegacz->pozycja = cel;

        if (biegWStronePuszki)
        {
            biegWStronePuszki = false;
            napisTury.setString("WRACAJ!");
            napisTury.setFillColor(sf::Color::Yellow);
        }
        else
        {
            biegacz->lotka.setPosition(biegacz->pozycjaStartowa);
            zmienTure(*(sf::RenderWindow*)nullptr);
        }
    }
    else
    {
        sf::Vector2f ruch = (kierunek / dystans) * szybkoscBiegu;
        biegacz->pozycja += ruch;
    }

    biegacz->lotka.setPosition(biegacz->pozycja);
}

void Gra::sprawdzWygrana()
{
    bool czyKtosWygral = false;

    if (poziomLewego <= 0.1f)
    {
        czyKoniecGry = true;
        czyKtosWygral = true;

        // Ustawiamy napis
        napisWygranej.setString("WYGRAL CZERWONY!");
        napisWygranej.setFillColor(sf::Color::Red);

        // Debug w konsoli
        std::cout << "KONIEC GRY! Wygrywa Czerwony!" << std::endl;
    }
    else if (poziomPrawego <= 0.1f)
    {
        czyKoniecGry = true;
        czyKtosWygral = true;

        // Ustawiamy napis
        napisWygranej.setString("WYGRAL NIEBIESKI!");
        napisWygranej.setFillColor(sf::Color::Blue);

        // Debug w konsoli
        std::cout << "KONIEC GRY! Wygrywa Niebieski!" << std::endl;
    }

    // Jeœli ktoœ wygra³, centrujemy napis
    if (czyKtosWygral)
    {
        sf::FloatRect bounds = napisWygranej.getLocalBounds();
        napisWygranej.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });

        // Pozycja na œrodku ekranu (dla 1920x1080)
        napisWygranej.setPosition({ 960.0f, 540.0f });

        // Wy³¹czamy fazê biegania, ¿eby nikt ju¿ nie biega³ w tle
        fazaBiegania = false;
    }
}

void Gra::aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja)
{
    if (czyKoniecGry) return;

    // WYWO£ANIE AI BOTA
    logikaBota(okno);

    if (fazaBiegania) return;

    graczLewy.aktualizujFizyke(okno, puszka, grawitacja);
    graczPrawy.aktualizujFizyke(okno, puszka, grawitacja);

    if (puszka.czyTrafiona && !fazaBiegania)
    {
        fazaBiegania = true;
        biegWStronePuszki = true;

        if (turaLewego) graczLewy.resetuj();
        else            graczPrawy.resetuj();

        puszka.blok.setFillColor(sf::Color::Red);
        aktualizujNapis();
    }
    else if (!puszka.czyTrafiona)
    {
        puszka.blok.setFillColor(sf::Color(192, 192, 192));
    }

    if (!fazaBiegania)
    {
        if (turaLewego && graczLewy.czyLeci) strzalWTok = true;
        if (!turaLewego && graczPrawy.czyLeci) strzalWTok = true;

        if (strzalWTok)
        {
            bool czyKoniecLotu = false;
            if (turaLewego && !graczLewy.czyLeci) czyKoniecLotu = true;
            if (!turaLewego && !graczPrawy.czyLeci) czyKoniecLotu = true;

            if (czyKoniecLotu && !puszka.czyTrafiona)
            {
                zmienTure(okno);
            }
        }
    }
}

void Gra::zmienTure(sf::RenderWindow& okno)
{
    strzalWTok = false;
    fazaBiegania = false;
    biegWStronePuszki = true;

    turaLewego = !turaLewego;
    puszka.czyTrafiona = false;

    graczLewy.resetuj();
    graczPrawy.resetuj();

    licznikBota = 0; // Resetujemy mózg bota
    aktualizujNapis();
}

void Gra::aktualizujNapis()
{
    if (czyKoniecGry) return;

    if (fazaBiegania)
    {
        if (turaLewego)
        {
            // Tura Czerwonego (Trafi³)
            if (graZBotem)
            {
                napisTury.setString("BOT BIEGNIE! Ty PIJ (SPACJA)!");
            }
            else
            {
                // PvP: Czerwony pije (Spacja), Niebieski biega (Enter)
                napisTury.setString("CZERWONY PIJ (SPACJA)\nNIEBIESKI BIEGAJ (ENTER)");
            }
            napisTury.setFillColor(sf::Color::White);
        }
        else
        {
            // Tura Niebieskiego (Trafi³)
            if (graZBotem)
            {
                napisTury.setString("BOT PIJE! Ty BIEGAJ (SPACJA)!");
            }
            else
            {
                // PvP: Niebieski pije (Enter), Czerwony biega (Spacja)
                napisTury.setString("NIEBIESKI PIJ (ENTER)\nCZERWONY BIEGAJ (SPACJA)");
            }
            napisTury.setFillColor(sf::Color::White);
        }
    }
    else
    {
        if (turaLewego)
        {
            napisTury.setString("Tura Czerwonego");
            napisTury.setFillColor(sf::Color::Red);
        }
        else
        {
            if (graZBotem) napisTury.setString("Tura Bota...");
            else           napisTury.setString("Tura Niebieskiego");
            napisTury.setFillColor(sf::Color::Blue);
        }
    }

    sf::FloatRect tekstBounds = napisTury.getLocalBounds();
    napisTury.setOrigin({ tekstBounds.size.x / 2.0f, tekstBounds.size.y / 2.0f });
    napisTury.setPosition({ 960.0f, 100.0f });
}

void Gra::rysuj(sf::RenderWindow& okno)
{
    okno.clear(sf::Color::Black);
    okno.draw(puszka.blok);
    okno.draw(tloPaskaLewego); okno.draw(piwoLewe);
    okno.draw(tloPaskaPrawego); okno.draw(piwoPrawe);
    okno.draw(graczLewy.lotka); okno.draw(graczPrawy.lotka);
    if (!czyKoniecGry && !fazaBiegania) {
        if (turaLewego) graczLewy.rysujCelowanie(okno);
        else if (!graZBotem) graczPrawy.rysujCelowanie(okno); // Bot nie rysuje linii celowania
    }
    if (czyKoniecGry) okno.draw(napisWygranej);
    else okno.draw(napisTury);
    okno.display();
}