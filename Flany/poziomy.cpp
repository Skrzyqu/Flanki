#include "Gra.h" // WA¯NE: To pozwala widzieæ klasê Gra
#include <cmath>
#include <iostream>
#include "przeszkoda.h"
#include "odbicie.h"
#include "pocisk.h"

#define srodekX 960.0f
#define srodekY 540.0f

void Gra::tutorialE2(sf::RenderWindow& okno)
{
    float przesuniecieS1 = 200;
    static przeszkoda sciana1;
    sciana1.rozmiar = { 30.0f, 280.0f };
    sciana1.pozycja = { srodekX - przesuniecieS1, srodekY + 340 };
    sciana1.blok.setSize(sciana1.rozmiar);
    sciana1.blok.setOrigin(sf::Vector2f(sciana1.rozmiar.x / 2.0f, sciana1.rozmiar.y / 2.0f));
    sciana1.blok.setPosition(sciana1.pozycja);
    sciana1.blok.setFillColor(sf::Color(255, 0, 0));
    sciana1.blok.setOutlineColor(sf::Color::Black);
    sciana1.blok.setOutlineThickness(2.0f);

    static przeszkoda sciana2;
    sciana2.rozmiar = sciana1.rozmiar;
    sciana2.pozycja = { srodekX + przesuniecieS1, sciana1.pozycja.y };
    sciana2.blok.setSize(sciana2.rozmiar);
    sciana2.blok.setOrigin(sf::Vector2f(sciana2.rozmiar.x / 2.0f, sciana2.rozmiar.y / 2.0f));
    sciana2.blok.setPosition(sciana2.pozycja);
    sciana2.blok.setFillColor(sf::Color::Blue);
    sciana2.blok.setOutlineColor(sf::Color::Black);
    sciana2.blok.setOutlineThickness(2.0f);

    if (graczLewy.czyLeci)
    {
        
        odbicie_przeszkoda(&graczLewy, &sciana1);
        odbicie_przeszkoda(&graczLewy, &sciana2);
        
    }
    else
    {
        
        odbicie_przeszkoda(&graczPrawy, &sciana1);
        
        odbicie_przeszkoda(&graczPrawy, &sciana2);
        
    }


    okno.draw(sciana2.blok);
    okno.draw(sciana1.blok);
}

