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

// Инициализация игры
// Инициализация игры
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

// Возвращатает значение 
int FindTeleportDestination(int pos) {
    // Портал 22 <-> 37
    if (pos == 22) return 37;
    if (pos == 37) return 22;
    return pos;
}

// Выбор какого игрока заморозить
int ChoosePlayerToFreeze(int currentPlayer) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "\n=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "   ВЫБЕРИТЕ ИГРОКА ДЛЯ ЗАМОРОЗКИ   \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
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

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "---------------------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

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

// Выводит инфу над картой
void ShowGameState() {
    system("cls");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);// жёлтый
    cout << "=============================================\n";
    cout << "         И Г Р А     Х О Д И Л К А          \n";
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // голубой
    cout << "Игроков: " << KolichestvoPlayers;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // зелёный
    cout << "   |   Текущий: " << (currentPlayer + 1) << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); 
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "  Позиции игроков:\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    for (int i = 0; i < KolichestvoPlayers; i++) {
        cout << "  ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "Игрок " << (i + 1) <<":";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); // розовый
        cout << "клетка " << playerPositions[i];
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        if (skipTurns[i] > 0) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // красный
            cout << " [пропуск: " << skipTurns[i] << "]";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        if (frozen[i]) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); //синий
            cout << " [заморожен]";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }

        if (i == currentPlayer) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << " <-- ХОДИТ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }
        cout << "\n";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << "  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9); cout << "S"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-старт  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); cout << "F"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-финиш  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); cout << "B"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-бонус  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); cout << "J"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-ловушка  ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); cout << "P"; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << "-портал\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
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
    cout << " двигается: ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    cout << pamyt;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << " -> ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << playerPositions[currentPlayer];
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    cout << "\n";
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
    int numerQuestion1 = rand() % 10;
    int otvetInput = 0;
    cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "        БОНУСНЫЙ ВОПРОС        \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);

    switch (numerQuestion1) {
    case 0:
        cout << "Чему равно число Пи с точностью до двух знаков после запятой?\n";
        cout << "1) 3.14  2) 3.16  3) 3.18: ";
        break;
    case 1:
        cout << "Сколько хромосом у человека?\n";
        cout << "1) 44  2) 46  3) 48: ";
        break;
    case 2:
        cout << "Кто написал 'Мастер и Маргарита'?\n";
        cout << "1) Булгаков  2) Достоевский  3) Толстой: ";
        break;
    case 3:
        cout << "Сколько будет 7! (факториал)?\n";
        cout << "1) 5040  2) 720  3) 40320: ";
        break;
    case 4:
        cout << "Какой химический элемент имеет символ 'Fe'?\n";
        cout << "1) Фтор  2) Железо  3) Фермий: ";
        break;
    case 5:
        cout << "Какой язык является родным для компьютера?\n";
        cout << "1) Java  2) Машинный код  3) Python: ";
        break;
    case 6:
        cout << "Год основания Москвы?\n";
        cout << "1) 1147  2) 1247  3) 1347: ";
        break;
    case 7:
        cout << "Сколько планет в Солнечной системе?\n";
        cout << "1) 8  2) 9  3) 10: ";
        break;
    case 8:
        cout << "Кто открыл закон всемирного тяготения?\n";
        cout << "1) Ньютон  2) Эйнштейн  3) Галилей: ";
        break;
    case 9:
        cout << "Сколько бит в одном байте?\n";
        cout << "1) 8  2) 16  3) 32: ";
        break;
    }

    otvetInput = GetNumberInput(1, 3);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "---------------------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    switch (numerQuestion1) {
    case 0:
        if (otvetInput == 1) {
            cout << "Верно! Пи ? 3.14\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: 3.14\n";
            return false;
        }
    case 1:
        if (otvetInput == 2) {
            cout << "Верно! 46 хромосом\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: 46\n";
            return false;
        }
    case 2:
        if (otvetInput == 1) {
            cout << "Верно! Михаил Булгаков\n";
            return true;
        }
        else {
            cout << "Неверно. Правильный ответ: Булгаков\n";
            return false;
        }
    case 3:
        if (otvetInput == 1) {
            cout << "Верно! 7! = 5040\n";
            return true;
        }
        else {
            cout << "Неверно. 7! = 5040\n";
            return false;
        }
    case 4:
        if (otvetInput == 2) {
            cout << "Верно! Fe - Железо\n";
            return true;
        }
        else {
            cout << "Неверно. Fe - Железо (Ferrum)\n";
            return false;
        }
    case 5:
        if (otvetInput == 2) {
            cout << "Верно! Машинный код\n";
            return true;
        }
        else {
            cout << "Неверно. Машинный код (бинарный)\n";
            return false;
        }
    case 6:
        if (otvetInput == 1) {
            cout << "Верно! 1147 год\n";
            return true;
        }
        else {
            cout << "Неверно. Москва основана в 1147\n";
            return false;
        }
    case 7:
        if (otvetInput == 1) {
            cout << "Верно! 8 планет\n";
            return true;
        }
        else {
            cout << "Неверно. 8 планет (Плутон - карликовая)\n";
            return false;
        }
    case 8:
        if (otvetInput == 1) {
            cout << "Верно! Исаак Ньютон\n";
            return true;
        }
        else {
            cout << "Неверно. Исаак Ньютон\n";
            return false;
        }
    case 9:
        if (otvetInput == 1) {
            cout << "Верно! 8 бит = 1 байт\n";
            return true;
        }
        else {
            cout << "Неверно. 8 бит = 1 байт\n";
            return false;
        }
    default:
        return false;
    }
}

// Взоимодействие с клеткой (Тп, Пропуск, Фриз)
void ProcessCell(int currentPlayer) {

    int pos = playerPositions[currentPlayer];
    char bukva = gameMap[pos];

    cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "----------------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
    cout << "         ЭФФЕКТ КЛЕТКИ " << pos << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "----------------------------------------\n";

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

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "----------------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

// Меняет currentPlayer на следующего игрока
void NextPlayer() {
    currentPlayer = (currentPlayer + 1) % KolichestvoPlayers;
}

// Номер хода
void ShowNumberXoda(int NumberXoda) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "           Х О Д   № " << NumberXoda << "           \n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "=============================================\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

//Процесс хода
void ProcessTurn() {
    if (skipTurns[currentPlayer] > 0) {
        cout << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "  Игрок " << (currentPlayer + 1) << " пропускает ход!\n";
        cout << "  Осталось пропусков: " << skipTurns[currentPlayer] << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        skipTurns[currentPlayer]--;
        return; // Выходим из функции - ход завершен
    }

    if (frozen[currentPlayer]) {
        cout << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        cout << "  Игрок " << (currentPlayer + 1) << " заморожен!\n";
        cout << "  Пропуск этого хода.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        frozen[currentPlayer] = false;
        return; // Выходим из функции - ход завершен
    }
    cout << "\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "-----------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "    БРОСОК КУБИКА\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "-----------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

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
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "-----------------------------------\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    MovePlayer(currentPlayer, chislo);

    if (CheckWin(currentPlayer)) {
        cout << "\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "  ПОБЕДИТЕЛЬ: ИГРОК " << (currentPlayer + 1) << "!\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "=============================================\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        return;
    }

    // Обрабатываем клетку только если игрок еще не выиграл
    if (!gameOver) {
        ProcessCell(currentPlayer);
    }
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

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            cout << "=============================================\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            cout << "       Ф И Н А Л Ь Н А Я   К А Р Т А       \n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            cout << "=============================================\n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            ShowMapWithPlayers();

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