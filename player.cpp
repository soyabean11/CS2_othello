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
    minimax_on = true;
    testingMinimax = false;
    board = new Board();

    std::cerr << "Color: " << side << std::endl;

    // code for testing minimax
    /*

    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ',
        'b', 'w', 'b', 'b', 'b', 'b', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };


    board ->setBoard(boardData); */
    mySide = side;
    if (side == BLACK)
        otherSide = WHITE;
    else
        otherSide = BLACK;
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
	return board_copy->count(mySide) - board_copy->count(otherSide);
}



int Player::heuristic_multiplier(Move *m) {
    //Cases: Move is in corner -> *3
    //       Adjacent move is in corner -> *-3
    //       On edge -> *2
    // Should be mutually exclusive events (i.e. not both on edge and
    // in corner count at same time)
    // Corner
    if (m == nullptr) {
        return 1;
    }
    int x = m->getX();
    int y = m->getY();

    // Corners
    if (((x == 0) || (x == 7)) && ((y == 0) || (y == 7)))
    {
        return 5;
    }
    // Cells adjacent to corners (non-diagonal)
    else if ((((x + 1 == 7) || (x - 1 == 0))
    && ((y == 0) || (y == 7)))
    || (((x == 7) || (x == 0))
    && ((y - 1 == 0) || (y + 1 == 7))))
    {
        return -1;
    }
    // Cells adjacent to corners on the diagonal
    else if (((y + 1 == 7) && ((x - 1 == 0) || (x + 1 == 7)))
    || ((y - 1 == 0) && ((x - 1 == 0) || (x + 1 == 7))))
    {
		return -2;
	}
    // Other cells on the edges
    else if ((x == 0) || (x == 7) || (y == 0) || (y == 7))
    {
        return 3;
    }
    // Nothing special
    return 1;
}

// Computes the value of a player's move for the heauristic method.
int Player::moveValue(Move *m){

	Board* board_copy = board->copy();
	board_copy->doMove(m, mySide);
	int moveScore = countScore(board_copy);

    if (moveScore > 0) {
        moveScore *= heuristic_multiplier(m);
    }

	delete board_copy;

	return moveScore;
}

// Computes the value of a player's move for minimax.
int Player::moveValue_minimax(Move *m1) {
    Board* new_board = board->copy();

     /* Pass if there are no available moves. */
    new_board -> doMove(m1, mySide);

    if (!new_board->hasMoves(otherSide)) {
        return countScore(new_board);
    }

    Move *m = new Move(0, 0);
    // Dummy score that is impossible.
    int current_score = 2000;
    int temp;

    // Loops through the board to find all pieces on Player's side. Possible
    // moves are around such points, and we check all such points.
    for (int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            m -> setX(x);
            m -> setY(y);
            if(new_board->checkMove(m, otherSide)) {
                // Checking if the move is better than the current
                // known best move.
                Board *move_board = new_board -> copy();
                move_board -> doMove(m, otherSide);
                temp = countScore(move_board);
                delete move_board;
                if (temp < current_score) {
                        current_score = temp;
                }
            }
        }
    }


    delete m;
    delete new_board;

    int moveScore = current_score;

    /*if (moveScore > 0) {
        moveScore *= heuristic_multiplier(m);
    } */
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
    int temp;
    // Temporary move - guaranteed to be updated since we know there exists
    // at least one possible move.
    Move *current_best = new Move(0, 0);

    // Loops through the board to find all pieces on Player's side. Possible
    // moves are around such points, and we check all such points.
    for (int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            m -> setX(x);
            m -> setY(y);
            if(board->checkMove(m, mySide)) {
                // Checking if the move is better than the current
                // known best move.
                temp = minimax (m, board, 2, mySide);

                if (temp > current_score) {
                        current_score = temp;
                        current_best->setX(m->getX());
                        current_best->setY(m->getY());
                }
            }
        }
    }
    delete m;

    // Make the chosen move on the
    board->doMove(current_best, mySide);

    Board* board_copy = board->copy();
    delete board_copy;

    return current_best;
}

int Player::score(Move *m, Board *b, Side current) {
    int temp = countScore(b);
    if (current == mySide)
        if (temp > 0) {
            return temp * heuristic_multiplier(m);
        }
        else {
            return temp;
        }
    else {
        if (temp < 0) {
            return -1 * temp * heuristic_multiplier(m);
        }
        else {
            return -1 * temp;
        }
    }
}

int Player::minimax(Move *move, Board *b, int depth, Side current) {

	if (depth == 0) {
		int temp = score(move, b, current);
        return temp;
	}

	Side other;
	if (current == WHITE) 	
		other = BLACK;
	else 
		other = WHITE;
		
    Board *next_board = board->copy();
    next_board->doMove(move, current);

    if (!board->hasMoves(other)) {
        return minimax(nullptr, next_board, depth - 1, other);
    }

    Move *m = new Move(0, 0);
    // Dummy score that is impossible.
    int current_score;
    
    if (current == mySide)
        current_score = -1000;
    else
        current_score = 1000;


    int temp;

    // Loops through the board to find all pieces on Player's side. Possible
    // moves are around such points, and we check all such points.
    for (int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            m -> setX(x);
            m -> setY(y);
            if(next_board->checkMove(m, other)) {

                // Checking if the move is better than the current
                // known best move.
                temp = minimax(m, next_board, depth - 1, other);

                if ((temp > current_score && current == mySide) ||
                        (temp < current_score && current == otherSide)) {
                    current_score = temp;
                }
            }
        }
    }

    return current_score;
}
