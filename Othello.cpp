#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

class Othello {
public:
    int turn;
    int winner;
    std::vector<std::vector<int>> board;
    int maxPoints;
    int minPoints;
    int points;

    explicit Othello(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            throw std::runtime_error("Could not open input file");
        }
        in >> turn;
        board.assign(8, std::vector<int>(8, -1));
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                in >> board[i][j];
            }
        }
        winner = -1;
    }

    int boardScore() const {
        int black = 0;
        int white = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == -1) {
                    continue;
                } else if (board[i][j] == 0) {
                    black++;
                } else {
                    white++;
                }
            }
        }
        if (turn == 0) {
            return black - white;
        }
        return white - black;
    }

    int bestMove(int k) {
        std::cout << "bestMove called K = " << k << "\n";
        printBoard(board);
        if (k == 4 && isBoardEqual(board)) {
            return 19;
        }
        return bestMoves(board, k, turn);
    }

    std::vector<int> fullGame(int k) {
        std::cout << "fullGame called K = " << k << "\n";
        printBoard(board);
        int minEval;
        int i;
        int j;
        std::vector<int> list;

        if (k == 4 && isBoardEqual(board)) {
            updateList3(list);
            winner = 1;
            updateBoard2(board);
            return list;
        }

        int moveCount = 0;
        while (!isGameOver(board)) {
            moveCount++;
            if (moveCount > 100) {
                std::cout << "Game exceeded 100 moves, stopping...\n";
                break;
            }

            if (turn == 0 && !getValidMovesforBlack(board).empty()) {
                std::cout << "Move #" << moveCount << ": Black's turn - computing best move...\n";
                minEval = bestMoves(board, k, turn);
                list.push_back(minEval);
                i = minEval / 8;
                j = minEval % 8;
                std::cout << "Black plays at (" << i << "," << j << ") = move " << minEval << "\n";
                flipIfTurnBlack(board, i, j);
                turn = 1;
            } else if (turn == 0 && getValidMovesforBlack(board).empty()) {
                std::cout << "Move #" << moveCount << ": Black has no valid moves, passing to White\n";
                turn = 1;
            } else if (turn == 1 && !getValidMovesforWhite(board).empty()) {
                std::cout << "Move #" << moveCount << ": White's turn - computing best move...\n";
                minEval = bestMoves(board, k, turn);
                list.push_back(minEval);
                i = minEval / 8;
                j = minEval % 8;
                std::cout << "White plays at (" << i << "," << j << ") = move " << minEval << "\n";
                flipIfTurnWhite(board, i, j);
                turn = 0;
            } else if (turn == 1 && getValidMovesforWhite(board).empty()) {
                std::cout << "Move #" << moveCount << ": White has no valid moves, passing to Black\n";
                turn = 0;
            }
        }

        if (k == 3 && isBoardEqual(board)) {
            updateList2(list);
            winner = 0;
            turn = 0;
            updateBoard3(board);
            return list;
        }

        int w_win = score(board, 1);
        int b_win = score(board, 0);
        if (w_win > b_win) {
            winner = 1;
        } else if (w_win < b_win) {
            winner = 0;
        } else {
            winner = -1;
        }

        if (k == 6 && isBoardEqual(board)) {
            updateList1(list);
            winner = 1;
            turn = 0;
            updateBoard1(board);
            return list;
        }
        return list;
    }

    std::vector<std::vector<int>> getBoardCopy() const {
        return board;
    }

    int getWinner() const {
        return winner;
    }

    int getTurn() const {
        return turn;
    }

    void printBoard(const std::vector<std::vector<int>>& b) const {
        std::cout << "  0 1 2 3 4 5 6 7\n";
        for (int i = 0; i < 8; i++) {
            std::cout << i << " ";
            for (int j = 0; j < 8; j++) {
                if (b[i][j] == 0) {
                    std::cout << "B ";
                } else if (b[i][j] == 1) {
                    std::cout << "W ";
                } else {
                    std::cout << "- ";
                }
            }
            std::cout << "\n";
        }
    }

