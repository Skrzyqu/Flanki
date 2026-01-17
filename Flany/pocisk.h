#pragma once
#include <SFML/Graphics.hpp>
#include "przeszkoda.h"

// --- KONFIGURACJA FIZYKI ---
constexpr auto precyzja = 1.0f;     ///< Próg prêdkoœci, poni¿ej którego uznajemy, ¿e obiekt siê zatrzyma³ (sleep threshold).
constexpr float MAX_NACIAG = 100.0f;  ///< Maksymalna d³ugoœæ wektora celowania (w pikselach).
constexpr float POZIOM_PODLOGI = 1030.0f;

/**
 * =========================================================================================
 * HIGH LEVEL OVERVIEW
 * =========================================================================================
 * Klasa symuluj¹ca balistykê pocisku (lotki/pi³ki).
 * Implementuje:
 * 1. Mechanikê procy (Slingshot) - obliczanie wektora pocz¹tkowego myszk¹.
 * 2. Fizykê Newtonowsk¹ - Ca³kowanie Eulera (Pozycja += Prêdkoœæ).
 * 3. Proste odbicia od pod³ogi ze strat¹ energii.
 * =========================================================================================
 */
class pocisk
{
public:
	// --- FIZYKA ---
	sf::Vector2f pozycja;
	sf::Vector2f predkosc; // Velocity vector (vx, vy)
	sf::CircleShape lotka{ 10.0f };

	// --- STANY ---
	bool czyLeci = false;
	bool czyCeluje = false; // Czy gracz trzyma naci¹gniêt¹ "gumê"
	sf::Vector2f pozycjaStartowa;

	pocisk(float x, float y, sf::Color kolor);

	/**
	 * @brief Obs³uguje interakcjê myszk¹ (Drag & Release).
	 * Ustawia wektor prêdkoœci pocz¹tkowej na podstawie naci¹gu.
	 */
	void obsluzWejscie(sf::Event event, const sf::RenderWindow& okno);

	/**
	 * @brief [SIMULATION] Krok fizyczny.
	 * Aplikuje grawitacjê, przesuwa obiekt i sprawdza kolizje.
	 */
	void aktualizujFizyke(sf::RenderWindow& okno, przeszkoda& cel, sf::Vector2f grawitacja);

	/** Rysuje liniê pomocnicz¹ (wektor naci¹gu) podczas celowania. */
	void rysujCelowanie(sf::RenderWindow& okno);

	void resetuj();
private:
	// --- METODY FIZYCZNE ---

	void zmiana_pozycji();
	void zmiana_predkosci(sf::Vector2f grawitacja);

	/** Obs³uguje kolizjê z doln¹ krawêdzi¹ ekranu (odbicie + t³umienie). */
	void odbicie_podloze(sf::RenderWindow* okno);

	/** Oblicza wizualn¹ pozycjê lotki "na gumce" podczas celowania (Clamping). */
	void aktualizujPozycjeCelowania(const sf::RenderWindow& okno);
};