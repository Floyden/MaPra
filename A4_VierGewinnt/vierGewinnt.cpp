// ...

#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 0;
const unsigned int LookAhead = 8;

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
    bool IsWinningMove(int x, Feld player) const;

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

bool Board::IsWinningMove(int x, Feld player) const
{
    int y = AnzahlZeilen - 1;
    for(; y >= 0 && At(x, y) != Feld::leer; y--);

    // check vertical
    int n = 1;
    for (int i = y - 1; i >= 0 && n != 4; i--)
    {
        if(At(x, i) == player) n++;
        else break;
    }
    for (int i = y + 1; i < AnzahlZeilen && n != 4; i++)
    {
        if(At(x, i) == player) n++;
        else break;
    }

    if ( n >= 4) return true;

    // check horizontal
    n = 1;
    for (int i = x - 1; i >= 0 && n != 4; i--)
    {
        if(At(i, y) == player) n++;
        else break;
    }
    for (int i = x + 1; i < AnzahlSpalten && n != 4; i++)
    {
        if(At(i, y) == player) n++;
        else break;
    }

    if ( n >= 4) return true;

    // check diagonal1
    n = 1;
    for (int i = x + 1, j = y + 1; i < AnzahlSpalten && j < AnzahlZeilen && n != 4; i++, j++)
    {
        if(At(i, j) == player) n++;
        else break;
    }
    for (int i = x + 1, j = y + 1; i >= 0 && j >= 0 && n != 4; i--, j--)
    {
        if(At(i, j) == player) n++;
        else break;
    }

    if ( n >= 4) return true;

    // check diagonal2
    n = 1;
    for (int i = x + 1, j = y - 1; i < AnzahlSpalten && j >= 0 && n != 4; i++, j--)
    {
        if(At(i, j) == player) n++;
        else break;
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < AnzahlZeilen && n != 4; i--, j++)
    {
        if(At(i, j) == player) n++;
        else break;
    }
    if ( n >= 4) return true;

    return false;
}

bool Board::Insert(int x, Feld color)
{
    if(x < 0 || x >= AnzahlSpalten )
        std::cout << "Wat" << '\n';
    for (int i = AnzahlZeilen - 1; i >= 0; i--)
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

double evaluate(Feld player, const Board& board, int last_move = -1)
{
    return 0.0;
    // // Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;
    //
    // // Check if Lost
    // int val = board.IsOver();
    //
    //
    // if(val == 0)
    // {
    //     return 0.0;
    // }
    //
    // if((val == 1 && player == Feld::rot) || (val == -1 && player == Feld::gelb))
    // {
    //     // std::cout << "player wins" << '\n';
    //     return 1.0;
    // }
    // else
    // {
    //     std::cout << "enemy wins" << '\n';
    //     return -1.0;
    // }
}

double Min(Feld, int, Board&);
// Find best case for the player
double Max(Feld player, int depth, Board& board)
{
    if(depth == 0)
        return evaluate(player, board);

    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;
    double bestVal = -2.0;

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.IsFree(i))
            continue;

        double val;
        if(board.IsWinningMove(i, player))
            val = 1.0;
        else
        {
            board.Insert(i, player);
            val = -Max(enemy, depth - 1, board);
            board.Remove(i);
        }
        if(val != 0)
        {
          // if(depth == 1) std::cout << "----";
          // if(depth == 2) std::cout << "---";
          // if(depth == 3) std::cout << "--";
          // // if(depth == 4) std::cout << "-";
          // std::cout << "> i: " << i << ", val: " << val << '\n';
        }

        if(val > bestVal)
        {
            bestVal = val;
        }
    }
    return bestVal;
}

// Finds the worst case for the player
double Min(Feld player, int depth, Board& board)
{
    if(depth == 0)
        return evaluate(player, board);

    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;
    double worstVal = 2.0;

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.IsFree(i))
            continue;

        double val;
        if(board.IsWinningMove(i, player))
            val = 1.0;
        else
        {
            board.Insert(i, player);
            val = Max(enemy, depth - 1, board);
            board.Remove(i);
        }
        // if(val != 0)
        {
          // if(depth == 1) std::cout << "----";
          // if(depth == 2) std::cout << "---";
          // if(depth == 3) std::cout << "--";
          // if(depth == 4) std::cout << "-";
          // std::cout << "> i: " << i << ", val: " << val << '\n';
        }

        if(val < worstVal)
        {
            worstVal = val;
        }
    }
    return worstVal;
}

//
int MinMax(Feld player, unsigned int depth, Board& board)
{
    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;

    std::cout << "Take Turn" << '\n';
    double bestVal = -2.0;

    std::vector<int> rngesus(0);

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.Insert(i, player))
            continue;

        double val;
        val = -Max(enemy, depth - 1, board);
        std::cout << "-> i: " << i << ", val: " << val << '\n';

        if(val > bestVal)
        {
            rngesus.clear();
            rngesus.push_back(i);
            bestVal = val;
        }
        else if(val == bestVal)
            rngesus.push_back(i);

        board.Remove(i);
    }

    int move =rngesus[rand() % rngesus.size()];
    std::cout << "Move: " << move << '\n';

    return move;
}

void Test()
{
    srand(time(NULL));
    Board b;
    b.Insert(5, Feld::rot);
    b.Insert(1, Feld::gelb);
    b.Insert(6, Feld::rot);
    b.Insert(2, Feld::gelb);

    int turn = MinMax(Feld::rot, 4, b);
    b.Insert(turn, Feld::rot);
    turn = MinMax(Feld::gelb, 4, b);
    b.Insert(turn, Feld::gelb);
    turn = MinMax(Feld::rot, 4, b);
}

int main()
{
    Test();
    Board board;
    int Zug, Gegenzug;

    // Netzwerkspiel? Rufe NetzwerkMain() auf.
#if 1
   Start(1);

    for(unsigned int Spiel = 1; Spiel <= AnzahlSpiele; Spiel++)
    {
        Gegenzug = 0;
        board.Reset();
        Feld playerCol;
        Feld enemyCol;

        if(Spiel % 2)
        {
            playerCol = Feld::gelb;
            enemyCol = Feld::rot;
        }
        else
        {
            std::cout << Spiel << '\n';
            playerCol = Feld::rot;
            enemyCol = Feld::gelb;
            Gegenzug = NaechsterZug(-1);
            board.Insert(Gegenzug, enemyCol);
            std::cout << "Enemy Plays: "<< Gegenzug << '\n';
        }


        while(Gegenzug != -1)
        {
            Zug = MinMax(playerCol, LookAhead, board);
            if(Zug < 0 || Zug > 6)
              std::cout << "Wat" << '\n';
            Gegenzug = NaechsterZug(Zug);

            if(Gegenzug < 0)
            {
                break;
            }
            std::cout << "Enemy Plays: "<< Gegenzug << '\n';
            board.Insert(Zug, playerCol);
            board.Insert(Gegenzug, enemyCol);

        }
// ...

    }
#endif
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
