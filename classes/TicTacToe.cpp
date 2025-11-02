#include "TicTacToe.h"

// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
/* This file was a derivative of the 'game.cpp' class, specifically designed to handle tic-tac-toe,
and has specific override functions for the tic-tac-toe rules.

Additionally, a portion of code has been uncommented in Game.cpp to enable AI players, and additional ImGui elements
were added into Application.cpp

Changed Files:
- setUpBoard()              - Sets up the tictactoe game by creating two players and a 3x3 grid
- actionForEmptyHolder()    - Used to place pieces on the board; check if the bit exists, and if it's empty, and will place
                              the active player's piece on the select bit if both conditionals are true. Returns true or false
                              depending on if the piece placement was successful
- stopGame()                - Clears out the board by interating through each bit on the 3x3 board, and clearing all associated
                              memory with the destroyBit() function.
- ownerAt()                 - A simple function to check the owner at a given point in the 3x3 board, given an int from 0-8.
                              returns a pointer to a Player when there is an owner, otherwise it returns nullptr
- checkForWinner()          - Checks for a winner by seeing if either player has made a winning triplet.
                              This was done by iterating through an array containing a set of 3 bitHolder indexes, and checking
                              if the owners for all bitHolders are the same. 
                              Uses ownerAt for help.
- checkForDraw()            - Is called to check if the board has reached a stalemate. If the board is full, it's a draw and returns
                              true. Otherwise, returns false. No need to check for winner in checkForDraw() because application.cpp
                              already checks for winner before checking for a draw.
- stateString()             - Returns a string based on the board state. Works by iterating through each bitHolder on the 3x3 board
                              and adds a number to the string based on the owner of the bitHolder. '0' for an empty bit holder,
                              '1' for an X, and '2' for a O.
- setStateString            - Generates the board based on a given state string. Iterates through the stateString, and adjusts the
                              data for the corresponding bitHolder accordingly. Places nothing in the bitHolder for '0' in the state string,
                              an X in the bitHolder for '1', and places an O in the bitHolder for '2'.
- updateAI                  - has simple random tic-tac-toe AI, though it's currently turned off
                              The AI is now running on a minimax algorithm with alpha-beta pruning.



*/
// -----------------------------------------------------------------------------

const int AI_PLAYER   = 1;      // index of the AI player (O)
const int HUMAN_PLAYER= 0;      // index of the human player (X)

TicTacToe::TicTacToe()
{
    std::cout << "start" << std::endl;
}

TicTacToe::~TicTacToe()
{
    std::cout << "end" << std::endl;
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
    for (int i = 0; i < _gameOptions.rowY; i++) {
        for (int j = 0; j < _gameOptions.rowX; j++) {
            _grid[i][j].initHolder(ImVec2((float)j*100, (float)i*100 + 20), "square.png", j, i);
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
            return false;
        }
    }
    // board is full. Assuming check for winner was called before hand, this is a draw
    return true;
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
    // if no 0's are found, the board is full
    for (int i = 0; i < 9; i++) {
        // if there is a nullptr in any of the squares, the game isn't over
        if (state[i] == '0') {
            return false;
        }
    }
    // board is full. Assuming check for winner was called before hand, this is a draw
    return true;
}

int aiBoardWinner(std::string& state) {
    static const int WinningTriples[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for (int i = 0; i < 8; i++) {
        const int *triple = WinningTriples[i];
        char player = state[triple[0]];
        if (player != '0' && player == state[triple[1]] && player == state[triple[2]]) {
            // just return a high value if there's a winner, not necessarily who it is
            return 10;
        }
    }
    return 0;
}

//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI() 
{
    // RANDOM AI
    // std::string open = "";
    // for (int i = 0; i < 9; i++) {
    //     if (ownerAt(i) == nullptr) {
    //         open += char(i);
    //     }
    // }

    // int index = open[rand() % open.length()];
    // int xcol = index % 3;
    // int ycol = index / 3;
    // BitHolder *holder = &_grid[ycol][xcol];
    // actionForEmptyHolder(holder);
    // endTurn();

    // NEGAMAX ALGORITHM - works for Player 2 (O)
    int bestMove = -1000;
    int bestSquare = -1;
    std::string state = stateString();
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') {
            // use "states" to simulate the board
            state[i] = '2';
            int result = -megamax(state, 0, HUMAN_PLAYER, -1000, 1000);
            std::cout << state << " | " << result << std::endl;
            if (result > bestMove) {
                bestMove = result;
                bestSquare = i;
            }
            state[i] = '0';
        }
    }

    // Confirm the move
    if (bestSquare != -1) {
        std::cout << bestSquare << std::endl;
        int xcol = bestSquare % 3;
        int ycol = bestSquare / 3;
        BitHolder *holder = &_grid[ycol][xcol];
        actionForEmptyHolder(holder);
        endTurn();
    }
}

int TicTacToe::megamax(std::string& state, int depth, int playerColor, int alpha, int beta) {
    // terminal state - somebody won
    int boardWinner = aiBoardWinner(state);
    if (boardWinner != 0) {
        // return value for scoring
        //std::cout << "consider winner " << -boardWinner << std::endl;
        return -boardWinner;
    }

    // terminal state - full board (draw)
    bool boardFull = aiBoardFull(state);
    // std::cout << "check draw " << boardFull << std::endl;
    if (boardFull) {
        // return value for scoring
        std::cout << "draw" << std::endl;
        return 0;
    }

    int bestVal = -1000;
    // std::cout << "keep goin - " << state << std::endl;
    // branch out into the next possible board outcomes 
    for (int i = 0; i < 9; i++) {
        if (state[i] == '0') {
            state[i] = playerColor == HUMAN_PLAYER ? '1' : '2';
            int nextPlayer = (playerColor == HUMAN_PLAYER) ? AI_PLAYER : HUMAN_PLAYER;
            // swap alpha and beta and make them negative to switch perspectives on the board
            int result = -megamax(state, depth+1, nextPlayer, -beta, -alpha);
            state[i] = '0';

            // get best result
            if (result > bestVal) {
                bestVal = result;
            }
            
            // retain the BEST score the of the curr player as alpha
            if (bestVal > alpha) {
                alpha = bestVal;
            }

            // "prune" this recursion, the opp has an advantage here
            if (alpha >= beta) {
                break;
            }

        }
    }
    return bestVal;
}

