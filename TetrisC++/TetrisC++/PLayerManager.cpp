#include "PlayerManager.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include "ConsoleUtils.h"

std::string PlayerManager::CurrentPlayer = "";

void PlayerManager::NewPlayer() {
    while (true) {
        system("cls");

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        std::string title = "=== CREATE NEW PLAYER ===";
        std::string hint = "(Press ESC to go back)";
        std::string label = "Name: ";

        int centerY = consoleHeight / 2;

        SetColor(11);
        SetCursor((consoleWidth - title.length()) / 2, centerY - 2);
        std::cout << title;

        SetColor(8);
        SetCursor((consoleWidth - hint.length()) / 2, centerY - 1);
        std::cout << hint;

        SetColor(7);
        int inputX = (consoleWidth - 20) / 2;
        SetCursor(inputX, centerY + 1);
        std::cout << label;

        std::string name = "";

        while (true) {
            int key = _getch();

            if (key == 27) {
                CurrentPlayer = "";
                system("cls");
                return;
            }

            if (key == 13) {
                if (!name.empty()) {
                    CurrentPlayer = name;
                    std::ofstream file("players.txt", std::ios::app);
                    file << name << "\n";
                    return;
                }
            }
            else if (key == 8 && !name.empty()) {
                name.pop_back();
            }
            else if (isprint(key)) {
                name += (char)key;
            }

            SetCursor(inputX + label.length(), centerY + 1);
            std::cout << std::string(20, ' ');
            SetCursor(inputX + label.length(), centerY + 1);
            std::cout << name;
        }
    }
}
void PlayerManager::ContinuePlayer() {
    system("cls");

    std::ifstream file("players.txt");
    if (!file) {
		SetColor(9);
        std::cout << "No players found.\nPress any key...";
        _getch();
        CurrentPlayer = "";
        return;
    }

    std::vector<std::string> players;
    std::string line;

    while (getline(file, line)) {
        if (!line.empty()) players.push_back(line);
    }

    if (players.empty()) {
		SetColor(9);
        std::cout << "No players found.\nPress any key...";
        _getch();
        CurrentPlayer = "";
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	SetColor(9);
    std::string title = "Select Player (ESC to go back):";
    int startY = consoleHeight / 2 - players.size() / 2;

    SetColor(9);
    SetCursor((consoleWidth - title.length()) / 2, startY - 2);
    std::cout << title;

    for (int i = 0; i < players.size(); i++) {
        std::string lineText = std::to_string(i + 1) + ". " + players[i];
        SetCursor((consoleWidth - lineText.length()) / 2, startY + i);
        std::cout << lineText;
    }

    std::string prompt = "Choice: ";
    int inputX = (consoleWidth - 20) / 2;

    SetCursor(inputX, startY + players.size() + 2);
    std::cout << prompt;

    std::string input = "";

    while (true) {
        int key = _getch();

        if (key == 27) {
            CurrentPlayer = "";
            system("cls");
            return;
        }

        if (key == 13) {
            int choice = atoi(input.c_str());
            if (choice >= 1 && choice <= players.size()) {
                CurrentPlayer = players[choice - 1];
                system("cls");
                return;
            }

            input = "";
            SetCursor(inputX, startY + players.size() + 4);
            std::cout << "Invalid choice. Try again: ";
            continue;
        }

        if (key == 8 && !input.empty()) {
            input.pop_back();
        }
        else if (isdigit(key)) {
            input += (char)key;
        }

        SetCursor(inputX + prompt.length(), startY + players.size() + 2);
        std::cout << std::string(10, ' ');
        SetCursor(inputX + prompt.length(), startY + players.size() + 2);
        std::cout << input;
    }
}

void PlayerManager::SaveScore(int score) {
    std::ifstream file("scores.txt");
    std::vector<std::pair<std::string, int>> scores;

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            int s = atoi(line.substr(pos + 1).c_str());
            scores.push_back({ name, s });
        }
    }
    file.close();

    bool found = false;

    for (auto& s : scores) {
        if (s.first == CurrentPlayer) {
            if (score > s.second)
                s.second = score;
            found = true;
        }
    }

    if (!found && !CurrentPlayer.empty())
        scores.push_back({ CurrentPlayer, score });

    std::ofstream out("scores.txt");
    for (auto& s : scores)
        out << s.first << ":" << s.second << "\n";
}

void PlayerManager::ShowScores() {
    system("cls");
    SetColor(13);
    std::ifstream file("scores.txt");
    if (!file) {
        std::cout << "No scores yet.\nPress any key...";
        _getch();
        return;
    }

    std::vector<std::pair<std::string, int>> scores;
    std::string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(':');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            int s = atoi(line.substr(pos + 1).c_str());
            scores.push_back({ name, s });
        }
    }

    if (scores.empty()) {
		
        std::cout << "No valid scores found.\nPress any key...";
        _getch();
        return;
    }

    sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
        return a.second > b.second;
        });
	
    std::cout << "=== SCOREBOARD ===\n\n";

    for (auto& s : scores)
        std::cout << s.first << " - " << s.second << "\n";

    std::cout << "\nPress any key...";
    _getch();
}

void PlayerManager::ClearPlayers() {
    system("cls");
	SetColor(12);
    std::cout << "Are you sure you want to delete ALL players? (y/n): ";

    if (_getch() == 'y') {
        std::ofstream("players.txt", std::ios::trunc);
        std::cout << "\nPlayers cleared.";
    }
    else {
        std::cout << "\nCancelled.";
    }

    std::cout << "\nPress any key...";
    _getch();
}

void PlayerManager::ClearScores() {
    system("cls");
	SetColor(14);
    std::cout << "Are you sure you want to delete ALL scores? (y/n): ";

    if (_getch() == 'y') {
        std::ofstream("scores.txt", std::ios::trunc);
        std::cout << "\nScores cleared.";
    }
    else {
        std::cout << "\nCancelled.";
    }

    std::cout << "\nPress any key...";
    _getch();
}
