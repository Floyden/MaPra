#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <iostream>

#include "student.h"
#include "sort.h"
#include "unit.h"

using namespace std;

template <typename T>
void einlesen(ifstream& ifs, vector<T>& feld)
{
    feld = vector<T>(
        istream_iterator<T>(ifs),
        istream_iterator<T>()
    );
}

template <typename T>
void auslesen(ostream& ofs, vector<T>& feld)
{
    for(auto& e: feld)
        ofs << e << "\n";
}

int main()
{
    ifstream iDouble("doubles.txt");
    ifstream iString("strings.txt");
    ifstream iStudent("studenten.txt");
    vector<double> doubles;
    vector<string> strings;
    vector<Student> studenten;

    einlesen(iDouble, doubles);
    einlesen(iString, strings);
    einlesen(iStudent, studenten);

    string input;
    while(input != "A" && input != "B" && input != "M")
    {
        cout << "Sortierverfahren[A/B/M]: ";
        cin >> input;
    }

    if(input == "A")
    {
        selectionsort(doubles);
        selectionsort(strings);
        selectionsort(studenten);
    }

    auslesen(cout, doubles);
    auslesen(cout, strings);
    auslesen(cout, studenten);

    ergebnis(doubles);
    ergebnis(strings);
    ergebnis(studenten);
}
