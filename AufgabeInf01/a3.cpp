//------------------------------------------------------------------------------
// Mapra WS 2000, A. Voss, A3: Primzahltests
//------------------------------------------------------------------------------

#include <iostream>
#include <cmath>

#include "a3.h"

//------------------------------------------------------------------------------
// bitte mit Leben fuellen:
//------------------------------------------------------------------------------

// Probe

lint PrimTest(lint n, const CSieb& Sieb)
{
	if(Sieb[n])
		return 1;

	for(int i = 2; i < n; i++)
	{
		if(!Sieb[i])
			continue;
		if(!(n % i))
			return i;
	}
	// Shouldn't be reached
	std::cout << "PrimTest: Reached unreachable code\n";
	return 0;
}

// Fermat

lint FermatTest(lint n, lint a)
{ return 0; }

// Miller-Rabin

lint MillerRabinTest(lint n, lint a, lint d, lint r)
{ return 0; }

// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod)
{ return 0; }

lint gcd(lint a, lint b)
{ return 0; }

void get_ds(lint n, lint& d, lint& s)
{ return; }

//------------------------------------------------------------------------------
// Hauptprogramm
//------------------------------------------------------------------------------

int main()
{
	CSieb Sieb;
	Sieb[0] = false;
	Sieb[1] = false;
	for(int i = 2; i < Sieb.length(); i++)
		Sieb[i] = true;

	for(int i = 2; i < Sieb.length(); i++)
	{
		if(!Sieb[i])
			continue;
		for(int j = i + i; i < Sieb.length(); j += i)
			Sieb[j] = false;
	}

	SiebTest(Sieb);

	// PrimTest
	
	// Fermat-Test

	// Miller-Rabin-Test

	return 0;
}