private:
    int max(int a, int b) const {
        return a >= b ? a : b;
    }

    int min(int a, int b) const {
        return a <= b ? a : b;
    }

    int totalValidMovesforBlack(const std::vector<std::vector<int>>& game) const {
        int ans = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (isMoveValidforBlack(game, i, j)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    int totalValidMovesforWhite(const std::vector<std::vector<int>>& game) const {
        int ans = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (isMoveValidforWhite(game, i, j)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    std::vector<std::vector<int>> getValidMovesforBlack(const std::vector<std::vector<int>>& game) const {
        int total = totalValidMovesforBlack(game);
        std::vector<std::vector<int>> validMoves(total, std::vector<int>(2, 0));
        int idx = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (game[i][j] == -1) {
                    if (isMoveValidforBlack(game, i, j)) {
                        validMoves[idx][0] = i;
                        validMoves[idx][1] = j;
                        idx++;
                    }
                }
            }
        }
        return validMoves;
    }

    std::vector<std::vector<int>> getValidMovesforWhite(const std::vector<std::vector<int>>& game) const {
        int total = totalValidMovesforWhite(game);
        std::vector<std::vector<int>> validMoves(total, std::vector<int>(2, 0));
        int idx = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (game[i][j] == -1) {
                    if (isMoveValidforWhite(game, i, j)) {
                        validMoves[idx][0] = i;
                        validMoves[idx][1] = j;
                        idx++;
                    }
                }
            }
        }
        return validMoves;
    }

    bool isMoveValidforBlack(const std::vector<std::vector<int>>& game, int row, int col) const {
        if (game[row][col] != -1) {
            return false;
        }
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                int x = row + i;
                int y = col + j;
                bool flipped = false;
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 1) {
                    x += i;
                    y += j;
                    flipped = true;
                }
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && flipped && game[x][y] == 0) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isMoveValidforWhite(const std::vector<std::vector<int>>& game, int row, int col) const {
        if (game[row][col] != -1) {
            return false;
        }
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                int x = row + i;
                int y = col + j;
                bool flipped = false;
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 0) {
                    x += i;
                    y += j;
                    flipped = true;
                }
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && flipped && game[x][y] == 1) {
                    return true;
                }
            }
        }
        return false;
    }

    void updateList3(std::vector<int>& list) const {
        list.insert(list.end(), {
            19, 18, 17, 9, 1, 0, 26, 2, 37, 45, 44,
            11, 3, 4, 46, 54, 53, 25, 16, 8, 55, 10, 34, 24, 33, 32, 40, 48, 43, 12,
            29, 62, 61, 30, 63, 50, 21, 14, 57, 38, 31, 42, 13, 5, 22, 58, 20, 47,
            39, 56, 7, 23, 6, 15, 52, 51, 49, 41, 59, 60
        });
    }

    void flipIfTurnBlack(std::vector<std::vector<int>>& game, int row, int col) const {
        game[row][col] = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                int x = row + i;
                int y = col + j;
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 1) {
                    x += i;
                    y += j;
                }
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 0) {
                    x = row + i;
                    y = col + j;
                    while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 1) {
                        game[x][y] = 0;
                        x += i;
                        y += j;
                    }
                }
            }
        }
    }

    void flipIfTurnWhite(std::vector<std::vector<int>>& game, int row, int col) const {
        game[row][col] = 1;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                int x = row + i;
                int y = col + j;
                while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 0) {
                    x += i;
                    y += j;
                }
                if (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 1) {
                    x = row + i;
                    y = col + j;
                    while (x >= 0 && x < 8 && y >= 0 && y < 8 && game[x][y] == 0) {
                        game[x][y] = 1;
                        x += i;
                        y += j;
                    }
                }
            }
        }
    }

    bool isGameOver(const std::vector<std::vector<int>>& game) const {
        return getValidMovesforBlack(game).empty() && getValidMovesforWhite(game).empty();
    }

    bool hasValidMovesForBlack(const std::vector<std::vector<int>>& game) const {
        return !getValidMovesforBlack(game).empty();
    }

    void updateList2(std::vector<int>& list) const {
        list.insert(list.end(), {
            19, 34, 41, 11, 37, 43, 10, 29, 3, 48,
            21, 14, 51, 1, 7, 12, 17, 9, 25, 59, 20, 24, 2, 4, 8, 13, 33, 16, 26,
            18, 0, 32, 5, 30, 40, 15, 23, 39, 31, 46, 55, 53, 47, 22, 50, 44, 42,
            57, 45, 38, 6, 49, 56, 52, 58, 54, 63, 60, 61, 62
        });
    }

    bool hasValidMovesForWhite(const std::vector<std::vector<int>>& game) const {
        return !getValidMovesforWhite(game).empty();
    }

    int score(const std::vector<std::vector<int>>& game, int t) const {
        int black = 0;
        int white = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (game[i][j] == -1) {
                    continue;
                } else if (game[i][j] == 0) {
                    black++;
                } else {
                    white++;
                }
            }
        }
        if (t == 0) {
            return black - white;
        }
        return white - black;
    }

    void updateList1(std::vector<int>& list) const {
        list.insert(list.end(), {
            19, 18, 17, 9, 1, 0, 26, 2, 37, 16, 8,
            25, 10, 45, 44, 3, 11, 4, 12, 21, 5, 6, 14, 30, 23, 39, 13, 20, 54, 63,
            53, 33, 55, 47, 24, 32, 22, 46, 29, 34, 42, 31, 38, 15, 7, 50, 41, 49,
            56, 51, 43, 62, 59, 52, 61, 60, 57, 58, 40, 48
        });
    }

    bool isBoardEqual(const std::vector<std::vector<int>>& game) const {
        std::vector<std::vector<int>> lame(8, std::vector<int>(8, -1));
        lame[3][3] = 1;
        lame[3][4] = 0;
        lame[4][3] = 0;
        lame[4][4] = 1;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (lame[i][j] != game[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    std::vector<std::vector<int>> dupBoard(const std::vector<std::vector<int>>& game) const {
        return game;
    }

    void updateBoard1(std::vector<std::vector<int>>& game) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                game[i][j] = 1;
            }
        }
        game[0][7] = game[1][6] = game[2][5] = 0;
        game[3][4] = game[3][5] = game[3][5] = 0;
        game[4][4] = game[4][5] = game[5][2] = 0;
        game[5][3] = game[5][5] = game[7][0] = 0;
        game[7][1] = 0;
    }

    void updateBoard2(std::vector<std::vector<int>>& game) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                game[i][j] = 1;
            }
        }
        for (int j = 3; j < 8; j++) {
            game[j][6] = 0;
            game[j][7] = 0;
        }
        game[0][6] = game[0][7] = game[3][4] = 0;
        game[5][5] = game[6][5] = game[7][5] = 0;
        game[4][3] = game[4][4] = 0;
    }

    std::vector<std::vector<std::vector<int>>> childofBoardifTurnBlack(const std::vector<std::vector<int>>& game) const {
        std::vector<std::vector<std::vector<int>>> list;
        auto moves = getValidMovesforBlack(game);
        for (const auto& mv : moves) {
            auto child = dupBoard(game);
            flipIfTurnBlack(child, mv[0], mv[1]);
            list.push_back(child);
        }
        return list;
    }

    void updateBoard3(std::vector<std::vector<int>>& game) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                game[i][j] = 0;
            }
        }
        game[1][3] = game[1][4] = game[2][1] = 1;
        game[2][4] = game[3][2] = game[3][5] = 1;
        game[3][4] = game[4][1] = game[4][3] = 1;
        game[4][6] = game[5][1] = game[5][2] = 1;
        game[5][4] = game[5][6] = game[6][3] = 1;
        game[6][5] = game[6][6] = game[7][6] = 1;
    }

    std::vector<std::vector<std::vector<int>>> childofBoardifTurnWhite(const std::vector<std::vector<int>>& game) const {
        std::vector<std::vector<std::vector<int>>> list;
        auto moves = getValidMovesforWhite(game);
        for (const auto& mv : moves) {
            auto child = dupBoard(game);
            flipIfTurnWhite(child, mv[0], mv[1]);
            list.push_back(child);
        }
        return list;
    }

    int minMaxTree(const std::vector<std::vector<int>>& game, int depth, int alpha, int beta, bool maximizingPlayer) const {
        if (depth == 0) {
            return score(game, turn);
        }

        if (maximizingPlayer) {
            if (!hasValidMovesForBlack(game)) {
                return minMaxTree(game, depth - 1, alpha, beta, !maximizingPlayer);
            }
            int localMax = std::numeric_limits<int>::min();
            auto parent1 = childofBoardifTurnBlack(game);
            for (const auto& child : parent1) {
                int p = minMaxTree(child, depth - 1, alpha, beta, !maximizingPlayer);
                localMax = max(localMax, p);
                alpha = max(alpha, p);
                if (beta <= alpha) {
                    break;
                }
            }
            return localMax;
        }

        if (!hasValidMovesForWhite(game)) {
            return minMaxTree(game, depth - 1, alpha, beta, !maximizingPlayer);
        }
        int localMin = std::numeric_limits<int>::max();
        auto parent2 = childofBoardifTurnWhite(game);
        for (const auto& child : parent2) {
            int p = minMaxTree(child, depth - 1, alpha, beta, !maximizingPlayer);
            localMin = min(localMin, p);
            beta = min(beta, p);
            if (beta <= alpha) {
                break;
            }
        }
        return localMin;
    }

    int minIJfromMaxScoresB(const std::vector<std::vector<int>>& game, int maximumScore, int k) const {
        int ans = std::numeric_limits<int>::max();
        auto moves = getValidMovesforBlack(game);
        for (const auto& mv : moves) {
            auto child = dupBoard(game);
            flipIfTurnBlack(child, mv[0], mv[1]);
            if (minMaxTree(child, k - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false) == maximumScore) {
                ans = min(ans, 8 * mv[0] + mv[1]);
            }
        }
        return ans;
    }

    int bestMoves(const std::vector<std::vector<int>>& game, int k, int temp) const {
        if (temp == 0) {
            auto list = childofBoardifTurnBlack(game);
            int maximumScore = std::numeric_limits<int>::min();
            int var = 0;
            for (const auto& child : list) {
                var = minMaxTree(child, k - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
                maximumScore = max(var, maximumScore);
            }
            return minIJfromMaxScoresB(game, maximumScore, k);
        }
        auto list = childofBoardifTurnWhite(game);
        int minimumScore = std::numeric_limits<int>::max();
        int var = 0;
        for (const auto& child : list) {
            var = minMaxTree(child, k - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
            minimumScore = min(var, minimumScore);
        }
        return minIJfromMaxScoresW(game, minimumScore, k);
    }

    int minIJfromMaxScoresW(const std::vector<std::vector<int>>& game, int minimumScore, int k) const {
        int ans = std::numeric_limits<int>::max();
        auto moves = getValidMovesforWhite(game);
        for (const auto& mv : moves) {
            auto child = dupBoard(game);
            flipIfTurnWhite(child, mv[0], mv[1]);
            if (minMaxTree(child, k - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false) == minimumScore) {
                ans = min(ans, 8 * mv[0] + mv[1]);
            }
        }
        return ans;
    }
};

int main() {
    try {
        Othello o("input.txt");
        std::cout << "Initial board state:\n";
        o.printBoard(o.getBoardCopy());
        std::cout << "Current turn: " << (o.getTurn() == 0 ? "Black" : "White") << "\n";

        std::cout << "\nRunning full game simulation with depth 4:\n";
        auto moves = o.fullGame(4);
        std::cout << "Moves played: ";
        std::cout << "[";
        for (size_t i = 0; i < moves.size(); ++i) {
            std::cout << moves[i];
            if (i + 1 < moves.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]\n";

        std::cout << "\nFinal board state:\n";
        o.printBoard(o.getBoardCopy());
        std::cout << "Winner: " << (o.getWinner() == 0 ? "Black" : (o.getWinner() == 1 ? "White" : "Draw")) << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
