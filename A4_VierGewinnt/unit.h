/*****************************************************************
*  Name       : unit.h                                           *
*  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),     *
*               Vier-Gewinnt mit Minimax-Algorithmus             *
*  Autor      : Juergen Bey, V.R., A.V. IGPM RWTH Aachen         *
*  Datum      : Nov '98, Jan '00                                 *
*  Sprache    : C++                                              *
*****************************************************************/

extern const unsigned int AnzahlZeilen;
extern const unsigned int AnzahlSpalten;
extern const unsigned int AnzahlSpiele;

static const int VERBINDUNGSFEHLER = -2;
static const int SPIELENDE         = -1;

// Schwierigkeitsgrad 0 = Eingabe ueber die Tatstatur
void Start ( unsigned int Schwierigkeitsgrad );

// Mit NaechsterZug uebergeben Sie die Nummer der Spalte (0...AnzahlSpalten-1),
// die Ihr Programm fuer den naechsten Spielstein ausgewaehlt hat.
// Zurueckgegeben wird Ihnen dann der naechste Zug der Praktikumsumgebung
// bzw. von der Tastatur.
int NaechsterZug ( int Spalte );

// Sendet Ihren Zug an den verbundenen Mitspieler. 
// Gueltige Werte sind der Spaltenindex (0..AnzahlSpalten-1) oder die Konstante,
// die signalisiert, dass das Spiel zu Ende ist und Sie keinen Zug mehr machen koennen.
// Gibt true zurueck, wenn das Senden erfolgreich war, ansonsten false.
bool SendeZug( int meinZug );

// Empfaengt einen gesendeten Zug eines verbundenen Mitspielers.
// Die empfangenen Werte koennen potentiell beliebig sein, muessen also auf ihre
// Gueltigkeit geprueft werden. Insbesondere wird bei einem Verbindungsfehler
// die Konstante VERBINDUNGSFEHLER zurueckgegeben.
int EmpfangeZug();




// Interne Methode, die von NetzwerkMain benutzt wird. Oeffnet einen Port an Ihrem Rechner,
// der auf eine eingehende Verbindung wartet und diese dann akzeptiert.
// Gibt true zurueck, sobald eine Verbindung hergestellt wurde und false, falls ein Fehler
// aufgetreten ist (z.B. port bereits in Benutzung)
bool WarteAufVerbindung( unsigned short port );

// Interne Methode, die von NetzwerkMain benutzt wird. Versucht, eine TCP/IP-Verbindung zu
// einem Computer mit der angegebenen IP aufzubauen, der auf dem angegebenen Port auf eine
// Verbindung wartet. Gibt true zurueck, falls die Verbindung erfolgreich hergestellt wurde;
// bei jeglichen Netzwerkfehlern (z.B. ungueltige IP) wird false zurueckgegeben.
bool Verbinde( const char* ip, unsigned short port );

// Interne Methode, die von NetzwerkMain benutzt wird. Schliesst eine bestehende Verbindung
// zu einem anderen Mitspieler.
void SchliesseVerbindung();

// Interne Methode, die von NetzwerkMain benutzt wird. Wird aufgerufen, um ein neues Netzwerkspiel
// zu initialisieren, welches die Farben tauscht und dann erneut Netzwerkspiel( MeineFarbe ) mit
// Ihrer neuen Spielerfarbe aufruft.
void NaechstesNetzwerkspiel();
