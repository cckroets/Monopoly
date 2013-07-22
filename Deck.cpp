//
//  Deck.cpp
//  MonopolyDistribution
//
//  Created by Curtis Kroetsch on 2013-07-17.
//  Copyright (c) 2013 Curtis Kroetsch. All rights reserved.
//

#include "Deck.h"
#include <algorithm>

/* ---------- ADVANCE-TO CARD ------------- */

// Find the closest place to move to
uint AdvanceTo::closest_place(uint pos) {

    /* For each possible place, pick the one closest to the player
    without going over it */
    for (uint i = 0; i < _places.size(); i++) {
        if (_places[i] > pos) return _places[i];        
    }
    return _places[0];
}

// Advance the player to the nearest place
void AdvanceTo::action(Player& P) {
    P.move(closest_place(P.posn()));
}

/* ---------- MOVE 3 STEPS BACK CARD ------------- */

void MoveBack3::action(Player& P) {
    P.move(P.posn() - 3);
}

/* ---------- Go To Jail CARD ------------- */

void GoDirToJail::action(Player& P) {
    P.goToJail();
}

/* ---------- Skip Jail CARD ------------- */

void SkipJail::action(Player& P) {
    _drawn = true;
    P.getCard(this);
}

/* ---------- Deck ------------- */

// Shuffle the deck
void Deck::shuffle() {

    std::random_shuffle(_cards.begin(), _cards.end());
    _cardPos = 0;
}

// Insert a card into the deck
void Deck::insert_card(Card* C) {
    _cards.push_back(C);
}

// Check to see if the next card is a taken goojf card
bool Deck::ghost_card() {
    return _cards.at(_cardPos)->drawn();
}

// Check to see if the deck is empty
bool Deck::empty() {
    
    if (_cardPos >= _cards.size()) return true;
    
    return (ghost_card() && _cardPos == _cards.size() - 1);
}

// Draw a card from the top of the deck
Card& Deck::draw_card() {

    // If the last card is a ghost card or the deck is empty, shuffle
    if (empty()) shuffle();
    
    // If the non-last card is a ghost card, skip it
    if (ghost_card()) _cardPos++;
    
    return *_cards.at(_cardPos++);
}


/* ---------- Chance Deck ------------- */

Deck* chance_deck() {
    
    Deck* ChanceDeck = new Deck;
    
    std::vector<uint> utils(2);
    utils[0] = 12;
    utils[1] = 28;
    
    std::vector<uint> rails(4);
    for (uint i = 0; i < rails.size(); i++)
        rails[i] = 5 + i*10;
    
    
    ChanceDeck->insert_card(new AdvanceTo(0));  // Go
    ChanceDeck->insert_card(new AdvanceTo(24)); // Illinois
    ChanceDeck->insert_card(new AdvanceTo(11)); // StCharles
    ChanceDeck->insert_card(new AdvanceTo(5));  // Reading
    ChanceDeck->insert_card(new AdvanceTo(39)); // BoardWalk
    
    ChanceDeck->insert_card(new AdvanceTo(utils)); // Utilities
    ChanceDeck->insert_card(new AdvanceTo(rails)); // Rails
    
    ChanceDeck->insert_card(new GoDirToJail); // Go to jail card
    ChanceDeck->insert_card(new MoveBack3);   // Move back 3 spaces
    
    for (uint i = 0; i < 6; i++) ChanceDeck->insert_card(new Card);
    
    ChanceDeck->shuffle();
    
    return ChanceDeck;
}

Deck* comm_deck() {
    
    Deck* CommDeck = new Deck;
    
    std::vector<uint> utils(2);
    utils[0] = 12;
    utils[1] = 28;
    
    std::vector<uint> rails(4);
    for (uint i = 0; i < rails.size(); i++)
        rails[i] = 5 + i*10;
    
    
    CommDeck->insert_card(new AdvanceTo(0));  // Go
    CommDeck->insert_card(new GoDirToJail); // Go to jail card

    for (uint i = 0; i < 13; i++) CommDeck->insert_card(new Card);
    
    CommDeck->shuffle();
    
    return CommDeck;
}


