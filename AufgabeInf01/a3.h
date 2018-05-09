#ifndef A3_HEADER
#define A3_HEADER

#include <cstdlib>

typedef unsigned long int lint;

/*----------------------------------------------------------------------------------------

TSieb enthaelt einen langen Vektor von bool und dient hauptsaechlich dazu, die Operatoren
zum Lesen (operator[] const) und Schreiben (operator[]) mit einem Index-Test auszustatten.

Am Besten wie folgt benutzen:

          typedef TSieb(unsigned int, 20000)   CSieb;
		                 (Indextyp)  (Laenge)

Auf die Laenge kann man mit

          CSieb::length()

oder
          CSieb Sieb;  Sieb.length()

zugreifen.

----------------------------------------------------------------------------------------*/
template <class LINT, LINT LEN>
class TSieb {

	enum { len = LEN };

	bool bSieb[len];

public:

	typedef LINT lint;

	TSieb() { for (lint i=0; i<len; ++i) bSieb[i]=true; }

	bool operator[](lint n) const
	{	if (n>=len)
		{
			std::cerr << "Falscher Index (" << n << ") in TSieb::op[]" << std::endl;
			exit(-1);
		}
	return bSieb[n];
	}

	bool& operator[](lint n)
	{	if (n>=len)
		{
			std::cerr << "Falscher Index (" << n << ") in TSieb::op[]" << std::endl;
			exit(-1);
		}
		return bSieb[n];
	}

	friend bool operator==(const TSieb& s1, const TSieb& s2)
	{
		lint i;
		for (i=0; (i<TSieb::length()) && (s1[i]==s2[i]); ++i) ;
		return (i==TSieb::length());
	}

	static lint length() { return len; }
};

typedef TSieb<lint,100000>	CSieb;

void SiebTest(const CSieb& sieb);


enum Test { Probe, Fermat, MillerRabin };

extern unsigned int MaxTestPrimzahl;
extern unsigned int AnzahlBeispiele;
lint HoleKandidaten(Test Methode, unsigned int nBsp);
void TestErgebnis(Test Methode, unsigned int nBsp, const lint f, const lint Basis=0, const lint r=0);


#endif