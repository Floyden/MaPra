#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include <vector>
#include <string>
#include "vektor.h"


class Matrix
{
  private:
    std::vector<Vektor> Mat;              // Daten

  public:
    explicit Matrix (size_t l=1, size_t m=1);         // Konstruktor mit Laengenangabe
    ~Matrix () {};                        // Destruktor
    Matrix  (const Matrix&);              // Kopierkonstruktor

    double& operator () (size_t, size_t);         // Zugriff auf Einträge des Vektors
    double  operator () (size_t, size_t) const;   // Zugriff auf falls Vektor const

    Matrix& operator =  (const Matrix&);  // Zuweisung
    Matrix& operator += (const Matrix&);  // Zuweisungen mit arithm. Operation
    Matrix& operator -= (const Matrix&);
    Matrix& operator *= (const double);
    Matrix& operator /= (const double);

    Matrix& ReDim   (size_t, size_t);                   // neue Laenge festlegen
    size_t  Zeilen  () const { return Mat.size(); }  // Laenge
    size_t  Spalten  () const { return Mat[0].Laenge(); }  // Laenge
    // double  Norm2   () const;                   // Euklidische Norm des Vektors
    // double  NormMax () const;                   // Maximum-Norm des Vektors
    //
    static void MatFehler (const std::string& str);   // Fehlermeldung ausgeben

    Matrix   operator +  (const Matrix&); // Addition
    Matrix   operator -  (const Matrix&); // Subtraktion
    Matrix   operator -  ();                // Vorzeichen

    // friend Vektor   operator *  (const double,  const Vektor&); // Vielfache
    // friend Vektor   operator *  (const Vektor&, const double);
    // friend Vektor   operator /  (const Vektor&, const double);
    // friend double   dot         (const Vektor&, const Vektor&); // Skalarprodukt
    //
    // friend bool     operator == (const Vektor&, const Vektor&); // Vergleich
    // friend bool     operator != (const Vektor&, const Vektor&);
    //
    // friend std::istream& operator >> (std::istream&, Vektor&);       // Eingabe
    // friend std::ostream& operator << (std::ostream&, const Vektor&); // Ausgabe
    //
    // friend Vektor   operator *  (const Matrix&, const Vektor&); // Matrix-Vektor-
    // friend Vektor   operator *  (const Vektor&, const Matrix&); // Multiplikation
};

#endif
