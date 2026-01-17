#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Bohater {
public:
    sf::Texture tekstura;
    std::vector<sf::Texture> klatkiAnimacji;
    std::vector<sf::Texture> klatkiBiegania;
    sf::Sprite duszek;

    sf::Vector2f pozycjaStartowa;
    bool twarzaWPrawo;
    
    // --- ZMIENNE DO ANIMACJI ---
    bool czyAnimujeRzut;
    bool czyJestGotowy;
    bool czyBiegnie;
    int obecnaKlatka;
    sf::Clock zegarAnimacji;
    float czasMiedzyKlatkami = 0.1f; // Szybkoœæ animacji (im mniej, tym szybciej)

    Bohater(float x, float y);

    void rysuj(sf::RenderWindow& okno);
    void podejdzDo(sf::Vector2f cel, float szybkosc);
    void zresetujPozycje();
    void ustawZwrot(bool wPrawo);

    void aktualizujAnimacje(); // To wywo³ujemy w ka¿dej klatce gry
    void wykonajRzut();        // To wywo³ujemy raz, gdy gracz strzela
    void ustawGotowosc(bool gotowy);
    void zatrzymajSie();
};