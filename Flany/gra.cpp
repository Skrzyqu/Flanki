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
    instrukcjaLewy(czcionka),
    instrukcjaPrawy(czcionka),
    napisWygranej(czcionka),
    graZBotem(trybBot)
{
    if (!teksturaTla.loadFromFile("Assets/Textures/agh_bg2.png")) {
        std::cerr << "[ERROR] Blad ladowania tla!" << std::endl;
    }
    tlo.setTexture(teksturaTla, true);

    sf::Vector2u wymiaryObrazka = teksturaTla.getSize();
    tlo.setScale(sf::Vector2f(1920.0f / wymiaryObrazka.x, 1080.0f / wymiaryObrazka.y));

    if (teksturaLotki.loadFromFile("Assets/Textures/lotka.png")) {
        teksturaLotki.setSmooth(true);
        graczLewy.lotka.setTexture(&teksturaLotki);
        graczPrawy.lotka.setTexture(&teksturaLotki);
    }

    // 1. POD£OGA
// 1. POD£OGA
    podloga.rozmiar = sf::Vector2f(1920.0f, 1080.0f - POZIOM_PODLOGI);
    podloga.pozycja = sf::Vector2f(0.0f, POZIOM_PODLOGI);
    podloga.blok.setSize(podloga.rozmiar);

    // --- ZMIANA: Dodanie tekstury wall.png (SFML 3.0) ---
    if (podloga.tekstura.loadFromFile("Assets/Textures/wall.png")) {
        // W³¹czamy powtarzanie tekstury
        podloga.tekstura.setRepeated(true);
        podloga.blok.setTexture(&podloga.tekstura);

        // SFML 3.0: sf::IntRect wymaga podania wektorów: {pozycja}, {rozmiar}
        // U¿ywamy inicjalizacji klamrowej {} dla sf::Vector2i
        podloga.blok.setTextureRect(sf::IntRect(
            { 0, 0 },
            { static_cast<int>(podloga.rozmiar.x), static_cast<int>(podloga.rozmiar.y) }
        ));

        // Resetujemy kolor na bia³y, aby widzieæ oryginaln¹ teksturê
        podloga.blok.setFillColor(sf::Color::White);
    }
    else {
        std::cerr << "[ERROR] Blad ladowania tekstury wall.png dla podlogi!" << std::endl;
        // Kolor awaryjny (zielony) w przypadku braku pliku
        podloga.blok.setFillColor(sf::Color(34, 139, 34));
    }
    // ----------------------------------------------------

    podloga.blok.setPosition(podloga.pozycja);
    podloga.czyPuszka = false;

    // 2. PUSZKA
    if (!puszka.tekstura.loadFromFile("Assets/Textures/beer.png")) {
        std::cerr << "[ERROR] Blad ladowania tekstury puszki!" << std::endl;
    }
    else {
        puszka.duszek.setTexture(puszka.tekstura, true);
        puszka.duszek.setScale(sf::Vector2f(1.5f, 1.5f));
        sf::FloatRect bounds = puszka.duszek.getGlobalBounds();
        puszka.rozmiar = sf::Vector2f(bounds.size.x, bounds.size.y);
    }
    puszka.czyPuszka = true;
    puszka.sprezystosc = 0.8f;
    puszka.blok.setSize(puszka.rozmiar);
    puszka.blok.setOrigin(sf::Vector2f(puszka.rozmiar.x / 2.0f, puszka.rozmiar.y));
    sf::FloatRect localBounds = puszka.duszek.getLocalBounds();
    puszka.duszek.setOrigin(sf::Vector2f(localBounds.size.x / 2.0f, localBounds.size.y));
    puszka.pozycja = sf::Vector2f(960.0f, POZIOM_PODLOGI);
    puszka.blok.setPosition(puszka.pozycja);
    puszka.duszek.setPosition(puszka.pozycja);

    // 3. UI
    sf::Vector2f rozmiarPaska(50.0f, 400.0f);
    tloPaskaLewego.setSize(rozmiarPaska);
    tloPaskaLewego.setFillColor(sf::Color(50, 50, 50));
    tloPaskaLewego.setPosition(sf::Vector2f(50.0f, 100.0f));

    piwoLewe.setSize(rozmiarPaska);
    piwoLewe.setFillColor(sf::Color(255, 200, 0));
    piwoLewe.setPosition(sf::Vector2f(50.0f, 100.0f));

    tloPaskaPrawego.setSize(rozmiarPaska);
    tloPaskaPrawego.setFillColor(sf::Color(50, 50, 50));
    tloPaskaPrawego.setPosition(sf::Vector2f(1820.0f, 100.0f));

    piwoPrawe.setSize(rozmiarPaska);
    piwoPrawe.setFillColor(sf::Color(255, 200, 0));
    piwoPrawe.setPosition(sf::Vector2f(1820.0f, 100.0f));

    skinLewy.ustawZwrot(true);
    skinPrawy.ustawZwrot(false);

    if (!czcionka.openFromFile("Assets/Fonts/PressStart2P-Regular.ttf")) {
        (void)czcionka.openFromFile("Assets/Fonts/arial.ttf");
    }

    // Konfiguracja tekstów instrukcji
    instrukcjaLewy.setFont(czcionka);
    instrukcjaLewy.setCharacterSize(15);
    instrukcjaLewy.setFillColor(sf::Color::Black);

    instrukcjaPrawy.setFont(czcionka);
    instrukcjaPrawy.setCharacterSize(15);
    instrukcjaPrawy.setFillColor(sf::Color::Black);

    napisWygranej.setFont(czcionka);
    napisWygranej.setCharacterSize(60);
    napisWygranej.setStyle(sf::Text::Bold);
    napisWygranej.setFillColor(sf::Color::Green);

    aktualizujNapis();
    skinLewy.ustawGotowosc(true);
    skinPrawy.ustawGotowosc(false);
}

