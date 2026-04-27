#pragma once
#include <string>

class PlayerManager {
public:
    static std::string CurrentPlayer;

    static void NewPlayer();
    static void ContinuePlayer();
    static void SaveScore(int score);
    static void ShowScores();
    static void ClearPlayers();
    static void ClearScores();
};

