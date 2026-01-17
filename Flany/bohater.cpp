#include "bohater.h"
#include <iostream>
#include <cmath>

Bohater::Bohater(float x, float y) : duszek(tekstura) {

    // --- 1. £ADUJEMY KLATKI RZUTU ---
    std::vector<std::string> pliki = {
        "AGH_skin-1.png",
        "AGH_skin-2.png",
        "AGH_skin-3.png",
        "AGH_skin-4.png"
    };
    klatkiAnimacji.resize(4);
    for (int i = 0; i < 4; i++) {
        if (!klatkiAnimacji[i].loadFromFile(pliki[i])) {
            std::cerr << "Blad ladowania klatki: " << pliki[i] << std::endl;
        }
    }

    // --- 2. £ADUJEMY KLATKI BIEGU ---
    std::vector<std::string> plikiBieg = {
        "AGH_skin-4.png", // Stanie
        "AGH_run2.png",   // Krok lewa
        "AGH_skin-4.png", // Stanie
        "AGH_run4.png"    // Krok prawa
    };
    klatkiBiegania.resize(4);
    for (int i = 0; i < 4; i++) {
        if (!klatkiBiegania[i].loadFromFile(plikiBieg[i])) {
            std::cerr << "Blad ladowania biegu: " << plikiBieg[i] << std::endl;
        }
    }

    // Ustawienia pocz¹tkowe
    duszek.setTexture(klatkiAnimacji[3], true);

    pozycjaStartowa = { x, y };
    duszek.setPosition(pozycjaStartowa);
    twarzaWPrawo = true;

    // Inicjalizacja zmiennych stanu
    czyAnimujeRzut = false;
    czyJestGotowy = false;
    czyBiegnie = false;
    obecnaKlatka = 3;

    // Origin i Skala
    sf::FloatRect bounds = duszek.getLocalBounds();
    duszek.setOrigin({ bounds.size.x / 2.0f, bounds.size.y });
    duszek.setScale({ 4.0f, 4.0f });
}

void Bohater::ustawGotowosc(bool gotowy) {
    if (czyBiegnie) return;

    czyJestGotowy = gotowy;

    if (czyJestGotowy) {
        obecnaKlatka = 0; // Gotowy do rzutu (Klatka 1)
    }
    else {
        obecnaKlatka = 3; // Luz (Klatka 4)
    }
    duszek.setTexture(klatkiAnimacji[obecnaKlatka], true);
}

void Bohater::wykonajRzut() {
    czyJestGotowy = false; // Rzucamy, wiêc przestajemy "czekaæ"
    czyAnimujeRzut = true;
    obecnaKlatka = 0;
    duszek.setTexture(klatkiAnimacji[0], true);
    zegarAnimacji.restart();
}

void Bohater::zatrzymajSie() {
    if (czyBiegnie) {
        czyBiegnie = false;
        // Po zatrzymaniu wracamy do pozycji luzu (Klatka 4)
        obecnaKlatka = 3;
        duszek.setTexture(klatkiAnimacji[3], true);
    }
}

void Bohater::aktualizujAnimacje() {
    // --- 1. PRIORYTET: RZUT ---
    if (czyAnimujeRzut) {
        if (zegarAnimacji.getElapsedTime().asSeconds() > czasMiedzyKlatkami) {
            obecnaKlatka++;
            zegarAnimacji.restart();

            if (obecnaKlatka >= klatkiAnimacji.size()) {
                czyAnimujeRzut = false;
                obecnaKlatka = 3; // Po rzucie wracamy do stania
            }

            // Zabezpieczenie przed wyjœciem poza tablicê
            if (obecnaKlatka < klatkiAnimacji.size()) {
                duszek.setTexture(klatkiAnimacji[obecnaKlatka], true);
            }
        }
    }
    // --- 2. PRIORYTET: BIEGANIE (TEGO BRAKOWA£O) ---
    else if (czyBiegnie) {
        if (zegarAnimacji.getElapsedTime().asSeconds() > czasMiedzyKlatkami) {
            obecnaKlatka++;
            zegarAnimacji.restart();

            // Pêtla biegania (jak dojdzie do koñca, wraca na 0)
            if (obecnaKlatka >= klatkiBiegania.size()) {
                obecnaKlatka = 0;
            }

            // Bierzemy teksturê z wektora BIEGANIA
            duszek.setTexture(klatkiBiegania[obecnaKlatka], true);
        }
    }
    // --- 3. PRIORYTET: STANIE (GOTOWOŒÆ / LUZ) ---
    else {
        int docelowaKlatka = czyJestGotowy ? 0 : 3;
        if (obecnaKlatka != docelowaKlatka) {
            obecnaKlatka = docelowaKlatka;
            duszek.setTexture(klatkiAnimacji[obecnaKlatka], true);
        }
    }
}

void Bohater::rysuj(sf::RenderWindow& okno) {
    okno.draw(duszek);
}

void Bohater::zresetujPozycje() {
    duszek.setPosition(pozycjaStartowa);
    czyBiegnie = false;
    czyAnimujeRzut = false;

    if (pozycjaStartowa.x < 960.0f) ustawZwrot(true);
    else ustawZwrot(false);
}

void Bohater::ustawZwrot(bool wPrawo) {
    float skalaX = std::abs(duszek.getScale().x);
    float skalaY = duszek.getScale().y;

    if (wPrawo) {
        duszek.setScale({ -skalaX, skalaY });
    }
    else {
        duszek.setScale({ skalaX, skalaY });
    }

}

void Bohater::podejdzDo(sf::Vector2f cel, float szybkosc) {

    // --- W£¥CZANIE ANIMACJI BIEGU ---
    if (!czyBiegnie) {
        czyBiegnie = true;
        obecnaKlatka = 0;
        zegarAnimacji.restart();
    }

    sf::Vector2f obecnaPozycja = duszek.getPosition();
    sf::Vector2f kierunek = cel - obecnaPozycja;
    float dystans = std::sqrt(kierunek.x * kierunek.x + kierunek.y * kierunek.y);

    if (kierunek.x > 0) ustawZwrot(true);
    else if (kierunek.x < 0) ustawZwrot(false);

    if (dystans > szybkosc) {
        sf::Vector2f ruch = (kierunek / dystans) * szybkosc;
        duszek.move(ruch);
    }
    else {
        duszek.setPosition(cel);
    }
}