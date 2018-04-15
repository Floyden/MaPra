/***********************************************************
*  Name       : meina1.cpp                                 *
*  Verwendung : Loesung der ersten Praktikumsaufgabe,      *
*               Nullstellen eines quadratischen Polynoms   *
*  Autor      :                                            *
*  Datum      :                                            *
*  Sprache    : C++                                        *
***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#include <iostream>
// Konstanten
const double FEHLERSCHRANKE = 1.0e-60;
const double DBL_MAX_SQRT = sqrt(DBL_MAX); // 1.0e20;

// ===== Hauptprogramm =====

int main()
{
  // Hier kommt nun Ihr Programm; viel Erfolg!
  for(int i = 1; i <= AnzahlBeispiele; i++)
  {
  double a, b, c;
  Start(i, a, b, c);


  bool komplex = false;
  double nst1;
  double nst2;
  double imag;

  // max 1 NST
  if(std::abs(a) < FEHLERSCHRANKE)
  {
    if(std::abs(b) < FEHLERSCHRANKE)
    {
      // polynom ist 0
      if(std::abs(c) < FEHLERSCHRANKE)
        Ergebnis(Unendlich);
      // keine nst
      else 
	      Ergebnis(0);
    }
    else
    {
      // hat eine NST
      double x = c / b;
      Ergebnis(1, false, x * - 1.0);
    }
  } 
  //Hat 2 NST
  else
  {
    double p = b / a;
    double q = c / a;

    //inhalt der wurzel
    double pq_sqrt = 0;
    // Normale pq-Formel
    if(std::abs(p) < DBL_MAX_SQRT)
    {
        double tmp = (p * p / 4.0) - q;
        if(tmp < 0)
        {
            komplex = true;
            tmp *= -1.0; // make positive
        }
        pq_sqrt = sqrt(tmp);
    }
    else // p ist zu groß, klammer p aus
    {
      double div = q / p;
      double div2 = div / p;
      double tmp = (1.0/4.0) - div2;
      if(tmp < 0)
      {
         komplex = true;
        tmp *= -1.0; // make positive
      }
      pq_sqrt = std::abs(p) * sqrt(tmp);
    }

    if(!komplex)
    {
        if(p > 0)
            nst1 = -p/2 - pq_sqrt;
        else
            nst1 = -p/2 + pq_sqrt;

        if(std::abs(nst1) < FEHLERSCHRANKE) // falls x = 0 eine NST ist
            nst2 = nst1;
        else
            nst2 = q / nst1;

        // switch if |nst1| < |nst2|
        if(std::abs(nst1) < std::abs(nst2))
        {
            double tmp = nst1;
            nst1 = nst2;
            nst2 = tmp;
        }
    }
    else
    {
        nst1 = -p/2;
        nst2 = pq_sqrt;
    }
  

      // eine nst, falls die wurzel null ist
      bool zwei_nst = (pq_sqrt) < FEHLERSCHRANKE;

      if(!zwei_nst)
        Ergebnis(2, komplex, nst1, nst2);
      else
        Ergebnis(1, false, nst1);
    }
  }
  return 0;
}
