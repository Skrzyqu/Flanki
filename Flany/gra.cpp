#include "Gra.h"
#include <cmath>
#include <iostream>

Gra::Gra(bool trybBot) :
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
    if (!teksturaTla.loadFromFile("agh_bg2.png")) {
        std::cerr << "[ERROR] Blad ladowania tla!" << std::endl;
    }
    tlo.setTexture(teksturaTla, true);

    // Skalowanie t³a
    sf::Vector2u wymiaryObrazka = teksturaTla.getSize();
    tlo.setScale({ 1920.0f / wymiaryObrazka.x, 1080.0f / wymiaryObrazka.y });

    // Tekstura lotki (opcjonalnie, z Twojego kodu)
    if (teksturaLotki.loadFromFile("lotka.png")) {
        teksturaLotki.setSmooth(true);
        graczLewy.lotka.setTexture(&teksturaLotki);
        graczPrawy.lotka.setTexture(&teksturaLotki);
    }

    // 1. POD£OGA
    podloga.rozmiar = { 1920.0f, 1080.0f - POZIOM_PODLOGI };
    podloga.pozycja = { 0.0f, POZIOM_PODLOGI };
    podloga.blok.setSize(podloga.rozmiar);
    podloga.blok.setFillColor(sf::Color(34, 139, 34));
    podloga.blok.setPosition(podloga.pozycja);
    podloga.czyPuszka = false; // To nie jest cel!

    // 2. PUSZKA (CEL)
    if (!puszka.tekstura.loadFromFile("beer.png")) {
        std::cerr << "[ERROR] Blad ladowania tekstury puszki!" << std::endl;
    }
    else {
        puszka.duszek.setTexture(puszka.tekstura, true);
        puszka.duszek.setScale({ 1.5f, 1.5f });
        sf::FloatRect bounds = puszka.duszek.getGlobalBounds();
        puszka.rozmiar = bounds.size;
    }

    // SCALENIE: Ustawiamy flagi, ¿e to jest cel gry i ma sprê¿ystoœæ
    puszka.czyPuszka = true;
    puszka.sprezystosc = 0.8f;

    puszka.blok.setSize(puszka.rozmiar);
    puszka.blok.setOrigin({ puszka.rozmiar.x / 2.0f, puszka.rozmiar.y });

    sf::FloatRect localBounds = puszka.duszek.getLocalBounds();
    puszka.duszek.setOrigin({ localBounds.size.x / 2.0f, localBounds.size.y });

    puszka.pozycja = { 960.0f, POZIOM_PODLOGI };
    puszka.blok.setPosition(puszka.pozycja);
    puszka.duszek.setPosition(puszka.pozycja);

    // 3. UI
    sf::Vector2f rozmiarPaska{ 50.0f, 400.0f };
    tloPaskaLewego.setSize(rozmiarPaska); tloPaskaLewego.setFillColor(sf::Color(50, 50, 50)); tloPaskaLewego.setPosition({ 50.0f, 100.0f });
    piwoLewe.setSize(rozmiarPaska); piwoLewe.setFillColor(sf::Color(255, 200, 0)); piwoLewe.setPosition({ 50.0f, 100.0f });

    tloPaskaPrawego.setSize(rozmiarPaska); tloPaskaPrawego.setFillColor(sf::Color(50, 50, 50)); tloPaskaPrawego.setPosition({ 1820.0f, 100.0f });
    piwoPrawe.setSize(rozmiarPaska); piwoPrawe.setFillColor(sf::Color(255, 200, 0)); piwoPrawe.setPosition({ 1820.0f, 100.0f });

    skinLewy.ustawZwrot(true);
    skinPrawy.ustawZwrot(false);

    (void)czcionka.openFromFile("arial.ttf");
    napisTury.setCharacterSize(60); napisTury.setStyle(sf::Text::Bold);
    napisWygranej.setCharacterSize(100); napisWygranej.setStyle(sf::Text::Bold); napisWygranej.setFillColor(sf::Color::Green);

    aktualizujNapis();
    skinLewy.ustawGotowosc(true);
    skinPrawy.ustawGotowosc(false);
}

void Gra::obsluzWejscie(sf::RenderWindow& okno) {
    // Twoja oryginalna funkcja z pliku gra.cpp (skopiuj j¹ stamt¹d, jest bez zmian)
    // Skrótowo tutaj: obs³uga ESC, Spacji i Entera do picia/biegania.
    while (const std::optional event = okno.pollEvent()) {
        if (event->is<sf::Event::Closed>()) okno.close();
        if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) okno.close();
        if (czyKoniecGry) continue;

        if (fazaBiegania) {
            if (event->is<sf::Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Space) {
                    if (turaLewego) obsluzPicie(); else obsluzBieganie();
                }
                if (!graZBotem && keyEvent->code == sf::Keyboard::Key::Enter) {
                    if (turaLewego) obsluzBieganie(); else obsluzPicie();
                }
            }
        }
        else if (!strzalWTok) {
            if (turaLewego) graczLewy.obsluzWejscie(*event, okno);
            else if (!graZBotem) graczPrawy.obsluzWejscie(*event, okno);
        }
    }
}

