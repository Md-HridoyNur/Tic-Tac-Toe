#include <iostream>
#include <vector>
#include <limits>
using namespace std;

enum class Mode { TWO_PLAYER, VS_COMPUTER };

void printBoard(const vector<char>& b) {
    cout << "\n";
    for (int i = 0; i < 9; i += 3) {
        cout << " " << b[i] << " | " << b[i+1] << " | " << b[i+2] << "\n";
        if (i < 6) cout << "---+---+---\n";
    }
    cout << "\n";
}

char checkWinner(const vector<char>& b) {
    const int wins[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };
    for (auto &w : wins) {
        if (b[w[0]] != ' ' && b[w[0]] == b[w[1]] && b[w[1]] == b[w[2]])
            return b[w[0]];
    }
    return ' ';
}

bool isBoardFull(const vector<char>& b) {
    for (char c : b) if (c == ' ') return false;
    return true;
}

int minimax(vector<char>& board, bool isMaximizing, int depth) {
    char winner = checkWinner(board);
    if (winner == 'O') return 10 - depth;
    if (winner == 'X') return depth - 10;
    if (isBoardFull(board)) return 0;

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = 'O';
                int val = minimax(board, false, depth + 1);
                board[i] = ' ';
                best = max(best, val);
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < 9; ++i) {
            if (board[i] == ' ') {
                board[i] = 'X';
                int val = minimax(board, true, depth + 1);
                board[i] = ' ';
                best = min(best, val);
            }
        }
        return best;
    }
}

int findBestMove(vector<char>& board) {
    int bestVal = numeric_limits<int>::min();
    int bestMove = -1;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == ' ') {
            board[i] = 'O';
            int moveVal = minimax(board, false, 0);
            board[i] = ' ';
            if (moveVal > bestVal) {
                bestVal = moveVal;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

int getPlayerMove(const vector<char>& board) {
    while (true) {
        cout << "Enter your move (1-9): ";
        int pos;
        if (!(cin >> pos)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number 1-9.\n";
            continue;
        }
        if (pos < 1 || pos > 9) {
            cout << "Choose a number between 1 and 9.\n";
            continue;
        }
        if (board[pos - 1] != ' ') {
            cout << "Cell already taken. Choose another.\n";
            continue;
        }
        return pos - 1;
    }
}

int main() {
    cout << "Tic-Tac-Toe\n";
    cout << "1) Two-player\n2) Play vs Computer (unbeatable)\nChoose mode (1 or 2): ";
    int m;
    while (!(cin >> m) || (m != 1 && m != 2)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter 1 or 2: ";
    }
    Mode mode = (m == 1) ? Mode::TWO_PLAYER : Mode::VS_COMPUTER;
    vector<char> board(9, ' ');
    cout << "\nBoard positions are numbered:\n";
    vector<char> numbered(9);
    for (int i = 0; i < 9; ++i) numbered[i] = '1' + i;
    printBoard(numbered);

    char current = 'X';
    while (true) {
        printBoard(board);
        if (mode == Mode::TWO_PLAYER) {
            cout << "Player " << current << "'s turn.\n";
            int move = getPlayerMove(board);
            board[move] = current;
        } else {
            if (current == 'X') {
                cout << "Your (X) turn.\n";
                int move = getPlayerMove(board);
                board[move] = 'X';
            } else {
                cout << "Computer (O) is thinking...\n";
                int move = findBestMove(board);
                if (move == -1) {
                    for (int i = 0; i < 9; ++i) if (board[i] == ' ') { move = i; break; }
                }
                board[move] = 'O';
                cout << "Computer chose position " << (move + 1) << ".\n";
            }
        }

        char winner = checkWinner(board);
        if (winner != ' ') {
            printBoard(board);
            cout << "Player " << winner << " wins! Congratulations!\n";
            break;
        }
        if (isBoardFull(board)) {
            printBoard(board);
            cout << "It's a tie!\n";
            break;
        }
        current = (current == 'X') ? 'O' : 'X';
    }

    cout << "Game over. Thanks for playing!\n";
    return 0;
}

