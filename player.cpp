#include "player.hpp"

// HELLO WORLD
/* SUP SURYA, PLEASE WORK
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */




// Making a second change here
// Testing what happens if two people change stuff
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    board = new Board();
    mySide = side;
    if (side == BLACK)
        otherSide = WHITE;
    else
        otherSide = BLACK;
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

// Checks and returns the value of the heuristic 
// (#your tokens - #opponent tokens) for the current player's side.

int Player::countScore(Board* board_copy) {
	int score = 0;
	
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board->occupied(i, j))
			{
				if (board->get(mySide, i, j))
				{
					score++;
				}
				else
				{
					score--;
				}
			}
		}
	}
	
	return score;
	
}

// Computes the value of a player's heuristic after a move is played
// on the board
int Player::moveValue(Move *m){
	int x = m->getX();
	int y = m->getY();
	Board* board_copy = board->copy();
	board_copy->doMove(m, mySide);
	int moveScore = countScore(board_copy);
	
	//Cases: Move is in corner -> *3
	//		 Adjacent move is in corner -> *-3
	//		 On edge -> *2
	// Should be mutually exclusive events (i.e. not both on edge and
	// in corner count at same time)
	// Corner
	if (((x == 0) || (x == 7)) && ((y == 0) || (y == 7)))
	{
		moveScore *= 3;
	}
	else if ((((x + 1 == 7) || (x - 1 == 0)) 
	&& ((y == 0) || (y == 7)))
	|| (((x == 7) || (x == 0))
	&& ((y - 1 == 0) || (y + 1 == 7))))
	{
		moveScore *= -3;
	}
	else if ((x == 0) || (x == 7) || (y == 0) || (y == 7))
	{
		moveScore *= 2;
	}
	
	delete board_copy;
	
	return moveScore;
	
}


/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

    /* Making the opponents move on the Player's copy of board. */
    board -> doMove(opponentsMove, otherSide);

    /* Pass if there are no available moves. */
    if (!board->hasMoves(mySide)) {
        return nullptr;
    }

    Move *m = new Move(0, 0);
    // Dummy score that is impossible.
    int current_score = -1000;
    // Temporary move - guaranteed to be updated since we know there exists
    // at least one possible move.
    Move *current_best = new Move(0, 0);

    // Loops through the board to find all pieces on Player's side. Possible
    // moves are around such points, and we check all such points.
    for (int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            // If the index corresponds to the Player's piece, we check the
            // four points around it.
            if (board->get(mySide, x, y)) {
                // Left point.
                m -> x = x - 2;
                m -> y = y;
                // Checking if it is a valid index.
                if (board->checkMove(m, mySide)) {
                    // Checking if the move is better than the current
                    // known best move.
                    if (moveValue(m) > current_score) {
                        current_score = moveValue(m);
                        current_best = m;
                    }
                }
                // Right point.
                m -> x = x + 2;
                m -> y = y;
                // Checking if it is a valid index.
                if (board->checkMove(m, mySide)) {
                    // Checking if the move is better than the current
                    // known best move.
                    if (moveValue(m) > current_score) {
                        current_score = moveValue(m);
                        current_best = m;
                    }
                }
                // Bottom point.
                m -> x = x;
                m -> y = y - 2;
                // Checking if it is a valid index.
                if (board->checkMove(m, mySide)) {
                    // Checking if the move is better than the current
                    // known best move.
                    if (moveValue(m) > current_score) {
                        current_score = moveValue(m);
                        current_best = m;
                    }
                }
                // Top point.
                m -> x = x;
                m -> y = y + 2;
                // Checking if it is a valid index.
                if (board->checkMove(m, mySide)) {
                    // Checking if the move is better than the current
                    // known best move.
                    if (moveValue(m) > current_score) {
                        current_score = moveValue(m);
                        current_best = m;
                    }
                }

            }
        }
    }

    // If the two pointers are not shared, we delete m.
    if (m != current_best) {
        delete m;
    }
    // Make the chosen move on the
    board->doMove(current_best, mySide);

    return current_best;
}
