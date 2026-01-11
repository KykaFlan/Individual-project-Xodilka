#include <iostream>      // äëÿ cout è cin
#include <conio.h>       // äëÿ _getch()
#include <cstdlib>       // äëÿ rand(), srand(), system() | Äëÿ ðàíäîìàéçåðà
#include <ctime>         // äëÿ time()                    | Äëÿ ðàíäîìàéçåðà
#include <windows.h>     // äëÿ ðàáîòû ñ êîíñîëüþ Windows
#include <vector>        // äëÿ vector
#include <algorithm>     // äëÿ sort()
 
#include "GamePlay.h"
#include "ProverkaInput.h"

using namespace std;

// Ãëîáàëüíûå ïåðåìåííûå
int KolichestvoPlayers = 2;                             // Êîëè÷åñòâî èãðîêîâ
int playerPositions[5] = { 0, 0, 0, 0, 0 };             // Ïîçèöèè âñåõ èãðîêîâ íà ïîëå
int skipTurns[5] = { 0, 0, 0, 0, 0 };                   // Ñ÷åò÷èêè ïðîïóùåííûõ õîäîâ         
bool frozen[5] = { false, false, false, false, false }; // Ôëàãè "çàìîðîçêè" èãðîêîâ
int currentPlayer = 0;                                  // Íîìåð òåêóùåãî èãðîêà, êîòîðûé õîäèò
bool gameOver = false;                                  // Ôëàã çàâåðøåíèÿ èãðû 
char gameMap[50];                                       // Èãðîâîå ïîëå èç 50 êëåòîê
int winnerPlayer = 0;                                   // Íîìåð ïîáåäèâøåãî èãðîêà

// Ðèñóåò ðàçðåç
void DrowRazrez() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=================================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Ïåðåêðàøèâàåò áóêâû äëÿ êàðòû è âûâîäèò èõ
void ColorBukva(char bukva) {
    if (bukva == 'J') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    else if (bukva == 'B') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    else if (bukva == 'P') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    else if (bukva == 'F') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    else if (bukva == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    cout << bukva;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Ïàðàìåòðû êàðòû è çàïóñê ðàíäîìàéçåðà
void InitGame() {
    for (int i = 0; i < 50; i++) {
        gameMap[i] = '.';
    }
    //Êëåòêè ñòàðòà è ôèíèøà
    gameMap[0] = 'S';
    gameMap[49] = 'F';

    //Êëåòêè ëîâóøêè
    gameMap[3] = 'J';
    gameMap[23] = 'J';
    gameMap[35] = 'J';
    gameMap[40] = 'J';
    gameMap[48] = 'J';

    //Êëåòêè áîíóñû
    gameMap[6] = 'B';
    gameMap[14] = 'B';
    gameMap[19] = 'B';
    gameMap[29] = 'B';
    gameMap[32] = 'B';
    gameMap[39] = 'B';
    gameMap[45] = 'B';

    //Êëåòêè ïîðòàëû
    gameMap[22] = 'P';
    gameMap[37] = 'P';

    // Äëÿ ðàíäîìàéçåðà
    srand(time(0));
}

// Âîçâðàùàåò âåêòîð ñ êëåòêàìè, ëèáî îíà ïóñòàÿ è òîãäà äëèííà 0, ëèáî íîìåðàìè èãðîêîâ, êîòîðûå ñòîÿò íà çàäàííîé êëåòêå. 
vector<int> GetPlayersOnCell(int ProverkKletka) {
    vector<int> players; // Ñîçäàåì ïóñòîé âåêòîð äëÿ õðàíåíèÿ íîìåðîâ èãðîêîâ
    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (playerPositions[i] == ProverkKletka) {
            players.push_back(i + 1);
        }
    }
    return players; //Âîçâðàùàåò âåêòîð ñ íîìåðàìè èãðîêîâ
}

// Âîçâðàùàòàåò çíà÷åíèå 
int FindTeleportDestination(int pos) {
    if (pos == 22) return 37;
    if (pos == 37) return 22;
    return pos;
}

// Âûáîð êàêîãî èãðîêà çàìîðîçèòü
int ChoosePlayerToFreeze(int currentPlayer) {
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "   ÂÛÁÅÐÈÒÅ ÈÃÐÎÊÀ ÄËß ÇÀÌÎÐÎÇÊÈ   \n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "Äîñòóïíûå èãðîêè:\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    // Ïîêàçûâåò èãðîêîâ êîòîðûõ çàìîðîçèòü
    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (i != currentPlayer) {
            cout << "  ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            cout << "Èãðîê " << (i + 1);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            cout << " - êëåòêà ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            cout << playerPositions[i];

            if (skipTurns[i] > 0) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << " (ïðîïóñê: " << skipTurns[i] << ")";
            }
            if (frozen[i]) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                cout << " (çàìîðîæåí)";
            }
            cout << "\n";
        }
    }
    DrowRazrez();
    int choice;
    // Âûáîð èãðîêà ñ ââîäîì
    while (true) {

        // Ââîä ñ ïðîâåðêîé ñ ó÷¸òîì êîëè÷åñòâà ó÷àñòèíêîâ
        choice = GetNumberInput(1, KolichestvoPlayers);

        if (choice == currentPlayer + 1) {
            cout << "Íåëüçÿ âûáðàòü ñàìîãî ñåáÿ!\n";
        }
        else if (choice < 1 || choice > KolichestvoPlayers) {
            cout << "Òàêîãî èãðîêà íåò!\n";
        }
        else {
            break;
        }
    }
    return choice - 1;
}

