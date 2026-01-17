#pragma once
#include <SFML/Graphics.hpp>

class przeszkoda
{
public:
    sf::Vector2f pozycja;
    sf::Vector2f rozmiar; // Oryginalny rozmiar (stoj¹cy)
    sf::RectangleShape blok;
    sf::Texture tekstura;
    sf::Sprite duszek;

    bool czyTrafiona = false;

    // --- NOWE ZMIENNE DO ANIMACJI ---
    bool wTrakcieUpadku = false;
    float aktualnyKat = 0.0f;
    float predkoscObrotu = 0.0f; // Ile stopni na klatkê

    przeszkoda();

    // Wyzwala animacjê upadku (zale¿nie od kierunku uderzenia)
    void przewroc(float predkoscPociskuX);

    // Aktualizuje klatkê animacji (obrót)
    void aktualizuj();

    // Resetuje puszkê do pionu
    void postaw();
};
