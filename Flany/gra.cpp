#include "Gra.h"
#include <cmath>
#include <iostream>

Gra::Gra(bool trybBot) :
    // [PERF] Lista inicjalizacyjna zapobiega podwójnej konstrucji obiektów (default + assign).
    tlo(teksturaTla),
    graczLewy(118.0f, 970.0f, sf::Color::White),
    graczPrawy(1802.0f, 970.0f, sf::Color::White),
    skinLewy(150.0f, POZIOM_PODLOGI),
    skinPrawy(1770.0f, POZIOM_PODLOGI),
    turaLewego(true),
    strzalWTok(false),
    napisTury(czcionka),
    napisWygranej(czcionka),
    graZBotem(trybBot)
{
    // [IO/BLOCKING] £adowanie zasobów w konstruktorze jest akceptowalne w ma³ych projektach,
    // ale w du¿ych silnikach powinno byæ asynchroniczne lub preloadowane przez AssetManager.
    if (!teksturaTla.loadFromFile("agh_bg2.png"))
    {
        std::cerr << "[ERROR] Blad ladowania tla! Sprawdz sciezke do pliku." << std::endl;
    }
    tlo.setTexture(teksturaTla, true);

    // Skalowanie t³a do rozdzielczoœci Full HD (Hardcoded resolution assumption).
    sf::Vector2u wymiaryObrazka = teksturaTla.getSize();
    float skalaX = 1920.0f / wymiaryObrazka.x;
    float skalaY = 1080.0f / wymiaryObrazka.y;
    tlo.setScale({ skalaX, skalaY });

    if (!teksturaLotki.loadFromFile("lotka.png"))
    {
        std::cerr << "[ERROR] Blad ladowania tekstury lotki!" << std::endl;
    }
    else
    {
        // W³¹czamy wyg³adzanie, ¿eby lotka ³adnie wygl¹da³a przy obrotach
        teksturaLotki.setSmooth(true);

        // Przypisanie tekstury do lewego gracza
        graczLewy.lotka.setTexture(&teksturaLotki);
        // Przypisanie tekstury do prawego gracza
        graczPrawy.lotka.setTexture(&teksturaLotki);
    }

    // 1. SETUP FIZYKI ŒWIATA (Pod³oga)
    podloga.rozmiar = { 1920.0f, 1080.0f - POZIOM_PODLOGI };
    podloga.pozycja = { 0.0f, POZIOM_PODLOGI };
    podloga.blok.setSize(podloga.rozmiar);
    podloga.blok.setFillColor(sf::Color(34, 139, 34)); // Zielony
    podloga.blok.setPosition(podloga.pozycja);

    // 2. SETUP CELU (Puszka)
    if (!puszka.tekstura.loadFromFile("beer.png"))
    {
        std::cerr << "[ERROR] Blad ladowania tekstury puszki!" << std::endl;
    }
    else
    {
		puszka.czyPuszka = true; //Ustawienie ¿e trafienie puszki koñczy gre
        puszka.duszek.setTexture(puszka.tekstura, true);
        puszka.duszek.setScale({ 1.5f, 1.5f });
        sf::FloatRect bounds = puszka.duszek.getGlobalBounds();
        puszka.rozmiar = bounds.size;
    }
    // Synchronizacja Collidera (Hitbox) z Visua³em (Sprite)
    puszka.blok.setSize(puszka.rozmiar);
    puszka.blok.setOrigin({ puszka.rozmiar.x / 2.0f, puszka.rozmiar.y });

    // Centrowanie origin sprite'a
    sf::FloatRect localBounds = puszka.duszek.getLocalBounds();
    puszka.duszek.setOrigin({ localBounds.size.x / 2.0f, localBounds.size.y });

    puszka.pozycja = { 960.0f, POZIOM_PODLOGI };
    puszka.blok.setPosition(puszka.pozycja);
    puszka.duszek.setPosition(puszka.pozycja);


    // 3. SETUP GUI (Paski Piwa)
    // [TODO]: Magiczne liczby (50.0f, 400.0f) powinny byæ wyci¹gniête do sta³ych konfiguracyjnych.
    sf::Vector2f rozmiarPaska{ 50.0f, 400.0f };

    // -- LEWY GRACZ (CZERWONY) --
    tloPaskaLewego.setSize(rozmiarPaska);
    tloPaskaLewego.setFillColor(sf::Color(50, 50, 50)); // Ciemnoszary t³o
    tloPaskaLewego.setPosition({ 50.0f, 100.0f });
    piwoLewe.setSize(rozmiarPaska);
    piwoLewe.setFillColor(sf::Color(255, 200, 0)); // Z³oty kolor piwa
    piwoLewe.setPosition({ 50.0f, 100.0f });
    skinLewy.ustawZwrot(true); // Twarz¹ w prawo

    // -- PRAWY GRACZ (NIEBIESKI) --
    tloPaskaPrawego.setSize(rozmiarPaska);
    tloPaskaPrawego.setFillColor(sf::Color(50, 50, 50));
    tloPaskaPrawego.setPosition({ 1820.0f, 100.0f });
    piwoPrawe.setSize(rozmiarPaska);
    piwoPrawe.setFillColor(sf::Color(255, 200, 0));
    piwoPrawe.setPosition({ 1820.0f, 100.0f });
    skinPrawy.ustawZwrot(false); // Twarz¹ w lewo

    (void)czcionka.openFromFile("arial.ttf");

    // Konfiguracja stylów tekstu
    napisTury.setCharacterSize(60);
    napisTury.setStyle(sf::Text::Bold);

    napisWygranej.setCharacterSize(100);
    napisWygranej.setStyle(sf::Text::Bold);
    napisWygranej.setFillColor(sf::Color::Green);
    napisWygranej.setString("");

    // Inicjalizacja stanu pocz¹tkowego
    aktualizujNapis();
    skinLewy.ustawGotowosc(true);
    skinPrawy.ustawGotowosc(false);
}

