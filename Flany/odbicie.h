#pragma once
#include "pocisk.h"
#include "przeszkoda.h"

// Forward declaration, ¿eby unikn¹æ pêtli include'ów jeœli pocisk.h includuje odbicie.h
class pocisk;
class przeszkoda;

bool CzyBok(pocisk* lotka, przeszkoda& puszka);
bool CzyGoraDol(pocisk* lotka, przeszkoda& puszka);
void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka);