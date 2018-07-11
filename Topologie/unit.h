/*****************************************************************
*  Name       : unitZ2.h                                         *
*  Verwendung : Schnittstelle zur Praktikumsumgebung (MAPRA),    *
*               Überprüfung von Stetigkeit im diskreten Fall     *
*  Autor      : Jens Berger, LNM RWTH Aachen                     *
*               Kolja Brix, IGPM RWTH Aachen                     *
*  Datum      : Mai 2010                                         *
*****************************************************************/

#ifndef _UNITZ2_H
#define _UNITZ2_H

#include<vector>

extern const int AnzahlMengensysteme;
extern const int AnzahlAbbildungen;

void Abbildung     (const int& Beispiel, std::vector<int>& f);
void Mengensystem  (const int& Beispiel, std::vector<std::vector<int> >& M);
void Ergebnis      (const int& Urbild, const int& Bild, const bool& stetig);

#endif