void Gra::obsluzWejscie(sf::RenderWindow& okno)
{
    // Standardowa pêtla zdarzeñ SFML
    while (const std::optional event = okno.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            okno.close();

        // Obs³uga wyjœcia (ESC)
        if (event->is<sf::Event::KeyPressed>())
        {
            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
                okno.close();
        }

        if (czyKoniecGry) continue;

        // --- STATE: FAZA BIEGANIA / PICIA (KARY) ---
        if (fazaBiegania)
        {
            if (event->is<sf::Event::KeyPressed>())
            {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

                // --- CONTROLLER: GRACZ LEWY ---
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    // Jeœli moja tura (trafi³em) -> Nagroda (pijê)
                    // Jeœli tura wroga (on trafi³) -> Kara (biegnê po pi³kê)
                    if (turaLewego) obsluzPicie();
                    else            obsluzBieganie();
                }

                // --- CONTROLLER: GRACZ PRAWY ---
                // Blokujemy input jeœli steruje nim BOT
                if (!graZBotem && keyEvent->code == sf::Keyboard::Key::Enter)
                {
                    if (turaLewego) obsluzBieganie();
                    else            obsluzPicie();
                }
            }
        }
        // --- STATE: FAZA CELOWANIA (GAMEPLAY) ---
        else if (!strzalWTok)
        {
            // Przekazanie obs³ugi myszki do odpowiedniego obiektu "Pocisk" (Slingshot logic)
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
    // Bot operuje tylko jako PRAWY (Niebieski)
    if (!graZBotem) return;

    // --- STATE: BOT STRZELA ---
    if (!fazaBiegania && !strzalWTok && !turaLewego)
    {
        licznikBota++;

        // OpóŸnienie (60 klatek ~= 1 sekunda przy 60FPS), aby gracz zauwa¿y³ zmianê tury.
        if (licznikBota > 60)
        {
            // --- MATH: OBLICZANIE TRAJEKTORII ---
            // Celujemy w œrodek puszki.
            // Równanie ruchu w osiach X i Y, aby trafiæ w punkt (x, y) w zadanym czasie t.
            sf::Vector2f cel = puszka.pozycja;
            cel.x += puszka.rozmiar.x / 2.0f;
            cel.y -= puszka.rozmiar.y / 2.0f;

            sf::Vector2f start = graczPrawy.pozycja;

            float g = 0.2f; // Sta³a grawitacji (musi byæ zgodna z grawitacj¹ w pocisk.cpp!)
            float czasLotu = 95.0f; // Czas w klatkach. Wy¿szy = wy¿sza parabola lotu.

            // v = d / t (dla X)
            // v = (d - 0.5*g*t^2) / t (dla Y, uwzglêdniaj¹c przyspieszenie)
            float vx = (cel.x - start.x) / czasLotu;
            float vy = (cel.y - start.y - 0.5f * g * czasLotu * czasLotu) / czasLotu;

            // --- HUMANIZACJA BOTA ---
            // Dodajemy szum (RNG), aby bot nie trafia³ idealnie za ka¿dym razem.
            // Symulacja b³êdu ludzkiej rêki.
            float bladX = (rand() % 30 - 15) / 10.0f; // -1.5 do 1.5
            float bladY = (rand() % 30 - 15) / 10.0f;

            graczPrawy.predkosc = { vx + bladX, vy + bladY };
            graczPrawy.czyLeci = true;

            licznikBota = 0;
        }
    }

    // --- STATE: BOT REAGUJE NA WYNIK RZUTU ---
    if (fazaBiegania)
    {
        licznikBota++;

        // Bot "mashuje" przyciski bardzo szybko (co 5 klatek).
        if (licznikBota > 5)
        {
            licznikBota = 0;

            if (turaLewego)
            {
                // Gracz trafi³ -> Bot musi biegaæ po pi³kê.
                obsluzBieganie();
            }
            else
            {
                // Bot trafi³ -> Bot pije piwo (nagroda).
                obsluzPicie();
            }
        }
    }
}

void Gra::obsluzPicie()
{
    if (czyKoniecGry) return;

    // Logika: Zmniejszamy wartoœæ 'poziomu' i aktualizujemy pasek UI.
    if (turaLewego)
    {
        if (poziomLewego > 0)
        {
            poziomLewego -= LYK_PIWA;
            if (poziomLewego <= 0.1f)
            {
                poziomLewego = 0.0f;
                sprawdzWygrana(); // Trigger koñca gry
            }

            // Aktualizacja UI: Skalowanie wysokoœci paska.
            // Poniewa¿ origin jest w (0,0), zmniejszamy Height, co daje efekt "znikania" od do³u w górê.
            float nowaWysokosc = (poziomLewego / 100.0f) * 400.0f;
            piwoLewe.setSize({ 50.0f, nowaWysokosc });
        }
    }
    else
    {
        // Analogicznie dla prawego gracza
        if (poziomPrawego > 0)
        {
            poziomPrawego -= LYK_PIWA;
            if (poziomPrawego <= 0.1f)
            {
                poziomPrawego = 0.0f;
                sprawdzWygrana();
            }

            float nowaWysokosc = (poziomPrawego / 100.0f) * 400.0f;
            piwoPrawe.setSize({ 50.0f, nowaWysokosc });
        }
    }
}

void Gra::obsluzBieganie()
{
    if (czyKoniecGry) return;

    // Ustalenie, kto jest "biegaczem" (ten, kto NIE rzuca³/zosta³ trafiony)
    Bohater* biegacz = turaLewego ? &skinPrawy : &skinLewy;
    sf::Vector2f cel;

    // Logika sekwencji: Bieg do puszki -> Powrót na start
    if (biegWStronePuszki)
    {
        cel = puszka.pozycja;
        cel.y = POZIOM_PODLOGI;
    }
    else
    {
        cel = biegacz->pozycjaStartowa;
    }

    // Sprawdzenie dystansu
    sf::Vector2f roznica = cel - biegacz->duszek.getPosition();
    float dystans = std::sqrt(roznica.x * roznica.x + roznica.y * roznica.y);

    // Jeœli dotarliœmy do celu (z marginesem równym prêdkoœci, ¿eby nie przeskoczyæ)
    if (dystans <= szybkoscBiegu)
    {
        biegacz->duszek.setPosition(cel); // Idealne doci¹gniêcie do punktu
        biegacz->zatrzymajSie();

        if (biegWStronePuszki)
        {
            // --- ETAP 1 ZAKOÑCZONY: DOTAR£ DO PUSZKI ---

            // NOWOŒÆ: Skoro dobieg³, to stawia puszkê!
            puszka.postaw();

            // Teraz zmienia cel na powrót
            biegWStronePuszki = false;
            napisTury.setString("WRACAJ!");
            napisTury.setFillColor(sf::Color::Yellow);
        }
        else
        {
            // --- ETAP 2 ZAKOÑCZONY: WRÓCI£ NA START ---
            // Koniec kary, zmiana tury
            zmienTure(*(sf::RenderWindow*)nullptr);
        }
    }
    else
    {
        // Jeœli jeszcze nie dotar³, idzie w stronê celu
        biegacz->podejdzDo(cel, szybkoscBiegu);
    }
}

void Gra::sprawdzWygrana()
{
    bool czyKtosWygral = false;

    if (poziomLewego <= 0.1f)
    {
        czyKoniecGry = true;
        czyKtosWygral = true;
        napisWygranej.setString("WYGRAL CZERWONY!");
        napisWygranej.setFillColor(sf::Color::Red);
        std::cout << "[GAME OVER] Wygrywa Czerwony!" << std::endl;
    }
    else if (poziomPrawego <= 0.1f)
    {
        czyKoniecGry = true;
        czyKtosWygral = true;
        napisWygranej.setString("WYGRAL NIEBIESKI!");
        napisWygranej.setFillColor(sf::Color::Blue);
        std::cout << "[GAME OVER] Wygrywa Niebieski!" << std::endl;
    }

    if (czyKtosWygral)
    {
        // Wyœrodkowanie napisu koñcowego
        sf::FloatRect bounds = napisWygranej.getLocalBounds();
        napisWygranej.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
        napisWygranej.setPosition({ 960.0f, 540.0f });

        fazaBiegania = false; // Zatrzymujemy logikê biegania
    }
}

void Gra::aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja)
{
    if (czyKoniecGry) return;

    // [VISUAL] Animacje
    skinLewy.aktualizujAnimacje();
    skinPrawy.aktualizujAnimacje();
    puszka.aktualizuj();

    // [AI]
    logikaBota(okno);

    // --- ZMIANA 1: FIZYKA MUSI DZIA£AÆ ZAWSZE ---
    // Usuwamy st¹d "if (fazaBiegania) return;", aby lotka mog³a siê odbiæ i spaœæ,
    // nawet gdy gracze ju¿ zaczynaj¹ biec.

    // [PHYSICS] Symulacja lotu
    graczLewy.aktualizujFizyke(okno, puszka, grawitacja);
    graczPrawy.aktualizujFizyke(okno, puszka, grawitacja);

    // --- ZMIANA 2: DETEKCJA TRAFIENIA (NATYCHMIASTOWA) ---
    // Sprawdzamy czy w tej klatce nast¹pi³o trafienie (ustawione w odbicie.cpp)
    if (puszka.czyTrafiona && !fazaBiegania)
    {
        // STARTUJEMY BIEGANIE OD RAZU
        fazaBiegania = true;
        biegWStronePuszki = true;

        puszka.blok.setFillColor(sf::Color::Red);
        aktualizujNapis();

        // WA¯NE: NIE resetujemy lotki (gracz.resetuj()). 
        // Pozwalamy jej lecieæ z now¹ prêdkoœci¹ nadan¹ przez odbicie.cpp.
    }

    // Reset koloru puszki (opcjonalne)
    if (!puszka.czyTrafiona)
    {
        puszka.blok.setFillColor(sf::Color(192, 192, 192));
    }

    // --- OBS£UGA RZUTU (TYLKO GDY NIE MA BIEGANIA) ---
    // Nadal blokujemy sterowanie rzutem, jeœli trwa bieganie,
    // ale fizyka (powy¿ej) dzia³a dalej.
    if (!fazaBiegania)
    {
        // ... (tutaj bez zmian: obs³uga skinLewy.wykonajRzut() itp.)
        if (turaLewego && graczLewy.czyLeci && !strzalWTok) skinLewy.wykonajRzut();
        if (!turaLewego && graczPrawy.czyLeci && !strzalWTok) skinPrawy.wykonajRzut();

        if (turaLewego && graczLewy.czyLeci) strzalWTok = true;
        if (!turaLewego && graczPrawy.czyLeci) strzalWTok = true;

        // Sprawdzamy PUD£O (tylko jeœli NIE trafiono)
        if (strzalWTok)
        {
            bool czyKoniecLotu = false;
            // Sprawdzamy czy lotka siê zatrzyma³a
            if (turaLewego && !graczLewy.czyLeci) czyKoniecLotu = true;
            if (!turaLewego && !graczPrawy.czyLeci) czyKoniecLotu = true;

            // Jeœli lotka spad³a i NIE trafi³a -> zmiana tury
            if (czyKoniecLotu && !puszka.czyTrafiona)
            {
                zmienTure(okno);
            }
        }
    }
}

