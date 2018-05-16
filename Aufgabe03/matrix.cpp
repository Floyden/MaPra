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

    for (size_t i = 0; i < Zeilen(); i++)
        for (size_t j = 0; j < Spalten(); j++)
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

    for (size_t i = 0; i < Zeilen(); i++)
        for (size_t j = 0; j < Spalten(); j++)
            (*this) (i,j) += mat(i, j);

    return *this;
}

Matrix& Matrix::operator -= (const Matrix& mat)
{
#ifndef NDEBUG
    if (Zeilen() != mat.Zeilen() || Spalten() != mat.Spalten())
        MatFehler("Inkompatible Dimensionen fuer 'Matrix -= Matrix'!");
#endif

    for (size_t i = 0; i < Zeilen(); i++)
        for (size_t j = 0; j < Spalten(); j++)
            (*this) (i,j) -= mat(i, j);

    return *this;
}

Matrix& Matrix::operator *= (const double x)
{
    for (size_t i = 0; i < Zeilen(); i++)
        for (size_t j = 0; j < Spalten(); j++)
            (*this) (i,j) *= x;

    return *this;
}

Matrix& Matrix::operator /= (const double x)
{
    for (size_t i = 0; i < Zeilen(); i++)
        for (size_t j = 0; j < Spalten(); j++)
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

Matrix operator + (const Matrix& a, const Matrix& b)
{
#ifndef NDEBUG
  if (a.Zeilen() != b.Zeilen() || a.Spalten() != b.Spalten())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
#endif

    Matrix c = a;
    return c += b;
}

Matrix operator - (const Matrix& a, const Matrix& b)
{
#ifndef NDEBUG
  if (a.Zeilen() != b.Zeilen() || a.Spalten() != b.Spalten())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix - Matrix'!");
#endif

    Matrix c = a;
    return c -= b;
}

Matrix operator-(const Matrix& a)
{
    Matrix t = a;
    return t *= -1.0;
}

Matrix Matrix::operator + (const Matrix& a)
{
#ifndef NDEBUG
  if (a.Zeilen() != Zeilen() || a.Spalten() != Spalten())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix + Matrix'!");
#endif

    Matrix c = a;
    return c += *this;
}

Matrix Matrix::operator - (const Matrix& a)
{
#ifndef NDEBUG
  if (a.Zeilen() != Zeilen() || a.Spalten() != Spalten())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix - Matrix'!");
#endif

    Matrix c = *this;
    return c -= a;
}

Matrix Matrix::operator-()
{
    Matrix t = *this;
    return t *= -1.0;
}

Matrix operator * (const Matrix& a, const Matrix& b)
{
#ifndef NDEBUG
  if (a.Spalten() != b.Zeilen())
    Matrix::MatFehler("Inkompatible Dimensionen fuer 'Matrix * Matrix'!");
#endif
    Matrix r(a.Zeilen(), b.Spalten());
    for(size_t i = 0; i < a.Zeilen(); i++)
        for(size_t j = 0; j < b.Spalten(); j++)
            for(size_t k = 0; k < b.Zeilen(); k++)
                r(i,j) += a(i,k) * b(k,j);
    return r;
}

Matrix operator * (const double x,  const Matrix& m)
{
    Matrix t = m;
    return t *= x;
}

Matrix operator * (const Matrix& m, const double x)
{
    Matrix t = m;
    return t *= x;
}

Matrix operator / (const Matrix& m, const double x)
{
    Matrix t = m;
    return t /= x;
}

void Matrix::MatFehler (const std::string& str)
{
    std::cerr << "\nMatrixfehler: " << str << '\n' << std::endl;
    exit(1);
}
