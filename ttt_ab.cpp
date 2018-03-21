// ttt_ab.cpp
// Compile with: g++ -std=c++11 -o ttt_ab ttt_ab.cpp ttt.cpp

#include <iostream>
#include <algorithm>
#include <map>
#include <limits>
#include "ttt.h"

enum class PlayerType {
    Human, Computer
};

int eval(const State &board, const Player &player) {
    int score = 0;
    // add individual scores
    for (int i = 0; i < 9; ++i) {
        if (board[i] == player) {
            score++;
        } else if (board[i] != player && board[i] != Player::None) {
            score--;
        }
    }
    // add winner scores
    if (getWinner(board) == player) {
        score += 100;
    } else if (getWinner(board) != player && getWinner(board) != Player::None) {
        score -= 100;
    }

    // check horizontal
    for (int yH = 0; yH < 3; ++yH) {
        for (int xH = 0; xH < 2; ++xH) {
            if (board[yH * 3 + xH] == board[yH * 3 + xH + 1]) {
                if (board[yH * 3 + xH] == player) {
                    score += 10;
                } else if (board[yH * 3 + xH] != Player::None) {
                    score -= 10;
                }
            }
        }
    }

    // check vertical
    for (int xV = 0; xV < 3; ++xV) {
        for (int yV = 0; yV < 2; ++yV) {
            if (board[yV * 3 + xV] == board[yV * 3 + xV + 3]) {
                if (board[xV * 3 + yV] == player) {
                    score += 10;
                } else if (board[xV * 3 + yV] != player && board[xV * 3 + yV] != Player::None) {
                    score -= 10;
                }
            }
        }
    }

    // check diagonals
    if ((board[0] == board[4] || board[4] == board[8]) || (board[2] == board[4] || board[4] == board[6])) {
        if (board[4] == player) {
            score += 10;
        } else if (board[4] != player && board[4] != Player::None) {
            score -= 10;
        }
    }

    return score;
}

Move alphaBeta(const State &board, int ply) {
    std::string type;
    AB bestAB;
    if (ply % 2 == 0) {
        type == "max";
        bestAB.score = INT32_MIN;
    } else {
        type == "min";
        bestAB.score = INT32_MAX;
    }
    std::vector<Move> moves = getMoves(board);

    if (moves.empty() || getWinner(board) != Player::None || ply == 0) {
        return eval(board, getCurrentPlayer(board));
    }

    for (int move:getMoves(board)) {
        State newBoard = board;
        doMove(newBoard, move);
        int evalScore = alphaBeta(board, ply - 1);
        std::cout << evalScore << "\t";
        if (type == "max") {
            if (evalScore > bestAB.score) {
                bestAB.score = evalScore;
                bestAB.move = move;
            }
        } else {
            if (evalScore < bestAB.score) {
                bestAB.score = evalScore;
                bestAB.move = move;
            }
        }
    }
    std::cout << std::endl;

    return bestAB.move;
}

int main() {
    std::srand(std::time(0));

    std::map<Player, PlayerType> playerType;
    playerType[Player::X] = PlayerType::Human;
    playerType[Player::O] = PlayerType::Computer;

    State board = {
            Player::None, Player::None, Player::None,
            Player::None, Player::None, Player::None,
            Player::None, Player::None, Player::None};
    std::cout << board << std::endl;

    std::vector<Move> moves = getMoves(board);
    while (moves.size() > 0) {
        if (playerType[getCurrentPlayer(board)] == PlayerType::Human) {
            std::cout << "+-+-+-+" << std::endl <<
                      "|0|1|2|" << std::endl <<
                      "+-+-+-+" << std::endl <<
                      "|3|4|5|" << std::endl <<
                      "+-+-+-+" << std::endl <<
                      "|6|7|8|" << std::endl <<
                      "+-+-+-+" << std::endl << std::endl;
            std::cout << "Enter a move ( ";
            for (Move m: moves) std::cout << m << " ";
            std::cout << "): ";
            Move m;
            std::cin >> m;
            board = doMove(board, m);
        } else {
            board = doMove(board, alphaBeta(board, 5));
        }
        std::cout << board << std::endl;
        moves = getMoves(board);
    }

    return 0;
}

