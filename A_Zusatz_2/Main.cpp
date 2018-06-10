#include <vector>
#include <deque>
#include <set>
#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>

struct Element
{
    int value, weight;
    bool in;
};

void ReadElements(std::ifstream& ifs, int& bagSize, std::vector<Element>& elements)
{
    auto feld = std::vector<int>(
        std::istream_iterator<int>(ifs),
        std::istream_iterator<int>());


    int elemCount = feld[0];
    bagSize = feld[1];
    elements.resize(elemCount);
    for (size_t i = 1; i <= elemCount; i++) {
        auto& e = elements[i - 1];
        e.value = feld[i * 2];
        e.weight = feld[i * 2 + 1];
        e.in = false;
    }
}

void PrintSol(size_t it, std::vector<Element>& elements, std::deque<int> tabu, int bagSize)
{
    int freeSpace = bagSize;
    int value = 0;

    std::cout << it << " Lsg. x=[";
    bool empty = true;
    for (size_t i = 0; i < elements.size(); i++) {
        if(elements[i].in)
        {
            std::cout << i + 1 << ",";
            freeSpace -= elements[i].weight;
            value += elements[i].value;
            empty = false;
        }
    }
    if(!empty)
        std::cout << '\b';

    std::cout << "], TL=[";
    empty = true;
    for (size_t i = 0; i < tabu.size(); i++) {
        std::cout << tabu[i] + 1 << ",";
        empty = false;
    }
    if(!empty)
        std::cout << '\b';
    std::cout << "], Frei=" << freeSpace <<
        " kg, Gewicht=" << bagSize - freeSpace <<
        " kg, Gewinn=" << value << " EUR\n";
}

bool Add(std::vector<Element>& elements, std::deque<int>& tabu, int& freeSpace, int& currentValue, int tabuSize)
{
    int max = -1;
    for (size_t i = 0; i < elements.size(); i++)
    {
        if(elements[i].in)
            continue;
        bool isTabu = false;
        for(auto& t: tabu)
            if(i == t)
                isTabu = true;
        if(isTabu)
            continue;
        if(elements[i].weight <= freeSpace)
        {
            if(max == -1 ||
                elements[i].value > elements[max].value ||
                (elements[i].value == elements[max].value && elements[i].weight < elements[max].weight))
                max = i;
        }
    }
    if(max == -1)
        return false;
    else
    {
        tabu.push_back(max);
        if(tabu.size() > tabuSize)
            tabu.pop_front();
        freeSpace -= elements[max].weight;
        currentValue += elements[max].value;
        elements[max].in = true;
        return true;
    }
}

void Clear(std::vector<Element>& elements, std::deque<int>& tabu, int& freeSpace, int& currentValue, int tabuSize)
{
    int min = -1;
    for (size_t i = 0; i < elements.size(); i++)
    {
        if(!elements[i].in)
            continue;
        bool isTabu = false;
        for(auto& t: tabu)
            if(i == t)
                isTabu = true;
        if(isTabu)
            continue;
        if(min == -1 ||
            elements[i].value < elements[min].value ||
            (elements[i].value == elements[min].value && elements[i].weight > elements[min].weight))
            min = i;
    }
    if(min != -1)
    {
        tabu.push_back(min);
        if(tabu.size() > tabuSize)
            tabu.pop_front();

        freeSpace += elements[min].weight;
        currentValue -= elements[min].value;
        elements[min].in = false;
    }
}

void TabuSearch(int bagSize, std::vector<Element>& elements, int tabuSize, int maxIter, int pC)
{
    std::deque<int> tabu;
    std::vector<int> best;
    int bestVal;

    int freeSpace = bagSize;
    int currentValue = 0;

    while(Add(elements, tabu, freeSpace, currentValue, tabuSize));
    tabu.clear();
    bestVal = currentValue;
    for (size_t i = 0; i < elements.size(); i++)
        if(elements[i].in)
            best.push_back(i);

    if(pC > 0)
        PrintSol(0, elements, tabu, bagSize);
    for(int i = 1; i < maxIter; i++)
    {
        if(!Add(elements, tabu, freeSpace, currentValue, tabuSize))
            Clear(elements, tabu, freeSpace, currentValue, tabuSize);
        else if(bestVal < currentValue)
        {
            best.clear();
            bestVal = currentValue;
            for (size_t i = 0; i < elements.size(); i++)
                if(elements[i].in)
                    best.push_back(i);

            std::cout << "---- neuer Gewinn: [";
            for (size_t i = 0; i < best.size(); i++)
                std::cout << best[i] + 1 << ",";
            std::cout << "\b] mit " << bestVal << " EUR ---\n";
        }

         if(pC && !(i % pC))
            PrintSol(i, elements, tabu, bagSize);
    }

    std::cout << "\nTD: " << tabuSize << '\n';
    std::cout << "Gefunden x=[";
    bool empty = true;
    for (size_t i = 0; i < elements.size(); i++) {
        if(elements[i].in)
        {
            std::cout << i + 1 << ",";
            empty = false;
        }
    }
    if(!empty)
        std::cout << '\b';

    std::cout << "],\n TL=[";
    empty = true;
    // for (size_t i = 0; i < tabu.size(); i++) {

    std::sort(tabu.begin(), tabu.end());
    for(auto&e: tabu)
    {
        std::cout << e + 1 << ",";
        empty = false;
    }
    if(!empty)
        std::cout << '\b';
    std::cout << "],\n";

    int mass = 0;
    for(auto& e: best)
        mass += elements[e].weight;
    std::cout << "Gewicht=" << mass <<
        " kg, Gewinn=" << bestVal << " EUR\n";
}

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        std::cout << "Bitte geben sie folgende Parameter an:" << '\n';
        std::cout << "./a.out <Datei> <Tabudauer> <Iterationen> <AusgabeFrequenz>" << '\n';
        return 1;
    }
    std::ifstream ifs(argv[1]);
    int bagSize;
    std::vector<Element> elements;
    ReadElements(ifs, bagSize, elements);
    TabuSearch(bagSize, elements,
        std::stoi(argv[2]),
        std::stoi(argv[3]),
        std::stoi(argv[4]));

}
