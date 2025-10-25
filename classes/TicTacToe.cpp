#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file was a 
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit* TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "o.png" : "x.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // set up 2 players
    setNumberOfPlayers(2);

    // set game board & relevant sprites
    _gameOptions.rowX = 3, _gameOptions.rowY = 3;
    for (int i = 0; i < _gameOptions.rowX; i++) {
        for (int j = 0; j < _gameOptions.rowY; j++) {
            _grid[i][j].initHolder(ImVec2((float)i*90, (float)j*90 + 20), "square.png", i, j);
        }
    }
    startGame();
}

bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) Guard clause: if holder is nullptr, fail fast.
    if (!holder) return false;

    // 2) Is it actually empty?
    Bit *hold_bit = holder->bit();
    if (hold_bit) return false;

    // 3) Place the current player's piece on this holder:
    int num = getCurrentPlayer()->playerNumber();
    Bit *new_bit = PieceForPlayer(num);
    new_bit->setPosition(holder->getPosition());
    holder->setBit(new_bit);
    return true;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit* bit, BitHolder*src, BitHolder*dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    // loop through the 3x3 array and call destroyBit on each square
    for (int i = 0; i < _gameOptions.rowX; i++) {
        for (int j = 0; j < _gameOptions.rowY; j++) {
            _grid[i][j].destroyBit();
        }
    }
}

//
// helper function for the winner check
//
Player* TicTacToe::ownerAt(int index ) const
{
    // index is 0..8, convert to x,y using:
    int y = index / 3;
    int x = index % 3;
    // if there is no bit at that location (in _grid) return nullptr
    // otherwise return the owner of the bit at that location using getOwner()
    if(!_grid[y][x].bit())  {
        return nullptr;
    } else {
        return _grid[y][x].bit()->getOwner(); 
    }
}

Player* TicTacToe::checkForWinner()
{
    // check all the winning triples
    static const int WinningTriples[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        
        // if there isn't an owner to the first item of any triplet, theres no winner here
        if (ownerAt(WinningTriples[i][0]) != nullptr) {

            // if there is an owner, check if any of the winning triples share the same owner
            // if so, return the owner of one of the squares
            if (ownerAt(WinningTriples[i][0]) == ownerAt(WinningTriples[i][1]) && ownerAt(WinningTriples[i][1]) == ownerAt(WinningTriples[i][2])) {
                std::cout << WinningTriples[i][0] << WinningTriples[i][1] << WinningTriples[i][2] << std::endl;
                return ownerAt(WinningTriples[i][0]);
            }
        }
    }
    // return null if theres no winner
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    // if any square is empty, return false
    for (int i = 0; i < 9; i++) {
        // if there is a nullptr in any of the squares, the game isn't over
        if (ownerAt(i) == nullptr) {
            std::cout << "keep goin" << std::endl;
            return false;
        }
    }

    // otherwise, if the board is full, but a winner can't be found, it's a draw!
    if (checkForWinner() != nullptr) {
        std::cout << "player " << checkForWinner()->playerNumber() << ' won' << std::endl;
        return true;
    } else {
        return false;  // someone won- it's not a draw
    }
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    std::string state;
    for (int i = 0; i < 9; i++) {
        if (!ownerAt(i)) {      // if there is no owner for the tile, state is 0
            state += '0';
            continue;
        } else {
            int num = ownerAt(i)->playerNumber();
            if (num == 0) {      // if the owner of the tile is X (0), set state to 1
                state += '1';
            } else {            // otherwise, set it to 2 (tile state for O, AKA index 1)
                state += '2';
            }
        }
    }
    return state;  // return the concatenated string
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // iterate from 0-8, for each letter of the string and for each square on the grid
    for (int i = 0; i < 9; i++) {
        int y = i / 3;
        int x = i % 3;
        _grid[y][x].destroyBit();
        if(s[i] == '1')  {
            _grid[y][x].setBit(PieceForPlayer(1));  // index of 1 for player 1
        } else if (s[i] == '2') {
            _grid[y][x].setBit(PieceForPlayer(0));  // index of 2 for player 2/AI agent
        }
    }
}

bool aiBoardFull(const std::string& state) {
    return state.find("0") == std::string::npos;
}

int aiBoardWinner(std::string& state, int depth, int playerColor) {
    static const int WinningTriples[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    return 1;
    for (int i = 0; i < 8; i++) {
        const int *triple = WinningTriples[i];
        char player = state[triple[0]];
        if (player != '0') {

        }
    }
}

//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // we will implement the AI in the next assignment!
    // int bestMove = 1000;
    // int bestSquare = 1;
    // std::string state = stateString();
    // for (int i = 0; i <9; i++) {
    //     if (state[i] != '0') {
    //         state[i] = '2';
    //         int result = megamax(state, 0, HUMAN_PLAYER);
    //         if (result > bestMove) {
    //             bestMove = result;
    //             bestSquare = i;
    //         }
    //         state[i] = '0';
    //     }
    //     if (bestSquare != 1) {
    //         int xcol = bestSquare % 3;
    //         int ycol = bestSquare / 3;
    //         BitHolder *holder = &_grid[ycol][xcol];
    //         actionForEmptyHolder(holder);
    //         endTurn();
    //     }
    // }
}

// int TicTacToe::megamax(std::string& state, int depth, int playerColor) {
//     int bestVal = 1000;
    
//     int boardWinner = aiBoardWinner(state, depth, playerColor);
//     if (boardWinner) {
//         // return to previous recursion
//         return boardWinner;
//     }
//     bool boardFull = aiBoardFull(state);
//     if (boardFull) {
//         //draw
//         return 0;
//     }
//     for (int i = 0; i < 9; i++) {
//         if (state[i] == '0') {
//             state[i] = playerColor == HUMAN_PLAYER ? '1' : '2';
//             int result = -megamax(state, depth+1, -playerColor);
//             if (result > bestVal) {
//                 bestVal = result;
//             }
//         }
//     }
// }

