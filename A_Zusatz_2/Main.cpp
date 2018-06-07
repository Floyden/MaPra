#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <fstream>

struct Element
{
    int value, weight;
    int touch;
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
        e.touch = 0;
        e.in = false;
    }
}

void PrintSol(size_t it, std::vector<Element>& elements, int bagSize)
{
    int freeSpace = bagSize;
    int value = 0;

    std::cout << it << " Lsg. x=[";
    for (size_t i = 0; i < elements.size(); i++) {
        if(elements[i].in)
        {
            std::cout << i << ",";
            freeSpace -= elements[i].weight;
            value += elements[i].value;
        }
    }
    std::cout << "], TL=[], Frei=" << freeSpace <<
        " kg Gewicht=" << bagSize - freeSpace << 
        " kg, Gewinn=" << value << " EUR\n";
}

void TabuSearch(int bagSize, std::vector<Element>& elements)
{
    int freeSpace = bagSize;
    int currentValue = 0;

    for (size_t i = 0; i < elements.size(); i++)
    {
        size_t max = 0;
        for (size_t j = 0; j < elements.size(); j++)
        {
            if(elements[j].in)
                continue;
            if(elements[i].value < elements[j].value && elements[j].weight <= freeSpace)
                max = j;
        }
        std::cout << i << '\n';
        if(max == 0 && elements[0].in || elements[0].weight > freeSpace)
            break; // found no more element that fits

        freeSpace -= elements[max].weight;
        currentValue += elements[max].value;
        elements[max].in = true;
    }

    PrintSol(0, elements, bagSize);
}

int main(int argc, char* argv[])
{
    std::ifstream ifs("data.1");
    int bagSize;
    std::vector<Element> elements;
    ReadElements(ifs, bagSize, elements);
    TabuSearch(bagSize, elements);


}
