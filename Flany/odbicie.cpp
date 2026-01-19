#include "odbicie.h"
#include "pocisk.h"     
#include "przeszkoda.h" 
#include <iostream>
#include <cmath>
#define NoClip 15.0f

bool CzyLewo(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiX = lotka->pozycja.x;
    float srodekblokuX = puszka.pozycja.x;
    float polowkaSzerokosciBloku = puszka.rozmiar.x / 2.0f;

    bool lewaObicie = fabs((srodeklotkiX + promienLotki) - (srodekblokuX - polowkaSzerokosciBloku)) <= NoClip;
    

    return lewaObicie;
}

bool CzyPrawo(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiX = lotka->pozycja.x;
    float srodekblokuX = puszka.pozycja.x;
    float polowkaSzerokosciBloku = puszka.rozmiar.x / 2.0f;

    bool prawaObicie = fabs((srodeklotkiX - promienLotki) - (srodekblokuX + polowkaSzerokosciBloku)) <= NoClip;
	return prawaObicie;
}


bool CzyDol(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiY = lotka->pozycja.y;
    float srodekblokuY = puszka.pozycja.y;
    float polowkaWysokosciBloku = puszka.rozmiar.y / 2.0f;

   
    bool dolneOdbicie = fabs((srodeklotkiY + promienLotki) - (srodekblokuY - polowkaWysokosciBloku)) <= NoClip;

	return dolneOdbicie;
}


bool CzyGora(pocisk* lotka, przeszkoda& puszka)
{
    float promienLotki = lotka->lotka.getRadius();
    float srodeklotkiY = lotka->pozycja.y;
    float srodekblokuY = puszka.pozycja.y;
    float polowkaWysokosciBloku = puszka.rozmiar.y / 2.0f;
    bool gorneOdbicie = fabs((srodeklotkiY + promienLotki)- (srodekblokuY - polowkaWysokosciBloku)) <= NoClip;
    return gorneOdbicie;
}


int StronaOdbicia(pocisk* lotka, przeszkoda& puszka)
{
	if (CzyLewo(lotka, puszka))  return 1;
	if (CzyPrawo(lotka, puszka)) return 2;
	if (CzyGora(lotka, puszka))  return 3;
	if (CzyDol(lotka, puszka))   return 4;
	return 0;
}



void odbicie_przeszkoda(pocisk* lotka, przeszkoda* puszka)
{
    sf::FloatRect granicePuszki = puszka->blok.getGlobalBounds();
    sf::FloatRect graniceLotki = lotka->lotka.getGlobalBounds();

    auto czyNachodza = granicePuszki.findIntersection(graniceLotki);

    if (czyNachodza)
    {
        // --- NAPRAWA: ZAPISUJEMY SI£Ê PRZED ODBICIEM ---
        // Musimy wiedzieæ jak mocno i w któr¹ stronê lecia³a lotka W MOMENCIE uderzenia,
        // zanim fizyka zmieni ten wektor na przeciwny.
        float silaUderzeniaX = lotka->predkosc.x;

        // --- FIZYKA ODBICIA (KOLEGA) ---
        // Tutaj zmieniamy prêdkoœæ lotki (odbicie)
       
        if (puszka->czyNiezniszczalna == false)
        {
			puszka->hp --;

        }
        
        
        
        switch (StronaOdbicia(lotka, *puszka))
        {
        case 1: // Lewa
            lotka->pozycja.x = lotka->pozycja.x - (fabs( (lotka->pozycja.x + lotka->lotka.getRadius()) - (puszka->pozycja.x-(puszka->rozmiar.x / 2.0f))));
            lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka->sprezystosc;
            break;
        case 2: // Prawa
            
            lotka->pozycja.x = lotka->pozycja.x + (fabs((lotka->pozycja.x - lotka->lotka.getRadius()) - (puszka->pozycja.x + (puszka->rozmiar.x / 2.0f))));
            lotka->predkosc.x = -lotka->predkosc.x * 0.5f * puszka->sprezystosc;
            break; 
		case 3: // Góra
            lotka->pozycja.y = lotka->pozycja.y - (fabs((lotka->pozycja.y + lotka->lotka.getRadius()) - (puszka->pozycja.y - (puszka->rozmiar.y / 2.0f))));
            lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka->sprezystosc;
			break;
		case 4: // Dó³
            lotka->pozycja.y = lotka->pozycja.y + (fabs((lotka->pozycja.y - lotka->lotka.getRadius()) - (puszka->pozycja.y + (puszka->rozmiar.y / 2.0f))));
            lotka->predkosc.y = -lotka->predkosc.y * 0.5f * puszka->sprezystosc;
			break;
        default:
            break;
        }


        // --- LOGIKA TRAFIENIA I ANIMACJI (TY) ---
        // Reagujemy tylko jeœli to jest cel gry (puszka), a nie œciana
        if (puszka->czyPuszka && !puszka->czyTrafiona)
        {
            std::cout << "TRAFIENIE! Przewracam puszke." << std::endl;
            puszka->czyTrafiona = true;

            // --- NAPRAWA: PRZEKAZUJEMY ORYGINALN¥ PRÊDKOŒÆ ---
            // U¿ywamy zmiennej silaUderzeniaX, a nie lotka->predkosc.x (która jest ju¿ odwrócona przez kod wy¿ej)
            puszka->przewroc(silaUderzeniaX);
        }
    }
}

