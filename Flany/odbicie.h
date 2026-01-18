#pragma once
#include "pocisk.h"
#include "przeszkoda.h"

// Forward declaration, ¿eby unikn¹æ pêtli include'ów jeœli pocisk.h includuje odbicie.h
class pocisk;
class przeszkoda;

bool CzyLewo(pocisk* lotka, przeszkoda& puszka);
bool CzyPrawo(pocisk* lotka, przeszkoda& puszka);
bool CzyGora(pocisk* lotka, przeszkoda& puszka);
bool CzyDol(pocisk* lotka, przeszkoda& puszka);
int StronaOdbicia(pocisk* lotka, przeszkoda& puszka);
void odbicie_przeszkoda(pocisk* lotka, przeszkoda* puszka);