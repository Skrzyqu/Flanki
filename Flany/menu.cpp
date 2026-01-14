#include "Menu.h"

Menu::Menu(float szerokosc, float wysokosc) :
    tekstMenu(czcionka) // Inicjalizacja tekstu
{
    // £adowanie czcionki
    (void)czcionka.openFromFile("arial.ttf");

    tekstMenu.setString("WYBIERZ TRYB:\n\n1. Gracz vs Gracz\n2. Gracz vs Komputer");
    tekstMenu.setCharacterSize(60);
    tekstMenu.setFillColor(sf::Color::White);
    tekstMenu.setStyle(sf::Text::Bold);

    // Centrowanie
    sf::FloatRect bounds = tekstMenu.getLocalBounds();
    tekstMenu.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    tekstMenu.setPosition({ szerokosc / 2.0f, wysokosc / 2.0f });
}

void Menu::rysuj(sf::RenderWindow& okno)
{
    okno.clear(sf::Color::Black);
    okno.draw(tekstMenu);
    okno.display();
}

int Menu::sprawdzWybor(sf::RenderWindow& okno)
{
    while (const std::optional event = okno.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            okno.close();
            return -1; // Kod zamkniêcia
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

            if (keyEvent->code == sf::Keyboard::Key::Num1) return 1; // PvP
            if (keyEvent->code == sf::Keyboard::Key::Num2) return 2; // PvE
            if (keyEvent->code == sf::Keyboard::Key::Escape) okno.close();
        }
    }
    return 0; // Brak wyboru w tej klatce
}