/*****************************************************************
*  Name       : unit.h                                           *
*  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),     *
*               Iterative Loesungsverfahren                      *
*  Autor      : Y. Zhang, IGPM RWTH Aachen                       *
*  Datum      : Feb 2014                                         *
*****************************************************************/

class Matrix;
class Vektor;

extern const int AnzahlBeispiele;
/**
   Eingabe: int Bsp, Matrix A, Vektro x0, Vektor b, double tol, int maxiter.
   Ausgabe: Matrix A, Vektor x0, Vektor b, double tol, int maxiter.
 */
void Start ( int Bsp, Matrix &A, Vektor &x0, Vektor &b, double &tol, int &maxiter);

/**
 Methode: 0 -> Jacobi, 1-> GS, 2->CG
 */
void Ergebnis ( Vektor &x, int Iterationen, int Methode );