void Gra::zmienTure(sf::RenderWindow& okno)
{
    // Reset flag stanu
    strzalWTok = false;
    fazaBiegania = false;
    biegWStronePuszki = true;

    // Prze³¹czenie gracza
    turaLewego = !turaLewego;
    puszka.czyTrafiona = false;

    // Reset obiektów fizycznych
    graczLewy.resetuj();
    graczPrawy.resetuj();
    puszka.postaw();

    // Reset obiektów wizualnych (powrót na pozycje startowe)
    skinLewy.zresetujPozycje();
    skinPrawy.zresetujPozycje();
    skinLewy.ustawGotowosc(turaLewego);
    skinPrawy.ustawGotowosc(!turaLewego);

    licznikBota = 0; // Reset "mózgu" bota
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
                // PvP: Instrukcje dla graczy
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
                napisTury.setString("NIEBIESKI PIJ (ENTER)\nCZERWONY BIEGAJ (SPACJA)");
            }
            napisTury.setFillColor(sf::Color::White);
        }
    }
    else
    {
        // Faza rzucania
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

    // Centrowanie tekstu
    sf::FloatRect tekstBounds = napisTury.getLocalBounds();
    napisTury.setOrigin({ tekstBounds.size.x / 2.0f, tekstBounds.size.y / 2.0f });
    napisTury.setPosition({ 960.0f, 100.0f });
}

