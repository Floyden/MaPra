#pragma once
#include <vector>

using namespace std;

template <class T>
void tausche(vector<T>& feld, size_t i, size_t j)
{
    T tmp = feld.at(i);
    feld.at(i) = feld.at(j);
    feld.at(j) = tmp;
}

template <class T>
void bubblesort(vector<T>& feld)
{
    for(int pizza = 0; pizza < feld.size() - 1; pizza++)
    {
        for(int roastbeef = 0 ; roastbeef < feld.size() - pizza - 1; roastbeef++)
        {
            if(feld[roastbeef] > feld[roastbeef + 1])
            {
                swap(feld[roastbeef], feld[roastbeef + 1]);
                    // tausche(feld,roastbeef,roastbeef+1);
            }
        }
    }
}

template <class T>
void selectionsort(vector<T>& feld)
{
        for(int i = 0; i < feld.size(); i++)
        {
                int minIdx = i;
                for(int j = i; j < feld.size(); j++)
                {
                        if(feld[j] < feld[minIdx])
                                minIdx = j;
                }
                tausche(feld, i, minIdx);
        }
}

template <class T>
void _mergeInner(T* left, int lLength, T* right, int rLength)
{
    //split
    if(lLength > 1)
    {
        T* _left = &(left[lLength / 2]);
        _mergeInner(left, lLength / 2, _left, (lLength + 1) / 2);
    }
    if(rLength > 1)
    {
        T* _right = &(right[rLength / 2]);
            _mergeInner(right, rLength / 2, _right, (rLength + 1) / 2);
    }

    //merge
    T* field = (T*) malloc((lLength + rLength) * sizeof(unsigned int));

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
        memcpy(&(field[l + r]), &(left[l]), (lLength - l) * sizeof(T));
    }
    else if(r < rLength)
    {
        memcpy(&(field[l + r]), &(right[r]), (rLength - r) * sizeof(T));
    }

    memcpy(left, field, (lLength + rLength) * sizeof(T));
    free(field);
}

template <class T>
void mergesort(vector<T>& feld)
{
    T* left = &((feld.data())[feld.size() / 2]);

    _mergeInner(feld.data(), feld.size() / 2, left, (feld.size() + 1) / 2);
}
