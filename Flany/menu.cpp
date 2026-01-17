#include "Menu.h"

Menu::Menu(float szerokosc, float wysokosc) :
    tekstMenu(czcionka) // Inicjalizacja listy inicjalizacyjnej
{
    // [IO/BLOCKING] Synchroniczne ³adowanie z dysku.
    // Akceptowalne w menu startowym, ale w trakcie gry powodowa³oby "klatkowanie".
    // W produkcji zasoby powinny pochodziæ z cache'a (ResourceManager).
    (void)czcionka.openFromFile("arial.ttf");

    // [TODO]: Hardcoded strings. W przysz³oœci przenieœæ do systemu lokalizacji (np. JSON/XML).
    tekstMenu.setString("WYBIERZ TRYB:\n\n1. Gracz vs Gracz\n2. Gracz vs Komputer");
    tekstMenu.setCharacterSize(60);
    tekstMenu.setFillColor(sf::Color::White);
    tekstMenu.setStyle(sf::Text::Bold);

    // --- LAYOUTING ---
    // Ustawiamy Pivot Point (Origin) na idealny œrodek tekstu,
    // co pozwala ³atwo wycentrowaæ go na ekranie bez skomplikowanej matematyki.
    sf::FloatRect bounds = tekstMenu.getLocalBounds();
    tekstMenu.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    tekstMenu.setPosition({ szerokosc / 2.0f, wysokosc / 2.0f });
}

void Menu::rysuj(sf::RenderWindow& okno)
{
    // Prosta pêtla renderowania: Clear -> Draw -> Display
    // Nie potrzebujemy tu skomplikowanego potoku graficznego (brak shaderów, œwiate³).
    okno.clear(sf::Color::Black);
    okno.draw(tekstMenu);
    okno.display();
}

int Menu::sprawdzWybor(sf::RenderWindow& okno)
{
    // [EVENT POLLING]
    // Przetwarzamy wszystkie zdarzenia z kolejki systemowej (OS).
    while (const std::optional event = okno.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            okno.close();
            return -1; // Kod zamkniêcia aplikacji
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

            // Mapowanie klawiszy na kody stanów gry
            if (keyEvent->code == sf::Keyboard::Key::Num1) return 1; // Tryb PvP
            if (keyEvent->code == sf::Keyboard::Key::Num2) return 2; // Tryb PvE
            if (keyEvent->code == sf::Keyboard::Key::Escape) okno.close();
        }
    }
    return 0; // Brak decyzji w tej klatce (zostañ w menu)
}