void Gra::tutorialE3(sf::RenderWindow& okno)
{
    float przesuniecie1 = 600;

    // trampolina L
    static przeszkoda trampolinaL;
    trampolinaL.rozmiar = { 200.0f, 10.0f };
    trampolinaL.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI-trampolinaL.rozmiar.y/2.0f};
    trampolinaL.blok.setSize(trampolinaL.rozmiar);
    trampolinaL.blok.setOrigin(sf::Vector2f(trampolinaL.rozmiar.x / 2.0f, trampolinaL.rozmiar.y / 2.0f));
    trampolinaL.blok.setPosition(trampolinaL.pozycja);
    trampolinaL.blok.setFillColor(sf::Color(0, 0, 0));
    trampolinaL.blok.setOutlineColor(sf::Color::Yellow);
    trampolinaL.blok.setOutlineThickness(2.0f);
    trampolinaL.sprezystosc = 3.0f; // BARDZO SPRÊ¯YSTE!

    // trampolina P
    static przeszkoda trampolinaP;
    trampolinaP.rozmiar = trampolinaL.rozmiar;
    trampolinaP.pozycja = { srodekX + przesuniecie1, trampolinaL.pozycja.y };
    trampolinaP.blok.setSize(trampolinaP.rozmiar);
    trampolinaP.blok.setOrigin(sf::Vector2f(trampolinaP.rozmiar.x / 2.0f, trampolinaP.rozmiar.y / 2.0f));
    trampolinaP.blok.setPosition(trampolinaP.pozycja);
    trampolinaP.blok.setFillColor(sf::Color(0, 0, 0));
    trampolinaP.blok.setOutlineColor(sf::Color::Yellow);
    trampolinaP.blok.setOutlineThickness(2.0f);
    trampolinaP.sprezystosc = 3.0f;

    //Sciana 1L
    
	przesuniecie1 -= 250.0f;

    static przeszkoda sciana1L;
	sciana1L.rozmiar = { 30.0f, 80.0f };
	sciana1L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - sciana1L.rozmiar.y / 2.0f };
	sciana1L.blok.setSize(sciana1L.rozmiar);
	sciana1L.blok.setOrigin(sf::Vector2f(sciana1L.rozmiar.x / 2.0f, sciana1L.rozmiar.y / 2.0f));
	sciana1L.blok.setPosition(sciana1L.pozycja);
	sciana1L.blok.setFillColor(sf::Color::Red);
	sciana1L.blok.setOutlineColor(sf::Color::Black);
	sciana1L.blok.setOutlineThickness(2.0f);
	//Sciana 1P
	static przeszkoda sciana1P;
    sciana1P = sciana1L;
	sciana1P.pozycja = { srodekX + przesuniecie1, POZIOM_PODLOGI - sciana1P.rozmiar.y / 2.0f };
    sciana1P.blok.setPosition(sciana1P.pozycja);
	sciana1P.blok.setFillColor(sf::Color::Blue);
	//Sciana 2L
	przesuniecie1 += 200.0f;
	static przeszkoda sciana2L;
	sciana2L.rozmiar = { 30.0f, 600.0f };
	sciana2L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - (sciana2L.rozmiar.y / 2.0f) -100};
	sciana2L.blok.setSize(sciana2L.rozmiar);
	sciana2L.blok.setOrigin(sf::Vector2f(sciana2L.rozmiar.x / 2.0f, sciana2L.rozmiar.y / 2.0f));
	sciana2L.blok.setPosition(sciana2L.pozycja);
	sciana2L.blok.setFillColor(sf::Color::Red);
	sciana2L.blok.setOutlineColor(sf::Color::Black);
	sciana2L.blok.setOutlineThickness(2.0f);
    //Sciana2P
	static przeszkoda sciana2P;
	sciana2P = sciana2L;
	sciana2P.pozycja = { srodekX + przesuniecie1, sciana2L.pozycja.y };
	sciana2P.blok.setPosition(sciana2P.pozycja);
	sciana2P.blok.setFillColor(sf::Color::Blue);

  
    // Sciany rysowanie

    if (graczLewy.czyLeci)
    {
        odbicie_przeszkoda(&graczLewy, &trampolinaL);
        odbicie_przeszkoda(&graczLewy, &trampolinaP);
		odbicie_przeszkoda(&graczLewy, &sciana1L);
		odbicie_przeszkoda(&graczLewy, &sciana1P);
		odbicie_przeszkoda(&graczLewy, &sciana2L);
		odbicie_przeszkoda(&graczLewy, &sciana2P);
    }
    else
    { 
        odbicie_przeszkoda(&graczPrawy, &trampolinaL);
        odbicie_przeszkoda(&graczPrawy, &trampolinaP);
		odbicie_przeszkoda(&graczPrawy, &sciana1L);
		odbicie_przeszkoda(&graczPrawy, &sciana1P);
		odbicie_przeszkoda(&graczPrawy, &sciana2L);
		odbicie_przeszkoda(&graczPrawy, &sciana2P);
    }
    
    
 
    okno.draw(trampolinaL.blok);
    okno.draw(trampolinaP.blok);
	okno.draw(sciana1L.blok);
	okno.draw(sciana1P.blok);
	okno.draw(sciana2L.blok);
	okno.draw(sciana2P.blok);
}

