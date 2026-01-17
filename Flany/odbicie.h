#pragma once
#include <SFML/Graphics.hpp>
#include "przeszkoda.h"
#include "pocisk.h"

bool CzyBok(pocisk* lotka, przeszkoda& puszka);
bool CzyGoraDol(pocisk* lotka, przeszkoda& puszka);
void odbicie_przeszkoda(pocisk* lotka, przeszkoda& puszka);