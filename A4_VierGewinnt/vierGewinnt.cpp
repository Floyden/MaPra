// ...

#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 0;
const unsigned int LookAhead = 2;

enum Feld
{ leer, gelb, rot };

class Board
{
public:
    Board() { mBoard = new Feld[AnzahlZeilen * AnzahlSpalten]; Reset(); }
    ~Board() { delete mBoard; }

    void Reset();

    bool IsFull() const;
    int IsOver() const;
    bool IsFree(int x) const;

    bool Insert(int x, Feld color);
    void Remove(int x);

    Feld& At(int i, int j) const { return mBoard[i + j * AnzahlSpalten]; }

    Feld* mBoard;
};

void Board::Reset()
{
    for (size_t i = 0; i < AnzahlZeilen * AnzahlSpalten; i++)
        mBoard[i] = Feld::leer;
}

bool Board::IsFull() const
{
    for (size_t i = 0; i < AnzahlSpalten; i++)
        if(At(i, 0) == leer) return false;
    return true;
}

bool Board::IsFree(int x) const
{
    return At(x, 0) == leer;
}

bool Board::Insert(int x, Feld color)
{
    if(x < 0 || x >= AnzahlSpalten )
        std::cout << "Wat" << '\n';
    for (size_t i = AnzahlZeilen - 1; i > 0; i--)
    {
        if(At(x, i) == Feld::leer)
        {
            At(x, i) = color;
            return true;
        }
    }
    return false;
}

void Board::Remove(int x)
{
    for (size_t i = 0; i < AnzahlZeilen; i++)
    {
        if(At(x, i) != Feld::leer)
        {
            At(x, i) = Feld::leer;
            return;
        }
    }
}

int Board::IsOver() const
{
    //Check vertical
    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        for (size_t j = 0; j < AnzahlZeilen - 3; )
        {
            Feld col = At(i,j);
            if(col == Feld::leer)
            {
                j++;
                continue;
            }

            // AnzahlSpalten AnzahlZeilen

            if(At(i, ++j) == col && At(i, ++j) == col && At(i, ++j) == col)
            {
                return col == Feld::rot ? 1 : -1;
            }
        }
    }

    for (size_t i = 0; i < AnzahlZeilen; i++)
    {
        for (size_t j = 0; j < AnzahlSpalten - 3; )
        {
            Feld col = At(j,i);
            if(col == Feld::leer)
            {
                j++;
                continue;
            }

            // AnzahlSpalten AnzahlZeilen

            if(At(++j, i) == col && At(++j, i) == col && At(++j, i) == col)
            {
                return col == Feld::rot ? 1 : -1;
            }
        }
    }

    // //Check diagonal (right downward)
    // for (size_t i = 0; i < AnzahlSpalten - 3; i++)
    // {
    //     for (size_t j = 0; j < AnzahlZeilen - 3; j++)
    //     {
    //         size_t x = i;
    //         size_t y = j;
    //         while(x < AnzahlSpalten - 3 && y < AnzahlZeilen - 3)
    //         {
    //             Feld col= At(x, y);
    //             if(At(++x, ++y) == col && At(++x, ++y) == col && At(++x, ++y) == col)
    //             {
    //                 return col == Feld::rot ? 1 : -1;
    //             }
    //         }
    //     }
    // }
    // //Check diagonal (right downward)
    // for (size_t i = AnzahlSpalten - 1; i >= 3; i--)
    // {
    //     for (size_t j = 0; j < AnzahlZeilen - 3; j++)
    //     {
    //         size_t x = i;
    //         size_t y = j;
    //         while(x >= 3 && y < AnzahlZeilen - 3)
    //         {
    //             Feld col= At(x, y);
    //             if(At(--x, ++y) == col && At(--x, ++y) == col && At(--x, ++y) == col)
    //             {
    //                 return col == Feld::rot ? 1 : -1;
    //             }
    //         }
    //     }
    // }

    return 0;
}

double evaluate(Feld player, const Board& board)
{
    // Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;

    // Check if Lost
    int val = board.IsOver();
    if(val == 0)
    {
        return 0.0;
    }

    return -val;

    // if((val == 1 && player == Feld::rot) || (val == -1 && player == Feld::gelb))
    // {
    //     std::cout << "player wins" << '\n';
    //     return 1.0;
    // }
    // else
    // {
    //     std::cout << "enemy wins" << '\n';
    //     return -1.0;
    // }
}

