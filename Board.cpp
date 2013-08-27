//
//  Board.cpp
//  MonopolyDistribution
//
//  Created by Curtis Kroetsch on 2013-07-17.
//  Copyright (c) 2013 Curtis Kroetsch. All rights reserved.
//

#include "Board.h"
#include <iomanip>



 
// Set up the board
MonopolyBoard::MonopolyBoard():  _lands(0) {
    
    _properties.push_back(Space("Go", Other));
    _properties.push_back(Space("Mediterranean", Other));
    _properties.push_back(Space("Community Chest (1)", Comm));
    _properties.push_back(Space("Baltic", Other));
    _properties.push_back(Space("Income Tax", Other));
    _properties.push_back(Space("Reading", Other));
    _properties.push_back(Space("Oriental", Other));
    _properties.push_back(Space("Chance (1)", Chance));
    _properties.push_back(Space("Vermont", Other));
    _properties.push_back(Space("Connecticut", Other));
    
    _properties.push_back(Space("Jail (Visiting)", Jail));
    _properties.push_back(Space("St. Charles", Other));
    _properties.push_back(Space("Electric Company", Other));
    _properties.push_back(Space("States", Other));
    _properties.push_back(Space("Virginia", Other));
    _properties.push_back(Space("Pennsylvania Ave.", Other));
    _properties.push_back(Space("St. James", Other));
    _properties.push_back(Space("Community Chest (2)", Comm));
    _properties.push_back(Space("Tenessee", Other));
    _properties.push_back(Space("New York", Other));
    
    _properties.push_back(Space("Free Parking", Other));
    _properties.push_back(Space("Kentucky", Other));
    _properties.push_back(Space("Chance (2)", Chance));
    _properties.push_back(Space("Indiana", Other));
    _properties.push_back(Space("Illinois", Other));
    _properties.push_back(Space("B. & 0.", Other));
    _properties.push_back(Space("Atlantic", Other));
    _properties.push_back(Space("Ventor", Other));
    _properties.push_back(Space("Water Works", Other));
    _properties.push_back(Space("Marvin Gardens", Other));

    _properties.push_back(Space("Go to Jail", GoToJail));
    _properties.push_back(Space("Pacific", Other));
    _properties.push_back(Space("North Carolina", Other));
    _properties.push_back(Space("Community Chest (3)", Comm));
    _properties.push_back(Space("Pennsylvania RR.", Other));
    _properties.push_back(Space("Short Line", Other));
    _properties.push_back(Space("Chance (3)", Chance));
    _properties.push_back(Space("Park Place", Other));
    _properties.push_back(Space("Luxury Tax", Other));
    _properties.push_back(Space("Boardwalk", Other));
    
    _ChanceCards = chance_deck();
    _CommunityCards = comm_deck();
    
}


// Play a game with a se number of turns 
void Game::play(uint total_turns) {
    
    while (total_turns) {
        next_player().roll_dice();
        _turn = (_turn + 1) % _players.size();
        total_turns--;
    }
}


void Player::move(uint np)  {
    _position = np;
    _gameboard.land_on(np);
}

// Complete a turn as a player who is not in jail
void Player::play_turn(uint roll) {
    
    uint new_pos = (posn() + roll) % _gameboard.size();
    
    move(new_pos);
    
    SpaceType space = _gameboard.type(new_pos);
    
    switch (space) {
        case (Other)    : break;
        case (Jail)     : break;
        case (GoToJail) : goToJail(); break;
        default         : _gameboard.drawCard(space).action(*this);
    }    
}

// Roll the dice and do the appropriate actions if in jail
void Player::roll_dice(uint successive_doubles) {
    
    // Roll both dice
    uint d1 = roll_die();
    uint d2 = roll_die();
    
    bool doubles = (d1 == d2);
    
    // If the player has rolled doubles thrice in a row, go to jail
    if (doubles && (successive_doubles == 2)) {
        goToJail();
        return;
    }
    
    /* If the player is free, has a card, or pays to leave jail: */
    if (!jailed() || use_card() || pay_fine()) {
        
        play_turn(d1 + d2);
        // If doubles were rolled, roll again
        if (doubles) roll_dice(++successive_doubles);
        
        // If the player is in jail but rolls doubles:
    } else if (jailed() && doubles) {
        _jailed = 0;
        play_turn(d1 + d2);
        
        // If the player remains in jail:
    } else _jailed--;
    
}
        
// Use a get out of jail free card to leave jail
bool Player::use_card() {
    
    if (_CardSlots.empty()) return false;
    else {
        _CardSlots.front()->use_card();
        _CardSlots.pop();
        return true;
    }
}

Card& MonopolyBoard::drawCard(SpaceType T) {
    
    if (T == Comm) return _CommunityCards->draw_card();
    else return _ChanceCards->draw_card();
}

#include <iostream>

bool SpaceLT(const Space& S1, const Space& S2) {
    
    return S1.lands() < S2.lands();
}

void print_bar(uint length) {
    
    const char token = ' ';
    for (int i = 0; i < length; i++) std::cout << token;
    std::cout << std::endl;
}

void MonopolyBoard::print_results() {
    

    std::vector<Space>::iterator S = _properties.begin();
    
    std::sort(_properties.begin(),_properties.end(), SpaceLT);
    
    for (; S != _properties.end(); S++) {
        double pct = 100.0 * S->lands() / _lands;
        std::cout  << std::setw(20) << std::left << S->name() << pct << "%\t";
        print_bar(10*pct);
    }
}



