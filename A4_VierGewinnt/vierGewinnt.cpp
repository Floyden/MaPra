// ...

#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "minimax.h"

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 5;
const unsigned int LookAhead = 6;


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
    bool WasWinningMove(int x, Feld player) const;

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

bool Board::WasWinningMove(int x, Feld player) const
{
    if(At(x, 0) == Feld::leer)
        return false;
    Board tmp = *this;
    tmp.Remove(x);
    return tmp.IsWinningMove(x, player);

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

int evaluate(Feld player, const Board& board, int last_move = -1)
{
    int score = 0;
    int hash[6] = {0};
    int hash2[7] = {0};
    for (size_t i = 0; i < AnzahlZeilen; i++)
    {
        for(size_t j = 0; j < AnzahlSpalten; j++)
        {
            int val = (int)board.At(j,i);
            hash[i] += val;
            hash2[j] += val;
            hash[i] *= 10;
        }
        hash2[i] *= 10;
        score += eval_map[hash[i]];
    }
    for(size_t j = 0; j < AnzahlSpalten; j++)
        score += eval_map[hash2[j]];

    if(player != Feld::gelb)
        score *= -1;
    return score;
}

// Find best case for the player
double Max(Feld player, int depth, Board& board)
{
    if(depth == 0)
        return evaluate(player, board);

    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;
    int bestVal = -32767;

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.IsFree(i))
            continue;

        double val;
        if(board.IsWinningMove(i, player))
            val = 32767;
        else
        {
            board.Insert(i, player);
            val = -Max(enemy, depth - 1, board);
            board.Remove(i);
        }
        if(val > bestVal)
        {
            bestVal = val;
        }
    }
    return bestVal;
}

//
int MinMax(Feld player, unsigned int depth, Board& board)
{
    Feld enemy = player == Feld::gelb ? Feld::rot : Feld::gelb;

    // std::cout << "Take Turn" << '\n';
    int bestVal = -32767;

    std::vector<int> rngesus(0);

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {

        if(!board.IsFree(i))
            continue;

        int val;

        if(board.IsWinningMove(i, player))
        {
            // std::cout << "kek" << '\n';
            val = 32767;
        }
        else
        {
            board.Insert(i, player);
            val = -Max(enemy, depth - 1, board);
            board.Remove(i);
        }
        // std::cout << "-> i: " << i << ", val: " << val << '\n';

        if(val > bestVal)
        {
            rngesus.clear();
            rngesus.push_back(i);
            bestVal = val;
        }
        else if(val == bestVal)
            rngesus.push_back(i);
    }

    srand(time(NULL));
    int move = rngesus[rand() % rngesus.size()];
    // std::cout << "Move: " << move << '\n';

    return move;
}

void Test()
{
    srand(time(NULL));
    Board b;
    b.Insert(0, Feld::rot);
    b.Insert(0, Feld::rot);
    b.Insert(0, Feld::rot);
    std::cout << b.IsWinningMove(0, Feld::rot) << '\n';
    b.Reset();

    b.Insert(0, Feld::rot);
    b.Insert(1, Feld::rot);
    b.Insert(2, Feld::rot);
    std::cout << b.IsWinningMove(3, Feld::rot) << '\n';
    b.Reset();

    b.Insert(0, Feld::rot);
    b.Insert(1, Feld::gelb);
    b.Insert(1, Feld::rot);
    b.Insert(2, Feld::gelb);
    b.Insert(2, Feld::gelb);
    b.Insert(2, Feld::rot);
    b.Insert(3, Feld::gelb);
    b.Insert(3, Feld::gelb);
    b.Insert(3, Feld::gelb);
    std::cout << b.IsWinningMove(3, Feld::rot) << '\n';
    b.Reset();

    b.Insert(3, Feld::rot);
    b.Insert(2, Feld::gelb);
    b.Insert(2, Feld::rot);
    b.Insert(1, Feld::gelb);
    b.Insert(1, Feld::gelb);
    b.Insert(1, Feld::rot);
    b.Insert(0, Feld::gelb);
    b.Insert(0, Feld::gelb);
    b.Insert(0, Feld::gelb);
    std::cout << b.IsWinningMove(0, Feld::rot) << '\n';
    // b.Insert(5, Feld::rot);
    // b.Insert(1, Feld::gelb);
    // b.Insert(6, Feld::rot);
    // b.Insert(2, Feld::gelb);
    //
    // int turn = MinMax(Feld::rot, 4, b);
    // b.Insert(turn, Feld::rot);
    // turn = MinMax(Feld::gelb, 4, b);
    // b.Insert(turn, Feld::gelb);
    // turn = MinMax(Feld::rot, 4, b);
}

int main()
{
    // Test();
    Board board;
    int Zug, Gegenzug;

#if 0
    // Netzwerkspiel? Rufe NetzwerkMain() auf.
    NetzwerkMain();
#else
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
            // std::cout << "Enemy Plays: "<< Gegenzug << '\n';
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
            // std::cout << "Enemy Plays: "<< Gegenzug << '\n';
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
    Board board;
    Feld enemyColor = MeineFarbe == Feld::gelb ? Feld::rot : Feld::gelb;

    if(MeineFarbe == Feld::gelb)
    {
        int command;
        while (true)
        {
            std::cout << "Eingabe: ";
            std::cin >> command;
            if (!std::cin)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
            else if (command >= 0 && command <= 6)
                break;
        }
        board.Insert(command, enemyColor);
        if(!SendeZug(command))
            return SpielStatus::Verbindungsfehler;
    }

    bool ende = false;
    while(!ende)
    {
        int enemy = EmpfangeZug();
        std::cout << "Empfange: " << enemy << "\n";
        if(enemy == SPIELENDE)
        {
            if(board.IsFull())
                return SpielStatus::Unentschieden;
            else
                return SpielStatus::Sieg;
        }
        if(enemy == VERBINDUNGSFEHLER)
            return SpielStatus::Verbindungsfehler;
        if(enemy < 0 || enemy > AnzahlSpalten)
            return SpielStatus::Verbindungsfehler;

        // this case is already checked for when sending the data, therefore connection issues
        if(!board.IsFree(enemy))
            return SpielStatus::Verbindungsfehler;
        if(board.IsWinningMove(enemy, enemyColor))
            break;

        board.Insert(enemy, enemyColor);
        if(board.IsFull())
        {
            SendeZug(SPIELENDE);
            return SpielStatus::Unentschieden;
        }

        int command;
        while (true)
        {
            std::cout << "Eingabe: ";
            std::cin >> command;
            if (!std::cin)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            }
            else if (command >= 0 && command <= 6 && board.Insert(command, MeineFarbe))
                break;
        }

        if(!SendeZug(command))
            return SpielStatus::Verbindungsfehler;
    }

    SendeZug(SPIELENDE);
    return SpielStatus::Niederlage;
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