// Âûâîäèò èíôó íàä êàðòîé
void ShowGameState() {
    system("cls");
    DrowRazrez();
    cout << "         È Ã Ð À     Õ Î Ä È Ë Ê À          \n";
    DrowRazrez();
    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "Èãðîêîâ: " << KolichestvoPlayers;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "   |   Òåêóùèé: " << (currentPlayer + 1) << "\n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "  Ïîçèöèè èãðîêîâ:\n";
    for (int i = 0; i < KolichestvoPlayers; i++) {
        cout << "  ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "Èãðîê " << (i + 1) <<":";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << "êëåòêà " << playerPositions[i];
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        if (skipTurns[i] > 0) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << " [ïðîïóñê: " << skipTurns[i] << "]";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        if (frozen[i]) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
            cout << " [çàìîðîæåí]";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        if (i == currentPlayer) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << " <-- ÕÎÄÈÒ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout << "\n";
    }
    DrowRazrez();
}

// Âûâîäèò êàðòó â êîíñîëè
void ShowMapWithPlayers() {
    const int N = 50;
    vector<int> playersOnCell[N]; // Äëÿ êàæäîé êëåòêè - ñïèñîê èãðîêîâ íà íåé

    for (int i = 0; i < N; i++) {
        playersOnCell[i] = GetPlayersOnCell(i); 
    }
    //ïîëó÷ëè âåêòîð, ãäå êòî íàõîèäòñÿ

    for (int i = 0; i < 5; i++) { // ðÿäû êàðòû
        cout << "  ";
        if (i % 2 == 0) {
            for (int j = 0; j < 10; j++) {
                int NumberKletka = i * 10 + j; // Íîìåð êëåòêè
                char bukva = gameMap[NumberKletka];                 // Áóêâà êëåòêè (S, F, B, J, P, .)
                vector<int> players = playersOnCell[NumberKletka]; // Èãðîêè íà ýòîé êëåòêå
                 cout << "[";
                if (!players.empty()) { // Åñëè âåêòîð players ÍÅ ïóñòîé, ò.å. ñòîèò íà ýòîé êëåòêå
                    sort(players.begin(), players.end()); 
                    ColorBukva(bukva); // Ïåðåêðàøèâàåò áóêâû äëÿ êàðòû è âûâîäèò èõ
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    for (int playerNum : players) { // range-based for loop
                        cout << playerNum;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    ColorBukva(bukva); // Ïåðåêðàøèâàåò áóêâû äëÿ êàðòû è âûâîäèò èõ
                }
                cout << "]";
                if (j != 9 ) {
                    cout << " -> ";
                }
            }
        }
        else {
            for (int j = 9; j >= 0; j--) {
                int index = i * 10 + j;
                char bukva = gameMap[index];
                vector<int> players = playersOnCell[index];
                cout << "[";
                if (!players.empty()) {
                    sort(players.begin(), players.end());
                    ColorBukva(bukva); // Ïåðåêðàøèâàåò áóêâû äëÿ êàðòû è âûâîäèò èõ
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    ColorBukva(bukva); // Ïåðåêðàøèâàåò áóêâû äëÿ êàðòû è âûâîäèò èõ
                }
                cout << "]";
                if (j != 0) {
                    cout << " <- ";
                }
            }
        }
        cout << "\n\n";
    }
    DrowRazrez();
    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); cout << "S"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ñòàðò  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); cout << "F"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ôèíèø  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); cout << "B"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-áîíóñ  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); cout << "J"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ëîâóøêà  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); cout << "P"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ïîðòàë\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Áðîñîê êóáèêà
int BrosokCube() {
    return rand() % 6 + 1;
}

