#include <iostream>
#include "ProverkaInput.h"
#include <cstdlib>

using namespace std;

int CheckKolichestvoPlayers(int KolichestvoPlayers) {
    bool ok = false;

    while (!ok && (2 > KolichestvoPlayers || KolichestvoPlayers > 5)) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else if (2 > KolichestvoPlayers || KolichestvoPlayers > 5) {
            cout << "Количество игроков не находится в диапозоне от 2 до 5 включительно.\n";
            cout << "Пожалуйства повторите ввод. ";
            cout << "Сколько игроков будет участвать в игре?\n";
            cout << "Количество: ";
            cin >> KolichestvoPlayers;
            system("cls");
        }
        else {
            ok = true;
        }
    }
	return KolichestvoPlayers;
}


int GetNumberInput(int MinNunber, int MaxNunber) {
    int input;
    while (true) {
        cout << "Введите число от " << MinNunber << " до " << MaxNunber << ": ";
        cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Ошибка! Введите число.\n";
        }
        else if (input < MinNunber || input > MaxNunber) {
            cout << "Число должно быть от " << MinNunber << " до " << MaxNunber << "!\n";
        }
        else {
            cin.ignore(1000, '\n');
            return input;
        }
    }
}