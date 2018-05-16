/***********************************************
*  Name       : vektor.cpp                     *
*  Verwendung : Vektorklasse                   *
*  Autor      :                                *
*  Datum      :                                *
***********************************************/

#include "vektor.h"
#include "matrix.h"
#include <iomanip>
#include <cmath>
#include <cstdlib>

// =======================
//      Konstruktoren
// =======================


// ----- Konstruktor -----

Vektor::Vektor(size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  this->ReDim(l);
}


// ----- Kopierkonstruktor -----

Vektor::Vektor(const Vektor& x):Vek(x.Vek){ }


// ===========================================
//      Vektorelement schreiben und lesen
// ===========================================


// ----- Schreib- und Lesezugriff auf Vektorelemente -----

double& Vektor::operator () (size_t i) {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}


// ----- Lesezugriff auf Elemente konstanter Vektoren -----

double Vektor::operator () (size_t i) const {
#ifndef NDEBUG
  if (i >= Vek.size())
    VekFehler("Ungueltiger Index!");
#endif

  return Vek[i];
}

// =====================
//      Zuweisungen
// =====================

// ----- Zuweisungsoperator "=" ----

Vektor& Vektor::operator = (const Vektor& x)
{
#ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor = Vektor'!");
#endif

  for (size_t i = 0; i < Vek.size(); i++)
    (*this) (i) = x(i);

  //oder: this->Vek=x.Vek;

  return *this;
}


// ----- Zuweisungsoperator mit Addition "+=" ----

Vektor& Vektor::operator += (const Vektor& x)
{
#ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor += Vektor'!");
#endif
    for (size_t i = 0; i < Vek.size(); i++)
        (*this)(i) += x(i);

    return *this;
}


// ----- Zuweisungsoperator mit Subtraktion "-=" ----

Vektor& Vektor::operator -= (const Vektor& x)
{
#ifndef NDEBUG
  if (Vek.size() != x.Laenge())
    VekFehler("Inkompatible Dimensionen fuer 'Vektor -= Vektor'!");
#endif
    for (size_t i = 0; i < Vek.size(); i++)
        (*this)(i) -= x(i);

    return *this;
}


// ----- Zuweisungsoperator mit Multiplikation "*=" ----

Vektor& Vektor::operator *= (const double c)
{
    for (size_t i = 0; i < Vek.size(); i++)
        (*this)(i) *= c;

    return *this;
}


// ----- Zuweisungsoperator mit Division "/=" ----

Vektor& Vektor::operator /= (const double c)
{
#ifndef NDEBUG
  if (c == 0)
    VekFehler("Division durch 0 in Vektor /= Skalar");
#endif
    for (size_t i = 0; i < Vek.size(); i++)
        (*this)(i) /= c;

    return *this;
}


// ==============================
//      Vektorlaenge aendern
// ==============================


// ----- Vektorlaenge aendern -----

Vektor& Vektor::ReDim (size_t l)
{
#ifndef NDEBUG
  if (l <= 0)
    VekFehler("Nur Vektoren mit positiver Laenge!");
#endif

  Vek.clear();
  Vek.resize(l,0);

  return *this;
}


// ======================
//      Vektornormen
// ======================


// ----- Euklidische Norm -----

double Vektor::Norm2() const
{
    double d = dot((*this), (*this));
    return sqrt(d);
}


// ----- Maximum-Norm -----

double Vektor::NormMax() const
{
    double m = 0;
    for(auto& e: Vek)
        if(abs(e) > m)
            m = abs(e);
    return m;
}


// ==================================
//      arithmetische Operatoren
// ==================================


// ----- Addition "+" -----

Vektor operator + (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor + Vektor'!");
#endif

  Vektor z = x;
  return z += y;
}


// ----- Subtraktion "-" -----

Vektor operator - (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor - Vektor'!");
#endif

    Vektor z = x;
    return z -= y;
}


// ----- Vorzeichen wechseln "-" -----

Vektor operator - (const Vektor& x)
{
    Vektor z = x;
    return z *= -1.0;
}


// ----- Skalarprodukt -----

double dot (const Vektor& x, const Vektor& y)
{
#ifndef NDEBUG
  if (x.Laenge() != y.Laenge())
    Vektor::VekFehler("Inkompatible Dimensionen fuer 'dot(Vektor, Vektor)'!");
#endif
    double val = 0;
    for (size_t i = 0; i < x.Laenge(); i++)
        val += x(i) * y(i);

    return val;
}


// ----- Multiplikation Skalar*Vektor "*" -----

Vektor operator * (const double c, const Vektor& x)
{
    Vektor z = x;
    return z *= c;
}


// ----- Multiplikation Vektor*Skalar "*" -----

Vektor operator * (const Vektor& x, const double c)
{
    Vektor z = x;
    return z *= c;
}


// ----- Division Vektor/Skalar "/" -----

Vektor operator / (const Vektor& x, const double c)
{
#ifndef NDEBUG
  if (c == 0)
    Vektor::VekFehler("Division durch 0 in Vektor / Skalar");
#endif

    Vektor z = x;
    return z /= c;
}


// ==============================
//      Vergleichsoperatoren
// ==============================


// ----- Test auf Gleichheit "==" -----

bool operator == (const Vektor& x, const Vektor& y)
{
  if (x.Laenge() !=y.Laenge()) return false;

  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    if (x(i)!=y(i)) return false;

  return true;
}


// ----- Test auf Ungleichheit "!=" -----

bool operator != (const Vektor& x, const Vektor& y)
{
    return !(x == y);
}


// ==========================
//      Ein- und Ausgabe
// ==========================


// ----- Ausgabe "<<" -----

std::ostream& operator << (std::ostream& s, const Vektor& x)
{
  s << std::setiosflags(std::ios::right);
  s << "# Laenge: " << x.Laenge();
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++)
    s << "\n" << x(i);

  return s << std::endl;
}


// ----- Eingabe ">>" -----

std::istream& operator >> (std::istream& s, Vektor& x)
{
  for (size_t i = 0, xsize = x.Laenge(); i < xsize; i++) {
	s >> x(i);
  }
  return s;
}

Vektor operator * (const Matrix& m, const Vektor& v)
{
#ifndef NDEBUG
    if (m.Spalten() != v.Laenge())
        Vektor::VekFehler("Inkompatible Dimensionen fuer 'Matrix * Vektor'!");
#endif
    Vektor r(m.Zeilen());
    for(size_t i = 0; i < m.Zeilen(); i++)
        for(size_t j = 0; j < m.Spalten(); j++)
            r(i) += m(i,j) * v(j);
    return r;
}
Vektor operator * (const Vektor& v, const Matrix& m)
{
#ifndef NDEBUG
    if (m.Zeilen() != v.Laenge())
        Vektor::VekFehler("Inkompatible Dimensionen fuer 'Vektor * Matrix'!");
#endif
    Vektor r(m.Spalten());
    for(size_t i = 0; i < m.Spalten(); i++)
        for(size_t j = 0; j < m.Zeilen(); j++)
            r(i) += m(j,i) * v(j);
    return r;
}


// ==========================
//      Fehlerbehandlung
// ==========================


// ----- Ausgabe der Fehlermeldung "str" -----

void Vektor::VekFehler (const std::string& str)
{
  std::cerr << "\nVektorfehler: " << str << '\n' << std::endl;
  exit(1);
}
