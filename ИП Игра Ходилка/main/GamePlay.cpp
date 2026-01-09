#include <iostream>      // для cout и cin
#include <conio.h>       // для _getch()
#include <cstdlib>       // для rand(), srand(), system()
#include <ctime>         // для time()
#include <windows.h>     // для работы с консолью Windows
#include <vector>        // для vector
#include <algorithm>     // для sort()

#include "GamePlay.h"
#include "ProverkaInput.h"

using namespace std;

// Глобальные переменные
int KolichestvoPlayers = 2;                             // Количество игроков
int playerPositions[5] = { 0, 0, 0, 0, 0 };             // Позиции всех игроков на поле
int skipTurns[5] = { 0, 0, 0, 0, 0 };                   // Счетчики пропущенных ходов         
bool frozen[5] = { false, false, false, false, false }; // Флаги "заморозки" игроков
int currentPlayer = 0;                                  // Номер текущего игрока, который ходит
bool gameOver = false;                                  // Флаг завершения игры 
char gameMap[50];                                       // Игровое поле из 50 клеток
int winnerPlayer = 0;                                   // Номер победившего игрока

// Рисует разделение (интерфейс)
void DrowRazrez() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "================================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Возвращает вектор с номерами игроков, которые стоят на заданной клетке.
vector<int> GetPlayersOnCell(int ProverkKletka) {
    vector<int> players; // Создаем пустой вектор для хранения номеров игроков
    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (playerPositions[i] == ProverkKletka) {
            players.push_back(i + 1);
        }
    }
    return players; //Возвращает вектор с номерами игроков
}

// Бросок кубика
int BrosokCube() {
    return rand() % 6 + 1;
}

// Ход с какой клетки на следующий (и переменные меняет и выводит ход)
void MovePlayer(int currentPlayer, int chislo) {
    int pamyt = playerPositions[currentPlayer];
    playerPositions[currentPlayer] += chislo;

    if (playerPositions[currentPlayer] > 49) {
        playerPositions[currentPlayer] = 49;
    }

    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "Игрок " << (currentPlayer + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << " двигается: " << pamyt << " -> ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << playerPositions[currentPlayer];
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Проверка на финиш
bool CheckWin(int currentPlayer) {
    if (playerPositions[currentPlayer] >= 49) {
        winnerPlayer = currentPlayer + 1;
        gameOver = true;
        return true;
    }
    return false;
}

// Вывод вопроса и ввод через проверку GetNumberInput
bool AskBonusQuestion() {
    const char* questions[] = {
        "Чему равно число Пи с точностью до двух знаков после запятой?\n1) 3.14  2) 3.16  3) 3.18",
        "Сколько хромосом у человека?\n1) 44  2) 46  3) 48",
        "Кто написал 'Мастер и Маргарита'?\n1) Булгаков  2) Достоевский  3) Толстой",
        "Сколько будет 7! (факториал)?\n1) 5040  2) 720  3) 40320",
        "Какой химический элемент имеет символ 'Fe'?\n1) Фтор  2) Железо  3) Фермий",
        "Какой язык является родным для компьютера?\n1) Java  2) Машинный код  3) Python",
        "Год основания Москвы?\n1) 1147  2) 1247  3) 1347",
        "Сколько планет в Солнечной системе?\n1) 8  2) 9  3) 10",
        "Кто открыл закон всемирного тяготения?\n1) Ньютон  2) Эйнштейн  3) Галилей",
        "Сколько бит в одном байте?\n1) 8  2) 16  3) 32"
    };
    
    const int answers[] = {1, 2, 1, 1, 2, 2, 1, 1, 1, 1};
    const char* feedback[] = {
        "Пи ≈ 3.14", "46 хромосом", "Булгаков", "7! = 5040", "Fe - Железо",
        "Машинный код", "1147 год", "8 планет", "Ньютон", "8 бит = 1 байт"
    };
    
    int qIndex = rand() % 10;
    
    cout << "\n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "        БОНУСНЫЙ ВОПРОС        \n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    
    cout << questions[qIndex] << ": ";
    int userAnswer = GetNumberInput(1, 3);
    
    DrowRazrez();
    bool isCorrect = (userAnswer == answers[qIndex]);
    
    if (isCorrect) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        cout << "Верно! " << feedback[qIndex] << "\n";
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Неверно. Правильный ответ: " << feedback[qIndex] << "\n";
    }
    
    return isCorrect;
}

// Выбор какого игрока заморозить
int ChoosePlayerToFreeze(int currentPlayer) {
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "   ВЫБЕРИТЕ ИГРОКА ДЛЯ ЗАМОРОЗКИ   \n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "Доступные игроки:\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    for (int i = 0; i < KolichestvoPlayers; i++) {
        if (i != currentPlayer) {
            cout << "  ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            cout << "Игрок " << (i + 1);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            cout << " - клетка ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            cout << playerPositions[i];

            if (skipTurns[i] > 0) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << " (пропуск: " << skipTurns[i] << ")";
            }
            if (frozen[i]) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                cout << " (заморожен)";
            }
            cout << "\n";
        }
    }
    DrowRazrez();
    int choice;
    while (true) {
        choice = GetNumberInput(1, KolichestvoPlayers);
        if (choice == currentPlayer + 1) {
            cout << "Нельзя выбрать самого себя!\n";
        }
        else if (choice < 1 || choice > KolichestvoPlayers) {
            cout << "Такого игрока нет!\n";
        }
        else {
            break;
        }
    }
    return choice - 1;
}

