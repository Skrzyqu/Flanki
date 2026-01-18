#include "Menu.h"
#include <optional>
#include <iostream>



Menu::Menu(float szerokosc, float wysokosc) :
    tekstMenu(czcionka), // Inicjalizacja listy inicjalizacyjnej
    spriteTlo(spriteTlo),
    spritePvP(spritePvP),
    spritePvE(spritePvE)
{
    // [IO/BLOCKING] Synchroniczne ³adowanie z dysku.
    // Akceptowalne w menu startowym, ale w trakcie gry powodowa³oby "klatkowanie".
    // W produkcji zasoby powinny pochodziæ z cache'a (ResourceManager).
    (void)czcionka.openFromFile("arial.ttf");

    // [TODO]: Hardcoded strings. W przysz³oœci przenieœæ do systemu lokalizacji (np. JSON/XML).
    tekstMenu.setString("WYBIERZ TRYB:\n\n");
        //1. Gracz vs Gracz\n2. Gracz vs Komputer");
    tekstMenu.setCharacterSize(60);
    tekstMenu.setFillColor(sf::Color::Black);
    tekstMenu.setStyle(sf::Text::Bold);

    if (!textureTlo.loadFromFile("menu_tlo.png"))
    {
        // Obs³uga b³êdu - tworzenie zastêpczej tekstury
        sf::Texture fallbackTexture;
        sf::Image fallbackImage({ 1920, 1080 }, sf::Color(20, 20, 40));
        textureTlo.loadFromImage(fallbackImage);
    }

    if (!texturePvP.loadFromFile("btn_pvp.png"))
    {
        sf::Texture fallbackTexture;
        sf::Image fallbackImage({ 200, 50 }, sf::Color(100, 100, 200));
        texturePvP.loadFromImage(fallbackImage);
    }

    if (!texturePvE.loadFromFile("btn_pve.png"))
    {
        sf::Texture fallbackTexture;
        sf::Image fallbackImage({ 200, 50 }, sf::Color(200, 100, 100));
        texturePvE.loadFromImage(fallbackImage);
    }

    
    spriteTlo = sf::Sprite(textureTlo); 
    spritePvP = sf::Sprite(texturePvP);
    spritePvE = sf::Sprite(texturePvE);
    

    // --- LAYOUTING ---
    // Ustawiamy Pivot Point (Origin) na idealny œrodek tekstu,
    // co pozwala ³atwo wycentrowaæ go na ekranie bez skomplikowanej matematyki.
    sf::Vector2u textureSize = textureTlo.getSize();
    if (textureSize.x > 0 && textureSize.y > 0) {
        // Obliczamy skalê dla rozci¹gniêcia
        float scaleX = szerokosc / static_cast<float>(textureSize.x);
        float scaleY = wysokosc / static_cast<float>(textureSize.y);
        spriteTlo.setScale(sf::Vector2f(scaleX, scaleY));
    }

    // Ustawiamy pozycjê t³a (zawsze w lewym górnym rogu)
    spriteTlo.setPosition(sf::Vector2f(0.f, 0.f));

	// Pozycjonowanie tekstu menu
    sf::FloatRect bounds = tekstMenu.getLocalBounds();
    tekstMenu.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
    // Tekst na górze ekranu
    tekstMenu.setPosition(sf::Vector2f(szerokosc / 2.0f, wysokosc * 0.15f));

	// Pozycjonowanie przycisków


    float przyciskSzerokosc = 400.0f;
    float przyciskWysokosc = 185.0f;

    // Skalujemy przyciski jeœli s¹ za du¿e/ma³e
    sf::Vector2u przyciskTextureSize = texturePvP.getSize();
    if (przyciskTextureSize.x > 0 && przyciskTextureSize.y > 0) {
        float scalePrzycisk = przyciskSzerokosc / przyciskTextureSize.x;
        spritePvP.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
        spritePvE.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
        // Aktualizujemy rozmiar po skalowaniu
        przyciskSzerokosc = spritePvP.getGlobalBounds().size.x;
        przyciskWysokosc = spritePvP.getGlobalBounds().size.y;
    }

    // Odstêp miêdzy przyciskami
    float odstep = 30.0f;

    // Pozycja pierwszego przycisku (PvP)
    float pozycjaX = (szerokosc - przyciskSzerokosc) / 2.0f;
    float pozycjaPvPY = wysokosc * 0.5f; // 50% wysokoœci ekranu

    spritePvP.setPosition(sf::Vector2f(pozycjaX, pozycjaPvPY));

    // Pozycja drugiego przycisku (PvE) - pod pierwszym
    float pozycjaPvEY = pozycjaPvPY + przyciskWysokosc + odstep;
    spritePvE.setPosition(sf::Vector2f(pozycjaX, pozycjaPvEY));
}

void Menu::rysuj(sf::RenderWindow& okno)
{
    // Prosta pêtla renderowania: Clear -> Draw -> Display
    // Nie potrzebujemy tu skomplikowanego potoku graficznego (brak shaderów, œwiate³).
    okno.clear(sf::Color::Black);
    okno.draw(spriteTlo);
    okno.draw(tekstMenu);
    okno.draw(spritePvP);
    okno.draw(spritePvE);
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

        if (event->is<sf::Event::MouseButtonPressed>())
        {
            const auto& mouse = event->getIf<sf::Event::MouseButtonPressed>();

            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos =
                    okno.mapPixelToCoords({ mouse->position.x, mouse->position.y });

                if (spritePvP.getGlobalBounds().contains(mousePos))
                    return 1; // PvP

                if (spritePvE.getGlobalBounds().contains(mousePos))
                    return 2; // PvE
            }
        }

        sf::Vector2f mousePos =
            okno.mapPixelToCoords(sf::Mouse::getPosition(okno));

        spritePvP.setColor(
            spritePvP.getGlobalBounds().contains(mousePos)
            ? sf::Color(255, 255, 255, 160)
            : sf::Color::White
        );

        spritePvE.setColor(
            spritePvE.getGlobalBounds().contains(mousePos)
            ? sf::Color(255, 255, 255, 160)
            : sf::Color::White
        );
    }


    return 0; // Brak decyzji w tej klatce (zostañ w menu)
}