// Õîä ñ êàêîé êëåòêè íà ñëåäóþùèé (è ïåðåìåííûå ìåíÿåò è âûâîäèò õîä)
void MovePlayer(int currentPlayer, int chislo) {
    int pamyt = playerPositions[currentPlayer];
    playerPositions[currentPlayer] += chislo;

    if (playerPositions[currentPlayer] > 49) {
        playerPositions[currentPlayer] = 49;
    }

    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "Èãðîê " << (currentPlayer + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << " äâèãàåòñÿ: " << pamyt << " -> ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << playerPositions[currentPlayer];
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << "\n";
}

// Ïðîâåðêà íà ôèíèø
bool CheckWin(int currentPlayer) {
    if (playerPositions[currentPlayer] >= 49) {
        winnerPlayer = currentPlayer + 1;
        gameOver = true;
        return true;
    }
    return false;
}

// Âûâîä âîïðîñà è ââîä ÷åðåç ïðîâåðêó GetNumberInput
bool AskBonusQuestion() {
    const char* questions[] = {
        "×åìó ðàâíî ÷èñëî Ïè ñ òî÷íîñòüþ äî äâóõ çíàêîâ ïîñëå çàïÿòîé?\n1) 3.14  2) 3.16  3) 3.18",
        "Ñêîëüêî õðîìîñîì ó ÷åëîâåêà?\n1) 44  2) 46  3) 48",
        "Êòî íàïèñàë 'Ìàñòåð è Ìàðãàðèòà'?\n1) Áóëãàêîâ  2) Äîñòîåâñêèé  3) Òîëñòîé",
        "Ñêîëüêî áóäåò 7! (ôàêòîðèàë)?\n1) 5040  2) 720  3) 40320",
        "Êàêîé õèìè÷åñêèé ýëåìåíò èìååò ñèìâîë 'Fe'?\n1) Ôòîð  2) Æåëåçî  3) Ôåðìèé",
        "Êàêîé ÿçûê ÿâëÿåòñÿ ðîäíûì äëÿ êîìïüþòåðà?\n1) Java  2) Ìàøèííûé êîä  3) Python",
        "Ãîä îñíîâàíèÿ Ìîñêâû?\n1) 1147  2) 1247  3) 1347",
        "Ñêîëüêî ïëàíåò â Ñîëíå÷íîé ñèñòåìå?\n1) 8  2) 9  3) 10",
        "Êòî îòêðûë çàêîí âñåìèðíîãî òÿãîòåíèÿ?\n1) Íüþòîí  2) Ýéíøòåéí  3) Ãàëèëåé",
        "Ñêîëüêî áèò â îäíîì áàéòå?\n1) 8  2) 16  3) 32"
    };
    
    const int answers[] = { 1, 2, 1, 1, 2, 2, 1, 1, 1, 1 };
    const char* feedback[] = {
        "Ïè ? 3.14", "46 õðîìîñîì", "Áóëãàêîâ", "7! = 5040", "Fe - Æåëåçî",
        "Ìàøèííûé êîä", "1147 ãîä", "8 ïëàíåò", "Íüþòîí", "8 áèò = 1 áàéò"
    };

    int qIndex = rand() % 10;

    // Âûâîä âîïðîñà
    cout << "\n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "               ÁÎÍÓÑÍÛÉ ÂÎÏÐÎÑ        \n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

    cout << questions[qIndex] << ": ";
    int userAnswer = GetNumberInput(1, 3);        
    DrowRazrez();
    // Ïðîâåðêà îòâåòà
    bool isCorrect = (userAnswer == answers[qIndex]);
    
    if (isCorrect) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        cout << "Âåðíî! " << feedback[qIndex] << "\n";
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Íåâåðíî. Ïðàâèëüíûé îòâåò: " << feedback[qIndex] << "\n";
    }

    return isCorrect;
}

