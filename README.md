DESCRIPTIONS FOR AI

Random AI:
Picks a random square that isn't taken and places a piece there

Every time the player makes there move, the random AI will create a substring of the current state string,
containing the character indexes of every index in the state string where the char is '0' (empty square).

The program then uses rand() to pick and random index and selects that to make a move.

Minimax AI
Uses a recursion algorithm that simulates all possible outcomes for the tic-tac-toe game before picking
the most optimal move

Every time the player makes a move, the program will create a new state string for each possible move they can make next.
The new state string is then pushed into the megamax algorithm, where the program will recursively simulate the rest of the game.

In the megamax algorithm takes the board state, depth (unecessary), and the current player. The recursion algorithm only stops
when the a player has either won, or if the board is full (draw). Otherwise, the recursion algorithm will make recursive calls
for each potential move for the opposing player in response to the move which was made in the previous recursive/function call.
Each recursive call returns a score, which is based on the terminal state. Winning rewards 10 points, while a draw returns nothing.

More importantly, each recursive call switches to the perspective of the opposing player (player turn is set as a parameter in the recursive call), so if the previous recursion made a move on behalf of O, the next recursive call will analyze the board situation and make a move from the perspective of X. 

Thus, checking for a winner is done by checking at what point in the function call the win was achieved. Since a win is only achieved on the player's turn (player X must place down the 3rd X in a row on their turn to win), and each recursion switches to the perspective of the opposing player, the program automatically knows who won based on whoever's turn the win was achieved on.

So when a terminal state is reached, and the recursion must return the most advantagious score, this score is flipped (made negative),
since whatever is most advantagious for Player X is least advantagious for Player O. The program then finds the smallest score
(greatest negative score), to decrease to the previous recursive call to minimze the opposing player's advantage.