void Gra::aktualizuj(sf::RenderWindow& okno, sf::Vector2f grawitacja) {
    if (czyKoniecGry) return;

    skinLewy.aktualizujAnimacje();
    skinPrawy.aktualizujAnimacje();
    puszka.aktualizuj(); // Twoja animacja upadku
    logikaBota(okno);

    // Fizyka dzia³a zawsze, ¿eby pi³ka mog³a siê toczyæ
    graczLewy.aktualizujFizyke(okno, puszka, grawitacja);
    graczPrawy.aktualizujFizyke(okno, puszka, grawitacja);

    // Detekcja trafienia
    if (puszka.czyTrafiona && !fazaBiegania) {
        fazaBiegania = true;
        biegWStronePuszki = true;
        puszka.blok.setFillColor(sf::Color::Red);
        aktualizujNapis();
    }
    else if (!puszka.czyTrafiona) {
        puszka.blok.setFillColor(sf::Color(192, 192, 192)); // Opcjonalnie reset koloru
    }

    if (!fazaBiegania) {
        if (turaLewego && graczLewy.czyLeci && !strzalWTok) skinLewy.wykonajRzut();
        if (!turaLewego && graczPrawy.czyLeci && !strzalWTok) skinPrawy.wykonajRzut();
        if (turaLewego && graczLewy.czyLeci) strzalWTok = true;
        if (!turaLewego && graczPrawy.czyLeci) strzalWTok = true;

        if (strzalWTok) {
            bool czyKoniecLotu = false;
            if (turaLewego && !graczLewy.czyLeci) czyKoniecLotu = true;
            if (!turaLewego && !graczPrawy.czyLeci) czyKoniecLotu = true;

            if (czyKoniecLotu && !puszka.czyTrafiona) zmienTure(okno);
        }
    }
}

void Gra::rysuj(sf::RenderWindow& okno)
{
    okno.clear(sf::Color::White);
    okno.draw(tlo);
    okno.draw(podloga.blok);

    // SCALENIE: Rysowanie poziomu trampolin
    tutorialE3(okno);

    okno.draw(puszka.duszek); // Rysujemy sprite (nie blok)
    okno.draw(tloPaskaLewego); okno.draw(piwoLewe);
    okno.draw(tloPaskaPrawego); okno.draw(piwoPrawe);

    skinLewy.rysuj(okno);
    skinPrawy.rysuj(okno);

    if (turaLewego) okno.draw(graczLewy.lotka);
    else okno.draw(graczPrawy.lotka);

    if (!czyKoniecGry && !fazaBiegania) {
        if (turaLewego) graczLewy.rysujCelowanie(okno);
        else if (!graZBotem) graczPrawy.rysujCelowanie(okno);
    }

    if (czyKoniecGry) okno.draw(napisWygranej);
    else okno.draw(napisTury);

    okno.display();
}

// Reszta metod pomocniczych (zmienTure, aktualizujNapis, obsluzBieganie, obsluzPicie, sprawdzWygrana, logikaBota)
// skopiuj je w ca³oœci ze swojego oryginalnego pliku gra.cpp - nie wymaga³y zmian.
// Poni¿ej wklejam je dla pewnoœci, ¿e masz komplet:

void Gra::zmienTure(sf::RenderWindow& okno) {
    strzalWTok = false;
    fazaBiegania = false;
    biegWStronePuszki = true;
    turaLewego = !turaLewego;
    puszka.czyTrafiona = false; // Reset flagi trafienia logicznego
    puszka.postaw();            // Reset animacji

    graczLewy.resetuj();
    graczPrawy.resetuj();
    skinLewy.zresetujPozycje();
    skinPrawy.zresetujPozycje();
    skinLewy.ustawGotowosc(turaLewego);
    skinPrawy.ustawGotowosc(!turaLewego);
    licznikBota = 0;
    aktualizujNapis();
}

