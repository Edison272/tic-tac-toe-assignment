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

AB Pruning
The alpha-pruning was simply a matter of adding a couple extra conditionals to the program to ensure that the recursion would stop after a certain point, so that the AI could save time. In this case, the minimax AI would be trying to see if a boardstate was worth pursuing if it would actually lead to a much more favorable outcome.

The mimimax function was modified in the header file to include arguments for alpha and beta. When the minimax function is first called, alpha (favor current player) and beta (favor opposition) would be set to -1000 and 1000 respectively. In the same way the program flips the boardWinner values so it knows who the board win is in favor of, the alpha and beta values are switched and have their signs flipped with each recursion, so that the alpha-beta pruning is specific to either perspective

The 'pruning' occurs during each loop within the recursion, where the minimax algorithm is testing every single board state, where alpha score will always be the boarstate score which has the highest potential. When the alpha scpre exceeds the beta score, it means that the AI has found a move which is better than the opposing player's best move. Because of this, the AI immediately breaks the loop (stop any further recursions) in order to save time

