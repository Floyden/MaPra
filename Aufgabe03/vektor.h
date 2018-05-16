/***********************************************
*  Name        : vektor.h                      *
*  Verwendung  : Header zu Vektorklasse        *
*  Autor       : V. Reichelt, IGPM RWTH Aachen *
*  Datum       : Nov '96 - Mai '00             *
*  Aktualisiert: K. Brix, Apr '07              *
*                Y. Zhang, Feb '14             *
*                F. Gruber, Apr '16            *
***********************************************/

#ifndef _VEKTOR_H                         // vektor.h nicht doppelt benutzen
#define _VEKTOR_H

#include <iostream>
#include <vector>
#include <string>

class Matrix;                             // fuer friend Matrix * Vektor etc.

class Vektor
{
  private:
    std::vector<double> Vek;              // Daten

  public:
    explicit Vektor (size_t l=1);         // Konstruktor mit Laengenangabe
    ~Vektor () {};                        // Destruktor
    Vektor  (const Vektor&);              // Kopierkonstruktor

    double& operator () (size_t);         // Zugriff auf Einträge des Vektors
    double  operator () (size_t) const;   // Zugriff auf falls Vektor const

    Vektor& operator =  (const Vektor&);  // Zuweisung
    Vektor& operator += (const Vektor&);  // Zuweisungen mit arithm. Operation
    Vektor& operator -= (const Vektor&);
    Vektor& operator *= (const double);
    Vektor& operator /= (const double);

    Vektor& ReDim   (size_t);                   // neue Laenge festlegen
    size_t  Laenge  () const { return Vek.size(); }  // Laenge
    double  Norm2   () const;                   // Euklidische Norm des Vektors
    double  NormMax () const;                   // Maximum-Norm des Vektors

    static void VekFehler (const std::string& str);   // Fehlermeldung ausgeben

    friend Vektor   operator +  (const Vektor&, const Vektor&); // Addition
    friend Vektor   operator -  (const Vektor&, const Vektor&); // Subtraktion
    friend Vektor   operator -  (const Vektor&);                // Vorzeichen

    friend Vektor   operator *  (const double,  const Vektor&); // Vielfache
    friend Vektor   operator *  (const Vektor&, const double);
    friend Vektor   operator /  (const Vektor&, const double);
    friend double   dot         (const Vektor&, const Vektor&); // Skalarprodukt

    friend bool     operator == (const Vektor&, const Vektor&); // Vergleich
    friend bool     operator != (const Vektor&, const Vektor&);

    friend std::istream& operator >> (std::istream&, Vektor&);       // Eingabe
    friend std::ostream& operator << (std::ostream&, const Vektor&); // Ausgabe

    friend Vektor   operator *  (const Matrix&, const Vektor&); // Matrix-Vektor-
    friend Vektor   operator *  (const Vektor&, const Matrix&); // Multiplikation
};

#endif