void Gra::rysuj(sf::RenderWindow& okno)
{
    // [RENDER]
    // Kolejnoœæ renderowania (Painter's Algorithm):
    // 1. T³o i pod³oga (najdalej)
    // 2. Obiekty gry (puszka, paski ¿ycia)
    // 3. Postacie
    // 4. Pociski (musz¹ byæ nad postaciami)
    // 5. UI / HUD (zawsze na wierzchu)

    okno.clear(sf::Color::White);
    okno.draw(tlo);
    okno.draw(podloga.blok);
    okno.draw(puszka.duszek);
    tutorialE3(okno);
    okno.draw(tloPaskaLewego); okno.draw(piwoLewe);
    okno.draw(tloPaskaPrawego); okno.draw(piwoPrawe);
   
    skinLewy.rysuj(okno);
    skinPrawy.rysuj(okno);

    if (turaLewego)
    {
        okno.draw(graczLewy.lotka);
    }
    else
    {
        okno.draw(graczPrawy.lotka);
    }

    // Linia celowania (tylko w fazie celowania)
    if (!czyKoniecGry && !fazaBiegania) {
        if (turaLewego) graczLewy.rysujCelowanie(okno);
        else if (!graZBotem) graczPrawy.rysujCelowanie(okno); // Bot celuje "w pamiêci", nie pokazujemy linii
    }

    if (czyKoniecGry) okno.draw(napisWygranej);
    else okno.draw(napisTury);



    okno.display();
}