#include <iostream>
#include <conio.h>  //для использования _getch()
#include <windows.h>

#include "ProverkaInput.h"
#include "OutConsol.h"
#include "GamePlay.h"


using namespace std;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Выводит приветствие 
    WelcomeX();

    cout << "Сколько игроков будет участвовать в игре? (минимум 2, максимум 5 игроков)\n";
    int KolichestvoPlayers;
    cout << "Количество: ";
    cin >> KolichestvoPlayers;
    cout << "\n";

    system("cls");

    // Проверяем ввод и сохраняем в глобальную переменную
    ::KolichestvoPlayers = CheckKolichestvoPlayers(KolichestvoPlayers);
    cout << "\n\n\n\n\n\n\n\n                    Отлично! Участников игры: " << ::KolichestvoPlayers;
    cout << "\n\n\n\n\n\nЧтобы продолжить нажмите любую клавишу...";
    _getch();

    system("cls");

    // Выводит правила игры
    GameRules();
    cout << "\nЧтобы продолжить нажмите любую клавишу...";
    _getch();
    system("cls");

    cout << "\n\n\n\n\n\n\n\n                           Игра начинается...\n\n\n\n\n";
    cout << "Нажмите любую клавишу...";
    _getch();
    system("cls");

    // Запускает игру
    StartGame();

    cout << "\n\n\nНажмите любую клавишу для завершения...";
    _getch();
    system("cls");
        
    // Получаем победителя
    int winner = GetWinner();
    // Выводит победителя
    EndGame(winner);

    return 0;
}