/********************************************************************
*  Name       : unit2b.h                                            *
*  Verwendung : Umgebung zu Praktikumsaufgabe (MaPra),              *
*               Sortieralgorithmen mit Templates                    *
*  Autor      : S. Gross, IGPM RWTH Aachen                          *
*  Datum      : Apr '13                                             *
*  Sprache    : C++                                                 *
********************************************************************/

#ifndef unit2b
#define unit2b

#include "student.h"
#include <iostream>
#include <vector>
#include <string>


// Funktionen zur Ueberpruefung der Ergebnisse
// geben zurueck, ob ein Fehler entdeckt wurde
bool ergebnis( const std::vector<double>& feld);
bool ergebnis( const std::vector<std::string>& feld);
bool ergebnis( const std::vector<Student>& feld);

#endif
