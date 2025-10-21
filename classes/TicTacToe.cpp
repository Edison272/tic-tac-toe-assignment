#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
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
    // here we should call setNumberOfPlayers to 2 and then set up the game options so the mouse knows to draw a 3x3 grid
    // _gameOptions has a rowX and rowY property we should set to 3
    // then we need to setup our 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    // we will use the initHolder function on each square to do this
    // finally we should call startGame to get everything going
    setNumberOfPlayers(2);
    _gameOptions.rowX = 3, _gameOptions.rowY = 3;
    for (int i = 0; i < _gameOptions.rowX; i++) {
        for (int j = 0; j < _gameOptions.rowY; j++) {
            _grid[i][j].initHolder(ImVec2((float)i*80, (float)j*80 + 10), "square.png", i, j);
            std::cout << i << ", " << j << std::endl;
        }
    }

    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // 1) Guard clause: if holder is nullptr, fail fast.
    //    (Beginner hint: always check pointers before using them.)
    if (!holder) return false;

    // 2) Is it actually empty?
    //    Ask the holder for its current Bit using the bit() function.
    //    If there is already a Bit in this holder, return false.
    Bit *hold_bit = holder->bit();
    if (hold_bit) return false;

    // 3) Place the current player's piece on this holder:
    //    - Figure out whose turn it is (getCurrentPlayer()->playerNumber()).
    //    - Create a Bit via PieceForPlayer(currentPlayerIndex).
    //    - Position it at the holder's position (holder->getPosition()).
    //    - Assign it to the holder: holder->setBit(newBit);
    int num = getCurrentPlayer()->playerNumber();
    Bit *new_bit = PieceForPlayer(num);
    new_bit->setPosition(holder->getPosition());
    holder->setBit(new_bit);
    

    // 4) Return whether we actually placed a piece. true = acted, false = ignored.
    return true; // replace with true if you complete a successful placement    
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
    // if any of them have the same owner return that player
    // winning triples are:
    // 0,1,2
    // 3,4,5
    // 6,7,8
    // 0,3,6
    // 1,4,7
    // 2,5,8
    // 0,4,8
    // 2,4,6
    // you can use the ownerAt helper function to get the owner of a square
    // for example, ownerAt(0) returns the owner of the top-left square
    // if there is no bit in that square, it returns nullptr
    // if you find a winning triple, return the player who owns that triple
    // otherwise return nullptr

    // Hint: Consider using an array to store the winning combinations
    // to avoid repetitive code
    static const int WinningTriples[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        
        // if there isn't an owner to the first item of any triplet, theres no winner here
        if (ownerAt(WinningTriples[i][0]) != nullptr) {

            // if there is an owner, check if any of the winning triples share the same owner
            if (ownerAt(WinningTriples[i][0]) == ownerAt(WinningTriples[i][1]) && ownerAt(WinningTriples[i][1]) == ownerAt(WinningTriples[i][2])) {
                std::cout << WinningTriples[i][0] << WinningTriples[i][1] << WinningTriples[i][2] << std::endl;
                return ownerAt(WinningTriples[i][0]);
            }
        }

    }
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    // if any square is empty, return false
    // otherwise return true
    for (int i = 0; i < 9; i++) {
        // if there is a nullptr in any of the squares, the game isn't over
        if (ownerAt(i) == nullptr) {
            return false;
        }
    }

    // if the board is full, but a winner can't be found, it's a draw!
    if (checkForWinner() != nullptr) {
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
    // return a string representing the current state of the board
    // the string should be 9 characters long, one for each square
    // each character should be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order should be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can build the string using a loop and the to_string function
    // for example, to convert an integer to a string, you can use std::to_string(1) which returns "1"
    // you can get the bit at each square using _grid[y][x].bit()
    // if the bit is not null, you can get its owner using bit->getOwner()->playerNumber()
    // remember that player numbers are zero-based, so add 1 to get '1' or '2'
    // if the bit is null, add '0' to the string
    // finally, return the constructed string
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
    return state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'

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

