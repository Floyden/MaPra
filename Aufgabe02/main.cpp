#include "unit.h"
#include <cstdlib>
#include <cstddef>
#include <cstring>

void bubblesort(unsigned int*& feld, int length)
{
        for(int pizza = 0; pizza < length - 1; pizza++)
        {
                for(int roastbeef = 0 ; roastbeef < length - pizza - 1; roastbeef++)
                {
                        if(feld[roastbeef] > feld[roastbeef + 1])
                        {
                                tausche(feld,roastbeef,roastbeef+1);
                        }
                }
        }
}

void insertionsort(unsigned int*& feld, int length)
{
        for(int i = 1; i < length; i++)
        {
                for(int j = i; j > 0; j--)
                {
                        if(feld[j] < feld[j - 1])
                                tausche(feld, j, j - 1);
                        else
                                break;
                }
        }
}

void selectionsort(unsigned int*& feld, int length)
{
        for(int i = 0; i < length; i++)
        {
                int minIdx = i;
                for(int j = i; j < length; j++)
                {
                        if(feld[j] < feld[minIdx])
                                minIdx = j;
                }
                tausche(feld, i, minIdx);
        }
}

void _mergeInner(unsigned int*& left, int lLength, unsigned int*& right, int rLength)
{
    //split
    if(lLength > 1)
    {
        unsigned int* _left = &(left[lLength / 2]);
        _mergeInner(left, lLength / 2, _left, (lLength + 1) / 2);
    }
    if(rLength > 1)
    {
        unsigned int* _right = &(right[rLength / 2]);
            _mergeInner(right, rLength / 2, _right, (rLength + 1) / 2);
    }

    //merge
    unsigned int* field = (unsigned int*) malloc((lLength + rLength) * sizeof(unsigned int));

    int l = 0;
    int r = 0;
    while(l < lLength && r < rLength)
    {
        if(left[l] < right[r])
        {
            field[l + r] = left[l];
            l++;
        }
        else
        {
            field[l + r] = right[r];
            r++;
        }
    }

    if(l < lLength)
    {
        memcpy(&(field[l + r]), &(left[l]), (lLength - l) * sizeof(int));
    }
    else if(r < rLength)
    {
        memcpy(&(field[l + r]), &(right[r]), (rLength - r) * sizeof(int));
    }


    memcpy(left, field, (lLength + rLength) * sizeof(int) );
    free(field);
}

void mergesort(unsigned int*& feld, int length)
{
    unsigned int* left = &(feld[length / 2]);
    _mergeInner(feld, length / 2, left, (length + 1) / 2);
}

void _heapify(unsigned int*& feld, int length, int pos)
{
        int next = 2 * pos + 1;
        while(next < length)
        {
                if(next + 1 < length && feld[next] < feld[next + 1])
                        next++;
                if(feld[pos] >= feld[next])
                        break;
                tausche(feld, pos, next);
                pos = next;
                next = 2 * pos + 1;
        }
}

void heapsort(unsigned int*& feld, int length)
{
    std::cout << "start" << '\n';
    for(int i = 0; i < length; i++)
        std::cout << feld[i] << " ";
    std::cout << std::endl;

    for(int i = 0; i < length; i++)
            _heapify(feld, length - i, i);

    std::cout << "heap" << '\n';
    for(int i = 0; i < length; i++)
        std::cout << feld[i] << " ";
    std::cout << std::endl;

    for(int i = length - 1; i >= 0; i--)
    {
            tausche(feld, 0, i);
            _heapify(feld, i, 0);
    }

    std::cout << "sorted" << '\n';
    for(int i = 0; i < length; i++)
        std::cout << feld[i] << " ";
    std::cout << std::endl;
}

void quicksort(unsigned int*& feld, int length)
{
    if(length <= 1)
        return;

    // std::cout << "start" << '\n';
    // for(int i = 0; i < length; i++)
    //     std::cout << feld[i] << " ";
    // std::cout << std::endl;

    int pivot = length - 1;
    int i = 0;
    int j = length - 2;
    while(i < j)
    {
        if(feld[i] <= feld[pivot])
            i++;
        else if(feld[j] > feld[pivot])
            j--;
        if(feld[i] > feld[pivot] && feld[j] <= feld[pivot])
            tausche(feld, i, j);

    }
    if(feld[j] > feld[pivot])
        tausche(feld, j, pivot);
    else
        j = pivot;

    // std::cout << "sorted" << '\n';
    // for(int i = 0; i < length; i++)
    //     std::cout << feld[i] << " ";
    // std::cout << std::endl;

    unsigned int* left = &(feld[j + 1]);
    quicksort(feld, j);
    quicksort(left, length - j - 1);
}

void quicksortMedian(unsigned int*& feld, int length)
{
    if(length <= 1)
        return;
    else if(length == 2 && feld[0] > feld[1])
    {
        tausche(feld, 0, 1);
        return;
    }


    if(feld[0] > feld [length / 2])
        tausche(feld, 0, length/2);
    if(feld[length - 1] > feld [length / 2])
        tausche(feld, length / 2, length - 1);

    int pivot = length - 1;
    int i = 0;
    int j = length - 2;
    while(i < j)
    {
        if(feld[i] <= feld[pivot])
            i++;
        else if(feld[j] > feld[pivot])
            j--;
        if(feld[i] > feld[pivot] && feld[j] <= feld[pivot])
            tausche(feld, i, j);

    }
    if(feld[j] > feld[pivot])
        tausche(feld, j, pivot);
    else
        j = pivot;

    unsigned int* left = &(feld[j + 1]);
    quicksortMedian(feld, j);
    quicksortMedian(left, length - j - 1);
}

int main()
{
    size_t length = 16;
    unsigned int *feld;


    for(int i = 1; i <= AnzahlBeispiele; i++)
    {
        start(i, length, feld);
        mergesort(feld, length);
        ergebnis(feld);
    }
}
