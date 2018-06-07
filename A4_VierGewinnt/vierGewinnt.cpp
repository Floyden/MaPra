// ...

#include <cmath>
#include <iostream>
#include <random>
#include "unit.h"
#include "minimax.h"

void NetzwerkMain();


const unsigned int Schwierigkeitsgrad = 4;
const unsigned int LookAhead = 5;


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

double _eval2(Feld col[4])
{
    int hash = col[0];
    for(int i = 1; i < 4; i++)
    {
        hash *= 10;
        hash += (int)col[i];
    }
    double val = eval_map[hash];
    return val;
    // Feld c = Feld::leer;
    // int n = 0;
    // for(int i = 0; i < 4; i++)
    // {
    //     if(col[i] == Feld::leer)
    //         continue;
    //     if(c == Feld::leer)
    //     {
    //         c = col[i];
    //         n++;
    //     }
    //     else
    //     {
    //         if(c == col[i])
    //             n++;
    //         else
    //         {
    //             return 0.0;
    //         }
    //     }
    // }
    //
    // const double Z0 = 0.0;
    // const double Z1 = 0.0;
    // const double Z2 = 0.05;
    // const double Z3 = 1.90;
    // const double Z4 = 1.00;
    //
    // double val = Z0;
    // if(n == 1)
    //     val == Z1;
    // else if(n == 2)
    //     val == Z2;
    // else if(n == 3)
    //     val == Z3;
    // else
    //     val == Z4;
    //
    // if(c == Feld::rot)
    //     val = -val;
    // return val;
}

// positive for yellow
double _eval(int x, int y, const Board& board)
{

    // x x x x | v v v | 0
    // x x x x | v v v | 1
    // x_x_x_x_|_v_v_v_| 2
    // />/>/>/>| 0 0 0 | 3
    // />/>/>/>| 0 0 0 | 4
    // />/>/>/>| 0 0 0 | 5
    double val = 0.0;
    Feld col[4];

    if(x < AnzahlSpalten - 3)
    {
        col[0] = board.At(x,y);
        col[1] = board.At(x + 1,y);
        col[2] = board.At(x + 2,y);
        col[3] = board.At(x + 3,y);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }

    if(y < AnzahlZeilen - 3)
    {
        col[4];
        col[0] = board.At(x,y);
        col[1] = board.At(x,y + 1);
        col[2] = board.At(x,y + 2);
        col[3] = board.At(x,y + 3);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }

    if(x < AnzahlSpalten - 3 && y < AnzahlZeilen - 3)
    {
        col[4];
        col[0] = board.At(x,y);
        col[1] = board.At(x + 1, y + 1);
        col[2] = board.At(x + 2, y + 2);
        col[3] = board.At(x + 3, y + 3);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }

    if(x >= 3 && y < AnzahlZeilen - 3)
    {
        col[4];
        col[0] = board.At(x,y);
        col[1] = board.At(x - 1, y + 1);
        col[2] = board.At(x - 2, y + 2);
        col[3] = board.At(x - 3, y + 3);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }

    if(x < AnzahlSpalten - 3 && y >= 3)
    {
        col[4];
        col[0] = board.At(x,y);
        col[1] = board.At(x + 1, y - 1);
        col[2] = board.At(x + 2, y - 2);
        col[3] = board.At(x + 3, y - 3);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }

    if(x >= 3 && y >= 3)
    {
        col[4];
        col[0] = board.At(x,y);
        col[1] = board.At(x - 1, y - 1);
        col[2] = board.At(x - 2, y - 2);
        col[3] = board.At(x - 3, y - 3);

        double v = _eval2(col);
        if(v == -1.0 || v == 1.0)
        {
            // std::cout << "Hey" << '\n';
            return v;
        }
        val += v;
    }


    if(val < -1.0 || val > 1.0)
    {
        std::cout << "Eval out of bounds, pls fix: " << val << '\n';
    }

    return val;
}

double evaluate(Feld player, const Board& board, int last_move = -1)
{
    double score = 0.0;
    for(int i = 0; i < AnzahlZeilen; i++)
    {
        for (int j = 0; j < AnzahlSpalten; j++)
        {
            double tmp = _eval(j,i, board);
            if(tmp == 1.0 || tmp == -1.0)
            {
                if(player != Feld::gelb)
                    tmp *= -1;
                return tmp;
            }

            score += tmp;
            // if(score < -1.0 || score > 1.0)
            // {
            //     std::cout << "Score out of bounds, pls fix: " << score << '\n';
            // }
        }
    }
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
    double bestVal = -1.0;

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {
        if(!board.IsFree(i))
            continue;

        double val = evaluate(player, board);
        // if(board.IsWinningMove(i, player))
        //     val = 1.0;
        if(val != 1.0 && val != -1.0)
        // else
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
    double bestVal = -1.0;

    std::vector<int> rngesus(0);

    for (size_t i = 0; i < AnzahlSpalten; i++)
    {

        if(!board.IsFree(i))
            continue;

        // double val;

        double val = evaluate(player, board);
        // if(board.IsWinningMove(i, player))
        //     val = 1.0;
        if(val != 1.0 && val != -1.0)
        // else
        {
            board.Insert(i, player);
            val = -Max(enemy, depth - 1, board);
            board.Remove(i);
        }

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

int main()
{
    // Test();
    Board board;
    int Zug, Gegenzug;

#if 0
    // Netzwerkspiel? Rufe NetzwerkMain() auf.
    NetzwerkMain();
#else
   Start(Schwierigkeitsgrad);

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