void Gra::tutorialE4(sf::RenderWindow& okno)
{
    
	float przesuniecie1 = 300;
    
    static przeszkoda ScianaHP1L;
	ScianaHP1L.rozmiar = { 20.0f, 50.0f };
	ScianaHP1L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI-ScianaHP1L.rozmiar.y/2.0f -300};
	ScianaHP1L.blok.setSize(ScianaHP1L.rozmiar);
	ScianaHP1L.blok.setOrigin(sf::Vector2f(ScianaHP1L.rozmiar.x / 2.0f, ScianaHP1L.rozmiar.y / 2.0f));
	ScianaHP1L.blok.setPosition(ScianaHP1L.pozycja);
	ScianaHP1L.blok.setFillColor(sf::Color::White);
	ScianaHP1L.blok.setOutlineThickness(3.0f);
    
    switch (ScianaHP1L.hp)
    {
    case 3:
        ScianaHP1L.blok.setOutlineColor(sf::Color::Green);
        break;
    case 2:
        ScianaHP1L.blok.setOutlineColor(sf::Color::Yellow);
        break;
    case 1:
        ScianaHP1L.blok.setOutlineColor(sf::Color::Red);
        break;
    }
    
    ScianaHP1L.czyNiezniszczalna = false;

    static przeszkoda ScianaHP1P= ScianaHP1L;
	ScianaHP1P.pozycja = { srodekX + przesuniecie1, ScianaHP1L.pozycja.y };
	ScianaHP1P.blok.setPosition(ScianaHP1P.pozycja);
	ScianaHP1P.blok.setOutlineThickness(3.0f);

    switch (ScianaHP1P.hp)
    {
    case 3:
        ScianaHP1P.blok.setOutlineColor(sf::Color::Green);
        break;
    case 2:
        ScianaHP1P.blok.setOutlineColor(sf::Color::Yellow);
        break;
    case 1:
        ScianaHP1P.blok.setOutlineColor(sf::Color::Red);
        break;
    }

        
    
    
    //
   
    static przeszkoda ScianaL1;
	ScianaL1.rozmiar = { 15.0f, 297.0f };
	ScianaL1.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - ScianaL1.rozmiar.y / 2.0f};
	ScianaL1.blok.setSize(ScianaL1.rozmiar);
	ScianaL1.blok.setOrigin(sf::Vector2f(ScianaL1.rozmiar.x / 2.0f, ScianaL1.rozmiar.y / 2.0f));
	ScianaL1.blok.setPosition(ScianaL1.pozycja);
	ScianaL1.blok.setFillColor(sf::Color::Red);
	ScianaL1.blok.setOutlineColor(sf::Color::Black);

    
	static przeszkoda ScianaP1 = ScianaL1;
	ScianaP1.pozycja = { srodekX + przesuniecie1, ScianaP1.pozycja.y };
	ScianaP1.blok.setPosition(ScianaP1.pozycja);
	ScianaP1.blok.setFillColor(sf::Color::Blue);
    


    static przeszkoda ScianaL2;
    ScianaL2.rozmiar = { 15.0f, 50.0f };
    ScianaL2.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - ScianaL2.rozmiar.y / 2.0f - 353 };
    ScianaL2.blok.setSize(ScianaL2.rozmiar);
    ScianaL2.blok.setOrigin(sf::Vector2f(ScianaL2.rozmiar.x / 2.0f, ScianaL2.rozmiar.y / 2.0f));
    ScianaL2.blok.setPosition(ScianaL2.pozycja);
    ScianaL2.blok.setFillColor(sf::Color::Red);
    ScianaL2.blok.setOutlineColor(sf::Color::Black);
    

    static przeszkoda ScianaP2=ScianaL2;
    ScianaP2.pozycja = { srodekX + przesuniecie1, ScianaP2.pozycja.y };
    ScianaP2.blok.setPosition(ScianaP2.pozycja);
    ScianaP2.blok.setFillColor(sf::Color::Blue);









    
    //trampoliny 1
    static przeszkoda trampolina1L;
    przesuniecie1 += 300;
    trampolina1L.rozmiar = { 10.0f, 100.0f };
    trampolina1L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - trampolina1L.rozmiar.y / 2.0f -100 };
    trampolina1L.blok.setSize(trampolina1L.rozmiar);
    trampolina1L.blok.setOrigin(sf::Vector2f(trampolina1L.rozmiar.x / 2.0f, trampolina1L.rozmiar.y / 2.0f));
    trampolina1L.blok.setPosition(trampolina1L.pozycja);
    trampolina1L.blok.setFillColor(sf::Color(0, 0, 0));
    trampolina1L.blok.setOutlineColor(sf::Color::Yellow);
    trampolina1L.blok.setOutlineThickness(2.0f);
    trampolina1L.sprezystosc = 3.0f; 


    static przeszkoda trampolina1P = trampolina1L;
    trampolina1P.pozycja = { srodekX + przesuniecie1, POZIOM_PODLOGI - trampolina1L.rozmiar.y / 2.0f - 100 };
    trampolina1P.blok.setPosition(trampolina1P.pozycja);


    //trampolina 2
    static przeszkoda trampolina2L;
    przesuniecie1 -= 100;
    trampolina2L.rozmiar = { 100.0f, 10.0f };
    trampolina2L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - trampolina2L.rozmiar.y / 2.0f  };
    trampolina2L.blok.setSize(trampolina2L.rozmiar);
    trampolina2L.blok.setOrigin(sf::Vector2f(trampolina2L.rozmiar.x / 2.0f, trampolina2L.rozmiar.y / 2.0f));
    trampolina2L.blok.setPosition(trampolina2L.pozycja);
    trampolina2L.blok.setFillColor(sf::Color(0, 0, 0));
    trampolina2L.blok.setOutlineColor(sf::Color::Yellow);
    trampolina2L.blok.setOutlineThickness(2.0f);
    trampolina2L.sprezystosc = 3.0f;


    static przeszkoda trampolina2P = trampolina2L;
    trampolina2P.pozycja = { srodekX + przesuniecie1, POZIOM_PODLOGI - trampolina2L.rozmiar.y / 2.0f };
    trampolina2P.blok.setPosition(trampolina2P.pozycja);

    //trampolina 3
    static przeszkoda trampolina3L;
    przesuniecie1 -= 140;
    trampolina3L.rozmiar = { 100.0f, 10.0f };
    trampolina3L.pozycja = { srodekX - przesuniecie1, POZIOM_PODLOGI - trampolina3L.rozmiar.y / 2.0f - 450 };
    trampolina3L.blok.setSize(trampolina3L.rozmiar);
    trampolina3L.blok.setOrigin(sf::Vector2f(trampolina3L.rozmiar.x / 2.0f, trampolina3L.rozmiar.y / 2.0f));
    trampolina3L.blok.setPosition(trampolina3L.pozycja);
    trampolina3L.blok.setFillColor(sf::Color(0, 0, 0));
    trampolina3L.blok.setOutlineColor(sf::Color::Yellow);
    trampolina3L.blok.setOutlineThickness(2.0f);
    trampolina3L.sprezystosc = 3.0f;


    static przeszkoda trampolina3P = trampolina3L;
    trampolina3P.pozycja = { srodekX + przesuniecie1, POZIOM_PODLOGI - trampolina3L.rozmiar.y / 2.0f - 450 };
    trampolina3P.blok.setPosition(trampolina3P.pozycja);

    
    if (graczLewy.czyLeci)
    {
        if (ScianaHP1L.hp > 0)
        {
            odbicie_przeszkoda(&graczLewy, &ScianaHP1L);
            okno.draw(ScianaHP1L.blok);
        }
        if (ScianaHP1P.hp>0)
        {
            odbicie_przeszkoda(&graczLewy, &ScianaHP1P);
            okno.draw(ScianaHP1P.blok);
		}



		odbicie_przeszkoda(&graczLewy, &ScianaL1);
		odbicie_przeszkoda(&graczLewy, &ScianaL2);
        odbicie_przeszkoda(&graczLewy, &ScianaP1);
        odbicie_przeszkoda(&graczLewy, &ScianaP2);
        odbicie_przeszkoda(&graczLewy, &trampolina1L);
        odbicie_przeszkoda(&graczLewy, &trampolina2L);
        odbicie_przeszkoda(&graczLewy, &trampolina3L);
        odbicie_przeszkoda(&graczLewy, &trampolina1P);
        odbicie_przeszkoda(&graczLewy, &trampolina2P);
        odbicie_przeszkoda(&graczLewy, &trampolina3P);
    
    
    
    }
   
    else
    {
        if (ScianaHP1L.hp > 0)
        {
            odbicie_przeszkoda(&graczPrawy, &ScianaHP1L);
            okno.draw(ScianaHP1L.blok);
        }
        if (ScianaHP1P.hp > 0)
        {
            odbicie_przeszkoda(&graczPrawy, &ScianaHP1P);
            okno.draw(ScianaHP1P.blok);
        }


        odbicie_przeszkoda(&graczPrawy, &ScianaL1);
        odbicie_przeszkoda(&graczPrawy, &ScianaL2);
        odbicie_przeszkoda(&graczPrawy, &ScianaP1);
        odbicie_przeszkoda(&graczPrawy, &ScianaP2);
        odbicie_przeszkoda(&graczPrawy, &trampolina1L);
        odbicie_przeszkoda(&graczPrawy, &trampolina2L);
        odbicie_przeszkoda(&graczPrawy, &trampolina3L);
        odbicie_przeszkoda(&graczPrawy, &trampolina1P);
        odbicie_przeszkoda(&graczPrawy, &trampolina2P);
        odbicie_przeszkoda(&graczPrawy, &trampolina3P);

    
    }


    okno.draw(ScianaL1.blok);
	okno.draw(ScianaL2.blok);
    okno.draw(ScianaP1.blok);
    okno.draw(trampolina1L.blok);
    okno.draw(trampolina2L.blok);
    okno.draw(trampolina3L.blok);
    okno.draw(ScianaP2.blok);
    okno.draw(trampolina1P.blok);
    okno.draw(trampolina2P.blok);
    okno.draw(trampolina3P.blok);
}