//
//  Board.h
//  MonopolyDistribution
//
//  Created by Curtis Kroetsch on 2013-07-17.
//  Copyright (c) 2013 Curtis Kroetsch. All rights reserved.
//

#ifndef __MonopolyDistribution__Board__
#define __MonopolyDistribution__Board__

#include "Deck.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <queue>

typedef unsigned int uint;
typedef std::string Name;

template<class Num>
inline bool max(Num a, Num b) {
    return (a < b) ? b : a;
};

// A type of space on the game board
enum SpaceType { Chance, Comm, GoToJail, Other, Jail = 10 };

class MonopolyBoard;
class SkipJail;
class Deck;
class Card;

// A player in the game
class Player {
    
private:
    uint _position;      // The player's position on the board
    uint _jailed;        // Turns left in jail [0,3]
    
    // Two card slots for possible GooJF cards
    std::queue<SkipJail*> _CardSlots;

    // The gameboard with all the properties and decks
    MonopolyBoard& _gameboard;
    
    // The player has a 50/50 shot that they will pay the fine
    bool pay_fine() {
        if (rand() % 2) _jailed = 0;
        return ! jailed();
    };
    
    // Use a get out of jail free card to leave jail
    bool use_card();
    
    // Return true iff the player is in jail
    bool jailed() { return _jailed; };

    // Roll a single die
    uint roll_die() { return (rand() % 6) + 1; };
    
    // Complete a turn outside of jail with the given roll
    void play_turn(uint);

public:
    
    uint posn()         { return _position; };
    
    // Move the player to their new position
    void move(uint np);
    
    // Receive a GooJF card
    void getCard(SkipJail* C) {
        _CardSlots.push(C);
    };
    
    // Send the player directly to jail
    void goToJail() {
        _position = Jail;
        _jailed = 3;
    };
    
    // Roll the pair of dice
    void roll_dice(uint=0);
    
    Player(MonopolyBoard& MB) :
        _position(0),
        _jailed(0),
        _gameboard(MB) { };
    
};




// A space on the monopoly board
class Space {
   
private:
    uint _lands;
    Name _name;
    SpaceType _type;
    
public:
    Name name()      const { return _name; };
    SpaceType type() const { return _type; };
    
    uint lands() const { return _lands; };
    void land() { _lands++; };
    Space(Name n, SpaceType t) : _name(n), _type(t), _lands(0) { };
};




// A given state of the monopoly board
class MonopolyBoard {
    
private:
    std::vector<Space> _properties;
    
    uint _lands;
    
    Deck* _ChanceCards;
    Deck* _CommunityCards;


public:
    
    Card& drawCard(SpaceType);
    
    size_t size() { return _properties.size(); };
    
    // Land on the vth board space 
    void land_on(uint v) {
        _lands++;
        _properties[v].land();
    }
    
    SpaceType type(uint i) {
        return _properties[i].type();
    }
    
    void print_results();

    MonopolyBoard();

};

class Game {
    
private:
    std::vector<Player> _players;
    MonopolyBoard _Board;
    uint _turn;
    
    Player& next_player() { return _players[_turn]; };
    
public:
    
    // Play a game with n players
    Game(uint numPlayers) :_players(numPlayers,Player(_Board)) { };
    
    void print() { _Board.print_results(); };
    
    // Play a specified number of turns
    void play(uint);
};










#endif /* defined(__MonopolyDistribution__Board__) */
