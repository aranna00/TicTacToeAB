// ttt_ab.cpp
// Compile with: g++ -std=c++11 -o ttt_ab ttt_ab.cpp ttt.cpp

#include <iostream>
#include <algorithm>
#include <map>
#include "ttt.h"

enum class PlayerType {
    Human, Computer
};

Player otherPlayer(Player player) {
    if (player == Player::O) {
        return Player::X;
    } else {
        return Player::O;
    }
}

int maxPly = 5;

int eval(const State &board, const Player &player) {
    int score = 0;
    // add individual scores
    for (int i = 0; i < 9; ++i) {
        if (board[i] == player) {
            score++;
        } else if (board[i] == otherPlayer(player)) {
            score--;
        }
    }

    // add winner scores
    if (getWinner(board) == player) {
        score += 100;
    } else if (getWinner(board) == otherPlayer(player)) {
        score -= 100;
    }

    // check horizontal
    for (int yH = 0; yH < 3; ++yH) {
        for (int xH = 0; xH < 2; ++xH) {
            if (board[yH * 3 + xH] == board[yH * 3 + xH + 1]) {
                if (board[yH * 3 + xH] == player) {
                    score += 10;
                } else if (board[yH * 3 + xH] == otherPlayer(player)) {
                    score -= 10;
                }
            }
        }
    }

    // check vertical
    for (int xV = 0; xV < 3; ++xV) {
        for (int yV = 0; yV < 2; ++yV) {
            if (board[yV * 3 + xV] == board[yV * 3 + xV + 3]) {
                if (board[yV * 3 + xV] == player) {
                    score += 10;
                } else if (board[yV * 3 + xV] == otherPlayer(player)) {
                    score -= 10;
                }
            }
        }
    }

    // check diagonals
    if (board[0] == board[4] || board[4] == board[8] || board[2] == board[4] || board[4] == board[6]) {
        if (board[4] == player) {
            score += 10;
        } else if (board[4] == otherPlayer(player)) {
            score -= 10;
        }
    }

    return score;
}

int alphaBeta(const State &board, int ply, Player maxPlayer = Player::None, int min = INT32_MIN, int max = INT32_MAX) {
    AB bestAB;
    if (maxPlayer == Player::None) {
        maxPlayer = getCurrentPlayer(board);
    }

    std::vector<Move> moves = getMoves(board);

    if (moves.empty() || ply == 0) {
        int score = eval(board, maxPlayer);
        return score;
    }

    if (maxPlayer == getCurrentPlayer(board)) {
        bestAB.score = min;
        for (int move:getMoves(board)) {
            State newBoard = doMove(board, move);
            int evalScore = alphaBeta(newBoard, ply - 1, maxPlayer, bestAB.score, max);
            if (evalScore > bestAB.score) {
                bestAB.score = evalScore;
                bestAB.move = move;
            }
            if (bestAB.score > max) {
                return max;
            }
        }

        if (ply != maxPly)
            return bestAB.score;

        return bestAB.move;
    } else {
        bestAB.score = max;
        for (int move:getMoves(board)) {
            State newBoard = doMove(board, move);
            int evalScore = alphaBeta(newBoard, ply - 1, maxPlayer, min, bestAB.score);
            if (evalScore < bestAB.score) {
                bestAB.score = evalScore;
                bestAB.move = move;
            }
            if (bestAB.score < min) {
                return min;
            };
        }

        if (ply != maxPly)
            return bestAB.score;

        return bestAB.move;
    }
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
            board = doMove(board, alphaBeta(board, maxPly));
        }
        std::cout << board << std::endl;
        moves = getMoves(board);
    }

    return 0;
}

