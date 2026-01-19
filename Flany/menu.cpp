#include "Menu.h"
#include <optional>
#include <iostream>



Menu::Menu(float szerokosc, float wysokosc) :
    tekstMenu(czcionka), // Inicjalizacja listy inicjalizacyjnej
    spriteTlo(spriteTlo),
    spritePvP(spritePvP),
    spritePvE(spritePvE),
    tekstMapy(czcionka),
	spriteMapa1(spriteMapa1),
    spriteMapa2(spriteMapa2),
    spriteMapa3(spriteMapa3),
	spriteBack(spriteBack),
	spriteStart(spriteStart),
    showMapSelection(false),
    selectedGameMode(0),
    selectedMap(0)
{
    // [IO/BLOCKING] Synchroniczne ładowanie z dysku.
    // Akceptowalne w menu startowym, ale w trakcie gry powodowałoby "klatkowanie".
    // W produkcji zasoby powinny pochodzić z cache'a (ResourceManager).
    (void)czcionka.openFromFile("PressStart2P-Regular.ttf");

    // [TODO]: Hardcoded strings. W przyszłości przenieść do systemu lokalizacji (np. JSON/XML).
    tekstMenu.setString("WYBIERZ TRYB:\n\n");
        //1. Gracz vs Gracz\n2. Gracz vs Komputer");
    tekstMenu.setCharacterSize(60);
    tekstMenu.setFillColor(sf::Color::Black);
    tekstMenu.setStyle(sf::Text::Bold);

    tekstMapy.setFont(czcionka);
    tekstMapy.setString("WYBIERZ MAPE:");
    tekstMapy.setCharacterSize(60);
    tekstMapy.setFillColor(sf::Color::Black);
    tekstMapy.setStyle(sf::Text::Bold);

    if (!textureTlo.loadFromFile("menu_tlo.png"))
    {
        // Obsługa błędu - tworzenie zastępczej tekstury
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

    if (!textureMapa1.loadFromFile("mapa1.png"))
    {
        sf::Image fallbackImage({ 400, 300 }, sf::Color(34, 139, 34)); 
        textureMapa1.loadFromImage(fallbackImage);
    }

    if (!textureMapa2.loadFromFile("mapa2.png"))
    {
        sf::Image fallbackImage({ 400, 300 }, sf::Color(210, 180, 140)); 
        textureMapa2.loadFromImage(fallbackImage);
    }

    if (!textureMapa3.loadFromFile("mapa3.png"))
    {
        sf::Image fallbackImage({ 400, 300 }, sf::Color(128, 128, 128)); 
        textureMapa3.loadFromImage(fallbackImage);
    }

    if (!textureBack.loadFromFile("btn_back.png"))
    {
        sf::Image fallbackImage({ 150, 50 }, sf::Color(100, 100, 100));
        textureBack.loadFromImage(fallbackImage);
    }
    spriteBack = sf::Sprite(textureBack);

    if (!textureStart.loadFromFile("btn_start.png"))
    {
        sf::Image fallbackImage({ 200, 80 }, sf::Color(50, 150, 50));
        textureStart.loadFromImage(fallbackImage);
    }
    spriteStart = sf::Sprite(textureStart);

    
    spriteTlo = sf::Sprite(textureTlo); 
    spritePvP = sf::Sprite(texturePvP);
    spritePvE = sf::Sprite(texturePvE);
    spriteMapa1 = sf::Sprite(textureMapa1);
    spriteMapa2 = sf::Sprite(textureMapa2);
    spriteMapa3 = sf::Sprite(textureMapa3);
    

    // --- LAYOUTING ---
    // Ustawiamy Pivot Point (Origin) na idealny środek tekstu,
    // co pozwala łatwo wycentrować go na ekranie bez skomplikowanej matematyki.
    sf::Vector2u textureSize = textureTlo.getSize();
    if (textureSize.x > 0 && textureSize.y > 0) {
        // Obliczamy skalę dla rozciągnięcia
        float scaleX = szerokosc / static_cast<float>(textureSize.x);
        float scaleY = wysokosc / static_cast<float>(textureSize.y);
        spriteTlo.setScale(sf::Vector2f(scaleX, scaleY));
    }

    // Ustawiamy pozycję tła (zawsze w lewym górnym rogu)
    spriteTlo.setPosition(sf::Vector2f(0.f, 0.f));

	// Pozycjonowanie tekstu menu
    sf::FloatRect bounds = tekstMenu.getLocalBounds();
    tekstMenu.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
    // Tekst na górze ekranu
    tekstMenu.setPosition(sf::Vector2f(szerokosc / 2.0f, wysokosc * 0.15f));

	// Pozycjonowanie przycisków


    float przyciskSzerokosc = 400.0f;
    float przyciskWysokosc = 185.0f;

    // Skalujemy przyciski jeśli są za duże/małe
    sf::Vector2u przyciskTextureSize = texturePvP.getSize();
    if (przyciskTextureSize.x > 0 && przyciskTextureSize.y > 0) {
        float scalePrzycisk = przyciskSzerokosc / przyciskTextureSize.x;
        spritePvP.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
        spritePvE.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
		spriteStart.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
        spriteBack.setScale(sf::Vector2f(scalePrzycisk, scalePrzycisk));
        // Aktualizujemy rozmiar po skalowaniu
        przyciskSzerokosc = spritePvP.getGlobalBounds().size.x;
        przyciskWysokosc = spritePvP.getGlobalBounds().size.y;
    }

    // Odstęp między przyciskami
    float odstep = 30.0f;

    // Pozycja pierwszego przycisku (PvP)
    float pozycjaX = (szerokosc - przyciskSzerokosc) / 2.0f;
    float pozycjaPvPY = wysokosc * 0.5f; // 50% wysokości ekranu

    spritePvP.setPosition(sf::Vector2f(pozycjaX, pozycjaPvPY));

    // Pozycja drugiego przycisku (PvE) - pod pierwszym
    float pozycjaPvEY = pozycjaPvPY + przyciskWysokosc + odstep;
    spritePvE.setPosition(sf::Vector2f(pozycjaX, pozycjaPvEY));

    // Pozycjonowanie tekstu wyboru mapy
    sf::FloatRect mapBounds = tekstMapy.getLocalBounds();
    tekstMapy.setOrigin(sf::Vector2f(mapBounds.size.x / 2.0f, mapBounds.size.y / 2.0f));
    tekstMapy.setPosition(sf::Vector2f(szerokosc / 2.0f, wysokosc * 0.15f));

    // Pozycjonowanie map (3 w rzędzie)
    
    float mapSpacing = 600.0f;
    float mapStartX = szerokosc / 2.0f;
    float mapY = wysokosc * 0.3f;

    spriteMapa1.setScale(sf::Vector2f(0.5f, 0.5f));
    spriteMapa2.setScale(sf::Vector2f(0.5f, 0.5f));
    spriteMapa3.setScale(sf::Vector2f(0.5f, 0.5f));
    
    sf::FloatRect boundsMapa2 = spriteMapa2.getGlobalBounds();
    float polowaSzerokosciMapy2 = boundsMapa2.size.x / 2.0f;

    spriteMapa2.setPosition(sf::Vector2f(mapStartX - polowaSzerokosciMapy2, mapY));
    

    float mapa2PosX = spriteMapa2.getPosition().x;

    spriteMapa1.setPosition(sf::Vector2f(mapa2PosX - mapSpacing, mapY));

    spriteMapa3.setPosition(sf::Vector2f(mapa2PosX + mapSpacing, mapY));

    // Przyciski
    spriteBack.setPosition(sf::Vector2f(50.0f, 50.0f));
    spriteStart.setPosition(sf::Vector2f(szerokosc - 450.0f, wysokosc - 250.0f));
}


    


void Menu::rysuj(sf::RenderWindow& okno)
{
    // Prosta pętla renderowania: Clear -> Draw -> Display
    // Nie potrzebujemy tu skomplikowanego potoku graficznego (brak shaderów, świateł).
    okno.clear(sf::Color::Black);
    okno.draw(spriteTlo);

    if (!showMapSelection) {
        // Ekran głównego menu
        okno.draw(tekstMenu);
        okno.draw(spritePvP);
        okno.draw(spritePvE);
    }
    else {
        // Ekran wyboru mapy
        okno.draw(tekstMapy);
        okno.draw(spriteMapa1);
        okno.draw(spriteMapa2);
        okno.draw(spriteMapa3);
        okno.draw(spriteBack);
        okno.draw(spriteStart);

        // Podświetlenie wybranej mapy
        if (selectedMap > 0) {
            sf::Sprite* selectedSprite = nullptr;
            switch (selectedMap) {
            case 1: selectedSprite = &spriteMapa1; break;
            case 2: selectedSprite = &spriteMapa2; break;
            case 3: selectedSprite = &spriteMapa3; break;
            }

            if (selectedSprite) {
                sf::RectangleShape highlight(
                    sf::Vector2f(selectedSprite->getGlobalBounds().size.x + 20,
                        selectedSprite->getGlobalBounds().size.y + 20)
                );
                highlight.setPosition(
                    sf::Vector2f(
                        selectedSprite->getPosition().x - 10,
                        selectedSprite->getPosition().y - 10));
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Green);
                highlight.setOutlineThickness(3.0f);
                okno.draw(highlight);
            }
        }
    }

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
            return -1; // Kod zamknięcia aplikacji
        }

        if (event->is<sf::Event::KeyPressed>())
        {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();

            if (!showMapSelection) {
                // Mapowanie klawiszy na kody stanów gry (menu główne)
                if (keyEvent->code == sf::Keyboard::Key::Num1) {
                    selectedGameMode = 2;
                    showMapSelection = true;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Num2) {
                    selectedGameMode = 1;
                    showMapSelection = true;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Escape) okno.close();
            }
            else {
                // Klawisze w ekranie wyboru mapy
                if (keyEvent->code == sf::Keyboard::Key::Num1) {
                    selectedMap = 1;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Num2) {
                    selectedMap = 2;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Num3) {
                    selectedMap = 3;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Enter ||
                    keyEvent->code == sf::Keyboard::Key::Space) {
                    // Start gry z wybraną mapą
                    if (selectedMap > 0) {
                        return 10 + selectedGameMode * 10 + selectedMap;
                        
                    }
                }
                if (keyEvent->code == sf::Keyboard::Key::Escape) {
                    // Powrót do menu głównego
                    showMapSelection = false;
                    selectedMap = 0;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Left) {
                    selectedMap = (selectedMap - 1) < 1 ? 3 : selectedMap - 1;
                    return 0;
                }
                if (keyEvent->code == sf::Keyboard::Key::Right) {
                    selectedMap = (selectedMap + 1) > 3 ? 1 : selectedMap + 1;
                    return 0;
                }
            }
        }

        if (event->is<sf::Event::MouseButtonPressed>())
        {
            const auto& mouse = event->getIf<sf::Event::MouseButtonPressed>();

            if (mouse->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mousePos =
                    okno.mapPixelToCoords({ mouse->position.x, mouse->position.y });

                if (!showMapSelection) {
                    // Menu główne
                    if (spritePvP.getGlobalBounds().contains(mousePos)) {
                        selectedGameMode = 2;
                        showMapSelection = true;
                        return 0;
                    }

                    if (spritePvE.getGlobalBounds().contains(mousePos)) {
                        selectedGameMode = 1;
                        showMapSelection = true;
                        return 0;
                    }
                }
                else {
                    // Ekran wyboru mapy
                    if (spriteMapa1.getGlobalBounds().contains(mousePos)) {
                        selectedMap = 1;
                        return 0;
                    }

                    if (spriteMapa2.getGlobalBounds().contains(mousePos)) {
                        selectedMap = 2;
                        return 0;
                    }

                    if (spriteMapa3.getGlobalBounds().contains(mousePos)) {
                        selectedMap = 3;
                        return 0;
                    }

                    // Przycisk powrotu
                    if (spriteBack.getGlobalBounds().contains(mousePos)) {
                        showMapSelection = false;
                        selectedMap = 0;
                        return 0;
                    }

                    // Przycisk startu gry
                    if (spriteStart.getGlobalBounds().contains(mousePos)) {
                        if (selectedMap > 0) {
                            return 10 + selectedGameMode * 10 + selectedMap;
                        }
                    }
                }
            }
        }

        sf::Vector2f mousePos =
            okno.mapPixelToCoords(sf::Mouse::getPosition(okno));

        if (!showMapSelection) {
            // Podświetlanie w menu głównym
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
        else {
            // Podświetlanie w wyborze mapy
            spriteMapa1.setColor(
                spriteMapa1.getGlobalBounds().contains(mousePos)
                ? sf::Color(255, 255, 255, 200)
                : (selectedMap == 1 ? sf::Color::White : sf::Color(255, 255, 255))
            );

            spriteMapa2.setColor(
                spriteMapa2.getGlobalBounds().contains(mousePos)
                ? sf::Color(255, 255, 255, 200)
                : (selectedMap == 2 ? sf::Color::White : sf::Color(255, 255, 255))
            );

            spriteMapa3.setColor(
                spriteMapa3.getGlobalBounds().contains(mousePos)
                ? sf::Color(255, 255, 255, 200)
                : (selectedMap == 3 ? sf::Color::White : sf::Color(255, 255, 255))
            );

            // Podświetlanie przycisków
            spriteBack.setColor(
                spriteBack.getGlobalBounds().contains(mousePos)
                ? sf::Color(255, 255, 255, 160)
                : sf::Color::White
            );

            spriteStart.setColor(
                spriteStart.getGlobalBounds().contains(mousePos)
                ? sf::Color(255, 255, 255, 160)
                : sf::Color::White
            );
        }
    }


    return 0; // Brak decyzji w tej klatce (zostań w menu)

}
