//
//  Deck.h
//  MonopolyDistribution
//
//  Created by Curtis Kroetsch on 2013-07-17.
//  Copyright (c) 2013 Curtis Kroetsch. All rights reserved.
//

#ifndef __MonopolyDistribution__Deck__
#define __MonopolyDistribution__Deck__

#include <iostream>
#include <vector>
#include "Board.h"

typedef unsigned int uint;
class Player;


// A card is either from the Community Chest or the Chance decks
class Card {
    
public:
    virtual void action(Player&) { };
    virtual bool drawn() { return false; };
    
    // A card that has no affect on the user
    Card() {}; 
};


// A card that advances the player to a specific location
class AdvanceTo : public Card {
    
private:
    // A list of all the possible places to advance to
    const std::vector<uint> _places;
    
    // Determine the place that is closest to pos
    uint closest_place(uint pos);
    
public:
    
    void action(Player&);
    
    // Advance to a specific place
    AdvanceTo(uint place) : _places(1,place) {};
    
    // Advance to the closest place in the set
    AdvanceTo(std::vector<uint> places) : _places(places) {};
};


// A card that makes the player move back 3 spaces from their current position
struct MoveBack3 : public Card {
    
    void action(Player&);
    MoveBack3() {};
};


// A card that sends the player to jail immediately
struct GoDirToJail : public Card {
        
    void action(Player&);
    GoDirToJail() {};
};

/* A card that can be kept until used, and can allow the player to leave jail */
class SkipJail : public Card {

private:
    bool _drawn;
    
public:
    
    bool drawn()    { return _drawn; };
    void use_card() { _drawn = false; };
    void action(Player&);
    
    SkipJail() : _drawn(false) { };
};


template <class T>
struct Destroyer {
    void operator() (T* ptr) { delete ptr; };
};

// There are two decks, Community Chest and Chance
class Deck {
    
private:
    std::vector<Card*> _cards; // Deck of cards
    uint _cardPos; 

    bool empty();
    bool ghost_card();
    
    Card* _skipJail;
    
public:
    
    void shuffle();          // Shuffle the deck
    void insert_card(Card*); // Insert a card into the deck
    Card& draw_card();       // Draw the next card from the deck
 
    Deck() : _cardPos(0), _skipJail(new SkipJail) {
        insert_card(_skipJail);
    } ; // Construct an empty deck of cards
    
    ~Deck() {
        delete _skipJail;
        std::for_each(_cards.begin(),_cards.end(),Destroyer<Card>());
    };
};



Deck* chance_deck();
Deck* comm_deck();



#endif /* defined(__MonopolyDistribution__Deck__) */
