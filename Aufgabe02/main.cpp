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
        if(lLength > 2)
    {
        unsigned int* _left = &(left[lLength / 2]);
        _mergeInner(left, lLength / 2, _left, (lLength + 1) / 2);
    }
        if(rLength > 2)
    {
        unsigned int* _right = &(right[rLength / 2]);
                _mergeInner(right, rLength / 2, _right, (rLength + 1) / 2);
    }
        //merge
        int* field = (int*) malloc((lLength + rLength) * sizeof(int));

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
                        right[l + r] = right[r];
                        r++;
                }
        }

        if(l != lLength)
                memcpy(&(field[l + r]), &(left[l]), lLength - l);
        else if(r != rLength)
                memcpy(&(field[l + r]), &(right[r]), rLength - r);

        memcpy(left, field, lLength + rLength);
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
        for(int i = 0; i < length; i++)
                _heapify(feld, length - i, i);
        for(int i = length - 1; i >= 0; i--)
        {
                tausche(feld, 0, i);
                _heapify(feld, i, 0);
        }
}

void quicksort(unsigned int*& feld, int length)
{
    int pivot = length - 1;
    int j;
        for(int i = 0, j = length - 2; i < j; i++)
    {
        if(feld[i] <= feld[pivot])
            continue;
        else
        {
            while(i < j)
            {
                if(feld[j] <= feld[pivot])
                    break;
                else
                    j--;
            }
            tausche(feld, i, j);
        }
    }
    tausche(feld, j, pivot);

    quicksort(feld, j);

    unsigned int* left = &(feld[j]);
    quicksort(left, length - j);
}

void quicksortMedian(unsigned int*& feld, int length)
{
    if(feld[0] > feld [length / 2])
        tausche(feld, 0, length/2);
    if(feld[length - 1] > feld [length / 2])
        tausche(feld, length / 2, length - 1);

    int pivot = length - 1;
    int j;
        for(int i = 0, j = length - 2; i < j; i++)
    {
        if(feld[i] <= feld[pivot])
            continue;
        else
        {
            while(i < j)
            {
                if(feld[j] <= feld[pivot])
                    break;
                else
                    j--;
            }
            tausche(feld, i, j);
        }
    }
    tausche(feld, j, pivot);

    quicksortMedian(feld, j);

    unsigned int* left = &(feld[j]);
    quicksortMedian(left, length - j);
}

int main()
{
    size_t length = 32;
    unsigned int *feld;

    start(1, length, feld);

    for(int i = 0; i < length; i++)
    std::cout << feld[i] << " ";
    std::cout << std::endl;

    selectionsort(feld, length);

    for(int i = 0; i < length; i++)
    std::cout << feld[i] << " ";
    std::cout << std::endl;
    ergebnis(feld);
}