// Возвращатает значение 
int FindTeleportDestination(int pos) {
    if (pos == 22) return 37;
    if (pos == 37) return 22;
    return pos;
}

// Взоимодействие с клеткой (Тп, Пропуск, Фриз)
void ProcessCell(int currentPlayer) {

    int pos = playerPositions[currentPlayer];
    char bukva = gameMap[pos];

    cout << "\n";
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "               ЭФФЕКТ КЛЕТКИ " << pos << "\n";
    DrowRazrez();

    int Color = 7;

    if (bukva == 'J') Color = 12;
    else if (bukva == 'B') Color = 11;
    else if (bukva == 'P') Color = 13;
    else if (bukva == 'F') Color = 10;
    else if (bukva == 'S') Color = 9;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);

    if (bukva == '.') {
        cout << "  Обычная клетка. Ничего не происходит.\n";
    }
    else if (bukva == 'J') {
        cout << "  ЛОВУШКА! Пропуск 1 хода.\n";
        skipTurns[currentPlayer]++;
    }
    else if (bukva == 'B') {
        cout << "  БОНУС! Ответьте на вопрос.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        if (AskBonusQuestion()) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << "\n  ПРАВИЛЬНО! Вы получаете награду!\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            if (rand() % 2 == 0) {
                cout << "  Награда: +2 шага вперед!\n";
                MovePlayer(currentPlayer, 2);
            }
            else {
                cout << "  Награда: Заморозка другого игрока!\n";
                int playerToFreeze = ChoosePlayerToFreeze(currentPlayer);                              
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                cout << "  Игрок " << (playerToFreeze + 1) << " заморожен на 1 ход!\n";
                frozen[playerToFreeze] = true;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);                
            }
        }
        else {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << "\n  НЕПРАВИЛЬНО! Вы лишаетесь бонуса.\n";
        }
    }
    else if (bukva == 'P') {
        cout << "  ТЕЛЕПОРТ! Мгновенное перемещение.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        int MovePortal = FindTeleportDestination(pos);
        int oldPos = playerPositions[currentPlayer];

        
        cout << "  Перемещение: " << oldPos << " -> " << MovePortal << "\n";
        playerPositions[currentPlayer] = MovePortal;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << "  Телепорты связаны: " << oldPos << " ? " << MovePortal << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        char newBukva = gameMap[MovePortal];        
    }
    else if (bukva == 'F') {
        cout << "  ФИНИШ! Вы достигли конечной клетки!\n";
    }
    else if (bukva == 'S') {
        cout << "  СТАРТ. Начальная позиция.\n";
    }
    DrowRazrez();
}

// Делаю поля карты
void InitGame() {
    for (int i = 0; i < 50; i++) {
        gameMap[i] = '.';
    }

    gameMap[0] = 'S';
    gameMap[49] = 'F';

    gameMap[3] = 'J';
    gameMap[23] = 'J';
    gameMap[35] = 'J';
    gameMap[40] = 'J';
    gameMap[48] = 'J';

    gameMap[6] = 'B';
    gameMap[14] = 'B';
    gameMap[19] = 'B';
    gameMap[29] = 'B';
    gameMap[32] = 'B';
    gameMap[39] = 'B';
    gameMap[45] = 'B';

    gameMap[22] = 'P';
    gameMap[37] = 'P';

    gameMap[8] = '.';
    gameMap[42] = '.';


    for (int i = 0; i < 5; i++) {
        playerPositions[i] = 0;
        skipTurns[i] = 0;
        frozen[i] = false;
    }
    currentPlayer = 0;
    gameOver = false;
    winnerPlayer = 0;

    srand(time(0));
}

// Выводит инфу над картой
void ShowGameState() {
    system("cls");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "===============================================\n";
    cout << "           И Г Р А     Х О Д И Л К А          \n";
    cout << "===============================================\n";
    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); 
    cout << "Игроков: " << KolichestvoPlayers;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "   |   Текущий: " << (currentPlayer + 1) << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    DrowRazrez();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "  Позиции игроков:\n";

    for (int i = 0; i < KolichestvoPlayers; i++) {
        cout << "  ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "Игрок " << (i + 1) <<": ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << "клетка " << playerPositions[i];

        if (skipTurns[i] > 0) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << " [пропуск: " << skipTurns[i] << "]";
        }
        if (frozen[i]) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); 
            cout << " [заморожен]";
        }
        if (i == currentPlayer) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << " <-- ХОДИТ";
        }
        cout << "\n";
    }
    DrowRazrez();
}

