#include "matrix.h"

Matrix::Matrix (size_t l, size_t m)
{

    #ifndef NDEBUG
      if (l <= 0)
        MatFehler("Nur Vektoren mit positiver Laenge!");
    #endif

      this->ReDim(l, m);
}

Matrix::Matrix (const Matrix& mat) : Mat(mat.Mat) {}


double& Matrix::operator () (size_t i, size_t j)
{
#ifndef NDEBUG
  if (j >= Mat.size() || i >= Mat[j].Laenge())
    MatFehler("Ungueltiger Index!");
#endif

    return Mat[j](i);
}

double  Matrix::operator () (size_t i, size_t j) const
{
#ifndef NDEBUG
  if (j >= Mat.size() || i >= Mat[j].Laenge())
    MatFehler("Ungueltiger Index!");
#endif

    return Mat[j](i);
}

Matrix& Matrix::operator =  (const Matrix& mat)
{
#ifndef NDEBUG
    if (Zeilen() != mat.Zeilen() || Spalten() != mat.Spalten())
        MatFehler("Inkompatible Dimensionen fuer 'Matrix = Matrix'!");
#endif

    for (size_t i = 0; i < Spalten(); i++)
        for (size_t j = 0; j < Zeilen(); j++)
            (*this) (i,j) = mat(i, j);

  //oder: this->Vek=x.Vek;

  return *this;

}

Matrix& Matrix::operator += (const Matrix& mat)
{
#ifndef NDEBUG
    if (Zeilen() != mat.Zeilen() || Spalten() != mat.Spalten())
        MatFehler("Inkompatible Dimensionen fuer 'Matrix += Matrix'!");
#endif

    for (size_t i = 0; i < Spalten(); i++)
        for (size_t j = 0; j < Zeilen(); j++)
            (*this) (i,j) += mat(i, j);

    return *this;
}

Matrix& Matrix::operator -= (const Matrix& mat)
{
#ifndef NDEBUG
    if (Zeilen() != mat.Zeilen() || Spalten() != mat.Spalten())
        MatFehler("Inkompatible Dimensionen fuer 'Matrix -= Matrix'!");
#endif

    for (size_t i = 0; i < Spalten(); i++)
        for (size_t j = 0; j < Zeilen(); j++)
            (*this) (i,j) -= mat(i, j);

    return *this;
}

Matrix& Matrix::operator *= (const double x)
{
    for (size_t i = 0; i < Spalten(); i++)
        for (size_t j = 0; j < Zeilen(); j++)
            (*this) (i,j) *= x;

    return *this;
}

Matrix& Matrix::operator /= (const double x)
{
    for (size_t i = 0; i < Spalten(); i++)
        for (size_t j = 0; j < Zeilen(); j++)
            (*this) (i,j) /= x;

    return *this;
}


Matrix& Matrix::ReDim (size_t l, size_t m)
{
#ifndef NDEBUG
  if (l <= 0)
    MatFehler("Nur Vektoren mit positiver Laenge!");
#endif

  Mat.clear();
  for(auto& vec: Mat)
    vec.ReDim(l);
  Mat.resize(m, Vektor(l));

  return *this;
}

Matrix Matrix::operator + (const Matrix& a)
{
#ifndef NDEBUG
  if (a.Zeilen() != Zeilen() || a.Spalten() != Spalten())
    MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
#endif

    Matrix b = a;
    return b += (*this);
}

Matrix Matrix::operator - (const Matrix& a)
{
#ifndef NDEBUG
  if (a.Zeilen() != Zeilen() || a.Spalten() != Spalten())
    MatFehler("Inkompatible Dimensionen fuer 'Matrix - Matrix'!");
#endif

    Matrix b = a;
    return b -= (*this);
}

Matrix Matrix::operator- ()
{
    Matrix t = *this;
    return t *= -1.0;
}

void Matrix::MatFehler (const std::string& str)
{
    std::cerr << "\nMatrixfehler: " << str << '\n' << std::endl;
    exit(1);
}
