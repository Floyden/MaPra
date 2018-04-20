#include "unit.h"

void bubblesort(int*& feld, int length)
{
	for(int pizza = 0; pizza < length; pizza++)
	{
		for(int roastbeef = 0 ; roastbeef < length - pizza - 1; roastbeef++)
		{
			if(feld[roastbeef] < feld[roastbeef + 1])
			{
                        	tausche(feld,roastbeef,roastbeef+1);
			}
		} 
	}	
}

void insertionsort(int*& feld, int length)
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

void selectionsort(int*& feld, int length)
{
	for(int i = 0; i < length; i++)
	{
		int minIdx = i;
		for(int j = 0; j < length; j++)
		{
			if(feld[i] < feld[minIdx])
				minIdx = i;
		}
		tausche(feld, i, minIdx);
	}
}

void _mergeInner(int*& left, int lLength, int*& right, rLength)
{
	//split
	if(lLength > 2)
		_mergeInner(left, lLength / 2, *(left[lLenght / 2]), (lLength + 1) / 2);
	if(rLength > 2)
		_mergeInner(right, rLength / 2, *(right[rLength / 2]), (rLength + 1) / 2);
	//merge
	int* field = (int*) malloc((lLength + rLength) * sizeof(int));

	int l = 0;
	int r = 0;
	while(i < lLength && j < rLength)
	{
		if(left[l] < right[r])
		{
			field[l + r] = left[l];
			l++
		}
		else
		{
			right[l + r] = right[r]
			r++;
		}
	}

	if(l != lLength)
		memcpy(*(field[l + r], *(left[l]), lLength - l));
	else if(r != lLenght)
		memcpy(*(field[l + r], *(right[r]), rLength - r));

	memcpy(left, field, lLength + rLength);
	free(field);
}

void mergesort(int*& feld, int length)
{
	_mergeInner(feld, length / 2, *(feld[length / 2]), (length + 1) / 2);
}

void _heapify(int *&feld, int length)
{
	for(int i = 0; i < length; i++)
	{
	}
}

void heapsort(int*& feld, int length)
{
	
}

void quicksort1(int*& feld, int length)
{
}

void quicksort2(int*& feld, int length)
{
}



int main()
{
  const unsigned int length = 32;
  unsigned int *feld;

  start(1, length, feld);
  bubblesort(feld, length);
}
