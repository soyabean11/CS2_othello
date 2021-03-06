#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {
private:
    Board *board;
    Side mySide;
    Side otherSide;
    bool minimax_on;
    int countScore (Board* board_copy);
    int heuristic_multiplier(Move *m, Board *temp);
    int minimax(Move *move, Board *b, int depth, Side current);
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