void Gra::obsluzWejscie(sf::RenderWindow& okno) {
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
    puszka.aktualizuj();
    logikaBota(okno);

    graczLewy.aktualizujFizyke(okno, puszka, grawitacja);
    graczPrawy.aktualizujFizyke(okno, puszka, grawitacja);

    if (puszka.czyTrafiona && !fazaBiegania) {
        fazaBiegania = true;
        biegWStronePuszki = true;
        puszka.blok.setFillColor(sf::Color::Red);
        aktualizujNapis();
    }
    else if (!puszka.czyTrafiona) {
        puszka.blok.setFillColor(sf::Color(192, 192, 192));
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

    switch (wybranyPoziom)
    {
    case 1: tutorialE2(okno); break;
    case 2: tutorialE3(okno); break;
    case 3: tutorialE4(okno); break;
    default: break;
    }

    okno.draw(puszka.duszek);
    okno.draw(tloPaskaLewego); okno.draw(piwoLewe);
    okno.draw(tloPaskaPrawego); okno.draw(piwoPrawe);

    skinLewy.rysuj(okno);
    skinPrawy.rysuj(okno);

    if (!czyKoniecGry) {
        sf::Vector2f posLewy = skinLewy.duszek.getPosition();
        instrukcjaLewy.setPosition(sf::Vector2f(posLewy.x, posLewy.y - 140.0f));

        sf::Vector2f posPrawy = skinPrawy.duszek.getPosition();
        instrukcjaPrawy.setPosition(sf::Vector2f(posPrawy.x, posPrawy.y - 140.0f));

        // --- NOWOŒÆ: Pulsowanie tekstu (Quick Time Event) ---
        if (fazaBiegania) {
            // Sinus daje wartoœæ od -1 do 1. Przesuwamy to na zakres np. 0.9 do 1.1
            float czas = zegarPulsowania.getElapsedTime().asSeconds();
            float predkoscPulsowania = 8.0f; // Szybkie bicie
            float amplituda = 0.15f;          // Jak mocno siê powiêksza

            float skala = 1.0f + std::sin(czas * predkoscPulsowania) * amplituda;

            // Ustawiamy skalê (dzia³a poprawnie, bo mamy ustawiony origin na œrodek)
            instrukcjaLewy.setScale(sf::Vector2f(skala, skala));
            instrukcjaPrawy.setScale(sf::Vector2f(skala, skala));

            // Opcjonalnie: Zmiana koloru na czerwony w rytm pulsowania
            // if (skala > 1.0f) instrukcjaLewy.setFillColor(sf::Color(200, 0, 0));
            // else instrukcjaLewy.setFillColor(sf::Color::Black);
        }
        else {
            // Reset skali gdy nie ma QTE
            instrukcjaLewy.setScale(sf::Vector2f(1.0f, 1.0f));
            instrukcjaPrawy.setScale(sf::Vector2f(1.0f, 1.0f));

            // Reset koloru
            // instrukcjaLewy.setFillColor(sf::Color::Black);
        }

        okno.draw(instrukcjaLewy);
        okno.draw(instrukcjaPrawy);
    }

    if (turaLewego) okno.draw(graczLewy.lotka);
    else okno.draw(graczPrawy.lotka);

    if (!czyKoniecGry && !fazaBiegania) {
        if (turaLewego) graczLewy.rysujCelowanie(okno);
        else if (!graZBotem) graczPrawy.rysujCelowanie(okno);
    }

    if (czyKoniecGry) okno.draw(napisWygranej);

    okno.display();
}

void Gra::zmienTure(sf::RenderWindow& okno) {
    strzalWTok = false;
    fazaBiegania = false;
    biegWStronePuszki = true;
    turaLewego = !turaLewego;
    puszka.czyTrafiona = false;
    puszka.postaw();

    graczLewy.resetuj();
    graczPrawy.resetuj();
    skinLewy.zresetujPozycje();
    skinPrawy.zresetujPozycje();
    skinLewy.ustawGotowosc(turaLewego);
    skinPrawy.ustawGotowosc(!turaLewego);
    licznikBota = 0;
    aktualizujNapis();
}

void Gra::centrujTekst(sf::Text& tekst) {
    sf::FloatRect bounds = tekst.getLocalBounds();
    tekst.setOrigin(sf::Vector2f(std::round(bounds.size.x / 2.0f), std::round(bounds.size.y / 2.0f)));
}

void Gra::aktualizujNapis() {
    if (czyKoniecGry) return;

    if (fazaBiegania) {
        if (turaLewego) {
            instrukcjaLewy.setString("PIJ [SPACJA]");
            if (graZBotem) instrukcjaPrawy.setString("BOT BIEGNIE");
            else instrukcjaPrawy.setString("BIEGAJ [ENTER]");
        }
        else {
            instrukcjaLewy.setString("BIEGAJ [SPACJA]");
            if (graZBotem) instrukcjaPrawy.setString("BOT PIJE");
            else instrukcjaPrawy.setString("PIJ [ENTER]");
        }
    }
    else {
        if (turaLewego) {
            instrukcjaLewy.setString("RZUCAJ");
            instrukcjaPrawy.setString("");
        }
        else {
            instrukcjaLewy.setString("");
            if (graZBotem) instrukcjaPrawy.setString("TURA BOTA");
            else instrukcjaPrawy.setString("RZUCAJ");
        }
    }

    centrujTekst(instrukcjaLewy);
    centrujTekst(instrukcjaPrawy);
}

void Gra::obsluzPicie() {
    if (czyKoniecGry) return;
    if (turaLewego) {
        if (poziomLewego > 0) {
            poziomLewego -= LYK_PIWA;
            if (poziomLewego <= 0.1f) { poziomLewego = 0.0f; sprawdzWygrana(); }
            piwoLewe.setSize(sf::Vector2f(50.0f, (poziomLewego / 100.0f) * 400.0f));
        }
    }
    else {
        if (poziomPrawego > 0) {
            poziomPrawego -= LYK_PIWA;
            if (poziomPrawego <= 0.1f) { poziomPrawego = 0.0f; sprawdzWygrana(); }
            piwoPrawe.setSize(sf::Vector2f(50.0f, (poziomPrawego / 100.0f) * 400.0f));
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
            puszka.postaw();
            biegWStronePuszki = false;

            if (turaLewego) {
                if (graZBotem) instrukcjaPrawy.setString("BOT WRACA");
                else instrukcjaPrawy.setString("WRACAJ [ENTER]");
            }
            else {
                instrukcjaLewy.setString("WRACAJ [SPACJA]");
            }
            centrujTekst(instrukcjaLewy);
            centrujTekst(instrukcjaPrawy);
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
        // Wygra³ gracz lewy
        if (graZBotem) {
            napisWygranej.setString("WYGRAL GRACZ!");
        }
        else {
            napisWygranej.setString("WYGRAL GRACZ 1!");
        }
        napisWygranej.setFillColor(sf::Color::Red);
        wygrana = true;
    }
    else if (poziomPrawego <= 0.1f) {
        // Wygra³ gracz prawy
        if (graZBotem) {
            napisWygranej.setString("WYGRAL BOT!");
        }
        else {
            napisWygranej.setString("WYGRAL GRACZ 2!");
        }
        napisWygranej.setFillColor(sf::Color::Blue);
        wygrana = true;
    }

    if (wygrana) {
        czyKoniecGry = true;
        sf::FloatRect bounds = napisWygranej.getLocalBounds();
        napisWygranej.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
        napisWygranej.setPosition(sf::Vector2f(960.0f, 540.0f));
        fazaBiegania = false;

        // Ukrywamy instrukcje po zakoñczeniu gry
        instrukcjaLewy.setString("");
        instrukcjaPrawy.setString("");
    }
}

void Gra::logikaBota(sf::RenderWindow& okno) {
    if (!graZBotem) return;
    if (!fazaBiegania && !strzalWTok && !turaLewego) {
        licznikBota++;
        if (licznikBota > 60) {
            sf::Vector2f cel = sf::Vector2f(puszka.pozycja.x + puszka.rozmiar.x / 2.0f, puszka.pozycja.y - puszka.rozmiar.y / 2.0f);
            sf::Vector2f start = graczPrawy.pozycja;
            float g = 0.2f, czasLotu = 130.0f;
            float vx = (cel.x - start.x) / czasLotu;
            float vy = (cel.y - start.y - 0.5f * g * czasLotu * czasLotu) / czasLotu;
            float bladX = (rand() % 30 - 15) / 10.0f;
            float bladY = (rand() % 30 - 15) / 10.0f;
            graczPrawy.predkosc = sf::Vector2f(vx + bladX, vy + bladY);
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