double Min(Feld player, unsigned int depth, Board& board)
{
    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;
    double bestVal = 2.0;
    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.Insert(i, player))
            continue;

        double val = evaluate(player, board);
        if(val != 0.0)
            std::cout << "--> i: " << i << ", val: " << val << '\n';
        if(val < bestVal || (val == bestVal && rand() % 2))
        {
            bestVal = val;
        }
        board.Remove(i);
    }
    return bestVal;
}

//
int MinMax(Feld player, unsigned int depth, Board& board)
{
    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;

    std::cout << "Take Turn" << '\n';
    double bestVal = -2.0;
    int bestMove = 0;
    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.Insert(i, player))
            continue;

        std::cout << "-> i: " << i<< '\n';
        double val;
        if(!board.IsOver())
            val = -Min(enemy, depth - 1, board);
        else
            val = evaluate(player, board);

        if(val > bestVal || (val == bestVal && rand() % 2))
        {
            if(bestVal == 1.0)
                std::cout << i << '\n';
            bestVal = val;
            bestMove = i;
        }

        board.Remove(i);
    }

    std::cout << "Move: " << bestMove << '\n';

    return bestMove;
}

void TestOver()
{
    Board board;
    board.Reset();
    std::cout << "board.At(0,0)" << '\n';
    std::cout << board.At(0,0) << '\n';
    std::cout << board.At(0,AnzahlZeilen - 1) << '\n';
    std::cout << board.At(AnzahlSpalten - 1, 0) << '\n';
    std::cout << board.At(AnzahlSpalten - 1,AnzahlZeilen - 1) << '\n';

    {
        board.Insert(0, Feld::rot);
        board.Insert(0, Feld::rot);
        board.Insert(0, Feld::rot);
        board.Insert(0, Feld::rot);

        int val = board.IsOver();
        if(val != 1)
        {
            std::cout << "Error at IsOver: Expected red to win at vertical, instead ";
            if(val == 0)
                std::cout << "no-one won" << '\n';
            else
                std::cout << "yellow won" << '\n';
            return;
        }

        board.Reset();
    }

    {
        board.Insert(0, Feld::rot);
        board.Insert(1, Feld::rot);
        board.Insert(2, Feld::rot);
        board.Insert(3, Feld::rot);
        int val = board.IsOver();
        if(val != 1)
        {
            std::cout << "Error at IsOver: Expected red to win at horizontal, instead ";
            if(val == 0)
                std::cout << "no-one won" << '\n';
            else
                std::cout << "yellow won" << '\n';
            return;
        }

        board.Reset();
    }
    {
        board.Insert(0, Feld::rot);
        board.Insert(1, Feld::gelb);
        board.Insert(2, Feld::rot);
        board.Insert(3, Feld::gelb);
        board.Insert(0, Feld::gelb);
        board.Insert(1, Feld::gelb);
        board.Insert(2, Feld::gelb);
        board.Insert(3, Feld::gelb);
        int val = board.IsOver();
        if(val != -1)
        {
            std::cout << "Error at IsOver: Expected yellow to win at horizontal, instead ";
            if(val == 0)
                std::cout << "no-one won" << '\n';
            else
                std::cout << "red won" << '\n';
            return;
        }

        board.Reset();
    }
}

int main()
{
    TestOver();
    Board board;
    int Zug, Gegenzug;

    // Netzwerkspiel? Rufe NetzwerkMain() auf.

   Start(1);

    for(unsigned int Spiel = 1; Spiel <= AnzahlSpiele; Spiel++)
    {
        Feld playerCol;
        Feld enemyCol;

        if((Spiel % 2))
        {
            playerCol = Feld::gelb;
            enemyCol = Feld::rot;
        }
        else
        {
            playerCol = Feld::rot;
            enemyCol = Feld::gelb;
            Gegenzug = NaechsterZug(-1);
            board.Insert(Gegenzug, enemyCol);
            std::cout << "Enemy Plays: "<< Gegenzug << '\n';
        }


        while(Gegenzug != -1)
        {
            Zug = MinMax(playerCol, LookAhead, board);
            Gegenzug = NaechsterZug(Zug);

            if(Gegenzug < 0)
            {
                board.Reset();
                break;
            }
            std::cout << "Enemy Plays: "<< Gegenzug << '\n';
            board.Insert(Zug, playerCol);
            board.Insert(Gegenzug, enemyCol);

        }
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
