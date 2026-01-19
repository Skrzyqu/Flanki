#pragma once
#include <SFML/Graphics.hpp>

class przeszkoda
{
public:
    sf::Vector2f pozycja;
    sf::Vector2f rozmiar;
    sf::RectangleShape blok;
    sf::Texture tekstura;
    sf::Sprite duszek;

    // --- LOGIKA STANU ---
    bool czyTrafiona = false;

    // --- POLA KOLEGI (Fizyka) ---
    bool czyPuszka = false;       // Czy to cel gry, czy œciana?
    float sprezystosc = 1.0f;     // Wspó³czynnik odbicia
	int hp = 3;// Punkty ¿ycia (ile trafieñ wytrzyma)
	bool czyNiezniszczalna = true; // Jeœli true, puszka nie mo¿e zostaæ zniszczona

    // --- TWOJE POLA (Animacja) ---
    bool wTrakcieUpadku = false;
    float aktualnyKat = 0.0f;
    float predkoscObrotu = 0.0f;

    przeszkoda();

    // Wyzwala animacjê upadku
    void przewroc(float predkoscPociskuX);

    // Aktualizuje klatkê animacji
    void aktualizuj();

    // Resetuje puszkê do pionu
    void postaw();
};