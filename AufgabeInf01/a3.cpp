//------------------------------------------------------------------------------
// Mapra WS 2000, A. Voss, A3: Primzahltests
//------------------------------------------------------------------------------

#include <iostream>
#include <cmath>

#include "a3.h"

//------------------------------------------------------------------------------
// bitte mit Leben fuellen:
//------------------------------------------------------------------------------
lint fastpow(lint, lint, lint);
// Probe

lint PrimTest(lint n, const CSieb& Sieb)
{
	// if(Sieb[n])
	// 	return 1;

	for(int i = 2; i < n; i++)
	{
		if(i < Sieb.length() && !Sieb[i])
			continue;
		if(!(n % i))
			return i;
	}
	// Shouldn't be reached
	return 1;
}

// Fermat

lint FermatTest(lint n, lint a)
{
	return fastpow(a, n-1, n);
}

// Miller-Rabin

lint MillerRabinTest(lint n, lint a, lint d, lint r)
{
	lint e = 1;
	for(lint i = 0; i < r; i++) e *= 2;
	return fastpow(a, e * d, n);
}

// Hilfsfunktionen

lint fastpow(lint nBase, lint nExp, lint nMod)
{
	nBase %= nMod;
	lint res = 1;
	while(nExp > 0)
	{
		if(nExp & 1)
			res = (res * nBase) % nMod;
		nBase = (nBase * nBase) % nMod;
		nExp >>= 1;
	}
	return res;
}

lint gcd(lint a, lint b)
{
	if( a < 1 || b < 1)
	{
		std::cout<< "Error < 0 " << std::endl;
		exit(-1);
	}
	lint y = 0;
	do
	{
		y = a%b;
		a = b;
		b = y;
	}while(b!=0);
	return a;

}

void get_ds(lint n, lint& d, lint& s)
{
	
	d = (n - 1) / 2;
	for (s = 1; s < n; s++)
	{
		if(d % 2)
			return;
		d /= 2;
	}
}

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
		for(int j = i + i; j < Sieb.length(); j += i)
		{
			Sieb[j] = false;
		}
	}

	SiebTest(Sieb);

	// PrimTest
	// for(int i = 1; i <= AnzahlBeispiele; i++)
	// {
	// 	lint val = HoleKandidaten(Test::Probe, i);
	//
	// 	lint res = PrimTest(val, Sieb);
	// 	TestErgebnis(Test::Probe, i	, res);
	// }

	// Fermat-Test
	for(int i = 1; i <= AnzahlBeispiele; i++)
	{
		lint val = HoleKandidaten(Test::Fermat, i);

		bool prim = true;
		for(int a = 0; a < MaxTestPrimzahl; a++)
		{
			if(!Sieb[a] || gcd(a, val) != 1)
				continue;
			lint y = FermatTest(val, a);
			if(y != 1)
				prim = false;

			TestErgebnis(Test::Fermat, i, y, a);
		}
		TestErgebnis(Test::Fermat, i, prim, 0);
	}


	// Miller-Rabin-Test
	for(int i = 1; i <= AnzahlBeispiele; i++)
	{
		lint val = HoleKandidaten(Test::MillerRabin, i);

		bool prim = true;
		for(int a = 0; prim && a <= MaxTestPrimzahl; a++)
		{
			if(!Sieb[a] || gcd(a, val) != 1)
				continue;
			lint d, s, tmp;
			get_ds(val, d, s);

			bool found = false;

			// lint y = MillerRabinTest(val, a, d, 0);
			// TestErgebnis(Test::MillerRabin, i, y, a, 0);
			// if(y == 1 || y == val - 1)
			// 	found = true;
			// for(lint r = 1; !found && r < s ; r++)
			// {
			// 	lint y = MillerRabinTest(val, a, d, r);
			// 	TestErgebnis(Test::MillerRabin, i, y, a, r);
			// 	if(y == val - 1)
			// 		found = true;
			// 	if(y <= 1)
			// 		break;
			// }
			for(lint r = 0; !found && r < s ; r++)
			{
				lint y = MillerRabinTest(val, a, d, r);
				TestErgebnis(Test::MillerRabin, i, y, a, r);

				if(y == 1)
				{
					if(r == 0)
						found  = true;
					else
						break;
				}
				else if(y == val - 1 )
				{
					found  = true;
				}
			}
			if(!found)
				prim = false;
		}
		TestErgebnis(Test::MillerRabin, i, prim, 0, 0);
	}

	return 0;
}