// Выводит карту в консоли
void ShowMapWithPlayers() {
    const int N = 50;
    vector<int> playersOnCell[N]; // Для каждой клетки - список игроков на ней
    for (int i = 0; i < N; i++) {
        playersOnCell[i] = GetPlayersOnCell(i); 
    }
    //получли вектор, где кто нахоидтся

    for (int i = 0; i < 5; i++) { // ряды карты
        cout << "  ";
        if (i % 2 == 0) {
            for (int j = 0; j < 10; j++) {
                int NumberKletka = i * 10 + j; // Номер клетки
                char bukva = gameMap[NumberKletka];                 // Буква клетки (S, F, B, J, P, .)
                vector<int> players = playersOnCell[NumberKletka]; // Игроки на этой клетке
                 cout << "[";
                if (!players.empty()) { // Если вектор players НЕ пустой, т.е. стоит на этой клетке
                    sort(players.begin(), players.end()); 
                    if (bukva == 'J') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    else if (bukva == 'B') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    else if (bukva == 'P') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                    else if (bukva == 'F') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    else if (bukva == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    cout << bukva;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    for (int playerNum : players) { // range-based for loop
                        cout << playerNum;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    if (bukva == 'J') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    else if (bukva == 'B') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    else if (bukva == 'P') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                    else if (bukva == 'F') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    else if (bukva == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);

                    cout << bukva;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
                    if (bukva == 'J') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    else if (bukva == 'B') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    else if (bukva == 'P') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                    else if (bukva == 'F') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    else if (bukva == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    cout << bukva;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    for (int playerNum : players) {
                        cout << playerNum;
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                }
                else {
                    if (bukva == 'J') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    else if (bukva == 'B') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                    else if (bukva == 'P') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                    else if (bukva == 'F') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    else if (bukva == 'S') SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    cout << bukva;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); 
    cout << "S"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-старт  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); cout << "F"; 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-финиш  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); cout << "B"; 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-бонус  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); cout << "J"; 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ловушка  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); cout << "P"; 
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-портал\n";
}

// Номер хода
void ShowNumberXoda(int NumberXoda) {
    DrowRazrez();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "                 Х О Д   № " << NumberXoda << "           \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    DrowRazrez();
}

//Процесс хода
void ProcessTurn() {
    if (skipTurns[currentPlayer] > 0) {
        cout << "\n";
        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "  Игрок " << (currentPlayer + 1) << " пропускает ход!\n";
        cout << "  Осталось пропусков: " << skipTurns[currentPlayer] << "\n";
        DrowRazrez();
        skipTurns[currentPlayer]--;
        return; // Выходим из функции - ход завершен
    }

    if (frozen[currentPlayer]) {
        cout << "\n";
        DrowRazrez();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        cout << "  Игрок " << (currentPlayer + 1) << " заморожен!\n";
        cout << "  Пропуск этого хода.\n";
        DrowRazrez();
        frozen[currentPlayer] = false;
        return; // Выходим из функции - ход завершен
    }
    cout << "\n";
    DrowRazrez();
    cout << "               БРОСОК КУБИКА\n";
    DrowRazrez();

    cout << "  Игрок ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << (currentPlayer + 1);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << ", нажмите любую клавишу...";
    _getch();

    int chislo = BrosokCube();
    cout << "\n  Выпало: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "[" << chislo << "]\n";
    DrowRazrez();

    MovePlayer(currentPlayer, chislo);

    if (CheckWin(currentPlayer)) {
        cout << "\n";
        DrowRazrez();
        cout << "  ПОБЕДИТЕЛЬ: ИГРОК " << (currentPlayer + 1) << "!\n";
        DrowRazrez();
        return;
    }

    // Обрабатываем клетку только если игрок еще не выиграл
    if (!gameOver) {
        ProcessCell(currentPlayer);
    }
}

// Меняет currentPlayer на следующего игрока
void NextPlayer() {
    currentPlayer = (currentPlayer + 1) % KolichestvoPlayers;
}

// Все вызовы игры
void StartGame() {

    InitGame();

    int NumberXoda = 1;

    while (!gameOver) {

        ShowGameState();

        ShowMapWithPlayers();

        ShowNumberXoda(NumberXoda);

        ProcessTurn();

        if (gameOver) {
            // Пауза перед показом финальной карты
            cout << "\nНажмите любую клавишу для просмотра финальной карты...";
            _getch();

            // Показываем финальную карту перед завершением
            system("cls");

            DrowRazrez();
            cout << "       Ф И Н А Л Ь Н А Я   К А Р Т А       \n";
            DrowRazrez();

            ShowMapWithPlayers();

            DrowRazrez();
            break;
        }

        NextPlayer();
        NumberXoda++;

        cout << "\nНажмите любую клавишу для продолжения...";
        _getch();
    }
}

int GetWinner() {
    return winnerPlayer;
}
