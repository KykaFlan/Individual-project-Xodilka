#pragma once

// Глобальные переменные
extern int KolichestvoPlayers;
extern int winnerPlayer;


// Функции игры (не все, только те которые вызываются вне GamePlay.cpp)
void StartGame();
int GetWinner();