void Gra::aktualizujNapis() {
    if (czyKoniecGry) return;
    if (fazaBiegania) {
        if (turaLewego) {
            if (graZBotem) napisTury.setString("BOT BIEGNIE! Ty PIJ (SPACJA)!");
            else napisTury.setString("CZERWONY PIJ (SPACJA)\nNIEBIESKI BIEGAJ (ENTER)");
            napisTury.setFillColor(sf::Color::White);
        }
        else {
            if (graZBotem) napisTury.setString("BOT PIJE! Ty BIEGAJ (SPACJA)!");
            else napisTury.setString("NIEBIESKI PIJ (ENTER)\nCZERWONY BIEGAJ (SPACJA)");
            napisTury.setFillColor(sf::Color::White);
        }
    }
    else {
        if (turaLewego) {
            napisTury.setString("Tura Czerwonego"); napisTury.setFillColor(sf::Color::Red);
        }
        else {
            if (graZBotem) napisTury.setString("Tura Bota..."); else napisTury.setString("Tura Niebieskiego");
            napisTury.setFillColor(sf::Color::Blue);
        }
    }
    sf::FloatRect bounds = napisTury.getLocalBounds();
    napisTury.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    napisTury.setPosition({ 960.0f, 100.0f });
}

void Gra::obsluzPicie() {
    if (czyKoniecGry) return;
    if (turaLewego) {
        if (poziomLewego > 0) {
            poziomLewego -= LYK_PIWA;
            if (poziomLewego <= 0.1f) { poziomLewego = 0.0f; sprawdzWygrana(); }
            piwoLewe.setSize({ 50.0f, (poziomLewego / 100.0f) * 400.0f });
        }
    }
    else {
        if (poziomPrawego > 0) {
            poziomPrawego -= LYK_PIWA;
            if (poziomPrawego <= 0.1f) { poziomPrawego = 0.0f; sprawdzWygrana(); }
            piwoPrawe.setSize({ 50.0f, (poziomPrawego / 100.0f) * 400.0f });
        }
    }
}

void Gra::obsluzBieganie() {
    if (czyKoniecGry) return;
    Bohater* biegacz = turaLewego ? &skinPrawy : &skinLewy;
    sf::Vector2f cel = biegWStronePuszki ? sf::Vector2f(puszka.pozycja.x, POZIOM_PODLOGI) : biegacz->pozycjaStartowa;

    sf::Vector2f roznica = cel - biegacz->duszek.getPosition();
    float dystans = std::sqrt(roznica.x * roznica.x + roznica.y * roznica.y);

    if (dystans <= szybkoscBiegu) {
        biegacz->duszek.setPosition(cel);
        biegacz->zatrzymajSie();
        if (biegWStronePuszki) {
            puszka.postaw(); // Stawia puszkê
            biegWStronePuszki = false;
            napisTury.setString("WRACAJ!");
            napisTury.setFillColor(sf::Color::Yellow);
        }
        else {
            zmienTure(*(sf::RenderWindow*)nullptr);
        }
    }
    else {
        biegacz->podejdzDo(cel, szybkoscBiegu);
    }
}

void Gra::sprawdzWygrana() {
    bool wygrana = false;
    if (poziomLewego <= 0.1f) {
        napisWygranej.setString("WYGRAL CZERWONY!"); napisWygranej.setFillColor(sf::Color::Red); wygrana = true;
    }
    else if (poziomPrawego <= 0.1f) {
        napisWygranej.setString("WYGRAL NIEBIESKI!"); napisWygranej.setFillColor(sf::Color::Blue); wygrana = true;
    }
    if (wygrana) {
        czyKoniecGry = true;
        sf::FloatRect bounds = napisWygranej.getLocalBounds();
        napisWygranej.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
        napisWygranej.setPosition({ 960.0f, 540.0f });
        fazaBiegania = false;
    }
}

void Gra::logikaBota(sf::RenderWindow& okno) {
    if (!graZBotem) return;
    if (!fazaBiegania && !strzalWTok && !turaLewego) {
        licznikBota++;
        if (licznikBota > 60) {
            sf::Vector2f cel = { puszka.pozycja.x + puszka.rozmiar.x / 2.0f, puszka.pozycja.y - puszka.rozmiar.y / 2.0f };
            sf::Vector2f start = graczPrawy.pozycja;
            float g = 0.2f, czasLotu = 95.0f;
            float vx = (cel.x - start.x) / czasLotu;
            float vy = (cel.y - start.y - 0.5f * g * czasLotu * czasLotu) / czasLotu;
            float bladX = (rand() % 30 - 15) / 10.0f;
            float bladY = (rand() % 30 - 15) / 10.0f;
            graczPrawy.predkosc = { vx + bladX, vy + bladY };
            graczPrawy.czyLeci = true;
            licznikBota = 0;
        }
    }
    if (fazaBiegania) {
        licznikBota++;
        if (licznikBota > 5) {
            licznikBota = 0;
            if (turaLewego) obsluzBieganie(); else obsluzPicie();
        }
    }
}