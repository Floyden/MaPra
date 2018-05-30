// ...

#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"

void NetzwerkMain();

// ...

const unsigned int Schwierigkeitsgrad = 0;

enum Feld
{ leer, gelb, rot };

// ...

int main()
{
    int Zug, Gegenzug;

    // Netzwerkspiel? Rufe NetzwerkMain() auf.

// ...

   Start(Schwierigkeitsgrad);

    for(unsigned int Spiel = 1; Spiel <= AnzahlSpiele; Spiel++)
    {
        // NaechsterZug()
// ...

    }

    return 0;
}

enum class SpielStatus {
    Verbindungsfehler,
    Niederlage,
    Unentschieden,
    Sieg
};

// Spielt ein einzelnes Netzwerkspiel gegen einen anderen Computer im Netzwerk.
// Sollte das Spiel beendet sein oder ein Netzwerkfehler auftreten, muss diese Methode
// das zugehoerige Element der Enumeration SpielStatus zurueckgeben.
SpielStatus Netzwerkspiel( Feld MeineFarbe ) {

    // TODO Implementiere Netzwerkprotokoll

    return SpielStatus::Verbindungsfehler;
}

void NetzwerkMain() {
    int command;

    // Einleseschleife für Befehle. Terminiert, wenn ein gültiger Befehl gewählt wurde.
    while (true) {
        std::cout << "\n";
        std::cout << "1 = VERBINDE  mit einem anderen Spieler\n";
        std::cout << "2 = WARTE     auf einen anderen Spieler" << std::endl;

        std::cin >> command;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        } else if (command >= 1 && command <= 2) {
            break;
        }
    }

    if (command == 0) {
        return;
    }

    Feld MeineFarbe=  gelb,
         GegnerFarbe= rot;

    if (command == 1) {
        std::string ip;
        unsigned short port = 0;

        std::cout << "Bitte geben Sie die IP ein, mit der Sie sich verbinden wollen. \n";
        std::cout << "Format: xxx.xxx.xxx.xxx" << std::endl;
        std::cin >> ip;
        std::cout << "Bitte geben Sie den Port ein, mit dem Sie sich verbinden wollen. \n";
        std::cin >> port;

        if (!Verbinde(ip.c_str(), port)) {
            std::cout << "Verbindung fehlgeschlagen." << std::endl;
            return;
        }

        std::cout << "Verbindung hergestellt." << std::endl;

        // Ich verbinde mich -> Meine Farbe ist rot
        std::swap(MeineFarbe, GegnerFarbe);
    } else {
        unsigned short port = 0;

        std::cout << "Bitte geben Sie den Port ein, mit dem sich Ihr Mitspieler verbinden soll. \n";
        std::cin >> port;
        std::cout << "Warte auf Gegner..." << std::endl;

        if (!WarteAufVerbindung(port)) {
            std::cout << "Kein Gegner gefunden.";
            return;
        }
        std::cout << "Verbindung hergestellt." << std::endl;

        // Ich warte auf Spieler -> meine Farbe ist gelb
    }

    bool nochEinSpiel = true;
    while (nochEinSpiel) {
        auto ergebnis = Netzwerkspiel( MeineFarbe );

        if (ergebnis == SpielStatus::Verbindungsfehler) {
            nochEinSpiel = false;
            std::cout << "Verbindungsfehler!" << std::endl;
        } else {
            switch(ergebnis) {
                case SpielStatus::Niederlage:
                    std::cout << "Sie haben verloren!" << std::endl;
                    break;
                case SpielStatus::Unentschieden:
                    std::cout << "Es gab ein Unentschieden!" << std::endl;
                    break;
                case SpielStatus::Sieg:
                    std::cout << "Sie haben gewonnen!" << std::endl;
                    break;
                default:
                    std::cout << "Verbindungsfehler!" << std::endl;
                    SchliesseVerbindung();
                    return;
            }

            std::cout << "Nochmal spielen? [y/n]" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            char antwort;
            std::cin >> antwort;
            nochEinSpiel = ! (!std::cin || (antwort != 'Y' && antwort != 'y') );

            if (nochEinSpiel) {
                std::swap( MeineFarbe, GegnerFarbe );
                NaechstesNetzwerkspiel();
            }
        }
    }

    SchliesseVerbindung();
}
