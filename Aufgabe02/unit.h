/********************************************************************
*  Name       : unit.h                                              *
*  Verwendung : Umgebung zu Praktikumsaufgabe (MAPRA),              *
*               Sortieralgorithmen                                  *
*  Autor      : K. Brix, IGPM RWTH Aachen                           *
*  Datum      : Mar '08                                             *
*  Sprache    : C++                                                 *
********************************************************************/

#ifndef __unit_h_
#define __unit_h_

#include <iostream>

// Konstanten

extern const unsigned int AnzahlBeispiele;

// Funktionen

void start(unsigned int beispiel, size_t &laenge, unsigned int *&feld);
bool ergebnis(unsigned int *feld);
void tausche(unsigned int *feld, size_t i, size_t j);

#endif
