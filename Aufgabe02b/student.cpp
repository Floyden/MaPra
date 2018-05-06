#include "student.h"
#include <cstring>

// Eingabeoperator ">>"
std::istream& operator>> ( std::istream& s, Student& a)
{
    s >> a.Vorname >> a.Nachname >> a.MatNr >> a.Note;
    return s;
}

// Ausgabeoperator "<<"
std::ostream& operator<< ( std::ostream& s, const Student& a)
{
    s << a.Vorname << ' '<< a.Nachname << ' ' << a.MatNr << ' ' << a.Note;
}

// Vergleichsoperator "<"
bool operator< ( const Student& a, const Student& b)
{
    if(a.Nachname < b.Nachname)
        return true;
    else if(a.Nachname > b.Nachname)
        return false;
    else if(a.Vorname < b.Vorname)
        return true;
    else
        return false;
}

// Vergleichsoperatoren "==" bzw. "!="
bool operator== ( const Student& a, const Student& b)
{
    return a.MatNr == b.MatNr;
}

bool operator!= ( const Student& a, const Student& b)
{
    return a.MatNr != b.MatNr;
}