// Âçîèìîäåéñòâèå ñ êëåòêîé (Òï, Ïðîïóñê, Ôðèç)
void ProcessCell(int currentPlayer) {

    int pos = playerPositions[currentPlayer];
    char bukva = gameMap[pos];
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "         ÝÔÔÅÊÒ ÊËÅÒÊÈ " << pos << "\n";
    DrowRazrez();

    int Color = 7;

    if (bukva == 'J') Color = 12;
    else if (bukva == 'B') Color = 11;
    else if (bukva == 'P') Color = 13;
    else if (bukva == 'F') Color = 10;
    else if (bukva == 'S') Color = 9;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);

    if (bukva == '.') {
        cout << "  Îáû÷íàÿ êëåòêà. Íè÷åãî íå ïðîèñõîäèò.\n";
    }
    else if (bukva == 'J') {
        cout << "  ËÎÂÓØÊÀ! Ïðîïóñê 1 õîäà.\n";
        skipTurns[currentPlayer]++;
    }
    else if (bukva == 'B') {
        cout << "  ÁÎÍÓÑ! Îòâåòüòå íà âîïðîñ.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        if (AskBonusQuestion()) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << "\n  ÏÐÀÂÈËÜÍÎ! Âû ïîëó÷àåòå íàãðàäó!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            if (rand() % 2 == 0) {
                cout << "  Íàãðàäà: +2 øàãà âïåðåä!\n";
                MovePlayer(currentPlayer, 2);
            }
            else {
                cout << "  Íàãðàäà: Çàìîðîçêà äðóãîãî èãðîêà!\n";
                int playerToFreeze = ChoosePlayerToFreeze(currentPlayer);                              
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                cout << "  Èãðîê " << (playerToFreeze + 1) << " çàìîðîæåí íà 1 õîä!\n";
                frozen[playerToFreeze] = true;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);                
            }
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << "\n  ÍÅÏÐÀÂÈËÜÍÎ! Âû ëèøàåòåñü áîíóñà.\n";
        }
    }
    else if (bukva == 'P') {
        cout << "  ÒÅËÅÏÎÐÒ! Ìãíîâåííîå ïåðåìåùåíèå.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        int MovePortal = FindTeleportDestination(pos);
        int oldPos = playerPositions[currentPlayer];

        
        cout << "  Ïåðåìåùåíèå: " << oldPos << " -> " << MovePortal << "\n";
        playerPositions[currentPlayer] = MovePortal;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << "  Òåëåïîðòû ñâÿçàíû: " << oldPos << " ? " << MovePortal << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);        
    }
    else if (bukva == 'F') {
        cout << "  ÔÈÍÈØ! Âû äîñòèãëè êîíå÷íîé êëåòêè!\n";
    }
    DrowRazrez();
}

//Ïðîöåññ õîäà
void ProcessTurn() {
    if (skipTurns[currentPlayer] > 0) {
        cout << "\n";
        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "  Èãðîê " << (currentPlayer + 1) << " ïðîïóñêàåò õîä!\n";
        cout << "  Îñòàëîñü ïðîïóñêîâ: " << skipTurns[currentPlayer] << "\n";
        DrowRazrez();
        skipTurns[currentPlayer]--;
        return;
    }

    if (frozen[currentPlayer]) {
        cout << "\n";
        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        cout << "  Èãðîê " << (currentPlayer + 1) << " çàìîðîæåí!\n";
        cout << "  Ïðîïóñê ýòîãî õîäà.\n";
        DrowRazrez();
        frozen[currentPlayer] = false;
        return;
    }
    cout << "\n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "               ÁÐÎÑÎÊ ÊÓÁÈÊÀ\n";
    DrowRazrez();

    cout << "  Èãðîê ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << (currentPlayer + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << ", íàæìèòå ëþáóþ êëàâèøó...";
    _getch();

    int chislo = BrosokCube();
    cout << "\n  Âûïàëî: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "[" << chislo << "]\n";
    DrowRazrez();

    MovePlayer(currentPlayer, chislo);

    if (CheckWin(currentPlayer)) {
        cout << "\n";
        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "  ÏÎÁÅÄÈÒÅËÜ: ÈÃÐÎÊ " << (currentPlayer + 1) << "!\n";
        DrowRazrez();
        return;
    }

    // Îáðàáàòûâàåì êëåòêó òîëüêî åñëè èãðîê åùå íå âûèãðàë
    ProcessCell(currentPlayer);    
}

// Âñå âûçîâû èãðû
void StartGame() {

    InitGame();
    int NumberXoda = 1;

    while (!gameOver) {
        ShowGameState();

        ShowMapWithPlayers();

        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        cout << "                Õ Î Ä   ¹ " << NumberXoda << "           \n";
        DrowRazrez();

        ProcessTurn();

        if (gameOver) {
            // Ïàóçà ïåðåä ïîêàçîì ôèíàëüíîé êàðòû
            cout << "\nÍàæìèòå ëþáóþ êëàâèøó äëÿ ïðîñìîòðà ôèíàëüíîé êàðòû...";
            _getch();

            // Ïîêàçûâàåì ôèíàëüíóþ êàðòó ïåðåä çàâåðøåíèåì
            system("cls");

            DrowRazrez();
            cout << "       Ô È Í À Ë Ü Í À ß   Ê À Ð Ò À       \n";
            DrowRazrez();

            ShowMapWithPlayers();

            break;
        }

        // Ïåðåäàþò õîäæ ñëåäóþùåìó èãðîêó
        currentPlayer = (currentPlayer + 1) % KolichestvoPlayers;
        NumberXoda++;

        cout << "\nÍàæìèòå ëþáóþ êëàâèøó äëÿ ïðîäîëæåíèÿ...";
        _getch();
    }
}

// Ïîêàçûâàåò ïîáåäèòåëÿ
int GetWinner() {
    return winnerPlayer;

}

